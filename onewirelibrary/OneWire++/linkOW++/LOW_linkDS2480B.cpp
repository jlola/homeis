/***************************************************************************
t                          LOW_linkDS2480B.cpp  -  description
                             -------------------
    begin                : Sat Jul 13 2002
    copyright            : (C) 2002 by Harald Roelle
    email                : roelle@informatik.uni-muenchen.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

 
#include "LOW_linkDS2480B.h"
#include "PoppyDebugTools.h"


LOW_linkDS2480B::LOW_linkDS2480B( const LOW_portSerialFactory::portSpecifier_t &inSerPortSpec, const RXPOL_val_t inRXPOL,
                                  const bool inHasExternalPower, const bool inAllowProgPulse) :
  LOW_link( true, inHasExternalPower, inAllowProgPulse),
  LOW_linkFlexibleSpeed( normal_speed, pdSlewRate_1_37, w1LowTime_11, soW0RecTime_10), // configure the recommeded optimal parameters as of app note #148
  receivePolarity( inRXPOL)
{
  serialPort   = LOW_portSerialFactory::new_portSerial( inSerPortSpec);
  
  resetLinkAdapter();
}

  
LOW_linkDS2480B::~LOW_linkDS2480B()
{
  serialPort->tty_flush();
  
  delete serialPort;
}


//=====================================================================================
//
// touch data methods
//
 
bool LOW_linkDS2480B::touchBit( const bool inSendBit, const strongPullup_t inPullup)
{
  commLock lock( *this);
  
  if ( inPullup != pullUp_NONE ) {
    setStrongPullupDuration_cmd( strongPullup_2_SPUD_val( inPullup));
  }
  
  return singleBit_cmd( inSendBit, (inPullup==pullUp_NONE)?false:true);
}

 
uint8_t LOW_linkDS2480B::touchByte( const uint8_t inSendByte, const strongPullup_t inPullup)
{
	STACK
  commLock lock( *this);
  
  if ( inPullup != pullUp_NONE ) {
    setStrongPullupDuration_cmd( strongPullup_2_SPUD_val( inPullup));
    pulse_cmd( true, false, true); // dummy pulse for arming
  }
  STACK_SECTION("setMode( data_mode);")
  setMode( data_mode);
  
  serialPort->tty_write( inSendByte);
  if ( inSendByte == SwitchToCommandMode_Cmd )
    serialPort->tty_write( inSendByte);
  
  uint8_t retValue = serialPort->tty_readByte( (inPullup==pullUp_NONE)?false:true);
      
  if ( inPullup != pullUp_NONE ) {
    pulse_cmd( false, false, true);  // dummy pilse for disarming
  }
  
  return retValue;
}

  
byteVec_t LOW_linkDS2480B::touchBlock( const byteVec_t &inBytes, const strongPullup_t inPullup)
{
	STACK
  if ( inBytes.size() == 0 ) return byteVec_t( 0);

  commLock lock( *this);

  //byteVec_t   retValue = byteVec_t( inBytes.size()-1);  // create it smaller, last byte will be pushed
  byteVec_t   retValue = byteVec_t(inBytes.size());
  //byteVec_t   writeBytes;
    
  uint8_t commandMode = SwitchToCommandMode_Cmd;
   // This breaks on RedHat 7.2:
   byteVec_t            writeBytes = byteVec_t( inBytes);
   byteVec_t::iterator  iter = writeBytes.begin();
   while( iter != writeBytes.end() ) {
    if ( *iter == SwitchToCommandMode_Cmd )
      writeBytes.insert( iter, commandMode);
     iter++;
   }


//  // don't send last byte, we'll handle it seperately
//  for( unsigned int a=0; a<inBytes.size(); a++) {
//    writeBytes.push_back( inBytes[a]);
//    if ( inBytes[a] == SwitchToCommandMode_Cmd )
//    writeBytes.push_back( inBytes[a]);
//  }
  
  setMode( data_mode);

  if ( writeBytes.size() > 0 ) {
    serialPort->tty_write( writeBytes);
    serialPort->tty_read( retValue, false);
  }
  
  // now the last byte with the optional pullup
  //uint8_t lastByte = touchByte( inBytes[inBytes.size()-1], inPullup);
  //retValue.push_back( lastByte);
  
  return retValue;
}

  
  
//=====================================================================================
//
// misc methods
//
  
void LOW_linkDS2480B::resetLinkAdapter()
{
	STACK
  commLock lock( *this);
  
  internalMode = command_mode;

  //
  // begin with the standard speed
  //
  serialPort->tty_configure( LOW_portSerial::none_flowControl, LOW_portSerial::bit8_size, 
                             LOW_portSerial::no_parity, LOW_portSerial::bit1_stopBit, 
                             LOW_portSerial::B9600_speed);
  
  serialPort->tty_flush(); // flush the buffers
    
  serialPort->tty_break(); // send a break to reset the DS2480
  LOW_platformMisc::milliSleep( 2); // delay to let line settle

  serialPort->tty_flush(); // flush the buffers
  
  //
  // let the DS2480 adapt to the serial speed
  //
  serialPort->tty_write( SerialSpeedAdapt_Cmd);
  serialPort->tty_flush( false, true);
  LOW_platformMisc::milliSleep( 4);
  
  //
  // configure desired serial speed and polarity
  //
  RBR_val_t serialSpeed;
  switch ( wireSpeed ) {
    case normal_speed:     serialSpeed = RBR_9_6;   break;
    case flexible_speed:   serialSpeed = RBR_9_6;   break;
    case overdrive_speed:  serialSpeed = RBR_57_6;  break;
    default:               throw comm_error( "Unknown wire speed detected", __FILE__, __LINE__);
  }
  try {
    setRS232BaudRate_cmd( serialSpeed, receivePolarity);
  }                // ignore errors, result can't be right
  catch ( ...) {}  // because we're just setting the correct baud rate
  
  //
  // now switch the port to the new speed
  //
  switch ( wireSpeed ) {
    case normal_speed:
      serialPort->tty_configure( LOW_portSerial::none_flowControl, LOW_portSerial::bit8_size, 
                                 LOW_portSerial::no_parity, LOW_portSerial::bit1_stopBit, 
                                 LOW_portSerial::B9600_speed);
      break;
    
    case flexible_speed:
      serialPort->tty_configure( LOW_portSerial::none_flowControl, LOW_portSerial::bit8_size, 
                                 LOW_portSerial::no_parity, LOW_portSerial::bit1_stopBit, 
                                 LOW_portSerial::B9600_speed);
      break;
    
    case overdrive_speed:
      serialPort->tty_configure( LOW_portSerial::none_flowControl, LOW_portSerial::bit8_size, 
                                 LOW_portSerial::no_parity, LOW_portSerial::bit1_stopBit, 
                                 LOW_portSerial::B57600_speed);
      break;
    
    default:
      throw comm_error( "Unknown wire speed detected", __FILE__, __LINE__);
  }
  
  // do it again, to test wheter it was successfull
  setRS232BaudRate_cmd( serialSpeed, receivePolarity);
  
  // restore former settings
  // DON'T CALL setWireSpeed( wireSpeed); HERE OR WE GET AN ENDLESS LOOP! SPEED SETTING ALREADY DONE!
  if ( wireSpeed == flexible_speed ) {
    setPullDownSlewRate( pdSlewRate);
    setWrite1LowTime( w1LowTime);
    setSampleOffsetWrite0Rec( soW0RecTime);
  }
}  


bool LOW_linkDS2480B::resetBus()
{
  commLock lock( *this);
  

  resetAnswer_t  answer;
  
  reset_cmd( &answer);

  hasProgramPulse = answer.isVppPresent;

  /** @todo Check if and how "alarming presence pulse" response works. */  
  switch( answer.busStatus ) {
    case oneWireShorted_busStat:        throw comm_error( "Short to ground detected", __FILE__, __LINE__);
    case noPresencePulse_busStat:       return false;
    case alarmingPresencePulse_busStat: // fall thru
    case presencePulse_busStat:         // fall thru
    default:                            return true;
  }
}

 
void LOW_linkDS2480B::strongPullup( const unsigned long inMilliSecs)
{
  commLock lock( *this);

  switch ( inMilliSecs ) {
    case   16: strongPullup( pullUp_16_4); return;
    case   64: strongPullup( pullUp_65_5); return;
    case  128: strongPullup( pullUp_131);  return;
    case  256: strongPullup( pullUp_262);  return;
    case  512: strongPullup( pullUp_524);  return;
    case 1024: strongPullup( pullUp_1048); return;
    default:   break;
  }
  
  pulse_cmd_manual( inMilliSecs, false);
}


