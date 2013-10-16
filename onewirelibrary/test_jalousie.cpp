/***************************************************************************
                          test_jalousie.cpp  -  description
                             -------------------
    begin                : Fri Jan 3 2003
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

#include "LOW_network.h"
#include "LOW_devDS2405.h"
#include "LOW_compJalousieController.h"
#include "LOW_compToggleSwitch.h"


#include "helper.h"


class tsJalousieMoveAction : public LOW_compTwinSwitch::actionReceiver {
public:

  tsJalousieMoveAction( LOW_compJalousieController &inJcComp) : jcComp( inJcComp)
  {
  };
  
  virtual ~tsJalousieMoveAction()
  {
  };

  void switchAction( LOW_compTwinSwitch */*inCaller*/, const unsigned int inSwitchNum, const bool inValue)
  {
    if ( inValue ) {
      manualJalMover = new LOW_compJalousieController::manualMove( jcComp);
      if ( inSwitchNum == 0 ) {
        LOW_helper_msglog::printMessage( "Moving jalousie up until switch released...\n");
        manualJalMover->moveUp();
      }
      else {
        LOW_helper_msglog::printMessage( "Moving jalousie down until switch released...\n");
        manualJalMover->moveDown();
      }
    }
    else {
      LOW_helper_msglog::printMessage( "...Jalousie stopped\n");
      delete manualJalMover;
    }
  };

  void shortSwitchAction( LOW_compTwinSwitch */*inCaller*/, const unsigned int inSwitchNum)
  {
    if ( inSwitchNum == 0 ) {
      LOW_helper_msglog::printMessage( "Moving jalousie completely up...\n");
      jcComp.setPosition( 0.0, 0.0);
      LOW_helper_msglog::printMessage( "...done.\n");
    }
    else {
      LOW_helper_msglog::printMessage( "Moving jalousie completely down...\n");
      jcComp.setPosition( 1.0, 1.0);
      LOW_helper_msglog::printMessage( "...done.\n");
    }
  };

private:
  LOW_compJalousieController              &jcComp;
  LOW_compJalousieController::manualMove  *manualJalMover;
};



void setPosition( LOW_compJalousieController &inJcComp)
{
  float pos, angle;
  hlp_printDashline();
  printf( "Enter position value (0 - 1.0) -> ");  scanf( "%f", &pos);
  printf( "Enter angle value (0 - 1.0)    -> ");  scanf( "%f", &angle);
  inJcComp.setPosition( pos, angle);
}


void manualMove( LOW_compJalousieController &inJcComp)
{
  // Build manual mover class. This will obtain a lock inside the controller component class
  LOW_compJalousieController::manualMove  *manualJalMover = new LOW_compJalousieController::manualMove( inJcComp);

  bool keepLooping = true;
  int  key         = 0;
  while ( keepLooping) {
    hlp_printDashline();
    printf( "Jalousie manual control menu\n");
    printf( "\n");
    printf( "Jalousie current status: ");
      if      ( key=='u' ) printf( "MOVING UP\n");
      else if ( key=='d' ) printf( "MOVING DOWN\n");
      else                 printf( "STOPPED\n");
    printf( "\n");
    printf( "      <u> Up\n");
    printf( "      <d> Down\n");
    printf( "  <space> Stop\n");
    printf( "\n");
    printf( "      <q> Back to jalousie menu\n");

    key = hlp_getMenuKey( "ud q");

    switch( key ) {
      case 'u':  manualJalMover->moveUp();    break;
      case 'd':  manualJalMover->moveDown();  break;
      case ' ':  manualJalMover->stopMove();  break;
      case 'q':  keepLooping = false;         break;
    }
  }

  // delete mover object and release the lock
  delete manualJalMover;
}


