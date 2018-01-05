/***************************************************************************
                          LOW_deviceID.cpp  -  description
                             -------------------
    begin                : Sat Jul 6 2002
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

#include "LOW_deviceID.h"
#include "LOW_helper_crc.h"
#include "HisException.h"

//=====================================================================================
//
// public constructors
//

LOW_deviceID::LOW_deviceID() :
  LOW_deviceIDRaw()
{
}

LOW_deviceID::LOW_deviceID( const LOW_deviceID &inDeviceID) :
  LOW_deviceIDRaw( inDeviceID)
{
  checkCRC();
}

LOW_deviceID::LOW_deviceID( const devRomID_t &inRomID) :
  LOW_deviceIDRaw( inRomID)
{
  checkCRC();
}

LOW_deviceID::LOW_deviceID( uint32_t inHighInt, uint32_t inLowInt) :
  LOW_deviceIDRaw( inHighInt, inLowInt)
{
  checkCRC();
}
  
LOW_deviceID::LOW_deviceID( const byteVec_t &inRomID) :
  LOW_deviceIDRaw( inRomID)
{
  checkCRC();
}

LOW_deviceID::LOW_deviceID( const LOW_deviceIDRaw &inDevIDRaw) :
  LOW_deviceIDRaw( inDevIDRaw)
{
  checkCRC();
}

    
LOW_deviceID::~LOW_deviceID()
{
}


//=====================================================================================
//
// private methods
//

void LOW_deviceID::checkCRC()
{

  if ( LOW_helper_CRC::calcCRC8( romID, sizeof( romID)) != 0 )
    throw HisException( "CRC checksum error in ROM ID", __FILE__, __LINE__);
}


void LOW_deviceID::setBit( const uint8_t /*inBitNum*/, const bool /*inValue*/)
{
}


void LOW_deviceID::setFamilyCode( const devFamCode_t /*inFamCode*/)
{
}
