/***************************************************************************
                          LOW_compTwinSwitch.h  -  description
                             -------------------
    begin                : Sat Jan 4 2003
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

#ifndef LOW_COMPTWINSWITCH_H
#define LOW_COMPTWINSWITCH_H


#include "LOW_component.h"
#include "LOW_devDS2406.h"


/**
    @todo Documentation of class.

  *@author Harald Roelle
  */
class LOW_compTwinSwitch : public LOW_component  {

//=======================================================================================
public: 

  //=====================================================================================
  //
  // exceptions
  //

  class_DERIVE_FROM_EXCEPTION( compTwinSwitch_error, component_error);



  //=====================================================================================
  //
  // type definitions
  //

  typedef std::vector<LOW_compTwinSwitch*> compTwinSwitchPtrVec_t;


  
  //=====================================================================================
  //
  // classes
  //

  class actionReceiver {
  //---------------------------------------------------------------------------------------
  public:
    //---------------------------------------------------------------------------------------
    // constructors
    actionReceiver();

    //---------------------------------------------------------------------------------------
    // methods
    virtual void switchAction( LOW_compTwinSwitch *inCaller, const unsigned int inSwitchNum, const bool inValue) = 0;
    virtual void shortSwitchAction( LOW_compTwinSwitch *inCaller, const unsigned int inSwitchNum) = 0;
  };


  
  //=====================================================================================
  //
  // constructors
  //

  LOW_compTwinSwitch( const LOW_devDS2406 &inPIODevice, actionReceiver &inActionRcv, const bool inDetectShortClicks);

  virtual ~LOW_compTwinSwitch();



  //=====================================================================================
  //
  // methods
  //

  virtual void handleAlarm();

  void alarmPollLoop();

  bool getDetectShortClicks();

  const LOW_devDS2406& getPioDevice() const;

  
//=======================================================================================
private:

  //=====================================================================================
  //
  // attributes
  //

  const LOW_devDS2406  &pioDevice;
  actionReceiver       &actionRcv;
  const bool           detectShortClicks;
  std::vector<bool>    switchStatesVec;



  //=====================================================================================
  //
  // methods
  //

  void setAlarmCondition();

  void readStatus(LOW_devDS2406::cmd_ChannelAccess::channelInfo_t &outChannelInfo);

  void resetLatches( LOW_devDS2406::cmd_ChannelAccess::channelInfo_t &outChannelInfo);

  void doSwitchAction( const unsigned int inSwitchNum, const bool inSensedLevel);
};

#endif


