/***************************************************************************
                          LOW_linkDS2490.cpp  -  description
                             -------------------
    begin                : Sun Oct 12 2003
    copyright            : (C) 2003 by Harald Roelle
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


#include "LOW_linkDS2490.h"
#include "LOW_device.h"
#include "PoppyDebugTools.h"


//=====================================================================================
//
// constructors
//

LOW_linkDS2490::LOW_linkDS2490( const LOW_portUsb_Factory::usbDeviceSpecifier_t inUsbDevSpec,
                                const bool inHasExternalPower, const bool inAllowProgPulse) :
  LOW_link( true, inHasExternalPower, inAllowProgPulse),
  LOW_linkFlexibleSpeed( normal_speed, pdSlewRate_1_37, w1LowTime_11, soW0RecTime_10) // use normal speed, but preconfigure the recommeded optimal parameters as of app note #148
{
  usbDevice = LOW_portUsb_Factory::new_portUsbDevice( inUsbDevSpec);

  if ( usbDevice->getVendorID() != usbVendorID || usbDevice->getProductID() != usbProductID )
    throw link_error( "Requested device has wrong vendor/product ID", __FILE__, __LINE__);

  commonConstructorActions();
}


LOW_linkDS2490::~LOW_linkDS2490()
{
  ctlCmd_resetDevice();
  usbDevice->releaseInterface( usbDefaultInterface);
  delete usbDevice;
}


//=====================================================================================
//
// Standard methods required by LOW_link
//

//-------------------------------------------------------------------------------------
//
// Bus touch (write/read) methods required by LOW_link
//

bool LOW_linkDS2490::touchBit( const bool inSendBit, const strongPullup_t inPullup)
{
	STACK
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "touchBit() called\n");

  commLock lock( *this);

  if ( inPullup != pullUp_NONE ) {
    comCmd_setDuration( strongPullup_2_spFactor( inPullup), // inTimeFactor
                        false,                              // inSpecifyProgPulse
                        resultOnError_rsltHdl,              // inResultHandling
                        true);                              // inImmediateExec
  }

  comCmd_bitIO( inSendBit,                        // inWriteBit
                inPullup==pullUp_NONE?false:true, // inDoStrongPullup
                false,                            // inSuppressPullupOnRead1
                resultOnError_rsltHdl,            // inResultHandling, resultOnError_rsltHdl is necessary to get response byte
                false);                           // inImmediateExec

  ctlCmd_startExecution();

  deviceFeedback_t  deviceFeedback;
  resultCodeVec_t   resultCodeVec;
  waitUntilIdle( deviceFeedback, resultCodeVec, 1000);

  if ( deviceFeedback.dataInBufferUsage != 1 )
    throw comm_error( "Unexpected data amount in IN buffer", __FILE__, __LINE__);

  uint8_t      readByte;
  unsigned int readSize = usbDevice->bulkRead( usbDataInEP, 1, &readByte, 1000);
  if ( readSize != 1 )
    throw comm_error( "Short read from IN buffer", __FILE__, __LINE__);
  
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "touchBit() leaving\n");

  return (readByte&0x01)==0x01;
}


uint8_t LOW_linkDS2490::touchByte( const uint8_t inSendByte, const strongPullup_t inPullup)
{
	STACK
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "touchByte() called (inSendByte: %d, inPullup: %d)\n", inSendByte, inPullup);

  commLock lock( *this);

  if ( inPullup != pullUp_NONE ) {
    comCmd_setDuration( strongPullup_2_spFactor( inPullup), // inTimeFactor
                        false,                              // inSpecifyProgPulse
                        resultOnError_rsltHdl,              // inResultHandling
                        true);                              // inImmediateExec
  }

  comCmd_byteIO( inSendByte,                       // inWriteByte
                 inPullup==pullUp_NONE?false:true, // inDoStrongPullup
                 resultOnError_rsltHdl,            // inResultHandling, resultOnError_rsltHdl is necessary to get response byte
                 false);                           // inImmediateExec

  ctlCmd_startExecution();

  deviceFeedback_t  deviceFeedback;
  resultCodeVec_t   resultCodeVec;
  waitUntilIdle( deviceFeedback, resultCodeVec, 1000);

  if ( deviceFeedback.dataInBufferUsage != 1 )
    throw comm_error( "Unexpected data amount in IN buffer", __FILE__, __LINE__);

  uint8_t      readByte;
  unsigned int readSize = usbDevice->bulkRead( usbDataInEP, 1, &readByte, 1000);
  if ( readSize != 1 )
    throw comm_error( "Short read from IN buffer", __FILE__, __LINE__);

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "touchByte() leaving\n");

  return readByte;
}


byteVec_t LOW_linkDS2490::touchBlock( const byteVec_t &inBytes, const strongPullup_t inPullup)
{
	STACK
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "touchBlock() called\n");

  commLock lock( *this);
  
  const unsigned int  maxBlockSize = (usbDataOutEP_FIFOsize < usbDataInEP_FIFOsize) ? usbDataOutEP_FIFOsize-1 : usbDataInEP_FIFOsize-1;
  const unsigned int  total        = inBytes.size();
  uint8_t             *writeBuffer = new uint8_t[total];
  uint8_t             *readBuffer  = new uint8_t[maxBlockSize];
  byteVec_t           retVec       = byteVec_t( total);
  
  if ( inPullup != pullUp_NONE ) {
    comCmd_setDuration( strongPullup_2_spFactor( inPullup), // inTimeFactor
                        false,                              // inSpecifyProgPulse
                        resultOnError_rsltHdl,              // inResultHandling
                        true);                              // inImmediateExec
  }

  try {
    std::copy( inBytes.begin(), inBytes.end(), writeBuffer);
    
    unsigned int  written   = 0;
    unsigned int  remaining = total;
    unsigned int  totalRead = 0;
    unsigned int  read      = 0;
    uint8_t       *writePtr = writeBuffer;
    do {
      const unsigned int blockSize = (remaining < maxBlockSize) ? remaining : maxBlockSize;
      
      written = usbDevice->bulkWrite( usbDataOutEP, blockSize, writePtr, 1000);
      remaining -= written;
      writePtr  += written;
    
      comCmd_blockIO( written,                                 // inWriteSize
                      false,                                   // inBusResetBefore
                      (remaining==0) ? (inPullup==pullUp_NONE ? false : true) : false, // inDoStrongPullup
                      resultOnError_rsltHdl,                   // inResultHandling,
                      false);                                  // inImmediateExec
   
      ctlCmd_startExecution();

      deviceFeedback_t  deviceFeedback;
      resultCodeVec_t   resultCodeVec;
      waitUntilIdle( deviceFeedback, resultCodeVec, 1000);

      read = usbDevice->bulkRead( usbDataInEP, written, readBuffer, 1000);

      if ( read != written )
        throw comm_error( "Short read from IN buffer", __FILE__, __LINE__);
      std::copy( readBuffer, readBuffer+read, retVec.begin()+totalRead);
      totalRead += read;
    }
    while ( remaining != 0);
  }
  catch( ... ) {
    delete[] writeBuffer;
    delete[] readBuffer;
    throw;
  }
  
  delete[] writeBuffer;
  delete[] readBuffer;
  
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "touchBlock() leaving\n");

  return retVec;
  
  
  byteVec_t retBytes = byteVec_t( inBytes.size());
  for( unsigned int a=0; a<inBytes.size()-1; ++a) {
    retBytes[a] = touchByte( inBytes[a], pullUp_NONE);
  }
  retBytes[inBytes.size()-1] = touchByte( inBytes[inBytes.size()-1], inPullup);

  return retBytes;
}


//-------------------------------------------------------------------------------------
//
// Higher level methods by LOW_link
  
LOW_deviceID::deviceIDVec_t LOW_linkDS2490::searchDevices( const bool inOnlyAlarm, const LOW_deviceIDRaw inPreload,
                                                           const LOW_deviceIDRaw::devFamCode_t inFamCode, const bool inDoReset)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "searchDevices() called\n");

  commLock lock( *this);

  const uint8_t                maxDevs = usbDataInEP_FIFOsize/sizeof( LOW_deviceIDRaw::devRomID_t) - 1;  // FIFO_size / ID_size - place_for_discr_vec
  LOW_deviceID::deviceIDVec_t  returnIDs;

  LOW_deviceIDRaw preloadVec = inPreload;

  // preload family type
  if ( inFamCode != LOW_device::anyDev_famCode )
    preloadVec.setFamilyCode( inFamCode);

  if ( resetBus() == false )
    return returnIDs;

  bool searchRunning = true;
  while ( searchRunning ) {

    unsigned int writeSize = usbDevice->bulkWrite( usbDataOutEP, preloadVec.getRomIDVec(), 1000);
    if ( writeSize != preloadVec.getRomIDVec().size() )
      throw comm_error( "Short write to OUT buffer", __FILE__, __LINE__);

    comCmd_searchAccess( maxDevs,              // inMaxDevNum
                         inOnlyAlarm ? LOW_device::SearchAlarmROM_COMMAND : LOW_device::SearchROM_COMMAND, // inSearchCommand
                         true,                 // inSearchWithoutFullAccess
                         true,                 // inReturnDiscrepancyInfo
                         true,                 // inBusResetBefore
                         true,                 // inFlushBuffersOnErr
                         resultAlways_rsltHdl, // inResultHandling
                         false);               // inImmediateExec

    ctlCmd_startExecution();

    deviceFeedback_t  deviceFeedback;
    resultCodeVec_t   resultCodeVec;
    waitUntilIdle( deviceFeedback, resultCodeVec, 30000);

    if ( (deviceFeedback.dataInBufferUsage % sizeof( LOW_deviceIDRaw::devRomID_t)) != 0 )
      throw comm_error( "Illegal byte count in IN buffer", __FILE__, __LINE__);

    unsigned int rawDevCnt = deviceFeedback.dataInBufferUsage / sizeof( LOW_deviceIDRaw::devRomID_t);
    LOW_deviceIDRaw::devRomID_t *readRawIDs = new LOW_deviceIDRaw::devRomID_t[rawDevCnt];

    try {
      unsigned int readSize = usbDevice->bulkRead( usbDataInEP, deviceFeedback.dataInBufferUsage, reinterpret_cast<LOW_portUsbDevice::msgData_t>(readRawIDs), 1000);
      if ( readSize != deviceFeedback.dataInBufferUsage )
        throw comm_error( "Short read from IN buffer", __FILE__, __LINE__);

      unsigned int realIDCnt = rawDevCnt<=maxDevs ? rawDevCnt : maxDevs;
      for( unsigned int a=0; a<realIDCnt; ++a) {
        LOW_deviceID newID = LOW_deviceID( readRawIDs[a]);
        if ( inFamCode!=LOW_device::anyDev_famCode && newID.getFamilyCode()!=inFamCode ) {
          searchRunning = false;
          break;
        }
        else
          returnIDs.push_back( newID);
      }

      if ( rawDevCnt <= maxDevs )
        searchRunning = false;
      else
        preloadVec = readRawIDs[rawDevCnt-1];
    }
    catch ( ... ) {
      delete[] readRawIDs;
      throw;
    }

    delete[] readRawIDs;
  }

  if ( inDoReset )
    resetBus();

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "searchDevices() leaving (returning %d IDs)\n", returnIDs.size());
  return returnIDs;
}


                                                     
//-------------------------------------------------------------------------------------
//
// Misc methods required by LOW_link
//

void LOW_linkDS2490::resetLinkAdapter()
{
	STACK
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetLinkAdapter() called\n");


  commLock lock( *this);

  // reset device
  ctlCmd_resetDevice();
  
  modCmd_setEnablePulse( true, allowProgPulse);
  modCmd_setEnableSpeedChange( false);
  
  // restore former settings
  setWireSpeed( wireSpeed);
  if ( wireSpeed == flexible_speed ) {
    setPullDownSlewRate( pdSlewRate);
    setWrite1LowTime( w1LowTime);
    setSampleOffsetWrite0Rec( soW0RecTime);
  }
  
  deviceFeedback_t devFeedback;
  resultCodeVec_t  resultCodeVec;
  readDeviceStatus( devFeedback, resultCodeVec);
  hasProgramPulse = devFeedback.progVoltagePresent;

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetLinkAdapter() leaving\n");
}


bool LOW_linkDS2490::resetBus()
{
	STACK
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus() called\n");

  commLock lock( *this);
  
  comCmd_oneWireReset( false,                // inLoopUntilPresenceDetect
                       false,                // inEnableSpeedChange
                       normal_OWSPEED,       // inSpeedSelector
                       true,                 // inFlushBuffersOnErr
                       resultAlways_rsltHdl, // inResultHandling
                       false);               // inImmediateExec
  ctlCmd_startExecution();
  
  deviceFeedback_t  deviceFeedback;
  resultCodeVec_t   resultCodeVec;
  waitUntilIdle( deviceFeedback, resultCodeVec, 1000);

//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): isStrongPullupEnabled       = %d\n", deviceFeedback.isStrongPullupEnabled);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): isProgPulseEnabled          = %d\n", deviceFeedback.isProgPulseEnabled);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): isDynSpeedChangeEnabled     = %d\n", deviceFeedback.isDynSpeedChangeEnabled);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): oneWireSpeed;               = %d\n", deviceFeedback.oneWireSpeed);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): strongPullupDurationFactor; = %d\n", deviceFeedback.strongPullupDurationFactor);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): progPulseDurationFactor;    = %d\n", deviceFeedback.progPulseDurationFactor);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): pulldownSlewRate;           = %d\n", deviceFeedback.pulldownSlewRate);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): write1LowTime;              = %d\n", deviceFeedback.write1LowTime);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): dsow0RecTime;               = %d\n", deviceFeedback.dsow0RecTime);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): strongPullupCurrentActive;  = %d\n", deviceFeedback.strongPullupCurrentActive);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): progPulseCurrentActive;     = %d\n", deviceFeedback.progPulseCurrentActive);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): progVoltagePresent;         = %d\n", deviceFeedback.progVoltagePresent);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): deviceExternalPowered;      = %d\n", deviceFeedback.deviceExternalPowered);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): deviceHalted;               = %d\n", deviceFeedback.deviceHalted);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): deviceIdle;                 = %d\n", deviceFeedback.deviceIdle);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): ep0FifoOverflowError;       = %d\n", deviceFeedback.ep0FifoOverflowError);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): currentCommCmd;             = %d\n", deviceFeedback.currentCommCmd);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): commCmdBufferUsage;         = %d\n", deviceFeedback.commCmdBufferUsage);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): dataOutBufferUsage;         = %d\n", deviceFeedback.dataOutBufferUsage);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): dataInBufferUsage;          = %d\n", deviceFeedback.dataInBufferUsage);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): resultCount;                = %d\n", deviceFeedback.resultCount);
//  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus(): resultCodeVec.size()        = %d\n", resultCodeVec.size());
  
  if ( resultCodeVec.size() == 0)
    throw comm_error( "Expected answer not found", __FILE__, __LINE__);

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "resetBus() leaving\n");

    /** @todo Check if and how "alarming presence pulse" response works. */
  if ( resultCodeVec.back().alarmingPresencePulse )  return true;
  if ( resultCodeVec.back().noPresencePulse )        return false;
  if ( resultCodeVec.back().shortToGround )          throw comm_error( "Short to ground detected", __FILE__, __LINE__);

  return true;
}