void manualMoveSwitch( LOW_network &inNet, LOW_compJalousieController &inJcComp)
{
  LOW_devDS2406         *pioDev        = 0;
  LOW_compToggleSwitch  *toggleSwComp  = 0;
  tsJalousieMoveAction  *actionMover   = new tsJalousieMoveAction( inJcComp);

  // try preselection for Haralds test setup
  if ( inNet.verifyDevice( LOW_deviceID(0x90000000, 0x249d9712)) ) {
    hlp_printDashline();
    LOW_helper_msglog::printMessage( "Harald's predefined setup: Selecting predefined DS2406.\n");
    pioDev = inNet.getDevice<LOW_devDS2406>(  LOW_deviceID(0x90000000, 0x249d9712));
  }
    
  if ( !pioDev ) {
    LOW_devDS2406::devDS2406PtrVec_t allPDevs = inNet.getDevices<LOW_devDS2406>();
    pioDev = hlp_selectSingleDevice( allPDevs);
  }

  toggleSwComp = new LOW_compToggleSwitch( *pioDev, *actionMover, true);

  hlp_printDashline();
  LOW_helper_msglog::printMessage( "Press switches to move jalousie...\n");

  hlp_printDashline();
  while ( true ) {
    if ( pioDev->verifyDevice( true) )
      toggleSwComp->handleAlarm();

    if ( hlp_kbhit() )
      break;
  }
  delete toggleSwComp;
  delete actionMover;
}


void calibrate( LOW_compJalousieController &inJcComp)
{
  LOW_platformMisc::timestamp_t  t_start, t_stop, t_diff;
  float                          t_open, t_close, t_angleOpen, t_angleClose;
    
  // Build manual mover class. This will obtain a lock inside the controller component class
  LOW_compJalousieController::manualMove  *manualJalMover = new LOW_compJalousieController::manualMove( inJcComp);

  hlp_printDashline();

  printf( "Moving up. Press any key when completely up.\n");
  manualJalMover->moveUp();
  hlp_getch( false, true);
  manualJalMover->stopMove();

  printf( "\n");
  printf( "Now closing time will be measured.\n");
  printf( "ATTENTION: Press any key immediately when moved down and motor has stopped.\n");
  printf( "Press any key to start.\n");
  hlp_getch( false, true);
  manualJalMover->moveDown();
  LOW_platformMisc::getTimestamp( t_start);
  hlp_getch( false, true);
  LOW_platformMisc::getTimestamp( t_stop);
  manualJalMover->stopMove();
  LOW_platformMisc::diffTimestamp( t_stop, t_start, t_diff);
  t_close = static_cast<float>(t_diff.sec)+(static_cast<float>(t_diff.milSec)/1000.0);

  printf( "\n");
  printf( "Moving to a new position. Please wait.\n");
  manualJalMover->moveUp();
  LOW_platformMisc::secSleep( static_cast<unsigned int>(t_close/2.0));
  manualJalMover->stopMove();

  printf( "\n");
  printf( "Now angle open/close times will be measured\n");

  printf( "\n");
  printf( "Now measuring angle closing time.\n");
  printf( "ATTENTION: Press any key immediately when jalousie starts to move.\n");
  printf( "Press any key to start.\n");
  hlp_getch( false, true);
  manualJalMover->moveDown();
  LOW_platformMisc::getTimestamp( t_start);
  hlp_getch( false, true);
  LOW_platformMisc::getTimestamp( t_stop);
  manualJalMover->stopMove();
  LOW_platformMisc::diffTimestamp( t_stop, t_start, t_diff);
  t_angleClose = static_cast<float>(t_diff.sec)+(static_cast<float>(t_diff.milSec)/1000.0);

  printf( "\n");
  printf( "Now measuring angle opening time time.\n");
  printf( "ATTENTION: Press any key immediately when jalousie starts to move.\n");
  printf( "Press any key to start.\n");
  hlp_getch( false, true);
  manualJalMover->moveUp();
  LOW_platformMisc::getTimestamp( t_start);
  hlp_getch( false, true);
  LOW_platformMisc::getTimestamp( t_stop);
  manualJalMover->stopMove();
  LOW_platformMisc::diffTimestamp( t_stop, t_start, t_diff);
  t_angleOpen = static_cast<float>(t_diff.sec)+(static_cast<float>(t_diff.milSec)/1000.0);
   
  printf( "\n");
  printf( "Moving to a new position. Please wait.\n");
  manualJalMover->moveDown();
  LOW_platformMisc::secSleep( static_cast<unsigned int>(t_close+0.5));
  manualJalMover->stopMove();

  printf( "\n");
  printf( "Now opening time will be measured.\n");
  printf( "ATTENTION: Press any key immediately when moved down and motor has stopped.\n");
  printf( "Press any key to start.\n");
  hlp_getch( false, true);
  manualJalMover->moveUp();
  LOW_platformMisc::getTimestamp( t_start);
  hlp_getch( false, true);
  LOW_platformMisc::getTimestamp( t_stop);
  manualJalMover->stopMove();
  LOW_platformMisc::diffTimestamp( t_stop, t_start, t_diff);
  t_open = static_cast<float>(t_diff.sec)+(static_cast<float>(t_diff.milSec)/1000.0);

  delete manualJalMover;

  inJcComp.setClosingTime( t_close);
  inJcComp.setOpeningTime( t_open);
  inJcComp.setAngleClosingTime( t_angleClose);
  inJcComp.setAngleOpeningTime( t_angleOpen);
         
  printf( "\n");
  printf( "Jalousie calibrated!\n");
}