void LOW_linkDS2480B::strongPullup( const strongPullup_t inPullupTimes)
{
  commLock lock( *this);

  unsigned int timeout = 0;
  switch( inPullupTimes ) {
    case pullUp_16_4:  // fall thru
    case pullUp_65_5:  // fall thru
    case pullUp_131:   // fall thru
    case pullUp_262:   // fall thru
    case pullUp_524:   timeout = 1;  break;
    case pullUp_1048:  timeout = 2;  break;
    default:           throw internal_error( "Unknown strongPullup value detected", __FILE__, __LINE__);
  }

  setStrongPullupDuration_cmd( strongPullup_2_SPUD_val( inPullupTimes));
  pulse_cmd( false, false, false, timeout);
}


void LOW_linkDS2480B::programPulse( const unsigned long inMicroSecs)
{
  commLock lock( *this);

  if ( ! allowProgPulse )
    throw notAllowed_error( "Program pulse not allowed", __FILE__, __LINE__);

  if ( ! hasProgramPulse )
    throw illegalLevel_error( "Program pulse not available", __FILE__, __LINE__);

  switch( inMicroSecs ) {
    case   32: programPulse( progPulse_32);   return;
    case   64: programPulse( progPulse_64);   return;
    case  128: programPulse( progPulse_128);  return;
    case  256: programPulse( progPulse_256);  return;
    case  512: programPulse( progPulse_512);  return;
    case 1024: programPulse( progPulse_1024); return;
    case 2048: programPulse( progPulse_2048); return;
    default:   break;
  }

  pulse_cmd_manual( inMicroSecs, true);
}


