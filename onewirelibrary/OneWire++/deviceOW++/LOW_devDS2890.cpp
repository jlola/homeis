/***************************************************************************
                          LOW_devDS2890.cpp  -  description
                             -------------------
    begin                : Sun Nov 9 2003
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

 
#include "LOW_devDS2890.h"


//=====================================================================================
//
// static initializer
//

const std::string LOW_devDS2890::familyName = "DS2890";

int LOW_devDS2890::initHelper = initialize();
int LOW_devDS2890::initialize()
{
  LOW_deviceFactory::registerSpecificCtor( familyCode, &LOW_devDS2890::new_Instance);
  return 0;
}



//=====================================================================================
//
// static methods
//

LOW_device* LOW_devDS2890::new_Instance( LOW_netSegment &inNetSegment, const LOW_deviceID &inDevID)
{
  return new LOW_devDS2890( inNetSegment, inDevID);
}



//=====================================================================================
//
// public constructors
//

LOW_devDS2890::LOW_devDS2890( LOW_netSegment &inSegment, const LOW_deviceID &inDevID) :
  LOW_device( inSegment, inDevID, familyCode)
{
  chargePumpAllowed = false;
  cmd_ReadControlRegister();  // initially read feature values
  cachedWiperPos = new wiperPosition_t[getPotentiometerCount()];
  for( unsigned int a=0; a<getPotentiometerCount(); ++a)
    cachedWiperPos[a] = 0;
}


LOW_devDS2890::~LOW_devDS2890()
{
  delete[] cachedWiperPos;
}


//=====================================================================================
//
// public methods
//

void LOW_devDS2890::setChargePumpAllowed( const bool inCharpePumpAllowed)
{
  chargePumpAllowed = inCharpePumpAllowed;
}


bool LOW_devDS2890::getChargePumpAllowed()
{
  return chargePumpAllowed;
}


bool LOW_devDS2890::getHasLinearPotElements() const
{
  return hasLinearPotElements;
}


bool LOW_devDS2890::getIsWiperSettingVolatile() const
{
  return isWiperSettingVolatile;
}


uint8_t LOW_devDS2890::getPotentiometerCount() const
{
  return potentiometerCount;
}


LOW_devDS2890::wiperPosition_t LOW_devDS2890::getWiperPositionsCount() const
{
  return wiperPositionsCount;
}


uint32_t LOW_devDS2890::getPotentiometerResistance() const
{
  return potentiometerResistance;
}


uint8_t LOW_devDS2890::getCachedWiperNumber()
{
  return cachedCurrentWiper;
}


LOW_devDS2890::wiperPosition_t LOW_devDS2890::getCachedWiperPosition( const uint8_t inControlledWiperNum)
{
  if ( inControlledWiperNum >= getPotentiometerCount() )
    throw devDS2890_error( "Illegal wiper number specified", __FILE__, __LINE__);
  
  return cachedWiperPos[inControlledWiperNum];
}


LOW_devDS2890::wiperPosition_t LOW_devDS2890::cmd_ReadPosition()
{
  linkLock  lock( *this);

  cmd_MatchROM();
  
  getLink().writeData( ReadPosition_COMMAND);

  byteVec_t dataFromDev = byteVec_t( 2);
  getLink().readData( dataFromDev); // 1st byte: control register, 2nd byte: position

  if ( dataFromDev[1]&wiperNum_ctlRegMsk != ((~dataFromDev[1]) & wiperNum_ctlRegMsk)>>2 )
    throw devDS2890_error( "Error sanity checking control register value", __FILE__, __LINE__);

  cachedCurrentWiper = dataFromDev[1] & wiperNum_ctlRegMsk;
  cachedWiperPos[cachedCurrentWiper] = dataFromDev[1];

  return dataFromDev[1];
}


void LOW_devDS2890::cmd_WritePosition( const LOW_devDS2890::wiperPosition_t inNewPosition)
{
  linkLock  lock( *this);

  if ( inNewPosition >= getWiperPositionsCount() )
    throw devDS2890_error( "New position exceeds maximal position", __FILE__, __LINE__);

  cmd_MatchROM();

  byteVec_t outputBytes;
  outputBytes.push_back( static_cast<uint8_t>( WritePosition_COMMAND));
  outputBytes.push_back( inNewPosition);
  getLink().writeData( outputBytes);
  
  uint8_t posCheckByte = getLink().readDataByte();
  if ( posCheckByte != inNewPosition) {
    getLink().resetBus();
    throw devDS2890_error( "Error verifying new position", __FILE__, __LINE__);
  }

  getLink().writeData( releaseCode_subcommand);

  bool errorBit = getLink().readDataBit();
  if ( errorBit == true ) {
    getLink().resetBus();
    throw devDS2890_error( "Error activating new position", __FILE__, __LINE__);
  }

  cachedWiperPos[cachedCurrentWiper] = inNewPosition;
}


LOW_devDS2890::controlRegister_t LOW_devDS2890::cmd_ReadControlRegister()
{
  linkLock  lock( *this);

  cmd_MatchROM();

  getLink().writeData( ReadControlRegister_COMMAND);

  byteVec_t dataFromDev = byteVec_t( 2);
  getLink().readData( dataFromDev); // 1st byte: feature register, 2nd byte: control register

  hasLinearPotElements   = dataFromDev[0] & potentiometerType_featRegMask;
  isWiperSettingVolatile = dataFromDev[0] & wiperSetVolatility_featRegMask;
  potentiometerCount     = ((dataFromDev[0]&potentiometerCount_featRegMask)>>2) + 1;

  switch ( (dataFromDev[0]&wiperPositions_featRegMask)>>4 ) {
    case 0x00: wiperPositionsCount =  32;  break;
    case 0x01: wiperPositionsCount =  64;  break;
    case 0x02: wiperPositionsCount = 128;  break;
    case 0x03: wiperPositionsCount = 256;  break;
  }

  switch ( (dataFromDev[0]&potentiometerResistance_featRegMask)>>6 ) {
    case 0x00: potentiometerResistance =   5000;  break;
    case 0x01: potentiometerResistance =  10000;  break;
    case 0x02: potentiometerResistance =  50000;  break;
    case 0x03: potentiometerResistance = 100000;  break;
  }

  if ( dataFromDev[1]&wiperNum_ctlRegMsk != ((~dataFromDev[1]) & invWiperNum_ctlRegMsk)>>2 )
    throw devDS2890_error( "Error sanity checking control register value", __FILE__, __LINE__);
  
  LOW_devDS2890::controlRegister_t retValue;
  retValue.controlledWiperNum = dataFromDev[1] & wiperNum_ctlRegMsk;
  retValue.isChargePumpOn     = (dataFromDev[1]&chargePump_ctlRegMsk==chargePump_ctlRegMsk);

  cachedCurrentWiper = retValue.controlledWiperNum;
      
  return retValue;
}


void LOW_devDS2890::cmd_WriteControlRegister( const uint8_t inControlledWiperNum, const bool inChargePumpOn)
{
  linkLock  lock( *this);

  if ( inControlledWiperNum >= getPotentiometerCount() )
    throw devDS2890_error( "Illegal wiper number specified", __FILE__, __LINE__);

  if ( inChargePumpOn && !chargePumpAllowed )
    throw devDS2890_error( "Charge pump requested but not allowed", __FILE__, __LINE__);

  uint8_t controlByte =   (inControlledWiperNum&wiperNum_ctlRegMsk)
                        | (((~inControlledWiperNum)<<2)&invWiperNum_ctlRegMsk)
                        | (inChargePumpOn==true?chargePump_ctlRegMsk:0x00);

  cmd_MatchROM();

  byteVec_t outputBytes;
  outputBytes.push_back( static_cast<uint8_t>( WriteControlRegister_COMMAND));
  outputBytes.push_back( controlByte);
  getLink().writeData( outputBytes);

  uint8_t checkByte = getLink().readDataByte();
  if ( checkByte == 0xff ) {
    getLink().resetBus();
    throw devDS2890_error( "Illegal control byte sent", __FILE__, __LINE__);
  }
  if ( checkByte != controlByte) {
    getLink().resetBus();
    throw devDS2890_error( "Error verifying control byte", __FILE__, __LINE__);
  }

  getLink().writeData( releaseCode_subcommand);

  bool errorBit = getLink().readDataBit();
  if ( errorBit == true ) {
    getLink().resetBus();
    throw devDS2890_error( "Error activating new control setting", __FILE__, __LINE__);
  }

  cachedCurrentWiper = inControlledWiperNum;
}


LOW_devDS2890::wiperPosition_t LOW_devDS2890::cmd_Increment()
{
  linkLock  lock( *this);

  cmd_MatchROM();

  getLink().writeData( Increment_COMMAND);
  cachedWiperPos[cachedCurrentWiper] = getLink().readDataByte();

  getLink().resetBus();  // leave inc/dec mode

  return cachedWiperPos[cachedCurrentWiper];
}


LOW_devDS2890::wiperPosition_t LOW_devDS2890::cmd_Decrement()
{
  linkLock  lock( *this);

  cmd_MatchROM();

  getLink().writeData( Decrement_COMMAND);
  cachedWiperPos[cachedCurrentWiper] = getLink().readDataByte();

  getLink().resetBus();  // leave inc/dec mode

  return cachedWiperPos[cachedCurrentWiper];
}