void LOW_linkDS2490::strongPullup( const unsigned long inMilliSecs)
{
	STACK
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "strongPullup(const unsigned long) called\n");

  commLock lock( *this);

  if ( inMilliSecs>=16 && inMilliSecs<=4064 && (inMilliSecs%16)==0 ) {  // 16ms <= length <= 4096ms AND dividable by 16ms
    strongPullupInternal( inMilliSecs/16);
    LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "strongPullup(const unsigned long) leaving\n");
    return;
  }

  comCmd_setDuration( 0x00,                  // inTimeFactor = infinite
                      false,                 // inSpecifyProgPulse
                      resultOnError_rsltHdl, // inResultHandling
                      true);                 // inImmediateExec

  comCmd_pulse( false,                  // inSpecifyProgPulse
                true,                   // inFlushBuffersOnErr,
                resultOnError_rsltHdl,  // inResultHandling
                true);                  // inImmediateExec

  LOW_platformMisc::milliSleep( inMilliSecs);
  ctlCmd_haltExecutionWhenDone();  // stop the pullup
  ctlCmd_resumeExecution();

  deviceFeedback_t  deviceFeedback;
  resultCodeVec_t   resultCodeVec;
  readDeviceStatus( deviceFeedback, resultCodeVec);

  if ( resultCodeVec.size() != 0)
    throw comm_error( "Unexpected error reply found", __FILE__, __LINE__);

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "strongPullup(const unsigned long) leaving\n");
}


