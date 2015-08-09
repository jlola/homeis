/***************************************************************************
                          LOW_link.cpp  -  description
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

 
#include "LOW_link.h"
#include "LOW_device.h"


//=====================================================================================
//
// constructors
//

LOW_link::LOW_link( const bool inHasProgramPulse, const bool inHasExternalPower, const bool inAllowProgPulse) :
  linkID( LOW_objectIDFactory::getNewObjectID()),
  hasProgramPulse( inHasProgramPulse),
  hasExternalPower( inHasExternalPower),
  allowProgPulse( inAllowProgPulse)
{
  __linkRecMutex = LOW_thread_Factory::new_mutex( LOW_thread_mutex::mutexKind_recursive);
}


LOW_link::~LOW_link()
{
  delete __linkRecMutex;
}



//=====================================================================================
//
// operator overloading
//
  
bool LOW_link::operator==(LOW_link &inLink) const
{
  return (linkID==inLink.linkID);
}


  
//=====================================================================================
//
// methods
//

uint32_t LOW_link::getID() const
{
  return linkID;
}


bool LOW_link::getHasProgramPulse() const
{
  return hasProgramPulse;
}

  
bool LOW_link::getHasExternalPower() const
{
  return hasExternalPower;
}


bool LOW_link::getAllowProgPulse() const
{
  return allowProgPulse;
}



//=====================================================================================
//
// Bus touch (write/read) methods
//

byteVec_t LOW_link::touchBlock( const byteVec_t &inBytes, const strongPullup_t inPullup)
{
  if ( inBytes.size() == 0 ) return byteVec_t( 0);

  commLock lock( *this);

  byteVec_t retVal = byteVec_t( inBytes.size());

  for( unsigned int i=0; i<inBytes.size()-1; i++) {
    retVal[i] = touchByte( inBytes[i], pullUp_NONE);
  }
  retVal[inBytes.size()-1] = touchByte( inBytes[inBytes.size()-1], inPullup);
  
  return retVal;
}


//=====================================================================================
//
// read data methods
//

bool LOW_link::readDataBit( const strongPullup_t inPullup)
{
  commLock lock( *this);

  return touchBit( true, inPullup);
}


uint8_t LOW_link::readDataByte( const strongPullup_t inPullup)
{
  commLock lock( *this);

  return touchByte( 0xff, inPullup);
}


void LOW_link::readData( byteVec_t &outBytes, const strongPullup_t inPullup)
{
  commLock lock( *this);

  byteVec_t  sendBytes = byteVec_t( outBytes.size(), 0xff);
  byteVec_t  recBytes;

  recBytes = touchBlock( sendBytes, inPullup);
  std::copy( recBytes.begin(), recBytes.end(), outBytes.begin());
}



//=====================================================================================
//
// write data methods
//

void LOW_link::writeData( const bool inSendBit, const strongPullup_t inPullup)
{
  commLock lock( *this);

  if ( touchBit( inSendBit, inPullup) != inSendBit )
    throw comm_error( "Response not equal to sent bit", __FILE__, __LINE__);
}


void LOW_link::writeData( const uint8_t inSendByte, const strongPullup_t inPullup)
{
  commLock lock( *this);

  if ( touchByte( inSendByte, inPullup) != inSendByte )
    throw comm_error( "Response not equal to sent byte", __FILE__, __LINE__);
}


void LOW_link::writeData( const byteVec_t &inSendBytes, const strongPullup_t inPullup)
{
  commLock lock( *this);

  byteVec_t readVec = touchBlock( inSendBytes, inPullup);

  for( unsigned int i=0; i<inSendBytes.size(); i++)
    if ( readVec[i] != inSendBytes[i] ) {
      throw comm_error( "Response not equal to sent byte", __FILE__, __LINE__);
    }
}



//=====================================================================================
//
// Higher level actions
//
bool firstend = false;

LOW_deviceID::deviceIDVec_t LOW_link::searchDevices( const bool inOnlyAlarm, const LOW_deviceIDRaw inPreload,
                                                     const LOW_deviceIDRaw::devFamCode_t inFamCode, const bool inDoReset)
{
  commLock lock( *this);

  LOW_deviceIDRaw                searchVec = LOW_deviceIDRaw( inPreload);
  LOW_deviceIDRaw                foundID;
  LOW_deviceIDRaw                discrVec;
  int                            lastDiscr = 0;
  LOW_deviceID::deviceIDVec_t    foundIDVec;

  // preload family type
  if ( inFamCode != LOW_device::anyDev_famCode )
    searchVec.setFamilyCode( inFamCode);

  while ( true ) {

    if ( resetBus() == false )
      return foundIDVec;
      //throw LOW_device::noDevice_error( "Reset indicated no devices", __FILE__, __LINE__);

    if ( inOnlyAlarm )
      writeData( LOW_device::SearchAlarmROM_COMMAND);
    else
      writeData( LOW_device::SearchROM_COMMAND);

    doSearchSequence( searchVec, foundID, discrVec);

    if ( foundID.getBit( 63)==true && discrVec.getBit( 63)==true) // no devices found
      return foundIDVec;
      //throw LOW_device::noDevice_error( "No devices during search found", __FILE__, __LINE__);

    LOW_deviceID newID = LOW_deviceID( foundID);

    if ( inFamCode!=LOW_device::anyDev_famCode && newID.getFamilyCode()!=inFamCode )
      break;

    foundIDVec.push_back( newID);

    // find last discrepancy
    int newDiscr = 0xff;
    for( int a=63 ; a>=0; a--) {
      if ( discrVec.getBit( a)==true && foundID.getBit( a)==false) {
        newDiscr = a;
        break;
      }
    }

    if ( newDiscr==0xff /*|| newDiscr==lastDiscr*/ ) {  // search has ended
    	if (firstend)
    		break;
    	else
    		firstend = true;
    }

    lastDiscr = newDiscr;

    // prepare next search vector
    for( int a=0; a<64; a++) {
      if ( a<lastDiscr ) {
        searchVec.setBit( a, foundID.getBit( a));
      }
      else if ( a==lastDiscr ) {
        searchVec.setBit( a, true);
      }
      else {
        searchVec.setBit( a, false);
      }
    }

  }

  if ( inDoReset )
    if ( resetBus() == false )
      throw LOW_device::noDevice_error( "Reset indicated no devices", __FILE__, __LINE__);

  return foundIDVec;
}


//=====================================================================================
//
// methods
//

void LOW_link::doSearchSequence( const LOW_deviceIDRaw &inBranchVector, 
                                 LOW_deviceIDRaw &outFoundID, LOW_deviceIDRaw &outDiscrVec)
{
  commLock lock( *this);
  
  for( int a=0; a<64; a++) {
    
    bool readB = readDataBit();
    bool cmplB = readDataBit();
  
    if ( readB == cmplB ) {
      writeData( inBranchVector.getBit( a));
      outFoundID.setBit( a, inBranchVector.getBit( a));
      outDiscrVec.setBit( a, true);
    }
    else {
      writeData( readB);
      outFoundID.setBit( a, readB);
      outDiscrVec.setBit( a, false);
    }
    
  }
}
