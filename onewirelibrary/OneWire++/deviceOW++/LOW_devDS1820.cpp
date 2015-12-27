/***************************************************************************
                          LOW_devDS1820.cpp  -  description
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

 
#include "LOW_devDS1820.h"
#include "LOW_platformMisc.h"
#include "LOW_helper_msglog.h"
#include "LOW_deviceFactory.h"
#include "LOW_netSegment.h"
#include "LOW_helper_crc.h"



//=====================================================================================
//
// static initializer
//

const std::string LOW_devDS1820::familyName = "DS1820";

int LOW_devDS1820::initHelper = initialize();
int LOW_devDS1820::initialize()
{
  LOW_deviceFactory::registerSpecificCtor( familyCode, &LOW_devDS1820::new_Instance);
  return 0;
}



//=====================================================================================
//
// static methods
//
  
LOW_device* LOW_devDS1820::new_Instance( LOW_netSegment &inNetSegment, const LOW_deviceID &inDevID)
{
  return new LOW_devDS1820( inNetSegment, inDevID);
}



//=====================================================================================
//
// public constructors
//

LOW_devDS1820::LOW_devDS1820( LOW_netSegment &inSegment, const LOW_deviceID &inDevID) : 
  LOW_device( inSegment, inDevID, familyCode)
{
  if ( inSegment.getHasExternalPower() )
    isExternalPowered = cmd_ReadPowerSupply();
  else
    isExternalPowered = false;

  cmd_RecallE2();
}


LOW_devDS1820::~LOW_devDS1820()
{
}


//=====================================================================================
//
// public methods
//

bool LOW_devDS1820::getIsExternalPowered() const
{
  return isExternalPowered;
}


void LOW_devDS1820::cmd_ConvertT() const
{
  linkLock  lock( *this);
  
  cmd_MatchROM();
  
  if ( isExternalPowered ) {
    getLink().writeData( ConvertT_COMMAND);
    
    // poll bits to detect conversion has finished
    while ( getLink().readDataBit() == false );
  }
  else {
    // pull up
    getLink().writeData( ConvertT_COMMAND, LOW_link::pullUp_1048);
  }
}

    
void LOW_devDS1820::cmd_ReadScratchpad( scratchpadDS1820_t *outScratchpad) const
{
  linkLock  lock( *this);
  
  cmd_MatchROM();  
  getLink().writeData( ReadScratchpad_COMMAND);
  
  byteVec_t scratchpad = byteVec_t( sizeof( scratchpadDS1820_t));
  getLink().readData( scratchpad);
  if ( LOW_helper_CRC::calcCRC8( scratchpad) != 0x00 )
    throw LOW_helper_CRC::crc_error( "CRC error in read scratchpad", __FILE__, __LINE__);

  for ( unsigned int a=0; a<=sizeof( scratchpadDS1820_t); a++)
    (reinterpret_cast<uint8_t *>(outScratchpad))[a] = scratchpad[a];
}


void LOW_devDS1820::cmd_WriteScratchpad( const uint8_t inTL, const uint8_t inTH) const
{
  linkLock  lock( *this);
  
  cmd_MatchROM();
  
  byteVec_t outData;
  outData.push_back( static_cast<uint8_t>( WriteScratchpad_COMMAND));
  outData.push_back( inTH);
  outData.push_back( inTL);
  
  getLink().writeData( outData);
}

    
void LOW_devDS1820::cmd_CopyScratchpad() const
{
  linkLock  lock( *this);
  
  cmd_MatchROM();
  
  if ( isExternalPowered ) {
    getLink().writeData( CopyScratchpad_COMMAND);
  }
  else {
    // pull up
    getLink().writeData( CopyScratchpad_COMMAND, LOW_link::pullUp_1048);
  }
}

    
void LOW_devDS1820::cmd_RecallE2() const
{
  linkLock  lock( *this);
  
  cmd_MatchROM();
  
  if ( isExternalPowered ) {
    getLink().writeData( RecallE2_COMMAND);
    
    // poll bits to detect recalling has finished
    while ( getLink().readDataBit() == false );
  }
  else {
    // pull up
    getLink().writeData( RecallE2_COMMAND, LOW_link::pullUp_1048);
  }
}


bool LOW_devDS1820::cmd_ReadPowerSupply() const
{
  linkLock  lock( *this);

  cmd_MatchROM();  
  getLink().writeData( ReadPowerSupply_COMMAND);
  
  return getLink().readDataBit();
}