void cmdDelayMeasuremnt( LOW_compJalousieController &inJcComp)
{
  unsigned int       t_sp2up, t_up2sp, t_sp2dn, t_dn2sp, t_up2dn, t_dn2up;

  if ( hlp_selectBoolOption( "DO NOT DO THIS TEST WITH ATTACHED RELAYS! Continue?", false) == false )
    return;

  inJcComp.measureTransitionDelays();
  inJcComp.getTransitionDelays( t_sp2up, t_up2sp, t_sp2dn, t_dn2sp, t_up2dn, t_dn2up);
                            
  hlp_printDashline();
  LOW_helper_msglog::printMessage( "    +==== %4.3f ====> +-----------+     \n", t_sp2up/1000.0);
  LOW_helper_msglog::printMessage( "    |                 | MOVING UP |     \n");
  LOW_helper_msglog::printMessage( "    |   +=== %4.3f == +-----------+     \n", t_up2sp/1000.0);
  LOW_helper_msglog::printMessage( "    |   V                  ^  |         \n");
  LOW_helper_msglog::printMessage( " +---------+               |  |         \n");
  LOW_helper_msglog::printMessage( " | STOPPED |         %4.3f |  | %4.3f   \n", t_dn2up/1000.0, t_up2dn/1000.0);
  LOW_helper_msglog::printMessage( " +---------+               |  |         \n");
  LOW_helper_msglog::printMessage( "    |   ^                  |  V         \n");
  LOW_helper_msglog::printMessage( "    |   +=== %4.3f == +-------------+   \n", t_dn2sp/1000.0);
  LOW_helper_msglog::printMessage( "    |                 | MOVING DOWN |   \n");
  LOW_helper_msglog::printMessage( "    +==== %4.3f ====> +-------------+   \n", t_sp2dn/1000.0);
}