void LOW_linkDS2480B::programPulse( const progPulse_t inPulseTime)
{
  commLock lock( *this);

  if ( ! allowProgPulse )
    throw notAllowed_error( "Program pulse not allowed", __FILE__, __LINE__);

  if ( ! hasProgramPulse )
    throw illegalLevel_error( "Program pulse not available", __FILE__, __LINE__);

  setProgPulseDuration_cmd( progPulse_2_PPD_val( inPulseTime));
  pulse_cmd( false, true, false);
}


void LOW_linkDS2480B::doSearchSequence( const LOW_deviceIDRaw &inBranchVector, 
                                        LOW_deviceIDRaw &outFoundID, LOW_deviceIDRaw &outDiscrVec)
{
  commLock lock( *this);
  
  byteVec_t  sendVector = byteVec_t( 16, 0x00);

  int bitPos = 0;
  for( int a=0; a<16; a++) {
    for( int b=0; b<4; b++) {
      if ( inBranchVector.getBit( bitPos)==true )
        sendVector[a] |= 0x01<<(2*b+1);
      
      bitPos++;
    }
  }
    
  searchAccelCtrl_cmd( true);
  byteVec_t recVector = touchBlock( sendVector);
  searchAccelCtrl_cmd( false);
  
  if ( recVector.size()!=16 )
    throw sizeMismatch_error( "Search received vector size mismatch", __FILE__, __LINE__);
  
  bitPos = 0;
  for( int a=0; a<16; a++) {
    for( int b=0; b<4; b++) {
      if ( recVector[a] & (0x01<<((2*b)+1)) )
        outFoundID.setBit( bitPos, true);
      else
        outFoundID.setBit( bitPos, false);
      
      if ( recVector[a] & (0x01<<(2*b)) )
        outDiscrVec.setBit( bitPos, true);
      else
        outDiscrVec.setBit( bitPos, false);
      
      bitPos++;
    }  
  }
  
}