void LOW_linkDS2490::strongPullup( const strongPullup_t inPullupTimes)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "strongPullup(const strongPullup_t) called\n");

  commLock lock( *this);

  strongPullupInternal( strongPullup_2_spFactor( inPullupTimes));

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "strongPullup(const strongPullup_t) leaving\n");
}


void LOW_linkDS2490::programPulse( const unsigned long inMicroSecs)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "programPulse(const unsigned long) called\n");

  commLock lock( *this);

  if ( ! allowProgPulse )
    throw notAllowed_error( "Program pulse not allowed", __FILE__, __LINE__);

  if ( ! hasProgramPulse )
    throw illegalLevel_error( "Program pulse not available", __FILE__, __LINE__);

  if ( inMicroSecs>=8 && inMicroSecs<=2032 && (inMicroSecs%8)==0 ) { // 8us <= length <= 2032us AND dividable by 8us
    progPulseInternal( inMicroSecs/8);
    LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "programPulse(const unsigned long) leaving\n");
    return;
  }

  comCmd_setDuration( 0x00,                  // inTimeFactor = infinite
                      true,                  // inSpecifyProgPulse
                      resultOnError_rsltHdl, // inResultHandling
                      true);                 // inImmediateExec

  comCmd_pulse( true,                  // inSpecifyProgPulse
                true,                  // inFlushBuffersOnErr,
                resultAlways_rsltHdl,  // inResultHandling
                true);                 // inImmediateExec

  LOW_platformMisc::microSleep( inMicroSecs);;
  ctlCmd_haltExecutionWhenDone();  // stop the pulse
  ctlCmd_resumeExecution();

  deviceFeedback_t  deviceFeedback;
  resultCodeVec_t   resultCodeVec;
  readDeviceStatus( deviceFeedback, resultCodeVec);

  if ( resultCodeVec.size() == 0)
    throw comm_error( "Expected answer not found", __FILE__, __LINE__);

  if ( resultCodeVec.back().progVoltageMissingOnBus )
    throw comm_error( "12V program pulse not detected on bus", __FILE__, __LINE__);

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "programPulse(const unsigned long) leaving\n");
}


void LOW_linkDS2490::programPulse( const progPulse_t inPulseTime)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "programPulse(const progPulse_tg) called\n");

  commLock lock( *this);

  if ( ! allowProgPulse )
    throw notAllowed_error( "Program pulse not allowed", __FILE__, __LINE__);

  if ( ! hasProgramPulse )
    throw illegalLevel_error( "Program pulse not available", __FILE__, __LINE__);

  progPulseInternal( progPulse_2_ppFactor( inPulseTime));

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "programPulse(const progPulse_tg) leaving\n");
}


void LOW_linkDS2490::doSearchSequence( const LOW_deviceIDRaw& /*inBranchVector*/,
                                       LOW_deviceIDRaw& /*outFoundID*/, LOW_deviceIDRaw& /*outDiscrVec*/)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "doSearchSequence() called\n");

  throw internal_error( "doSearchSequence() not implemented. Should never be called as searchDevices() is overloaded.", __FILE__, __LINE__);
} 



//=====================================================================================
//
// Standard methods required by LOW_linkFlexibleSpeed
//

void LOW_linkDS2490::setWireSpeed( const wireSpeed_t inWireSpeed)
{
	STACK
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "setWireSpeed() called\n");

  commLock lock( *this);

  LOW_linkFlexibleSpeed::setWireSpeed( inWireSpeed); // conserve the setting via superclass

  modCmd_setOneWireSpeed( wireSpeed_2_OWSPEED_val( inWireSpeed));

  // restore former settings
  if ( wireSpeed == flexible_speed ) {
    setPullDownSlewRate( pdSlewRate);
    setWrite1LowTime( w1LowTime);
    setSampleOffsetWrite0Rec( soW0RecTime);
  }

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "setWireSpeed() leaving\n");
}


LOW_linkDS2490::wireSpeed_t LOW_linkDS2490::getWireSpeed()
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "getWireSpeed() called\n");

  commLock lock( *this);

  deviceFeedback_t devFeedback;
  resultCodeVec_t  resultCodeVec;
  readDeviceStatus( devFeedback, resultCodeVec);

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "getWireSpeed() leaving\n");
  return OWSPEED_val_2_wireSpeed( devFeedback.oneWireSpeed);
}


void LOW_linkDS2490::setPullDownSlewRate( const pdSlewRate_t inPDSR)
{
	STACK
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "setPullDownSlewRate() called\n");

  commLock lock( *this);

  LOW_linkFlexibleSpeed::setPullDownSlewRate( inPDSR);  // conserve the setting via superclass

  modCmd_setPulldownSlewRate( pdSlewRate_2_PDSR_val( inPDSR));

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "setPullDownSlewRate() leaving\n");
}


LOW_linkDS2490::pdSlewRate_t LOW_linkDS2490::getPullDownSlewRate()
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "getPullDownSlewRate() called\n");

  commLock lock( *this);

  LOW_linkFlexibleSpeed::getPullDownSlewRate();  // sanity checking in superclass

  deviceFeedback_t devFeedback;
  resultCodeVec_t  resultCodeVec;
  readDeviceStatus( devFeedback, resultCodeVec);

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "getPullDownSlewRate() leaving\n");
  return PDSR_val_2_pdSlewRate( devFeedback.pulldownSlewRate);
}


void LOW_linkDS2490::setWrite1LowTime( const w1LowTime_t inW1LT)
{
	STACK
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "setWrite1LowTime() called\n");

  commLock lock( *this);

  LOW_linkFlexibleSpeed::setWrite1LowTime( inW1LT);  // conserve the setting via superclass

  modCmd_setWrite1LowTime( w1LowTime_2_W1LT_val( inW1LT));

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "setWrite1LowTime() leaving\n");
}


LOW_linkDS2490::w1LowTime_t LOW_linkDS2490::getWrite1LowTime()
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "getWrite1LowTime() called\n");

  commLock lock( *this);

  LOW_linkFlexibleSpeed::getWrite1LowTime();  // sanity checking in superclass

  deviceFeedback_t devFeedback;
  resultCodeVec_t  resultCodeVec;
  readDeviceStatus( devFeedback, resultCodeVec);

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "getWrite1LowTime() leaving\n");
  return W1LT_val_2_w1LowTime( devFeedback.write1LowTime);
}


void LOW_linkDS2490::setSampleOffsetWrite0Rec( const soW0RecTime_t inSOW0RT)
{
	STACK
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "setSampleOffsetWrite0Rec() called\n");

  commLock lock( *this);

  LOW_linkFlexibleSpeed::setSampleOffsetWrite0Rec( inSOW0RT);  // conserve the setting via superclass

  modCmd_setDsoW0RecoveryTime( soW0RecTime_2_SOW0RT_val( inSOW0RT));

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "setSampleOffsetWrite0Rec() leaving\n");
}


