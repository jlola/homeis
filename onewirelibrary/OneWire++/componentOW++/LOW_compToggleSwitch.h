/***************************************************************************
                          LOW_compToggleSwitch.h  -  description
                             -------------------
    begin                : Sun Jan 5 2003
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

#ifndef LOW_COMPTOGGLESWITCH_H
#define LOW_COMPTOGGLESWITCH_H


#include "LOW_compTwinSwitch.h"


/**
    @todo Documentation of class.

  *@author Harald Roelle
  */
class LOW_compToggleSwitch : public LOW_compTwinSwitch, public LOW_compTwinSwitch::actionReceiver {

//=======================================================================================
public:

  //=====================================================================================
  //
  // exceptions
  //

  class_DERIVE_FROM_EXCEPTION( compToggleSwitch_error, component_error);



  //=====================================================================================
  //
  // type definitions
  //

  typedef std::vector<LOW_compToggleSwitch*> compToggleSwitchPtrVec_t;
  
  typedef LOW_compTwinSwitch::actionReceiver actionReceiver;

  

  //=====================================================================================
  //
  // constructors
  //

  LOW_compToggleSwitch( const LOW_devDS2406 &inPIODevice, actionReceiver &inActionRcv,
                        const bool inDetectShortClicks);

  virtual ~LOW_compToggleSwitch();



//=======================================================================================
private:

  //=====================================================================================
  //
  // type definitions
  //

  typedef enum { noLock_state=0, locked_state} tgSwState_t;
  

  
  //=====================================================================================
  //
  // attributes
  //

  actionReceiver   &toggleActionRcv;
  tgSwState_t      toggleState;
  unsigned int     lockedSwitch;

  
  //=====================================================================================
  //
  // methods
  //

  // implementing action receiver
  void switchAction( LOW_compTwinSwitch *inCaller, const unsigned int inSwitchNum, const bool inValue);
  
  // implementing action receiver
  void shortSwitchAction( LOW_compTwinSwitch *inCaller, const unsigned int inSwitchNum);
};

#endif