//=====================================================================================
//
// methods required by LOW_linkFlexibleSpeed
//
  
void LOW_linkDS2480B::setWireSpeed( const wireSpeed_t inWireSpeed)
{
  commLock lock( *this);
  
  LOW_linkFlexibleSpeed::setWireSpeed( inWireSpeed); // conserve the setting via superclass
  
  resetLinkAdapter();
  resetBus();
}


LOW_linkDS2480B::wireSpeed_t LOW_linkDS2480B::getWireSpeed()
{
  commLock lock( *this);

  return wireSpeed;
}


void LOW_linkDS2480B::setPullDownSlewRate( const pdSlewRate_t inPDSR)
{
  commLock lock( *this);
  
  LOW_linkFlexibleSpeed::setPullDownSlewRate( inPDSR);  // conserve the setting via superclass
  
  setPullDownSlewRate_cmd( pdSlewRate_2_PDSR_val( inPDSR));
}


LOW_linkDS2480B::pdSlewRate_t LOW_linkDS2480B::getPullDownSlewRate()
{
  commLock lock( *this);
  
  LOW_linkFlexibleSpeed::getPullDownSlewRate();  // sanity checking in superclass
  
  return PDSR_val_2_pdSlewRate( getPullDownSlewRate_cmd());
}


void LOW_linkDS2480B::setWrite1LowTime( const w1LowTime_t inW1LT)
{
  commLock lock( *this);
  
  LOW_linkFlexibleSpeed::setWrite1LowTime( inW1LT);  // conserve the setting via superclass
  
  setWrite1LowTime_cmd( w1LowTime_2_W1LT_val(inW1LT));
}


LOW_linkDS2480B::w1LowTime_t LOW_linkDS2480B::getWrite1LowTime()
{
  commLock lock( *this);
  
  LOW_linkFlexibleSpeed::getWrite1LowTime();  // sanity checking in superclass
  
  return W1LT_val_2_w1LowTime( getWrite1LowTime_cmd());
}


void LOW_linkDS2480B::setSampleOffsetWrite0Rec( const soW0RecTime_t inSOW0RT)
{
  commLock lock( *this);
  
  LOW_linkFlexibleSpeed::setSampleOffsetWrite0Rec( inSOW0RT);  // conserve the setting via superclass

  setSampleOffsetWrite0Rec_cmd( soW0RecTime_2_SOW0RT_val(inSOW0RT));
}


LOW_linkDS2480B::soW0RecTime_t LOW_linkDS2480B::getSampleOffsetWrite0Rec()
{
  commLock lock( *this);
  
  LOW_linkFlexibleSpeed::getSampleOffsetWrite0Rec();  // sanity checking in superclass
  
  return SOW0RT_val_2_soW0RecTime( getSampleOffsetWrite0Rec_cmd());
}



//=====================================================================================
//
// DS2480B commands
//


void LOW_linkDS2480B::reset_cmd( resetAnswer_t *outAnswer)
{
  // locking done in publicly accessible methods
  
  setMode( command_mode);

  serialPort->tty_flush();
  
  uint8_t outByte = Reset_Cmd | (wireSpeed_2_OWSPEED_val( wireSpeed)&0x03)<<2;
  serialPort->tty_write( outByte);
  
  uint8_t reply = serialPort->tty_readByte();
  
  if ( (reply&0xc0) != (outByte&0xc0) )
    throw comm_error( "Unexpected reply from reset command", __FILE__, __LINE__);

  outAnswer->busStatus     = static_cast<busStatus_t>(reply&0x03);
  outAnswer->isVppPresent  = (reply>>5)&0x01;
  outAnswer->chipRevision  = (reply>>2)&0x07;
}