LOW_linkDS2490::soW0RecTime_t LOW_linkDS2490::getSampleOffsetWrite0Rec()
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "getSampleOffsetWrite0Rec() called\n");

  commLock lock( *this);

  LOW_linkFlexibleSpeed::getSampleOffsetWrite0Rec();  // sanity checking in superclass

  deviceFeedback_t devFeedback;
  resultCodeVec_t  resultCodeVec;
  readDeviceStatus( devFeedback, resultCodeVec);

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "getSampleOffsetWrite0Rec() leaving\n");
  return SOW0RT_val_2_soW0RecTime( devFeedback.dsow0RecTime);
}




//=====================================================================================
//
// private methods
//


//-------------------------------------------------------------------------------------
//
// Mid-layer methods.
//

void LOW_linkDS2490::commonConstructorActions()
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "commonConstructorActions() called\n");

  // locking done in publicly accessible methods

  usbDevice->claimInterface( usbDefaultInterface);
  
  resetLinkAdapter();

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "commonConstructorActions() leaving\n");
}


void LOW_linkDS2490::readDeviceStatus( deviceFeedback_t &outDevFeedback, resultCodeVec_t &outResultCodeVec)
{
	STACK
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "readDeviceStatus() called\n");

  // locking done in publicly accessible methods

  deviceFeedbackRaw_t  feedbackData;

  unsigned int readBytes = usbDevice->bulkRead( usbStatusInEP, sizeof( feedbackData),
                                                reinterpret_cast<LOW_portUsbDevice::msgData_t>(&feedbackData), 500);
  if ( readBytes < 16 )
    throw comm_error( "Short read from status endpoint", __FILE__, __LINE__);

  // analyze standard data
  outDevFeedback.isStrongPullupEnabled      = feedbackData.enableFlags & SPUE_bitmask;
  outDevFeedback.isProgPulseEnabled         = feedbackData.enableFlags & PRGE_bitmask;
  outDevFeedback.isDynSpeedChangeEnabled    = feedbackData.enableFlags & SPCE_bitmask;
  outDevFeedback.oneWireSpeed               = static_cast<OWSPEED_val_t>(feedbackData.oneWireSpeed);
  outDevFeedback.strongPullupDurationFactor = feedbackData.strongPullupDuration;
  outDevFeedback.progPulseDurationFactor    = feedbackData.progPulseDuration;
  outDevFeedback.pulldownSlewRate           = static_cast<PDSR_val_t>(feedbackData.pulldownSlewRate);
  outDevFeedback.write1LowTime              = static_cast<W1LT_val_t>(feedbackData.write1LowTime);
  outDevFeedback.dsow0RecTime               = static_cast<SOW0RT_val_t>(feedbackData.dsow0RecTime);
  outDevFeedback.strongPullupCurrentActive  = feedbackData.deviceStatusFlags & SPUA_bitmask;
  outDevFeedback.progPulseCurrentActive     = feedbackData.deviceStatusFlags & PRGA_bitmask;
  outDevFeedback.progVoltagePresent         = feedbackData.deviceStatusFlags & VP12_bitmask;
  outDevFeedback.deviceExternalPowered      = feedbackData.deviceStatusFlags & PMOD_bitmask;
  outDevFeedback.deviceHalted               = feedbackData.deviceStatusFlags & HALT_bitmask;
  outDevFeedback.deviceIdle                 = feedbackData.deviceStatusFlags & IDLE_bitmask;
  outDevFeedback.ep0FifoOverflowError       = feedbackData.deviceStatusFlags & EP0F_bitmask;
  outDevFeedback.currentCommCmd             = (feedbackData.commCmdHI<<8) | feedbackData.commCmdLO;
  outDevFeedback.commCmdBufferUsage         = feedbackData.commCmdBufferStatus;
  outDevFeedback.dataOutBufferUsage         = feedbackData.dataOutBufferStatus;
  outDevFeedback.dataInBufferUsage          = feedbackData.dataInBufferStatus;
  outDevFeedback.resultCount                = readBytes - 16;

//  printf( "isStrongPullupEnabled: %d\n", outDevFeedback.isStrongPullupEnabled);
//  printf( "isProgPulseEnabled: %d\n", outDevFeedback.isProgPulseEnabled);
//  printf( "isDynSpeedChangeEnabled: %d\n", outDevFeedback.isDynSpeedChangeEnabled);
//  printf( "oneWireSpeed: %d\n", outDevFeedback.oneWireSpeed);
//  printf( "strongPullupDurationFactor: %d\n", outDevFeedback.strongPullupDurationFactor);
//  printf( "progPulseDurationFactor: %d\n", outDevFeedback.progPulseDurationFactor);
//  printf( "pulldownSlewRate: %d\n", outDevFeedback.pulldownSlewRate);
//  printf( "write1LowTime: %d\n", outDevFeedback.write1LowTime);
//  printf( "dsow0RecTime: %d\n", outDevFeedback.dsow0RecTime);
//  printf( "strongPullupCurrentActive: %d\n", outDevFeedback.strongPullupCurrentActive);
//  printf( "progPulseCurrentActive: %d\n", outDevFeedback.progPulseCurrentActive);
//  printf( "progVoltagePresent: %d\n", outDevFeedback.progVoltagePresent);
//  printf( "deviceExternalPowered: %d\n", outDevFeedback.deviceExternalPowered);
//  printf( "deviceHalted: %d\n", outDevFeedback.deviceHalted);
//  printf( "deviceIdle: %d\n", outDevFeedback.deviceIdle);
//  printf( "ep0FifoOverflowError: %d\n", outDevFeedback.ep0FifoOverflowError);
//  printf( "currentCommCmd: %d\n", outDevFeedback.currentCommCmd);
//  printf( "commCmdBufferUsage: %d\n", outDevFeedback.commCmdBufferUsage);
//  printf( "dataOutBufferUsage: %d\n", outDevFeedback.dataOutBufferUsage);
//  printf( "dataInBufferUsage: %d\n", outDevFeedback.dataInBufferUsage);
//  printf( "resultCount: %d\n", outDevFeedback.resultCount);

  outDevFeedback.compoundResults.deviceDetected             = false;
  outDevFeedback.compoundResults.searchAccessDeviceUnderrun = false;
  outDevFeedback.compoundResults.pageIsRedirected           = false;
  outDevFeedback.compoundResults.crcError                   = false;
  outDevFeedback.compoundResults.compareError               = false;
  outDevFeedback.compoundResults.progVoltageMissingOnBus    = false;
  outDevFeedback.compoundResults.alarmingPresencePulse      = false;
  outDevFeedback.compoundResults.shortToGround              = false;
  outDevFeedback.compoundResults.noPresencePulse            = false;
  
  // analyze extra result codes and calc compound results
  for( int a=0; a<outDevFeedback.resultCount; a++) {
    resultCode_t theResult = { false, false, false, false, false, false, false, false, false};

    if ( feedbackData.resultCode[a] == devDetect_code ) {
      theResult.deviceDetected             = true;
    }
    else {
      theResult.searchAccessDeviceUnderrun = feedbackData.resultCode[a] & EOS_bitmask;
      theResult.pageIsRedirected           = feedbackData.resultCode[a] & RDP_bitmask;
      theResult.crcError                   = feedbackData.resultCode[a] & CRC_bitmask;
      theResult.compareError               = feedbackData.resultCode[a] & CMP_bitmask;
      theResult.progVoltageMissingOnBus    = feedbackData.resultCode[a] & VPP_bitmask;
      theResult.alarmingPresencePulse      = feedbackData.resultCode[a] & APP_bitmask;
      theResult.shortToGround              = feedbackData.resultCode[a] & SH_bitmask;
      theResult.noPresencePulse            = feedbackData.resultCode[a] & NRS_bitmask;
    }

    outDevFeedback.compoundResults.deviceDetected             |= theResult.deviceDetected;
    outDevFeedback.compoundResults.searchAccessDeviceUnderrun |= theResult.searchAccessDeviceUnderrun;
    outDevFeedback.compoundResults.pageIsRedirected           |= theResult.pageIsRedirected;
    outDevFeedback.compoundResults.crcError                   |= theResult.crcError;
    outDevFeedback.compoundResults.compareError               |= theResult.compareError;
    outDevFeedback.compoundResults.progVoltageMissingOnBus    |= theResult.progVoltageMissingOnBus;
    outDevFeedback.compoundResults.alarmingPresencePulse      |= theResult.alarmingPresencePulse;
    outDevFeedback.compoundResults.shortToGround              |= theResult.shortToGround;
    outDevFeedback.compoundResults.noPresencePulse            |= theResult.noPresencePulse;

    outResultCodeVec.push_back( theResult);
  }

  // ep0 fifo overrun makes the device unusable anyway so we handle it here centrally
  if ( outDevFeedback.ep0FifoOverflowError ) {
    ctlCmd_resetDevice();
    throw comm_error( "EP0 FIFO overrund. Reset command was sent to device.", __FILE__, __LINE__);
  }

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "readDeviceStatus() leaving\n");
}


