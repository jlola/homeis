/***************************************************************************
                          LOW_compTempSensor.cpp  -  description
                             -------------------
    begin                : Fri Jul 12 2002
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

#include <stdio.h>
 
#include "LOW_compTempSensor.h"


//=====================================================================================
//
// constructors
//
  
LOW_compTempSensor::LOW_compTempSensor( const LOW_devDS1820 &inSensorDevice) :
  sensorDevice( inSensorDevice)
{
  recallAlarmLevels();
}


LOW_compTempSensor::~LOW_compTempSensor()
{
}



//=====================================================================================
//
// methods
//
  
float LOW_compTempSensor::getTemperature( const bool inDoConversion)
{
  LOW_devDS1820::scratchpadDS1820_t  scratchpad;
    
  for( int a=0; a<3; a++) {
    if ( inDoConversion )
      sensorDevice.cmd_ConvertT();
    
    sensorDevice.cmd_ReadScratchpad( &scratchpad);
    if ( ! (scratchpad.tempLSB==0xaa && scratchpad.tempMSB==0x00) )
      break;
  }    
  if ( scratchpad.tempLSB==0xaa && scratchpad.tempMSB==0x00 )
    throw data_error( "Illeagal data in scratchpad", __FILE__, __LINE__);

  //int16_t halfDegBit = scratchpad.tempLSB & 0x01;
  int16_t intPart    = (scratchpad.tempMSB<<8) | (scratchpad.tempMSB==0x00?0x00:0x80) | (scratchpad.tempLSB>>1);
  float   floatPart  = -0.25 + (static_cast<float>(scratchpad.cntPerCelsius-scratchpad.cntRemain))/(static_cast<float>(scratchpad.cntPerCelsius));
    
  return (static_cast<float>(intPart))+floatPart;
}


void LOW_compTempSensor::recallAlarmLevels()
{
  sensorDevice.cmd_RecallE2();
}

  
void LOW_compTempSensor::getAlarmLevels( int8_t &outLowBound, int8_t &outHighBound)
{
  LOW_devDS1820::scratchpadDS1820_t  scratchpad;
    
  sensorDevice.cmd_ReadScratchpad( &scratchpad);
  
  outLowBound  = (scratchpad.tL&0x7f) * ((scratchpad.tL&0x80)?-1:1);
  outHighBound = (scratchpad.tH&0x7f) * ((scratchpad.tH&0x80)?-1:1);
}

  
void LOW_compTempSensor::setAlarmLevels( const int8_t inLowBound, const int8_t inHighBound, const bool inPermanent)
{
  if ( (inLowBound <= -55) || (inLowBound >= inHighBound) || (inHighBound >= 125) )
    throw data_error( "Illegal range given. Must be -55 < lowBound < highBound < 125.", __FILE__, __LINE__);
  
  uint8_t tl = (inLowBound<0)  ? ((-inLowBound&0x7f)  | 0x80) : (inLowBound&0x7f);
  uint8_t th = (inHighBound<0) ? ((-inHighBound&0x7f) | 0x80) : (inHighBound&0x7f);
  
  sensorDevice.cmd_WriteScratchpad( tl, th);
  
  if ( inPermanent ) {
    sensorDevice.cmd_CopyScratchpad();
  }
}


bool LOW_compTempSensor::checkAlarm( const bool inRecalcTemp)
{
  if (inRecalcTemp)
    sensorDevice.cmd_ConvertT();
  
  return sensorDevice.verifyDevice( true);
}


bool LOW_compTempSensor::getIsExternalPowered() const
{
  return sensorDevice.getIsExternalPowered();
}

