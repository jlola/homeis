/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Fri Jul 26 15:42:10 CEST 2002
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include "LOW_network.h"
#include "LOW_linkPassiveSerial.h"
#include "LOW_linkDS2480B.h"
#include "LOW_linkDS2490.h"
#include "LOW_helper_msglog.h"
#include "LOW_objectSynchronizer.h"


#include "LOW_devDS1820.h"
#include "LOW_compTempSensor.h"

#include "helper.h"
#include "test_link.h"
#include "test_net.h"
#include "test_temp.h"
#include "test_ds2405.h"
#include "test_ds2406.h"
#include "test_ds2890.h"
#include "test_ds2413.h"
#include "test_switch.h"
#include "test_jalousie.h"
#include "test_thread.h"
#include "test_lock.h"
#include "test_msgLogging.h"



int main3( int /*argc*/, char** /*argv*/)
{
  int                     exitStatus = EXIT_SUCCESS;
  LOW_link::linkPtrVec_t  oneWireLinks;

  try {
        
    //
    // Automatically log any exception when it is created
    //
    //LOW_exception::setLogOnCreation( true);

    // if debugging is enabled then enable according log level
#ifdef DEBUG_LOCKING
    //LOW_helper_msglog::setDebugLevelEnabled( LOW_helper_msglog::objSync_getLock_dl);
    //LOW_helper_msglog::setDebugLevelEnabled( LOW_helper_msglog::objSync_lockFailed_dl);
#endif

    //
    // Uncomment to activate various debugging levels
    //
    //LOW_helper_msglog::setDebugLevelEnabled( LOW_helper_msglog::portSerial_dl);
    //LOW_helper_msglog::setDebugLevelEnabled( LOW_helper_msglog::devDS1820_dl);
    //LOW_helper_msglog::setDebugLevelEnabled( LOW_helper_msglog::linkLock_dl);
    //LOW_helper_msglog::setDebugLevelEnabled( LOW_helper_msglog::linkDS2490_dl);

    //
    // This is our whole network
    //
    LOW_helper_msglog::printMessage( "Creating network class.\n");
    LOW_network  oneWireNet;
    std::string hostName = LOW_platformMisc::getHostname();
    // predefined setup for Harald's machine
    if ( hostName == "raspberrypi" ) {

      // stuff for the active adapter
//      LOW_linkDS2480B        *activeLink = 0;
//      try {
//        hlp_printDashline();
//        LOW_helper_msglog::printMessage( "Harald's predefined setup: Adding active adapter to network.\n");
//        LOW_portSerialFactory::portSpecifier_t  ttyS0 = LOW_portSerialFactory::portSpecifier_t( "/dev/ttyAMA0");
//        activeLink = new LOW_linkDS2480B( ttyS0, LOW_linkDS2480B::RXPOL_INV, true, true);
//        oneWireNet.addLink( activeLink);
//        oneWireLinks.push_back( activeLink);
//      }
//      catch( LOW_exception ex) {
//        ex.logException();
//        if ( activeLink ) delete activeLink;
//      }

//      // stuff for the passive adapter
      LOW_linkPassiveSerial  *passiveLink = 0;
      try {
        hlp_printDashline();
        LOW_helper_msglog::printMessage( "Harald's predefined setup: Adding passive adapter to network.\n");
        LOW_portSerialFactory::portSpecifier_t  ttyS1 = LOW_portSerialFactory::portSpecifier_t( "/dev/ttyAMA0");
        passiveLink = new LOW_linkPassiveSerial( ttyS1);
        oneWireNet.addLink( passiveLink);
        //LOW_devDS1820::devDS1820PtrVec_t allTDevs = oneWireNet.getDevices<LOW_devDS1820>();
        //getInfoDS1820( hlp_selectDevices( allTDevs));
        //tempMenu( oneWireNet);

        oneWireLinks.push_back( passiveLink);
      }
      catch( LOW_exception ex) {
        ex.logException();
        if ( passiveLink ) delete passiveLink;
      }


      // stuff for the USB adapter
//      try {
//        hlp_printDashline();
//        LOW_helper_msglog::printMessage( "Harald's predefined setup: Adding all USB adapters to network.\n");
//
//        LOW_portUsb_Factory::usbDevSpecVec_t  specifierList = LOW_portUsb_Factory::getPortSpecifiers( LOW_linkDS2490::usbVendorID, LOW_linkDS2490::usbProductID);
//        for( unsigned int a=0; a<specifierList.size(); a++) {
//          LOW_linkDS2490  *usbLink = 0;
//          try {
//            usbLink = new LOW_linkDS2490( specifierList[a], true, true);
//            oneWireNet.addLink( usbLink);
//            oneWireLinks.push_back( usbLink);
//          }
//          catch( LOW_exception ex) {
//            ex.logException();
//            if ( usbLink ) delete usbLink;
//          }
//        }
//      }
//      catch( LOW_exception ex) {
//        ex.logException();
//      }

    }

        
    bool keepLooping = true;
    while ( keepLooping) {
      hlp_printDashline();
      printf( "OneWire++ Library Testing Main Menu\n");
      printf( "\n");
      printf( "  <l> Links\n");
      printf( "\n");
      printf( "  <n> Network\n");
      printf( "\n");
      printf( "  <t> Temperature devices\n");
      printf( "  <1> PIO DS2413 devices\n");
      printf( "  <5> PIO DS2505 devices\n");
      printf( "  <6> PIO DS2506 devices\n");
      printf( "  <9> Digital potentiometer DS2890 devices\n");
      printf( "\n");
      printf( "  <s> Switch components\n");
      printf( "  <j> Jalousie components\n");
      printf( "\n");
      printf( "  <m> Message logging and debugging\n");
      printf( "  <o> Locks test\n");
      printf( "  <h> Thread test\n");
      printf( "\n");
      printf( "  <q> Quit\n");

      int key = hlp_getMenuKey( "lnt1569sjmohq");

      switch( key ) {
        case 'l': linkMenu( oneWireLinks, oneWireNet); break;
        case 'n': netMenu( oneWireNet);                break;
        //case 't': tempMenu( oneWireNet);               break;
        case '1': ds2413Menu( oneWireNet);				break;
        case '5': ds2405Menu( oneWireNet);             break;
        case '6': ds2406Menu( oneWireNet);             break;
        case '9': ds2890Menu( oneWireNet);             break;
        //case 's': switchMenu( oneWireNet);             break;
        //case 'j': jalousieMenu( oneWireNet);           break;
        case 'm': msgLoggingMenu();                    break;
        case 'o': lockMenu( oneWireNet);               break;
        case 'h': threadMenu( oneWireNet);             break;
        case 'q': keepLooping=false;                   break;
      }
    }
  }
  catch( LOW_exception & ex) {
    ex.logException();
    exitStatus = EXIT_FAILURE;
  }

  for( unsigned int a=0; a<oneWireLinks.size(); a++ )
    delete oneWireLinks[a];
        
  return exitStatus;
}