void LOW_linkDS2490::waitUntilIdle( deviceFeedback_t &outDeviceFeedback, resultCodeVec_t &outResultCodeVec,
                                    const LOW_portUsbDevice::usbTimeout_t inTimeout)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "waitUntilIdle() called\n");

  // locking done in publicly accessible methods

  static const unsigned long sleepTime_ms = 1;

  resultCode_t cmpdResults = { false, false, false, false, false, false, false, false, false};
  
  for ( unsigned int a=0; a<(inTimeout/sleepTime_ms)+1; ++a) {
    deviceFeedback_t  feedbackData;
    readDeviceStatus( feedbackData, outResultCodeVec);
    
    cmpdResults.deviceDetected             |= feedbackData.compoundResults.deviceDetected;
    cmpdResults.searchAccessDeviceUnderrun |= feedbackData.compoundResults.searchAccessDeviceUnderrun;
    cmpdResults.pageIsRedirected           |= feedbackData.compoundResults.pageIsRedirected;
    cmpdResults.crcError                   |= feedbackData.compoundResults.crcError;
    cmpdResults.compareError               |= feedbackData.compoundResults.compareError;
    cmpdResults.progVoltageMissingOnBus    |= feedbackData.compoundResults.progVoltageMissingOnBus;
    cmpdResults.alarmingPresencePulse      |= feedbackData.compoundResults.alarmingPresencePulse;
    cmpdResults.shortToGround              |= feedbackData.compoundResults.shortToGround;
    cmpdResults.noPresencePulse            |= feedbackData.compoundResults.noPresencePulse;
    
    if ( feedbackData.deviceIdle && feedbackData.commCmdBufferUsage==0 ) {
      outDeviceFeedback = feedbackData;
      outDeviceFeedback.compoundResults = cmpdResults;
      LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "waitUntilIdle() leaving\n");
      return;
    }

    LOW_platformMisc::milliSleep( sleepTime_ms);
  }
  throw comm_error( "Timeout waiting for device to become idle.", __FILE__, __LINE__);
}


void LOW_linkDS2490::strongPullupInternal( const unsigned int inPullupFactor)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "strongPullupInternal() called\n");

  // locking done in publicly accessible methods

  comCmd_setDuration( inPullupFactor,          // inTimeFactor
                      false,                   // inSpecifyProgPulse
                      resultOnError_rsltHdl,   // inResultHandling
                      true);                   // inImmediateExec

  comCmd_pulse( false,                 // inSpecifyProgPulse
                true,                  // inFlushBuffersOnErr,
                resultOnError_rsltHdl, // inResultHandling
                true);                 // inImmediateExec

  deviceFeedback_t  deviceFeedback;
  resultCodeVec_t   resultCodeVec;
  waitUntilIdle( deviceFeedback, resultCodeVec, (16*inPullupFactor) + 1000);  // (16ms * factor) + 1s

  if ( resultCodeVec.size() != 0)
    throw comm_error( "Unexpected error reply found", __FILE__, __LINE__);

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "strongPullupInternal() leaving\n");
}


void LOW_linkDS2490::progPulseInternal( const unsigned int inPulseFactor)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "progPulseInternal() called\n");

  // locking done in publicly accessible methods

  comCmd_setDuration( inPulseFactor,           // inTimeFactor
                      true,                    // inSpecifyProgPulse
                      resultOnError_rsltHdl,   // inResultHandling
                      true);                   // inImmediateExec

  comCmd_pulse( true,                  // inSpecifyProgPulse
                true,                  // inFlushBuffersOnErr,
                resultAlways_rsltHdl,  // inResultHandling
                true);                 // inImmediateExec

  deviceFeedback_t  deviceFeedback;
  resultCodeVec_t   resultCodeVec;
  waitUntilIdle( deviceFeedback, resultCodeVec, (8*inPulseFactor)/1000 + 1000);  // (8us * factor)/1000us + 1s

  if ( resultCodeVec.size() == 0)
    throw comm_error( "Expected answer not found", __FILE__, __LINE__);

  if ( resultCodeVec.back().progVoltageMissingOnBus )
    throw comm_error( "12V program pulse not detected on bus", __FILE__, __LINE__);

  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "progPulseInternal() leaving\n");
}



//-------------------------------------------------------------------------------------
//
// Control command methods.
//

void LOW_linkDS2490::ctlCmd_resetDevice()
{
	STACK
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "ctlCmd_resetDevice() called\n");

  // locking done in publicly accessible methods

  usbDevice->controlMsg( vendorCmd_requestType, control_cmdType, resetDevice_ctlCmd,
                         0x0000, 0x0000, 0, ctlCmd_usbTimeout);
}


void LOW_linkDS2490::ctlCmd_startExecution()
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "ctlCmd_startExecution() called\n");

  // locking done in publicly accessible methods

  usbDevice->controlMsg( vendorCmd_requestType, control_cmdType, startExe_ctlCmd,
                         0x0000, 0x0000, 0, ctlCmd_usbTimeout);
}


void LOW_linkDS2490::ctlCmd_resumeExecution()
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "ctlCmd_resumeExecution() called\n");

  // locking done in publicly accessible methods

  usbDevice->controlMsg( vendorCmd_requestType, control_cmdType, resumeExe_ctlCmd,
                         0x0000, 0x0000, 0, ctlCmd_usbTimeout);
}


void LOW_linkDS2490::ctlCmd_haltExecutionWhenIdle()
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "ctlCmd_haltExecutionWhenIdle() called\n");

  // locking done in publicly accessible methods

  usbDevice->controlMsg( vendorCmd_requestType, control_cmdType, haltExeIdle_ctlCmd,
                         0x0000, 0x0000, 0, ctlCmd_usbTimeout);
}


void LOW_linkDS2490::ctlCmd_haltExecutionWhenDone()
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "ctlCmd_haltExecutionWhenDone() called\n");

  // locking done in publicly accessible methods

  usbDevice->controlMsg( vendorCmd_requestType, control_cmdType, haltExeDone_ctlCmd,
                         0x0000, 0x0000, 0, ctlCmd_usbTimeout);
}


void LOW_linkDS2490::ctlCmd_flushCommCmds()
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "ctlCmd_flushCommCmds() called\n");

  // locking done in publicly accessible methods

  usbDevice->controlMsg( vendorCmd_requestType, control_cmdType, flushCommCmds_ctlCmd,
                         0x0000, 0x0000, 0, ctlCmd_usbTimeout);
}


void LOW_linkDS2490::ctlCmd_flushDataRcvBuffer()
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "ctlCmd_flushDataRcvBuffer() called\n");

  // locking done in publicly accessible methods

  usbDevice->controlMsg( vendorCmd_requestType, control_cmdType, flushRcvBuffer_ctlCmd,
                         0x0000, 0x0000, 0, ctlCmd_usbTimeout);
}