bool LOW_linkDS2480B::singleBit_cmd( const bool inBitVal, const bool inStrongPullup)
{
  // locking done in publicly accessible methods
  
  setMode( command_mode);

  uint8_t outByte = SingleBit_Cmd | (static_cast<uint8_t>(inBitVal))<<4
                                  | (wireSpeed_2_OWSPEED_val( wireSpeed)&0x03)<<2
                                  | (static_cast<uint8_t>(inStrongPullup))<<1;
  serialPort->tty_write( outByte);
  
  uint8_t reply = serialPort->tty_readByte();
  
  if ( (reply&0xfc) != (outByte&0xfc) )
    throw comm_error( "Unexpected reply from single bit command", __FILE__, __LINE__);

  bool retVal = reply&0x01;

  if ( inStrongPullup ) {
    reply = serialPort->tty_readByte();
    if ( (reply&0xfc) != 0xec )
      throw comm_error( "Unexpected reply from single bit strong pullup response", __FILE__, __LINE__);
  }
  
  return retVal;
}


void LOW_linkDS2480B::pulse_cmd( const bool inArm, const bool inProgPulse, const bool inImmidiateTerm, const unsigned int inSecTimeout)
{
  // locking done in publicly accessible methods

  setMode( command_mode);

  byteVec_t  outBytes;

  outBytes.push_back( Pulse_Cmd | (static_cast<uint8_t>(inProgPulse))<<4
                                | (0x03)<<2
                                | (static_cast<uint8_t>(inArm))<<1);
  if ( inImmidiateTerm )
    outBytes.push_back( static_cast<uint8_t>( PulseTermination_Cmd));

  serialPort->tty_write( outBytes);

  uint8_t reply = serialPort->tty_readByte( false, inSecTimeout);

  if ( (reply&0xfc) != (outBytes[0]&0xfc) )
    throw comm_error( "Unexpected reply from pulse command", __FILE__, __LINE__);
}


void LOW_linkDS2480B::pulse_cmd_manual( const unsigned long inSleepTime, const bool inProgPulse)
{
  // locking done in publicly accessible methods

  setMode( command_mode);

  if ( inProgPulse )
    setProgPulseDuration_cmd( PPD_inf);
  else
    setStrongPullupDuration_cmd( SPUD_inf);

  uint8_t outByte = Pulse_Cmd | (static_cast<uint8_t>(inProgPulse))<<4
                              | (0x03)<<2;

  serialPort->tty_write( outByte);

  if ( inProgPulse )
    LOW_platformMisc::microSleep( inSleepTime);
  else
    LOW_platformMisc::milliSleep( inSleepTime);

  serialPort->tty_write( static_cast<uint8_t>( PulseTermination_Cmd));

  uint8_t reply = serialPort->tty_readByte( false);

  if ( (reply&0xfc) != (outByte&0xfc) )
    throw comm_error( "Unexpected reply from pulse command", __FILE__, __LINE__);
}


void LOW_linkDS2480B::searchAccelCtrl_cmd( const bool inAccelOn)
{
  // locking done in publicly accessible methods
  
  setMode( command_mode);

  uint8_t outByte = SearchAccel_Cmd | (static_cast<uint8_t>(inAccelOn))<<4
                                    | (wireSpeed_2_OWSPEED_val( wireSpeed)&0x03)<<2 ;
  serialPort->tty_write( outByte);
}



//=====================================================================================
//
// DS2480B configuration commands
//
  
void LOW_linkDS2480B::setPullDownSlewRate_cmd( const PDSR_val_t inPDSR)
{
  // locking done in publicly accessible methods

  writeConfigValue( PullDownSlewRate_cfgCmd, inPDSR);
}

LOW_linkDS2480B::PDSR_val_t LOW_linkDS2480B::getPullDownSlewRate_cmd()
{
  // locking done in publicly accessible methods

  return static_cast<PDSR_val_t>(readConfigValue( PullDownSlewRate_cfgCmd));
}


void LOW_linkDS2480B::setProgPulseDuration_cmd( const PPD_val_t inPPD)
{
  // locking done in publicly accessible methods

  writeConfigValue( ProgPulseDuration_cfgCmd, inPPD);
}

LOW_linkDS2480B::PPD_val_t LOW_linkDS2480B::getProgPulseDuration_cmd()
{
  // locking done in publicly accessible methods

  return static_cast<PPD_val_t>(readConfigValue( ProgPulseDuration_cfgCmd));
}


