/*
 * test_ds2413.cpp
 *
 *  Created on: Feb 6, 2013
 *      Author: Josef Lola
 */

#include "LOW_network.h"
#include "LOW_devDS2413.h"
#include "test_ds2413.h"

#include "helper.h"


void getChannelInfo( const LOW_devDS2413::devDS2413PtrVec_t &inDevs, bool inDoLooping)
{
  bool keepLooping = inDoLooping;
  int key;
  do {
    hlp_printDashline();
    for ( unsigned int a=0; a<inDevs.size(); ++a) {
      std::string idStr = inDevs[a]->getID().getRomIDString();
      LOW_helper_msglog::printMessage( "PIO search conditions of DS2413 device: #%d:\n", a);
      LOW_helper_msglog::printMessage( "  ROM ID:                %s\n", idStr.c_str());

      LOW_devDS2413 *dev = inDevs[a];
      uint8_t val;
      dev->cmd_ReadStatus(val);
      LOW_helper_msglog::printMessage( "  PIO A output FlipFlop: %d\n", (val & LOW_devDS2413::PIOAOutLatchStateBit)>0);
   	  LOW_helper_msglog::printMessage( "  PIO B output FlipFlop: %d\n", (val & LOW_devDS2413::PIOBOutLatchStateBit)>0);
   	  LOW_helper_msglog::printMessage( "  PIO A current level:   %d\n", (val & LOW_devDS2413::PIOAPinStateBit)>0);
      LOW_helper_msglog::printMessage( "  PIO B current level:   %d\n", (val & LOW_devDS2413::PIOBPinStateBit)>0);

      LOW_helper_msglog::printMessage( "  Press 'n' to switch tranzistors On, 'q' to exit loop  \n");
      key = hlp_getMenuKey( "nq");
      if (key=='q')  {  keepLooping = false; break; }

      dev->cmd_WritePIO(LOW_devDS2413::pioTransistorOn,LOW_devDS2413::pioTransistorOn);

      dev->cmd_ReadStatus(val);

      LOW_helper_msglog::printMessage( "  PIO A output FlipFlop: %d\n", (val & LOW_devDS2413::PIOAOutLatchStateBit)>0);
   	  LOW_helper_msglog::printMessage( "  PIO B output FlipFlop: %d\n", (val & LOW_devDS2413::PIOBOutLatchStateBit)>0);
  	  LOW_helper_msglog::printMessage( "  PIO A current level:   %d\n", (val & LOW_devDS2413::PIOAPinStateBit)>0);
      LOW_helper_msglog::printMessage( "  PIO B current level:   %d\n", (val & LOW_devDS2413::PIOBPinStateBit)>0);

      LOW_helper_msglog::printMessage( "  Press 'n' to switch tranzistors off, 'q' to exit loop  \n");
      key = hlp_getMenuKey( "nq");
      if (key=='q')  {  keepLooping = false; break; }

      dev->cmd_WritePIO(LOW_devDS2413::pioTransistorOff,LOW_devDS2413::pioTransistorOff);

      dev->cmd_ReadStatus(val);

      LOW_helper_msglog::printMessage( "  PIO A output FlipFlop: %d\n", (val & LOW_devDS2413::PIOAOutLatchStateBit)>0);
	  LOW_helper_msglog::printMessage( "  PIO B output FlipFlop: %d\n", (val & LOW_devDS2413::PIOBOutLatchStateBit)>0);
	  LOW_helper_msglog::printMessage( "  PIO A current level:   %d\n", (val & LOW_devDS2413::PIOAPinStateBit)>0);
      LOW_helper_msglog::printMessage( "  PIO B current level:   %d\n", (val & LOW_devDS2413::PIOBPinStateBit)>0);

      LOW_helper_msglog::printMessage( "  \n");

      if ( keepLooping ) {
    	  LOW_helper_msglog::printMessage( "  Press 'n' to continue, 'q' to exit loop  \n");
    	  key = hlp_getMenuKey( "nq");
    	  if (key=='q')  {  keepLooping = false; break; }
      }
    }
  } while ( keepLooping );
}


void resetLatches( const LOW_devDS2413::devDS2413PtrVec_t &inDevs)
{
  for ( unsigned int a=0; a<inDevs.size(); ++a) {
	  //switch all tranzistors off
	  inDevs[a]->cmd_WritePIO(LOW_devDS2413::pioTransistorOff,
			  	  	  	  	  LOW_devDS2413::pioTransistorOff);
  }
}


void ds2413Menu( LOW_network &inNet)
{
  bool keepLooping = true;
  while ( keepLooping) {
    hlp_printDashline();
    printf( "PIO DS2413 device menu\n");
    printf( "\n");
    printf( "  <g> Get DS2413 devices\n");
    printf( "  <s> Search all DS2413 devices\n");
    printf( "\n");
    printf( "  <i>      Get channel info of DS2413 devices\n");
    printf( "  <l> Loop get channel info of DS2413 devices\n");
    printf( "  <r> Reset latches of DS2413 devices\n");
    printf( "\n");
    printf( "  <q> Back to main menu\n");

    int key = hlp_getMenuKey( "gsailq");

    LOW_devDS2413::devDS2413PtrVec_t allPDevs = inNet.getDevices<LOW_devDS2413>();

    try {
      switch( key ) {
        case 'g': hlp_getDevices<LOW_devDS2413>( inNet);                           break;
        case 's': hlp_searchDevices<LOW_devDS2413>( inNet, false);                 break;
        case 'a': hlp_searchDevices<LOW_devDS2413>( inNet, true);                  break;
        case 'i': getChannelInfo( hlp_selectDevices( allPDevs), false);            break;
        //case 'v': setSearchCondition( hlp_selectDevices( allPDevs));               break;
        case 'l': getChannelInfo( hlp_selectDevices( allPDevs), true);             break;
        case 'r': resetLatches( hlp_selectDevices( allPDevs));                     break;
        case 'q': keepLooping=false;                                               break;
      }
    }
    catch( LOW_exception ex) {
      ex.logException();
    }
  }
}