void LOW_linkDS2490::ctlCmd_flushDataXmtBuffer()
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "ctlCmd_flushDataXmtBuffer() called\n");

  // locking done in publicly accessible methods

  usbDevice->controlMsg( vendorCmd_requestType, control_cmdType, flushXmtBuffer_ctlCmd,
                         0x0000, 0x0000, 0, ctlCmd_usbTimeout);
}


void LOW_linkDS2490::ctlCmd_getCommCmds( byteVec_t &outBytes)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "ctlCmd_getCommCmds() called\n");

  // locking done in publicly accessible methods

  usbDevice->controlMsg( vendorCmd_requestType, control_cmdType, getCommCmds_ctlCmd,
                         0x0000, outBytes, ctlCmd_usbTimeout);
}



//-------------------------------------------------------------------------------------
//
// Mode command methods.
//
  
void LOW_linkDS2490::modCmd_setEnablePulse( const bool inEnableStrongPullup, const bool inEnableProgPulse)
{
	STACK
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "modCmd_setEnablePulse() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wIndex_t indexVal = 0x0000;
  
  if ( inEnableStrongPullup ) indexVal |= 0x0002;
  if ( inEnableProgPulse )    indexVal |= 0x0001;
  
  usbDevice->controlMsg( vendorCmd_requestType, mode_cmdType, pulseEn_modCmd,
                         indexVal, 0x0000, 0, modCmd_usbTimeout);
}

  
void LOW_linkDS2490::modCmd_setEnableSpeedChange( const bool inEnableSpeedChange)
{
	STACK
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "modCmd_setEnableSpeedChange() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wIndex_t indexVal = 0x0000;
  
  if ( inEnableSpeedChange ) indexVal |= 0x0001;
  
  usbDevice->controlMsg( vendorCmd_requestType, mode_cmdType, speedChangeEn_modCmd,
                         indexVal, 0x0000, 0, modCmd_usbTimeout);
}

  
void LOW_linkDS2490::modCmd_setOneWireSpeed( const OWSPEED_val_t inWireSpeed)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "modCmd_setOneWireSpeed() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wIndex_t indexVal = 0x0000 | inWireSpeed;
  
  usbDevice->controlMsg( vendorCmd_requestType, mode_cmdType, oneWireSpeed_modCmd,
                         indexVal, 0x0000, 0, modCmd_usbTimeout);
}

  
void LOW_linkDS2490::modCmd_setStrongPullupDuration( const uint8_t inSpuDurationFactor)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "modCmd_setStrongPullupDuration() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wIndex_t indexVal = 0x0000 | inSpuDurationFactor;
  
  usbDevice->controlMsg( vendorCmd_requestType, mode_cmdType, strongPuDuration_modCmd,
                         indexVal, 0x0000, 0, modCmd_usbTimeout);
}

  
void LOW_linkDS2490::modCmd_setProgPulseDuration( const uint8_t inPpDurationFactor)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "modCmd_setProgPulseDuration() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wIndex_t indexVal = 0x0000 | inPpDurationFactor;
  
  usbDevice->controlMsg( vendorCmd_requestType, mode_cmdType, progPulseDuration_modCmd,
                         indexVal, 0x0000, 0, modCmd_usbTimeout);
}

  
void LOW_linkDS2490::modCmd_setPulldownSlewRate( const PDSR_val_t inPDSR)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "modCmd_setPulldownSlewRate() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wIndex_t indexVal = 0x0000 | inPDSR;
  
  usbDevice->controlMsg( vendorCmd_requestType, mode_cmdType, pulldownSlewRate_modCmd,
                         indexVal, 0x0000, 0, modCmd_usbTimeout);
}

  
void LOW_linkDS2490::modCmd_setWrite1LowTime( const W1LT_val_t inW1LT)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "modCmd_setWrite1LowTime() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wIndex_t indexVal = 0x0000 | inW1LT;
  
  usbDevice->controlMsg( vendorCmd_requestType, mode_cmdType, write1LowTime_modCmd,
                         indexVal, 0x0000, 0, modCmd_usbTimeout);
}

  
void LOW_linkDS2490::modCmd_setDsoW0RecoveryTime( const SOW0RT_val_t inSOW0RT)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "modCmd_setDsoW0RecoveryTime() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wIndex_t indexVal = 0x0000 | inSOW0RT;
  
  usbDevice->controlMsg( vendorCmd_requestType, mode_cmdType, dsow0Trec_modCmd,
                         indexVal, 0x0000, 0, modCmd_usbTimeout);
}



//-------------------------------------------------------------------------------------
//
// Communication command methods.
//

void LOW_linkDS2490::comCmd_setDuration( const uint8_t inTimeFactor,
                                         const bool inSpecifyProgPulse,
                                         const resultHandling_t inResultHandling,
                                         const bool inImmediateExec)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "comCmd_setDuration() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wValue_t valueCmd = setDuration_comCmdBase;
  if ( inSpecifyProgPulse ) valueCmd |= TYPE_bitmask;
  handleCommonComCmdBits( valueCmd, inResultHandling, inImmediateExec);

  LOW_portUsbDevice::wIndex_t indexVal = 0x0000 | inTimeFactor;

  usbDevice->controlMsg( vendorCmd_requestType, comm_cmdType, valueCmd,
                         indexVal, 0x0000, 0, comCmd_usbTimeout);
}


void LOW_linkDS2490::comCmd_pulse( const bool inSpecifyProgPulse,
                                   const bool inFlushBuffersOnErr,
                                   const resultHandling_t inResultHandling,
                                   const bool inImmediateExec)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "comCmd_pulse() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wValue_t valueCmd = pulse_comCmdBase;
  if ( inSpecifyProgPulse )  valueCmd |= TYPE_bitmask;
  if ( inFlushBuffersOnErr ) valueCmd |= F_bitmask;
  handleCommonComCmdBits( valueCmd, inResultHandling, inImmediateExec);

  usbDevice->controlMsg( vendorCmd_requestType, comm_cmdType, valueCmd,
                         0x0000, 0x0000, 0, comCmd_usbTimeout);
}


void LOW_linkDS2490::comCmd_oneWireReset( const bool inLoopUntilPresenceDetect,
                                          const bool inEnableSpeedChange,
                                          const OWSPEED_val_t inSpeedSelector,
                                          const bool inFlushBuffersOnErr,
                                          const resultHandling_t inResultHandling,
                                          const bool inImmediateExec)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "comCmd_oneWireReset() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wValue_t valueCmd = oneWireReset_comCmdBase;
  if ( inLoopUntilPresenceDetect ) valueCmd |= PST_bitmask;
  if ( inEnableSpeedChange )       valueCmd |= SE_bitmask;
  if ( inFlushBuffersOnErr )       valueCmd |= F_bitmask;
  handleCommonComCmdBits( valueCmd, inResultHandling, inImmediateExec);

  LOW_portUsbDevice::wIndex_t indexVal = 0x0000 | inSpeedSelector;

  usbDevice->controlMsg( vendorCmd_requestType, comm_cmdType, valueCmd,
                         indexVal, 0x0000, 0, comCmd_usbTimeout);
}


void LOW_linkDS2490::comCmd_bitIO( const bool inWriteBit,
                                   const bool inDoStrongPullup,
                                   const bool inSuppressPullupOnRead1,
                                   const resultHandling_t inResultHandling,
                                   const bool inImmediateExec)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "comCmd_bitIO() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wValue_t valueCmd = bitIO_comCmdBase;
  if ( inWriteBit )              valueCmd |= D_bitmask;
  if ( inDoStrongPullup )        valueCmd |= SPU_bitmask;
  if ( inSuppressPullupOnRead1 ) valueCmd |= CIB_bitmask;
  handleCommonComCmdBits( valueCmd, inResultHandling, inImmediateExec);

  usbDevice->controlMsg( vendorCmd_requestType, comm_cmdType, valueCmd,
                         0x0000, 0x0000, 0, comCmd_usbTimeout);
}


