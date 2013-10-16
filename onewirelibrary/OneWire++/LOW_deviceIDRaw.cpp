/***************************************************************************
                          LOW_deviceIDRaw.cpp  -  description
                             -------------------
    begin                : Thu Jul 18 2002
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

 
#include <stdio.h> // needed for snprintf()
#include <string>

#include "LOW_deviceIDRaw.h"
#include "converter.h"
using namespace std;

//=====================================================================================
//
// public constructors
//

LOW_deviceIDRaw::LOW_deviceIDRaw()
{
  for( unsigned int a=0; a<sizeof( romID); a++)
    romID[a] = 0x00;
}

LOW_deviceIDRaw::LOW_deviceIDRaw( const LOW_deviceIDRaw &inDeviceIDRaw) :
  LOW_objectSynchronizer( inDeviceIDRaw)
{
  for( unsigned int a=0; a<sizeof( romID); a++)
    romID[a] = inDeviceIDRaw.romID[a];
}

LOW_deviceIDRaw::LOW_deviceIDRaw( const devRomID_t &inRomID)
{
  for( unsigned int a=0; a<sizeof( romID); a++) {
    romID[a] = inRomID[a];
  }
}

LOW_deviceIDRaw::LOW_deviceIDRaw( uint32_t inHighInt, uint32_t inLowInt)
{
  for( unsigned int a=0; a<4; a++) {
    romID[a] = inLowInt & 0xff;
    inLowInt>>=8;
  }
  for( unsigned int a=4; a<8; a++) {
    romID[a] = inHighInt & 0xff;
    inHighInt>>=8;
  }
}
  
LOW_deviceIDRaw::LOW_deviceIDRaw( const byteVec_t &inRomID)
{
  if ( inRomID.size() != sizeof( romID) )
    throw sizeMismatch_error( "Byte vector length does not match", __FILE__, __LINE__);
    
  for( unsigned int a=0; a<sizeof( romID); a++)
    romID[a] = inRomID[a];
}


LOW_deviceIDRaw::~LOW_deviceIDRaw()
{
}


//=====================================================================================
//
// operator overloading
//

bool LOW_deviceIDRaw::operator==(const LOW_deviceIDRaw &inDID) const
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  for( unsigned int a=0; a<8; a++)
    if ( inDID.romID[a] != romID[a] )
      return false;
  return true;
}

bool LOW_deviceIDRaw::operator!=(const LOW_deviceIDRaw &inDID) const
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  return !(inDID.romID==romID);
}

bool LOW_deviceIDRaw::operator<(const LOW_deviceIDRaw &inDID) const
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  for( int a=7; a>=0; a--) {
    if( romID[a] < inDID.romID[a] )
      return true;
    if( romID[a] > inDID.romID[a] )
      return false;
  }
  return false;  // they are identical
}


//=====================================================================================
//
// public methods
//

void LOW_deviceIDRaw::getRomID( devRomID_t& outID) const
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  for( unsigned int a=0; a<sizeof( romID); a++)
    outID[a] = romID[a];
}


byteVec_t LOW_deviceIDRaw::getRomIDVec() const
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  byteVec_t retVal = byteVec_t( sizeof( romID));
  
  for( unsigned int a=0; a<sizeof( romID); a++)
    retVal[a] = romID[a];
  
  return retVal;
}
  

std::string LOW_deviceIDRaw::getRomIDString() const
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  const unsigned int bufLen = 2*sizeof(romID) + 1;  // the +1 is the terminating null byte
  char               buffer[bufLen];
  char               *tmpBuffer = buffer;
      
  for( int a=sizeof( romID)-1; a>=0; --a) {
    snprintf( tmpBuffer, bufLen, "%02x", romID[a]);
    tmpBuffer += 2;
  }
      
  return std::string( buffer);
}


LOW_deviceIDRaw::devCRC_t LOW_deviceIDRaw::getCRC() const
{
  __LOW_SYNCHRONIZE_METHOD_READ__
  
  return romID[7];
}


void LOW_deviceIDRaw::getSerialNum( devSerNum_t& outSerNum) const
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  for( int a=0; a<6; a++)
    outSerNum[a] = romID[a+1];
}


LOW_deviceIDRaw::devFamCode_t LOW_deviceIDRaw::getFamilyCode() const
{
  __LOW_SYNCHRONIZE_METHOD_READ__
  
  return romID[0];
}
  

void LOW_deviceIDRaw::setFamilyCode( const devFamCode_t inFamCode)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE__

  romID[0] = inFamCode;
}
  

bool LOW_deviceIDRaw::getBit( uint8_t inBitNum) const
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  if ( inBitNum > 63 )
    throw range_error( "Index out of range", __FILE__, __LINE__);
  
  return (romID[inBitNum/8]&(0x01<<(inBitNum%8)))==0?false:true;
}


void LOW_deviceIDRaw::setBit( const uint8_t inBitNum, const bool inValue)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE__

  if ( inBitNum > 63 )
    throw range_error( "Index out of range", __FILE__, __LINE__);
  
  if ( inValue )
    romID[inBitNum/8] |= 0x01<<(inBitNum%8);
  else
    romID[inBitNum/8] &= ~(0x01<<(inBitNum%8));
}