void LOW_linkDS2480B::setStrongPullupDuration_cmd( const SPUD_val_t inSPUD)
{
  // locking done in publicly accessible methods

  writeConfigValue( StrongPullupDuration_cfgCmd, inSPUD);
}

LOW_linkDS2480B::SPUD_val_t LOW_linkDS2480B::getStrongPullupDuration_cmd()
{
  // locking done in publicly accessible methods

  return static_cast<SPUD_val_t>(readConfigValue( StrongPullupDuration_cfgCmd));
}


void LOW_linkDS2480B::setWrite1LowTime_cmd( const W1LT_val_t inW1LT)
{
  // locking done in publicly accessible methods

  writeConfigValue( Write1LowTime_cfgCmd, inW1LT);
}

LOW_linkDS2480B::W1LT_val_t LOW_linkDS2480B::getWrite1LowTime_cmd()
{
  // locking done in publicly accessible methods

  return static_cast<W1LT_val_t>(readConfigValue( Write1LowTime_cfgCmd));
}


void LOW_linkDS2480B::setSampleOffsetWrite0Rec_cmd( const SOW0RT_val_t inSOW0RT)
{
  // locking done in publicly accessible methods

  writeConfigValue( SampleOffsetWrite0Rec_cfgCmd, inSOW0RT);
}

LOW_linkDS2480B::SOW0RT_val_t LOW_linkDS2480B::getSampleOffsetWrite0Rec_cmd()
{
  // locking done in publicly accessible methods

  return static_cast<SOW0RT_val_t>(readConfigValue( SampleOffsetWrite0Rec_cfgCmd));
}


void LOW_linkDS2480B::setLoadSensorThreshold_cmd( const LST_val_t inLST)
{
  // locking done in publicly accessible methods

  writeConfigValue( LoadSensorThreshold_cfgCmd, inLST);
}

LOW_linkDS2480B::LST_val_t LOW_linkDS2480B::getLoadSensorThreshold_cmd()
{
  // locking done in publicly accessible methods

  return static_cast<LST_val_t>(readConfigValue( LoadSensorThreshold_cfgCmd));
}


void LOW_linkDS2480B::setRS232BaudRate_cmd( const RBR_val_t inRBR, const RXPOL_val_t inRXPOL)
{
  // locking done in publicly accessible methods

  writeConfigValue( RS232BaudRate_cfgCmd, ((inRXPOL&0x01)<<2) | (inRBR&0x03) );
}

LOW_linkDS2480B::RBR_val_t LOW_linkDS2480B::getRS232BaudRate_cmd()
{
  // locking done in publicly accessible methods

  return static_cast<RBR_val_t>(readConfigValue( RS232BaudRate_cfgCmd) & 0x03);
}

LOW_linkDS2480B::RXPOL_val_t LOW_linkDS2480B::getRS232RxPol_cmd()
{
  // locking done in publicly accessible methods

  return static_cast<RXPOL_val_t>((readConfigValue( RS232BaudRate_cfgCmd)>>2) & 0x01);
}



//=====================================================================================
//
// internal methods
//

void LOW_linkDS2480B::writeConfigValue( const uint8_t inParamCode, const uint8_t inParamValue)
{
  // locking done in publicly accessible methods

  uint8_t outByte       = (inParamCode&0x07)<<4 | (inParamValue&0x07)<<1 | 0x01;
  uint8_t expectedReply = outByte & ~0x01;
  
  setMode( command_mode);

  serialPort->tty_write( outByte);
  if ( serialPort->tty_readByte() != expectedReply )
    throw comm_error( "Unexpected reply while setting config value", __FILE__, __LINE__);
}
  
  
uint8_t LOW_linkDS2480B::readConfigValue( const uint8_t inParamCode)
{
  // locking done in publicly accessible methods

  uint8_t outByte = (inParamCode&0x07)<<1 | 0x01;
  
  setMode( command_mode);
  
  serialPort->tty_write( outByte);
  
  uint8_t reply = serialPort->tty_readByte();
  if ( (reply&0x01)!=0x00 || (reply&0x70)!=0x00 )
    throw comm_error( "Unexpected reply while getting config value", __FILE__, __LINE__);

  return (reply>>1)&0x07;
}