void LOW_linkDS2490::comCmd_byteIO( const uint8_t inWriteByte,
                                    const bool inDoStrongPullup,
                                    const resultHandling_t inResultHandling,
                                    const bool inImmediateExec)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "comCmd_byteIO() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wValue_t valueCmd = byteIO_comCmdBase;
  if ( inDoStrongPullup ) valueCmd |= SPU_bitmask;
  handleCommonComCmdBits( valueCmd, inResultHandling, inImmediateExec);

  LOW_portUsbDevice::wIndex_t indexVal = 0x0000 | inWriteByte;

  usbDevice->controlMsg( vendorCmd_requestType, comm_cmdType, valueCmd,
                         indexVal, 0x0000, 0, comCmd_usbTimeout);
}


void LOW_linkDS2490::comCmd_blockIO( const uint16_t inWriteSize,
                                     const bool inBusResetBefore,
                                     const bool inDoStrongPullup,
                                     const resultHandling_t inResultHandling,
                                     const bool inImmediateExec)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "comCmd_blockIO() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wValue_t valueCmd = blockIO_comCmdBase;
  if ( inBusResetBefore ) valueCmd |= RST_bitmask;
  if ( inDoStrongPullup ) valueCmd |= SPU_bitmask;
  handleCommonComCmdBits( valueCmd, inResultHandling, inImmediateExec);

  LOW_portUsbDevice::wIndex_t indexVal = inWriteSize;

  usbDevice->controlMsg( vendorCmd_requestType, comm_cmdType, valueCmd,
                         indexVal, 0x0000, 0, comCmd_usbTimeout);
}


void LOW_linkDS2490::comCmd_matchAccess( const uint8_t inMatchCommand,
                                         const bool inBusResetBefore,
                                         const bool inEnableSpeedChange,
                                         const OWSPEED_val_t inSpeedSelector,
                                         const resultHandling_t inResultHandling,
                                         const bool inImmediateExec)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "comCmd_matchAccess() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wValue_t valueCmd = matchAccess_comCmdBase;
  if ( inBusResetBefore )    valueCmd |= RST_bitmask;
  if ( inEnableSpeedChange ) valueCmd |= SE_bitmask;
  handleCommonComCmdBits( valueCmd, inResultHandling, inImmediateExec);

  LOW_portUsbDevice::wIndex_t indexVal = ((inSpeedSelector&0xff)<<8) | (inMatchCommand&0xff);

  usbDevice->controlMsg( vendorCmd_requestType, comm_cmdType, valueCmd,
                         indexVal, 0x0000, 0, comCmd_usbTimeout);
}


void LOW_linkDS2490::comCmd_readStraight( const uint8_t inWritePreambleSize,
                                          const uint16_t inReadSize,
                                          const bool inBusResetBefore,
                                          const resultHandling_t inResultHandling,
                                          const bool inImmediateExec)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "comCmd_readStraight() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wValue_t valueCmd = readStraight_comCmdBase;
  if ( inBusResetBefore ) valueCmd = 0x0000 | ((inWritePreambleSize&0xff)<<8);

  // special bit positions for this command
  switch ( inResultHandling ) {
    case noResult_rsltHdl:      valueCmd |= 0x0004; break;  // NTF=x  ICP=1
    case resultOnError_rsltHdl:                     break;  // NTF=0  ICP=0
    case resultAlways_rsltHdl:  valueCmd |= 0x0008; break;  // NTF=1  ICP=0
    default :                   throw internal_error( "Unknown resultHandling_t value detected", __FILE__, __LINE__);
  }
  if ( inBusResetBefore )       valueCmd |= 0x0002;  // RST=1
  if ( inImmediateExec )        valueCmd |= 0x0001;  // IM=1
  
  LOW_portUsbDevice::wIndex_t indexVal = inReadSize;

  LOW_portUsbDevice::wLength_t lengthVal = 0x0000 | inWritePreambleSize;

  usbDevice->controlMsg( vendorCmd_requestType, comm_cmdType, valueCmd,
                         indexVal, lengthVal, 0, comCmd_usbTimeout);
}


void LOW_linkDS2490::comCmd_doAndRelease( const uint8_t inWritePreambleSize,
                                          const bool inDoReadOperation,
                                          const bool inDoStrongPullup,
                                          const bool inFlushBuffersOnErr,
                                          const resultHandling_t inResultHandling,
                                          const bool inImmediateExec)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "comCmd_doAndRelease() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wValue_t valueCmd = doAndRelease_comCmdBase;
  if ( inDoReadOperation )   valueCmd |= R_bitmask;
  if ( inDoStrongPullup )    valueCmd |= SPU_bitmask;
  if ( inFlushBuffersOnErr ) valueCmd |= F_bitmask;
  handleCommonComCmdBits( valueCmd, inResultHandling, inImmediateExec);

  LOW_portUsbDevice::wIndex_t indexVal = 0x0000 | inWritePreambleSize;

  usbDevice->controlMsg( vendorCmd_requestType, comm_cmdType, valueCmd,
                         indexVal, 0x0000, 0, comCmd_usbTimeout);
}


void LOW_linkDS2490::comCmd_setPath( const uint8_t inPreloadPathSize,
                                     const bool inBusResetBefore,
                                     const bool inFlushBuffersOnErr,
                                     const resultHandling_t inResultHandling,
                                     const bool inImmediateExec)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "comCmd_setPath() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wValue_t valueCmd = setPath_comCmdBase;
  if ( inBusResetBefore )    valueCmd |= RST_bitmask;
  if ( inFlushBuffersOnErr ) valueCmd |= F_bitmask;
  handleCommonComCmdBits( valueCmd, inResultHandling, inImmediateExec);

  LOW_portUsbDevice::wIndex_t indexVal = 0x0000 | inPreloadPathSize;

  usbDevice->controlMsg( vendorCmd_requestType, comm_cmdType, valueCmd,
                         indexVal, 0x0000, 0, comCmd_usbTimeout);
}


void LOW_linkDS2490::comCmd_writeSramPage( const uint8_t inWriteSize,
                                           const bool inShortPreambleSize,
                                           const bool inActivateCrc16,
                                           const bool inFlushBuffersOnErr,
                                           const resultHandling_t inResultHandling,
                                           const bool inImmediateExec)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "comCmd_writeSramPage() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wValue_t valueCmd = writeSramPage_comCmdBase;
  if ( inShortPreambleSize ) valueCmd |= PS_bitmask;
  if ( inActivateCrc16 )     valueCmd |= DT_bitmask;
  if ( inFlushBuffersOnErr ) valueCmd |= F_bitmask;
  handleCommonComCmdBits( valueCmd, inResultHandling, inImmediateExec);

  LOW_portUsbDevice::wIndex_t indexVal = 0x0000 | inWriteSize;

  usbDevice->controlMsg( vendorCmd_requestType, comm_cmdType, valueCmd,
                         indexVal, 0x0000, 0, comCmd_usbTimeout);
}


void LOW_linkDS2490::comCmd_writeEprom( const uint16_t inWriteSize,
                                        const bool inActivateCrc16,
                                        const bool inCheck0BitsOnly,
                                        const bool inFlushBuffersOnErr,
                                        const resultHandling_t inResultHandling,
                                        const bool inImmediateExec)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "comCmd_writeEprom() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wValue_t valueCmd = writeEprom_comCmdBase;
  if ( inActivateCrc16 )     valueCmd |= DT_bitmask;
  if ( inCheck0BitsOnly )    valueCmd |= Z_bitmask;
  if ( inFlushBuffersOnErr ) valueCmd |= F_bitmask;
  handleCommonComCmdBits( valueCmd, inResultHandling, inImmediateExec);

  LOW_portUsbDevice::wIndex_t indexVal = inWriteSize;

  usbDevice->controlMsg( vendorCmd_requestType, comm_cmdType, valueCmd,
                         indexVal, 0x0000, 0, comCmd_usbTimeout);
}


