/***************************************************************************
                          LOW_compTempSensor.h  -  description
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

#ifndef LOW_COMPTEMPSENSOR_H
#define LOW_COMPTEMPSENSOR_H


#include "LOW_component.h"
#include "LOW_devDS1820.h"



/**
    @todo Documentation of class.

  *@author Harald Roelle
  */
class LOW_compTempSensor : public LOW_component  {

//=======================================================================================
public:
  
  //=====================================================================================
  //
  // exceptions
  //
 
  class_DERIVE_FROM_EXCEPTION( data_error, component_error);

    
  //=====================================================================================
  //
  // constructors
  //
  
  LOW_compTempSensor( const LOW_devDS1820 &inSensorDevice);
  
  ~LOW_compTempSensor();

  
  //=====================================================================================
  //
  // methods
  //
  
  float getTemperature( const bool inDoConversion = true);

  void recallAlarmLevels();

  void getAlarmLevels( int8_t &outLowBound, int8_t &outHighBound);

  void setAlarmLevels( const int8_t inLowBound, const int8_t inHighBound, const bool inPermanent = true);
  
  bool checkAlarm( const bool inRecalcTemp = true);
    
  bool getIsExternalPowered() const;

            
//=======================================================================================
protected:
  
  //=====================================================================================
  //
  // attributes
  //
  
  const LOW_devDS1820  &sensorDevice;

};

#endif
