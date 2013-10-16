/***************************************************************************
                          test_switch.cpp  -  description
                             -------------------
    begin                : Wed Jan 8 2003
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


#include <typeinfo>


#include "helper.h"
 
#include "LOW_network.h"
#include "LOW_compTwinSwitch.h"
#include "LOW_compToggleSwitch.h"


class tsActionPrint : public LOW_compTwinSwitch::actionReceiver {
public:
  tsActionPrint()
  {
  };
  
  virtual ~tsActionPrint()
  {
  };

  void switchAction( LOW_compTwinSwitch * /*inCaller*/, const unsigned int inSwitchNum, const bool inValue)
  {
    LOW_helper_msglog::printMessage( "SWITCH ACTION: switch num: %d  value: %d\n", inSwitchNum, inValue);
  };

  void shortSwitchAction( LOW_compTwinSwitch * /*inCaller*/, const unsigned int inSwitchNum)
  {
    LOW_helper_msglog::printMessage( "SHORT SWITCH ACTION: switch num: %d\n", inSwitchNum);
  };
};


bool chooseShortDetect()
{
  return hlp_selectBoolOption( "Detect short clicks", false);
}


void activityLoop( LOW_compTwinSwitch &inSwComp)
{
  LOW_devDS2406 theDev = inSwComp.getPioDevice();

  hlp_printDashline();
  printf( "Polling for active PIO device...\n");

  hlp_printDashline();
  while ( true ) {
    if ( theDev.verifyDevice( true) )
      inSwComp.handleAlarm();

    if ( hlp_kbhit() )
      break;
  }
}


void switchMenu( LOW_network &inNet)
{
  LOW_devDS2406         *pioDev        = 0;
  LOW_compTwinSwitch    *switchComp    = 0;
  tsActionPrint         *actionPrinter = new tsActionPrint();
     
  // try preselection for Haralds test setup
  if ( inNet.verifyDevice( LOW_deviceID(0x90000000, 0x249d9712)) ) {
    hlp_printDashline();
    LOW_helper_msglog::printMessage( "Harald's predefined setup: Using predefined DS2406.\n");
    pioDev = inNet.getDevice<LOW_devDS2406>(  LOW_deviceID(0x90000000, 0x249d9712));
  }
    
  bool keepLooping = true;
  while ( keepLooping) {

    try {
      hlp_printDashline();
      printf( "Switch component menu\n");
      printf( "\n");

      if ( ! pioDev ) {
        printf( "  <p> Select PIO device\n");
        printf( "  \n");
        printf( "  <q> Back to main menu\n");
        int key = hlp_getMenuKey( "pq");
        LOW_devDS2406::devDS2406PtrVec_t allPDevs = inNet.getDevices<LOW_devDS2406>();
        switch( key ) {
          case 'p': pioDev = hlp_selectSingleDevice( allPDevs);  break;
          case 'q': keepLooping=false;                           break;
        }
      }

      else if ( pioDev && !switchComp ) {
        printf( "  PIO device: %s\n", pioDev->getID().getRomIDString().c_str());
        printf( "  \n");
        printf( "  <t> Create twin switch component\n");
        printf( "  <g> Create toggle switch component\n");
        printf( "  <x> Clear PIO device selection\n");
        printf( "  \n");
        printf( "  <q> Back to main menu\n");
        int key = hlp_getMenuKey( "tgxq");
        switch( key ) {
          case 't': switchComp = new LOW_compTwinSwitch( *pioDev, *actionPrinter, chooseShortDetect());
                    break;
          case 'g': switchComp = new LOW_compToggleSwitch( *pioDev, *actionPrinter, chooseShortDetect());
                    break;
          case 'x': pioDev = 0;
                    break;
          case 'q': keepLooping=false;
                    break;
        }
      }

      else if ( pioDev &&  switchComp ) {
        printf( "  PIO device: %s\n", pioDev->getID().getRomIDString().c_str());
        printf( "  \n");
        printf( "  %s switch component %s short click detection exists.\n",
                typeid(*switchComp)==typeid(LOW_compTwinSwitch)   ? "Twin"   :
                typeid(*switchComp)==typeid(LOW_compToggleSwitch) ? "Toggle" : "UNKNOWN",
                switchComp->getDetectShortClicks()==true?"with":"without" );
        printf( "  \n");
        printf( "  <l> Activity loop\n");
        printf( "  <x> Destroy switch component\n");
        printf( "  \n");
        printf( "  <q> Back to main menu\n");
        int key = hlp_getMenuKey( "lxq");
        switch( key ) {
          case 'l': activityLoop( *switchComp);          break;
          case 'x': delete switchComp;   switchComp=0;   break;
          case 'q': keepLooping=false;                   break;
        }
      }
    }
    catch( LOW_exception ex) {
      ex.logException();
    }
  }

  if ( switchComp )
    delete switchComp;

  delete actionPrinter;
}