void LOW_linkDS2490::comCmd_readCrcProtPage( const uint8_t inPageCount,
                                             const uint8_t inPageSizeLog2,
                                             const bool inShortPreambleSize,
                                             const bool inActivateCrc16,
                                             const bool inFlushBuffersOnErr,
                                             const resultHandling_t inResultHandling,
                                             const bool inImmediateExec)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "comCmd_readCrcProtPage() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wValue_t valueCmd = readCrcProtPage_comCmdBase;
  if ( inShortPreambleSize ) valueCmd |= PS_bitmask;
  if ( inActivateCrc16 )     valueCmd |= DT_bitmask;
  if ( inFlushBuffersOnErr ) valueCmd |= F_bitmask;
  handleCommonComCmdBits( valueCmd, inResultHandling, inImmediateExec);

  LOW_portUsbDevice::wIndex_t indexVal = ((inPageCount&0xff)<<8) | (inPageSizeLog2&0xff);

  usbDevice->controlMsg( vendorCmd_requestType, comm_cmdType, valueCmd,
                         indexVal, 0x0000, 0, comCmd_usbTimeout);
}


void LOW_linkDS2490::comCmd_readRedirectPageCrc( const uint8_t inPageNumber,
                                                 const uint8_t inPageSize,
                                                 const bool inFollowRedirect,
                                                 const bool inFlushBuffersOnErr,
                                                 const resultHandling_t inResultHandling,
                                                 const bool inImmediateExec)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "comCmd_readRedirectPageCrc() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wValue_t valueCmd = readRedirectPageCrc_comCmdBase;
  if ( inFollowRedirect )    valueCmd |= CH_bitmask;
  if ( inFlushBuffersOnErr ) valueCmd |= F_bitmask;
  handleCommonComCmdBits( valueCmd, inResultHandling, inImmediateExec);

  LOW_portUsbDevice::wIndex_t indexVal = ((inPageNumber&0xff)<<8) | (inPageSize&0xff);

  usbDevice->controlMsg( vendorCmd_requestType, comm_cmdType, valueCmd,
                         indexVal, 0x0000, 0, comCmd_usbTimeout);
}


void LOW_linkDS2490::comCmd_searchAccess( const uint8_t inMaxDevNum,
                                          const uint8_t inSearchCommand,
                                          const bool inSearchWithoutFullAccess,
                                          const bool inReturnDiscrepancyInfo,
                                          const bool inBusResetBefore,
                                          const bool inFlushBuffersOnErr,
                                          const resultHandling_t inResultHandling,
                                          const bool inImmediateExec)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "comCmd_searchAccess() called\n");

  // locking done in publicly accessible methods

  LOW_portUsbDevice::wValue_t valueCmd = searchAccess_comCmdBase;
  if ( inSearchWithoutFullAccess ) valueCmd |= SM_bitmask;
  if ( inReturnDiscrepancyInfo )   valueCmd |= RTS_bitmask;
  if ( inBusResetBefore )          valueCmd |= RST_bitmask;
  if ( inFlushBuffersOnErr )       valueCmd |= F_bitmask;
  handleCommonComCmdBits( valueCmd, inResultHandling, inImmediateExec);

  LOW_portUsbDevice::wIndex_t indexVal = ((inMaxDevNum&0xff)<<8) | (inSearchCommand&0xff);

  usbDevice->controlMsg( vendorCmd_requestType, comm_cmdType, valueCmd,
                         indexVal, 0x0000, 0, comCmd_usbTimeout);
}


void LOW_linkDS2490::handleCommonComCmdBits( LOW_portUsbDevice::wValue_t &inOutValueCmd,
                                             const resultHandling_t inResultHandling, const bool inImmediateExec)
{
  LOW_helper_msglog::printDebug( LOW_helper_msglog::linkDS2490_dl, "handleCommonComCmdBits() called\n");

  // locking done in publicly accessible methods

  // special bit positions for this command
  switch ( inResultHandling ) {
    case noResult_rsltHdl:      inOutValueCmd |= ICP_bitmask; break;  // NTF=x  ICP=1
    case resultOnError_rsltHdl:                               break;  // NTF=0  ICP=0
    case resultAlways_rsltHdl:  inOutValueCmd |= NTF_bitmask; break;  // NTF=1  ICP=0
    default :                   throw internal_error( "Unknown resultHandling_t value detected", __FILE__, __LINE__);
  }
  if ( inImmediateExec )        inOutValueCmd |= IM_bitmask;  // IM=1
}




//-------------------------------------------------------------------------------------
//
// Enumeration type conversion methods
//

const LOW_linkDS2490::pdSlewRate_t LOW_linkDS2490::PDSR_val_2_pdSlewRate( const PDSR_val_t inPDSR_val) const
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

const LOW_linkDS2490::PDSR_val_t LOW_linkDS2490::pdSlewRate_2_PDSR_val( const pdSlewRate_t inPdSlewRate_t) const
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


const LOW_linkDS2490::w1LowTime_t LOW_linkDS2490::W1LT_val_2_w1LowTime( const W1LT_val_t inW1LT_val) const
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

const LOW_linkDS2490::W1LT_val_t LOW_linkDS2490::w1LowTime_2_W1LT_val( const w1LowTime_t inW1LowTime) const
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


const LOW_linkDS2490::soW0RecTime_t LOW_linkDS2490::SOW0RT_val_2_soW0RecTime( const SOW0RT_val_t inSOW0RT_val) const
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

const LOW_linkDS2490::SOW0RT_val_t LOW_linkDS2490::soW0RecTime_2_SOW0RT_val( const soW0RecTime_t inSoW0RecTime) const
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


const LOW_linkDS2490::wireSpeed_t LOW_linkDS2490::OWSPEED_val_2_wireSpeed( const OWSPEED_val_t inOWSPEED_val) const
{
  // fully const, no locking necessary

  switch( inOWSPEED_val ) {
    case normal_OWSPEED:    return normal_speed;
    case flexible_OWSPEED:  return flexible_speed;
    case overdrive_OWSPEED: return overdrive_speed;
    default:                throw internal_error( "Unknown OWSPEED_val_t value detected", __FILE__, __LINE__);
  }
}

const LOW_linkDS2490::OWSPEED_val_t LOW_linkDS2490::wireSpeed_2_OWSPEED_val( const wireSpeed_t inWireSpeed) const
{
  // fully const, no locking necessary

  switch( inWireSpeed ) {
    case normal_speed:    return normal_OWSPEED;
    case flexible_speed:  return flexible_OWSPEED;
    case overdrive_speed: return overdrive_OWSPEED;
    default:              throw internal_error( "Unknown wireSpeed_t value detected", __FILE__, __LINE__);
  }
}


const uint8_t LOW_linkDS2490::strongPullup_2_spFactor( const strongPullup_t inPullup) const
{
  // fully const, no locking necessary

  switch( inPullup ) {
    case pullUp_16_4:  return  1;
    case pullUp_65_5:  return  4;
    case pullUp_131:   return  8;
    case pullUp_262:   return 16;
    case pullUp_524:   return 32;
    case pullUp_1048:  return 64;
    default:           throw internal_error( "Unknown strongPullup_t value detected", __FILE__, __LINE__);
  }
}


const uint8_t LOW_linkDS2490::progPulse_2_ppFactor( const progPulse_t inPulse) const
{
  // fully const, no locking necessary

  switch( inPulse ) {
    case progPulse_32:   return   4;
    case progPulse_64:   return   8;
    case progPulse_128:  return  16;
    case progPulse_256:  return  32;
    case progPulse_512:  return  64;
    case progPulse_1024: return 128;
    case progPulse_2048: return 254;
    default:             throw internal_error( "Unknown progPulse_t value detected", __FILE__, __LINE__);
  }
}

