/***************************************************************************
                          LOW_compJalousieController.cpp  -  description
                             -------------------
    begin                : Wed Sep 4 2002
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

 
#include <memory>


#include "LOW_compJalousieController.h"

#include "LOW_netSegment.h"
#include "LOW_platformMisc.h"
#include "LOW_thread_Factory.h"



//=====================================================================================
//
// static attributes
//

int  LOW_compJalousieController::jcCounter = 0;



//=====================================================================================
//
// constructors
//

LOW_compJalousieController::LOW_compJalousieController( const LOW_devDS2405 &inPowerControl, 
                                                        const LOW_devDS2405 &inDirectionControl,
                                                        const float inClosingTime, const float inOpeningTime,
                                                        const float inAngleClosingTime, const float inAngleOpeningTime) :
  jcSerialNum( jcCounter++),
  powerControl(inPowerControl),
  directionControl(inDirectionControl),
  closingTime(inClosingTime),
  openingTime(inOpeningTime),
  angleClosingTime(inAngleClosingTime),
  angleOpeningTime(inAngleOpeningTime)
{
  if ( powerControl.getID() == directionControl.getID() )
    throw compJalousieController_error( "Same device for powerControl and directionControl.", __FILE__, __LINE__);

  if ( !powerControl.getNetSegment().getHasExternalPower() || !directionControl.getNetSegment().getHasExternalPower() )
    throw compJalousieController_error( "Network segment must supply power.", __FILE__, __LINE__);
  
  __linkRecMutex = LOW_thread_Factory::new_mutex( LOW_thread_mutex::mutexKind_recursive);
  
  try {
    jcLock lockIt = jcLock( *this);
    
    // make sure power control is off
    if ( powerControl.cmd_SearchActive() == true ) {
      powerControl.cmd_Match(); // toggle output
      if ( powerControl.cmd_SearchActive() == true ) // is it still active?
        throw compJalousieController_error( "Cannot initialize powerControl.", __FILE__, __LINE__);
    }
  
    // make sure direction control is upwards
    if ( directionControl.cmd_SearchActive() == true ) {
      directionControl.cmd_Match(); // toggle output
      if ( directionControl.cmd_SearchActive() == true ) // is it still active?
        throw compJalousieController_error( "Cannot initialize directionControl.", __FILE__, __LINE__);
    }
  }
  catch (...) {
    delete __linkRecMutex;
    throw;
  }
  
  powerIsOn       = false;
  directionIsDown = false;

  stop2upDelay   =  70;
  up2stopDelay   =  70;
  stop2downDelay = 140;
  down2stopDelay = 140;
  up2downDelay   =  70;
  down2upDelay   =  70;
}


LOW_compJalousieController::~LOW_compJalousieController()
{
  try {
    jcLock lockIt = jcLock( *this);    
    stopMove();
  }
  catch (...) {
    delete __linkRecMutex;
    throw;
  }
  
  delete __linkRecMutex;
}



//=====================================================================================
//
// public methods
//

void LOW_compJalousieController::setPosition( float inPos, float inAngle)
{
  jcLock lockIt = jcLock( *this);
  
  if ( inPos<0.0 || inPos>1.0 )
    throw compJalousieController_error( "Illegal position parameter.", __FILE__, __LINE__);
  
  if ( inAngle<0.0 || inAngle>1.0 )
    throw compJalousieController_error( "Illegal angle parameter.", __FILE__, __LINE__);

  // end position in lower half
  if ( inPos > 0.5 ) {

    // move completely down
    moveDownStop( static_cast<unsigned long>((closingTime*1000)+fullMovementAdditionMillis));

    if ( inPos != 1.0 ) {
      // move up to desired position
      moveUpStop( static_cast<unsigned long>(((1.0-inPos)*(openingTime*1000))));
      
      if ( inAngle != 0.0 ) {
        // move down to close
        moveDownStop( static_cast<unsigned long>(inAngle*(angleClosingTime*1000)));
      }
    }
    else if ( inAngle != 1.0 ) {
      //move up to open
      moveUpStop( static_cast<unsigned long>((1.0-inAngle)*(angleOpeningTime*1000)));
    }
  }
  
  // end position in upper half
  else {

    // move completely up
    moveUpStop( static_cast<unsigned long>((openingTime*1000)+fullMovementAdditionMillis));

    if ( inPos != 0.0 ) {
      // move down to desired position
      moveDownStop( static_cast<unsigned long>(inPos*(closingTime*1000)));

      if ( inAngle != 1.0 ) {
        //move up to open
        moveUpStop( static_cast<unsigned long>((1.0-inAngle)*(angleOpeningTime*1000)));
      }
    }
    else if ( inAngle != 0.0 ) {
      // move down to close
      moveDownStop( static_cast<unsigned long>(inAngle*(angleClosingTime*1000)));
    }
  }
}


float LOW_compJalousieController::getClosingTime() const
{
  return closingTime;
}


void LOW_compJalousieController::setClosingTime( const float inClosingTime)
{
  jcLock lockIt = jcLock( *this);
  closingTime = inClosingTime;
}


float LOW_compJalousieController::getOpeningTime() const
{
  return openingTime;
}


void LOW_compJalousieController::setOpeningTime( const float inOpeningTime)
{
  jcLock lockIt = jcLock( *this);
  openingTime = inOpeningTime;
}


float LOW_compJalousieController::getAngleClosingTime() const
{
  return angleClosingTime;
}


void LOW_compJalousieController::setAngleClosingTime( const float inAngleClosingTime)
{
  jcLock lockIt = jcLock( *this);
  angleClosingTime = inAngleClosingTime;
}


float LOW_compJalousieController::getAngleOpeningTime() const
{
  return angleOpeningTime;
}


void LOW_compJalousieController::setAngleOpeningTime( const float inAngleOpeningTime)
{
  jcLock lockIt = jcLock( *this);
  angleOpeningTime = inAngleOpeningTime;
}


void LOW_compJalousieController::measureTransitionDelays()
{
  jcLock lockIt = jcLock( *this);

  const unsigned int             loopCnt = 10;
  std::vector<unsigned int>      t_sp2up, t_up2sp, t_sp2dn, t_dn2sp, t_up2dn, t_dn2up;
  LOW_platformMisc::timestamp_t  t_start, t_stop, t_diff;

  for( unsigned int a=0; a<loopCnt; a++) {
    LOW_platformMisc::getTimestamp( t_start);
    moveUp();
    LOW_platformMisc::getTimestamp( t_stop);
    LOW_platformMisc::diffTimestamp( t_stop, t_start, t_diff);
    t_sp2up.push_back( t_diff.milSec);

    LOW_platformMisc::getTimestamp( t_start);
    stopMove();
    LOW_platformMisc::getTimestamp( t_stop);
    LOW_platformMisc::diffTimestamp( t_stop, t_start, t_diff);
    t_up2sp.push_back( t_diff.milSec);

    LOW_platformMisc::getTimestamp( t_start);
    moveDown();
    LOW_platformMisc::getTimestamp( t_stop);
    LOW_platformMisc::diffTimestamp( t_stop, t_start, t_diff);
    t_sp2dn.push_back( t_diff.milSec);

    LOW_platformMisc::getTimestamp( t_start);
    stopMove();
    LOW_platformMisc::getTimestamp( t_stop);
    LOW_platformMisc::diffTimestamp( t_stop, t_start, t_diff);
    t_dn2sp.push_back( t_diff.milSec);

    moveUp();
    LOW_platformMisc::getTimestamp( t_start);
    moveDown();
    LOW_platformMisc::getTimestamp( t_stop);
    LOW_platformMisc::diffTimestamp( t_stop, t_start, t_diff);
    t_up2dn.push_back( t_diff.milSec);

    moveDown();
    LOW_platformMisc::getTimestamp( t_start);
    moveUp();
    LOW_platformMisc::getTimestamp( t_stop);
    LOW_platformMisc::diffTimestamp( t_stop, t_start, t_diff);
    t_dn2up.push_back( t_diff.milSec);

    stopMove();
  }

  stop2upDelay   = averageMillis( t_sp2up);
  up2stopDelay   = averageMillis( t_up2sp);
  stop2downDelay = averageMillis( t_sp2dn);
  down2stopDelay = averageMillis( t_dn2sp);
  up2downDelay   = averageMillis( t_up2dn);
  down2upDelay   = averageMillis( t_dn2up);
}


void LOW_compJalousieController::getTransitionDelays( unsigned int &outStop2upDelay, unsigned int &outUp2stopDelay,
                                                      unsigned int &outStop2downDelay, unsigned int &outDown2stopDelay,
                                                      unsigned int &outUp2downDelay, unsigned int &outDown2upDelay)
{
  outStop2upDelay   = stop2upDelay;
  outUp2stopDelay   = up2stopDelay;
  outStop2downDelay = stop2downDelay;
  outDown2stopDelay = down2stopDelay;
  outUp2downDelay   = up2downDelay;
  outDown2upDelay   = down2upDelay;
}


void LOW_compJalousieController::setTransitionDelays( const unsigned int inStop2upDelay, const unsigned int inUp2stopDelay,
                                                      const unsigned int inStop2downDelay, const unsigned int inDown2stopDelay,
                                                      const unsigned int inUp2downDelay, const unsigned int inDown2upDelay)
{
  jcLock lockIt = jcLock( *this);

  stop2upDelay   = inStop2upDelay;
  up2stopDelay   = inUp2stopDelay;
  stop2downDelay = inStop2downDelay;
  down2stopDelay = inDown2stopDelay;
  up2downDelay   = inUp2downDelay;
  down2upDelay   = inDown2upDelay;
}



//=====================================================================================
//
// protected methods
//
  
void LOW_compJalousieController::moveUp()
{
  // internal function, no locking here

  setDirectionUp();
  setPowerOn();
}


void LOW_compJalousieController::moveDown()
{
  // internal function, no locking here
  
  setDirectionDown();
  setPowerOn();
}


void LOW_compJalousieController::stopMove()
{
  // internal function, no locking here

  setPowerOff();
  setDirectionUp();
}


void LOW_compJalousieController::moveUpStop( const unsigned long inMillis)
{
  // internal function, no locking here

  unsigned long  sleepTime;
  if ( ( (static_cast<long>(inMillis)) - (static_cast<int>(up2stopDelay)) ) < 0 )
    sleepTime = 0;
  else
    sleepTime = inMillis-up2stopDelay;
    
  moveUp();
  LOW_platformMisc::milliSleep( sleepTime);
  stopMove();
}


void LOW_compJalousieController::moveDownStop( const unsigned long inMillis)
{
  // internal function, no locking here

  unsigned long  sleepTime;
  if ( ( (static_cast<long>(inMillis)) - (static_cast<int>(up2stopDelay)) ) < 0 )
    sleepTime = 0;
  else
    sleepTime = inMillis-down2stopDelay;

  moveDown();
  LOW_platformMisc::milliSleep( sleepTime);
  stopMove();
}


  
//=====================================================================================
//
// private methods
//

void LOW_compJalousieController::setPowerOn()
{
  if ( powerIsOn )
    return;
  
  if ( powerControl.cmd_MatchRead() == true )
    throw compJalousieController_error( "Error setting powerControl.", __FILE__, __LINE__);
  
  powerIsOn = true;
}


void LOW_compJalousieController::setPowerOff()
{
  if ( ! powerIsOn )
    return;
  
  if ( powerControl.cmd_MatchRead() == false )
    throw compJalousieController_error( "Error setting powerControl.", __FILE__, __LINE__);
  
  powerIsOn = false;
}


void LOW_compJalousieController::setDirectionDown()
{
  if ( directionIsDown )
    return;
  
  if ( directionControl.cmd_MatchRead() == true )
    throw compJalousieController_error( "Error setting directionControl.", __FILE__, __LINE__);
  
  directionIsDown = true;
}


void LOW_compJalousieController::setDirectionUp()
{
  if ( ! directionIsDown )
    return;

  if ( directionControl.cmd_MatchRead() == false )
    throw compJalousieController_error( "Error setting directionControl.", __FILE__, __LINE__);
  
  directionIsDown = false;
}


unsigned int LOW_compJalousieController::averageMillis( const std::vector<unsigned int> &inVals)
{
  unsigned long avg = 0;
  for ( unsigned int a=0; a<inVals.size(); a++)
    avg += inVals[a];
  avg /= inVals.size();

  return avg;
}



//=====================================================================================
//
// CLASS: manualMove
//

LOW_compJalousieController::manualMove::manualMove( LOW_compJalousieController &inJCComp) :
  jcLock( inJCComp),
  jcComp( inJCComp)
{
}


LOW_compJalousieController::manualMove::~manualMove()
{
  stopMove();
}


void LOW_compJalousieController::manualMove::moveUp()
{
  jcComp.moveUp();
}


void LOW_compJalousieController::manualMove::moveDown()
{
  jcComp.moveDown();
}


void LOW_compJalousieController::manualMove::stopMove()
{
  jcComp.stopMove();
}