void jalousieMenu( LOW_network &inNet)
{
  LOW_devDS2405               *powerCtl     = 0;
  LOW_devDS2405               *directionCtl = 0;
  LOW_compJalousieController  *jcComp       = 0;

  // try preselection for Haralds test setup
  if ( inNet.verifyDevice( LOW_deviceID(0x27000000, 0x16c86005)) && inNet.verifyDevice( LOW_deviceID(0xfd000000, 0x16c09905)) ) {
    hlp_printDashline();
    LOW_helper_msglog::printMessage( "Harald's predefined setup: Using predefined DS2405 devices.\n");
    powerCtl     = inNet.getDevice<LOW_devDS2405>( LOW_deviceID(0x27000000, 0x16c86005));
    directionCtl = inNet.getDevice<LOW_devDS2405>( LOW_deviceID(0xfd000000, 0x16c09905));
    jcComp       = new LOW_compJalousieController( *powerCtl, *directionCtl, 10.0, 10.0, 1.5, 1.5);
  }

  bool keepLooping = true;
  while ( keepLooping) {

    try {

      if ( powerCtl && directionCtl && ! jcComp ) {
        try {
          jcComp = new LOW_compJalousieController( *powerCtl, *directionCtl, 10.0, 10.0, 1.5, 1.5);
        }
        catch( LOW_exception ex) {
          hlp_printDashline();
          ex.logException();
          powerCtl     = 0;
          directionCtl = 0;
        }
      }

      hlp_printDashline();
      printf( "Jalousie component menu\n");
      printf( "\n");

      if ( !jcComp ) {
        if ( powerCtl )      printf( "  Power control device:     %s\n", powerCtl->getID().getRomIDString().c_str());
        else                 printf( "  <p> Select power control device\n");

        if ( directionCtl )  printf( "  Direction control device: %s\n", directionCtl->getID().getRomIDString().c_str());
        else                 printf( "  <d> Select direction control device\n");

        printf( "  \n");
        printf( "  <q> Back to main menu\n");
        int key = hlp_getMenuKey( "dpq");
        LOW_devDS2405::devDS2405PtrVec_t allPDevs = inNet.getDevices<LOW_devDS2405>();
        switch( key ) {
          case 'p': powerCtl = hlp_selectSingleDevice( allPDevs);      break;
          case 'd': directionCtl = hlp_selectSingleDevice( allPDevs);  break;
          case 'q': keepLooping=false;                                 break;
        }
      }

      else if ( jcComp ) {
        printf( "  <s> Set position\n");
        printf( "  <m> Manual move\n");
        printf( "  <n> Manual move by switch\n");
        printf( "  \n");
        printf( "  <c> Calibrate move times\n");
        printf( "  <d> Command delay measuremnt\n");
        printf( "  <g> Set closing time, now: %f s\n", jcComp->getClosingTime());
        printf( "  <h> Set opening time, now: %f s\n", jcComp->getOpeningTime());
        printf( "  <v> Set angle closing time, now: %f s\n", jcComp->getAngleClosingTime());
        printf( "  <b> Set angle opening time, now: %f s\n", jcComp->getAngleOpeningTime());
        printf( "  \n");
        printf( "  <x> Clear device selection\n");
        printf( "  \n");
        printf( "  <q> Back to main menu\n");
        int key = hlp_getMenuKey( "smncdghvbxq");
        float t;
        switch( key ) {
          case 's': setPosition( *jcComp);                 break;
          case 'm': manualMove( *jcComp);                  break;
          case 'n': manualMoveSwitch( inNet, *jcComp);     break;
          case 'c': calibrate( *jcComp);                   break;
          case 'd': cmdDelayMeasuremnt( *jcComp);          break;
          case 'g': hlp_printDashline();
                    printf( "Enter closing time: ");  scanf( "%f", &t);
                    jcComp->setClosingTime( t);
                    break;
          case 'h': hlp_printDashline();
                    printf( "Enter opening time: ");  scanf( "%f", &t);
                    jcComp->setOpeningTime( t);
                    break;
          case 'v': hlp_printDashline();
                    printf( "Enter angle closing time: ");  scanf( "%f", &t);
                    jcComp->setAngleClosingTime( t);
                    break;
          case 'b': hlp_printDashline();
                    printf( "Enter angle opening time: ");  scanf( "%f", &t);
                    jcComp->setAngleOpeningTime( t);
                    break;
          case 'x': delete jcComp;
                    jcComp       = 0;
                    powerCtl     = 0;
                    directionCtl = 0;
                    break;
          case 'q': keepLooping=false;
                    break;
        }
      }
    }
    catch( LOW_exception ex) {
      ex.logException();
    }
  }

  if ( jcComp )
    delete jcComp;
}