void LOW_linkDS2480B::setMode( const internalMode_t inMode)
{
  // locking done in publicly accessible methods
	STACK
  if ( internalMode == inMode )
    return;

  if      ( internalMode==command_mode && inMode==data_mode ) {
    serialPort->tty_write( SwitchToDataMode_Cmd);
    internalMode = inMode;
  }
  else if ( internalMode==data_mode && inMode==command_mode ) {
    serialPort->tty_write( SwitchToCommandMode_Cmd);
    internalMode = inMode;
  }
  else {
    throw internal_error( "Illegal mode transition detected", __FILE__, __LINE__);
  }
}




//=====================================================================================
//
// Enumeration type conversion methods
//

const LOW_linkDS2480B::SPUD_val_t LOW_linkDS2480B::strongPullup_2_SPUD_val( const strongPullup_t inStrongPullup) const
{
  // fully const, no locking necessary

  switch( inStrongPullup ) {
    case pullUp_16_4:  return SPUD_16_4;
    case pullUp_65_5:  return SPUD_65_5;
    case pullUp_131:   return SPUD_131;
    case pullUp_262:   return SPUD_262;
    case pullUp_524:   return SPUD_524;
    case pullUp_1048:  return SPUD_1048;
    default:           throw internal_error( "Unknown strongPullup_t value detected", __FILE__, __LINE__);
  }
}


const LOW_linkDS2480B::PPD_val_t LOW_linkDS2480B::progPulse_2_PPD_val( const progPulse_t inProgPulse) const
{
  // fully const, no locking necessary

  switch( inProgPulse ) {
    case progPulse_32:    return PPD_32;
    case progPulse_64:    return PPD_64;
    case progPulse_128:   return PPD_128;
    case progPulse_256:   return PPD_256;
    case progPulse_512:   return PPD_512;
    case progPulse_1024:  return PPD_1024;
    case progPulse_2048:  return PPD_2048;
    default:              throw internal_error( "Unknown progPulse_t value detected", __FILE__, __LINE__);
  }
}


const LOW_linkDS2480B::pdSlewRate_t LOW_linkDS2480B::PDSR_val_2_pdSlewRate( const PDSR_val_t inPDSR_val) const
{
  // fully const, no locking necessary

  switch( inPDSR_val ) {
    case PDSR_15:   return pdSlewRate_15;
    case PDSR_2_2:  return pdSlewRate_2_2;
    case PDSR_1_65: return pdSlewRate_1_65;
    case PDSR_1_37: return pdSlewRate_1_37;
    case PDSR_1_1:  return pdSlewRate_1_1;
    case PDSR_0_83: return pdSlewRate_0_83;
    case PDSR_0_7:  return pdSlewRate_0_7;
    case PDSR_0_55: return pdSlewRate_0_55;
    default:           throw internal_error( "Unknown PDSR_val_t value detected", __FILE__, __LINE__);
  }
}

const LOW_linkDS2480B::PDSR_val_t LOW_linkDS2480B::pdSlewRate_2_PDSR_val( const pdSlewRate_t inPdSlewRate_t) const
{
  // fully const, no locking necessary

  switch( inPdSlewRate_t ) {
    case pdSlewRate_15:    return PDSR_15;
    case pdSlewRate_2_2:   return PDSR_2_2;
    case pdSlewRate_1_65:  return PDSR_1_65;
    case pdSlewRate_1_37:  return PDSR_1_37;
    case pdSlewRate_1_1:   return PDSR_1_1;
    case pdSlewRate_0_83:  return PDSR_0_83;
    case pdSlewRate_0_7:   return PDSR_0_7;
    case pdSlewRate_0_55:  return PDSR_0_55;
    default:           throw internal_error( "Unknown pdSlewRate_t value detected", __FILE__, __LINE__);
  }
}


