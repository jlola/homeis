/***************************************************************************
                          LOW_compToggleSwitch.cpp  -  description
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

#include "LOW_compToggleSwitch.h"



//=====================================================================================
//
// constructors
//

LOW_compToggleSwitch::LOW_compToggleSwitch( const LOW_devDS2406 &inPIODevice, actionReceiver &inActionRcv,
                                            const bool inDetectShortClicks) :
  LOW_compTwinSwitch( inPIODevice, *this, inDetectShortClicks),
  toggleActionRcv( inActionRcv)
{
  toggleState = noLock_state;
}


LOW_compToggleSwitch::~LOW_compToggleSwitch()
{
}



//=====================================================================================
//
// private methods
//

void LOW_compToggleSwitch::switchAction( LOW_compTwinSwitch */*inCaller*/, const unsigned int inSwitchNum, const bool inValue)
{
  switch ( toggleState ) {

    case noLock_state:
      if ( inValue == false )
        break;
        
      lockedSwitch = inSwitchNum;
      toggleState =  locked_state;

      toggleActionRcv.switchAction( this, inSwitchNum, inValue);

      break;
      
    case locked_state:
      if ( inSwitchNum != lockedSwitch )
        break;

      if ( inValue == true )
        break;

      toggleState =  noLock_state;

      toggleActionRcv.switchAction( this, inSwitchNum, inValue);

      break;
  }
}


void LOW_compToggleSwitch::shortSwitchAction( LOW_compTwinSwitch */*inCaller*/, const unsigned int inSwitchNum)
{
  if ( toggleState != noLock_state )
    return;

  toggleActionRcv.shortSwitchAction( this, inSwitchNum);
}
