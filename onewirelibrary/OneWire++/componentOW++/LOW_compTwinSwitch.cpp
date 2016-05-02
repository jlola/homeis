/***************************************************************************
                          LOW_compTwinSwitch.cpp  -  description
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


#include "LOW_compTwinSwitch.h"
#include "LOW_netSegment.h"



//=====================================================================================
//
// constructors
//

LOW_compTwinSwitch::LOW_compTwinSwitch( const LOW_devDS2406 &inPIODevice, actionReceiver &inActionRcv,
                                        const bool inDetectShortClicks) :
  pioDevice( inPIODevice),
  actionRcv( inActionRcv),
  detectShortClicks( inDetectShortClicks)
{
  if ( !pioDevice.getNetSegment().getHasExternalPower() )
    throw compTwinSwitch_error( "Network segment must supply power.", __FILE__, __LINE__);

  if ( ! pioDevice.getHasPioB() )
    switchStatesVec = std::vector<bool>( 2, false);
  else
    switchStatesVec = std::vector<bool>( 1, false);
    
  LOW_devDS2406::cmd_ChannelAccess::channelInfo_t  dummy;
  resetLatches( dummy);

  setAlarmCondition();
}


LOW_compTwinSwitch::~LOW_compTwinSwitch()
{
}



//=====================================================================================
//
// public methods
//

void LOW_compTwinSwitch::handleAlarm()
{
  LOW_devDS2406::cmd_ChannelAccess::channelInfo_t  chInfoRead, chInfoReset;

  // two steps needed here, otherwise latch status cannot be read
  readStatus( chInfoRead);
  resetLatches( chInfoReset);
  
  // switch A
  if ( chInfoRead.activityLatch_pioA )
    // using current status after latch reset (and not the one of read status)
    // enables us to detect a button release during latch reset
    doSwitchAction( 0, chInfoReset.sensedLevel_pioA);

  // switch B if present
  if ( pioDevice.getHasPioB() && chInfoRead.activityLatch_pioB )
    // using current status after latch reset (and not the one of read status)
    // enables us to detect a button release during latch reset
    doSwitchAction( 1, chInfoReset.sensedLevel_pioB);
}


void LOW_compTwinSwitch::alarmPollLoop()
{
  while( ! pioDevice.verifyDevice( true) );
}


bool LOW_compTwinSwitch::getDetectShortClicks()
{
  return detectShortClicks;
}

const LOW_devDS2406& LOW_compTwinSwitch::getPioDevice() const
{
  return pioDevice;
}



//=====================================================================================
//
// private methods
//

void LOW_compTwinSwitch::setAlarmCondition()
{
  pioDevice.setSearchCondition( LOW_devDS2406::chanBothSelect, LOW_devDS2406::latchSelect,
                                LOW_devDS2406::activeHigh,
                                LOW_devDS2406::pioTransistorOff, LOW_devDS2406::pioTransistorOff);
}


//void LOW_compTwinSwitch::readStatus( LOW_devDS2406::cmd_ChannelAccess::channelInfo_t &outChannelInfo)
//{
//  LOW_devDS2406::cmd_ChannelAccess readAccess =
//    LOW_devDS2406::cmd_ChannelAccess( pioDevice,
//                                      LOW_devDS2406::cmd_ChannelAccess::CRC_disable,
//                                      LOW_devDS2406::chanBothSelect,
//                                      LOW_devDS2406::cmd_ChannelAccess::asyncInterleaveMode,
//                                      LOW_devDS2406::cmd_ChannelAccess::noToggleMode,
//                                      LOW_devDS2406::cmd_ChannelAccess::readMode,
//                                      LOW_devDS2406::cmd_ChannelAccess::noResetLatches);
//  outChannelInfo = readAccess.getChannelInfo();
//}


//void LOW_compTwinSwitch::resetLatches( LOW_devDS2406::cmd_ChannelAccess::channelInfo_t &outChannelInfo)
//{
//  LOW_devDS2406::cmd_ChannelAccess clearLatchAccess  =
//    LOW_devDS2406::cmd_ChannelAccess( pioDevice,
//                                      LOW_devDS2406::cmd_ChannelAccess::CRC_disable,
//                                      LOW_devDS2406::chanBothSelect,
//                                      LOW_devDS2406::cmd_ChannelAccess::asyncInterleaveMode,
//                                      LOW_devDS2406::cmd_ChannelAccess::noToggleMode,
//                                      LOW_devDS2406::cmd_ChannelAccess::readMode,
//                                      LOW_devDS2406::cmd_ChannelAccess::resetLatches);
//  outChannelInfo = clearLatchAccess.getChannelInfo();
//}


/*
old logical state | current level | detect short clicks H called procs | new state
==================+===============+=====================H==============+==========
 0                | 0             | 0                   H on, off      | 0
------------------+---------------+---------------------H--------------+----------
 0                | 0             | 1                   H shortClick   | 0
------------------+---------------+---------------------H--------------+----------
 0                | 1             | 0                   H              |
------------------+---------------+---------------------H on           | 1
 0                | 1             | 1                   H              |
------------------+---------------+---------------------H--------------+----------
 1                | 0             | 0                   H              |
------------------+---------------+---------------------H off          | 0
 1                | 0             | 1                   H              |
------------------+---------------+---------------------H--------------+----------
 1                | 1             | 0                   H              |
------------------+---------------+---------------------H off, on      | 1
 1                | 1             | 1                   H              |
------------------+---------------+---------------------H--------------+----------
*/
void LOW_compTwinSwitch::doSwitchAction( const unsigned int inSwitchNum, const bool inSensedLevel)
{
  bool oldLogState = switchStatesVec[inSwitchNum];

  if ( !oldLogState && !inSensedLevel && !detectShortClicks ) {
    switchStatesVec[inSwitchNum] = false;
    actionRcv.switchAction( this, inSwitchNum, true);
    actionRcv.switchAction( this, inSwitchNum, false);
  }

  if ( !oldLogState && !inSensedLevel &&  detectShortClicks ) {
    switchStatesVec[inSwitchNum] = false;
    actionRcv.shortSwitchAction( this, inSwitchNum);
  }

  if ( !oldLogState &&  inSensedLevel ) {
    switchStatesVec[inSwitchNum] = true;
    actionRcv.switchAction( this, inSwitchNum, true);
  }

  if (  oldLogState && !inSensedLevel ) {
    switchStatesVec[inSwitchNum] = false;
    actionRcv.switchAction( this, inSwitchNum, false);
  }

  if (  oldLogState &&  inSensedLevel ) {
    switchStatesVec[inSwitchNum] = true;
    actionRcv.switchAction( this, inSwitchNum, false);
    actionRcv.switchAction( this, inSwitchNum, true);
  }
}



//=====================================================================================
//
// CLASS: actionReceiver
//

LOW_compTwinSwitch::actionReceiver::actionReceiver()
{
}