const LOW_linkDS2480B::w1LowTime_t LOW_linkDS2480B::W1LT_val_2_w1LowTime( const W1LT_val_t inW1LT_val) const
{
  // fully const, no locking necessary

  switch( inW1LT_val ) {
    case W1LT_8:  return w1LowTime_8;
    case W1LT_9:  return w1LowTime_9;
    case W1LT_10: return w1LowTime_10;
    case W1LT_11: return w1LowTime_11;
    case W1LT_12: return w1LowTime_12;
    case W1LT_13: return w1LowTime_13;
    case W1LT_14: return w1LowTime_14;
    case W1LT_15: return w1LowTime_15;
    default:      throw internal_error( "Unknown W1LT_val_t value detected", __FILE__, __LINE__);
  }
}

const LOW_linkDS2480B::W1LT_val_t LOW_linkDS2480B::w1LowTime_2_W1LT_val( const w1LowTime_t inW1LowTime) const
{
  // fully const, no locking necessary

  switch( inW1LowTime ) {
    case w1LowTime_8:  return W1LT_8;
    case w1LowTime_9:  return W1LT_9;
    case w1LowTime_10: return W1LT_10;
    case w1LowTime_11: return W1LT_11;
    case w1LowTime_12: return W1LT_12;
    case w1LowTime_13: return W1LT_13;
    case w1LowTime_14: return W1LT_14;
    case w1LowTime_15: return W1LT_15;
    default:           throw internal_error( "Unknown w1LowTime_t value detected", __FILE__, __LINE__);
  }
}


const LOW_linkDS2480B::soW0RecTime_t LOW_linkDS2480B::SOW0RT_val_2_soW0RecTime( const SOW0RT_val_t inSOW0RT_val) const
{
  // fully const, no locking necessary

  switch( inSOW0RT_val ) {
    case SOW0RT_3:  return soW0RecTime_3;
    case SOW0RT_4:  return soW0RecTime_4;
    case SOW0RT_5:  return soW0RecTime_5;
    case SOW0RT_6:  return soW0RecTime_6;
    case SOW0RT_7:  return soW0RecTime_7;
    case SOW0RT_8:  return soW0RecTime_8;
    case SOW0RT_9:  return soW0RecTime_9;
    case SOW0RT_10: return soW0RecTime_10;
    default:        throw internal_error( "Unknown SOW0RT_val_t value detected", __FILE__, __LINE__);
  }
}

const LOW_linkDS2480B::SOW0RT_val_t LOW_linkDS2480B::soW0RecTime_2_SOW0RT_val( const soW0RecTime_t inSoW0RecTime) const
{
  // fully const, no locking necessary

  switch( inSoW0RecTime ) {
    case soW0RecTime_3:  return SOW0RT_3;
    case soW0RecTime_4:  return SOW0RT_4;
    case soW0RecTime_5:  return SOW0RT_5;
    case soW0RecTime_6:  return SOW0RT_6;
    case soW0RecTime_7:  return SOW0RT_7;
    case soW0RecTime_8:  return SOW0RT_8;
    case soW0RecTime_9:  return SOW0RT_9;
    case soW0RecTime_10: return SOW0RT_10;
    default:             throw internal_error( "Unknown soW0RecTime_t value detected", __FILE__, __LINE__);
  }
}


const LOW_linkDS2480B::wireSpeed_t LOW_linkDS2480B::OWSPEED_val_2_wireSpeed( const OWSPEED_val_t inOWSPEED_val) const
{
  // fully const, no locking necessary

  switch( inOWSPEED_val ) {
    case normal_OWSPEED:    return normal_speed;
    case flexible_OWSPEED:  return flexible_speed;
    case overdrive_OWSPEED: return overdrive_speed;
    default:                throw internal_error( "Unknown OWSPEED_val_t value detected", __FILE__, __LINE__);
  }
}

const LOW_linkDS2480B::OWSPEED_val_t LOW_linkDS2480B::wireSpeed_2_OWSPEED_val( const wireSpeed_t inWireSpeed) const
{
  // fully const, no locking necessary

  switch( inWireSpeed ) {
    case normal_speed:    return normal_OWSPEED;
    case flexible_speed:  return flexible_OWSPEED;
    case overdrive_speed: return overdrive_OWSPEED;
    default:              throw internal_error( "Unknown wireSpeed_t value detected", __FILE__, __LINE__);
  }
}

