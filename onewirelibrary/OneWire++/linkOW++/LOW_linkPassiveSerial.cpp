/***************************************************************************
                          LOW_linkPassiveSerial.cpp  -  description
                             -------------------
    begin                : Sun Jul 7 2002
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


#include "LOW_linkPassiveSerial.h"



//=====================================================================================
//
// constructors
//

LOW_linkPassiveSerial::LOW_linkPassiveSerial( const LOW_portSerialFactory::portSpecifier_t &inSerPortSpec,
                                              const bool inAllowProgPulse) :
  LOW_link( false, true, inAllowProgPulse)
{
  serialPort   = LOW_portSerialFactory::new_portSerial( inSerPortSpec);

  resetLinkAdapter();
  resetBus(); // to detect a missing adapter
}

  

LOW_linkPassiveSerial::~LOW_linkPassiveSerial()
{
  serialPort->tty_flush();

  delete serialPort;
}



//=====================================================================================
//
// touch data methods
//

bool LOW_linkPassiveSerial::touchBit( const bool inSendBit, const strongPullup_t inPullup)
{
  commLock lock( *this);
  
  uint8_t         serSendByte;
  bool            retVal;

  serialPort->tty_flush();

  // get bit ready to be sent
  serSendByte = inSendBit ? 0xFF : 0x00;

  // Send the bit
  serialPort->tty_write( serSendByte);

  uint8_t tmpByte = serialPort->tty_readByte();        
  if ( tmpByte == 0xFF )
    retVal = true;
  else
    retVal = false;
  
  strongPullup( inPullup);
  
  return retVal;
}  
    

uint8_t LOW_linkPassiveSerial::touchByte( const uint8_t inSendByte, const strongPullup_t inPullup)
{
  commLock lock( *this);
  
  uint8_t  mask    = 0x01;
  uint8_t  retVal  = 0x00;
  
  for( int a=0; a<8; a++) {
    retVal |= touchBit( inSendByte & mask) ? mask : 0x00;
    mask <<= 1;
  }
  
  strongPullup( inPullup);
  
  return retVal;
}

  
//=====================================================================================
//
// misc methods
//

void LOW_linkPassiveSerial::resetLinkAdapter()
{
  commLock lock( *this);
  
  serialPort->tty_configure( LOW_portSerial::none_flowControl, LOW_portSerial::bit6_size, 
                             LOW_portSerial::no_parity, LOW_portSerial::bit1_stopBit, LOW_portSerial::B115200_speed);
  
  serialPort->tty_flush();
}

    
bool LOW_linkPassiveSerial::resetBus()
{
  commLock lock( *this);
  
  const uint8_t   resetSendByte = 0xF0;
  
  bool            devFound = false;
    
  try {
    
    // Flush the input and output buffers
    serialPort->tty_flush();
    
    serialPort->tty_configure( LOW_portSerial::none_flowControl, LOW_portSerial::bit8_size, 
                               LOW_portSerial::no_parity, LOW_portSerial::bit1_stopBit, LOW_portSerial::B9600_speed);
    
    // Send the reset pulse
    serialPort->tty_write( resetSendByte);

    uint8_t tmpByte = serialPort->tty_readByte();

    if( tmpByte == 0 )  // if answer is still zero, then it is a short to ground
      throw comm_error( "Possible short to ground detected", __FILE__, __LINE__);

    // If the byte read is not what we sent, check for an error
    // For now just return a 1 and discount any errors??
    if( tmpByte != resetSendByte )
      devFound = true; // got a response of some type
    else
      devFound = false; // no device responding
  }
  catch( ... ) {
    
    try { 
      serialPort->tty_configure( LOW_portSerial::none_flowControl, LOW_portSerial::bit8_size,
                                 LOW_portSerial::no_parity, LOW_portSerial::bit1_stopBit, LOW_portSerial::B115200_speed);
    }
    catch( ... ) {}
    
    throw;
  }  
  
  serialPort->tty_configure( LOW_portSerial::none_flowControl, LOW_portSerial::bit8_size,
                             LOW_portSerial::no_parity, LOW_portSerial::bit1_stopBit, LOW_portSerial::B115200_speed);

  return devFound;
}


void LOW_linkPassiveSerial::strongPullup( const unsigned long /*inMilliSecs*/)
{
  commLock lock( *this);
  LOW_platformMisc::secSleep( strongPullupEmuTime);
}


void LOW_linkPassiveSerial::strongPullup( const strongPullup_t /*inPullupTimes*/)
{
  commLock lock( *this);
  //LOW_platformMisc::secSleep( strongPullupEmuTime);
  //LOW_platformMisc::milliSleep(50);
}


void LOW_linkPassiveSerial::programPulse( const unsigned long /*inMicroSecs*/)
{
  commLock lock( *this);

  if ( ! allowProgPulse )
    throw notAllowed_error( "Program pulse not allowed", __FILE__, __LINE__);

  if ( ! hasProgramPulse )
    throw illegalLevel_error( "Program pulse not available", __FILE__, __LINE__);

  throw internal_error( "Program pulse not available, should never be reached", __FILE__, __LINE__);
}


void LOW_linkPassiveSerial::programPulse( const progPulse_t /*inPulseTime*/)
{
  commLock lock( *this);

  if ( ! allowProgPulse )
    throw notAllowed_error( "Program pulse not allowed", __FILE__, __LINE__);

  if ( ! hasProgramPulse )
    throw illegalLevel_error( "Program pulse not available", __FILE__, __LINE__);

  throw internal_error( "Program pulse not available, should never be reached", __FILE__, __LINE__);
}
