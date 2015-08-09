/***************************************************************************
                          test_msgLogging.cpp  -  description
                             -------------------
    begin                : Sun Oct 26 2003
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

#include "LOW_exception.h"
#include "LOW_helper_msglog.h"


#include "helper.h"



void msgLoggingMenu()
{
  bool keepLooping = true;
  while ( keepLooping) {
    hlp_printDashline();
    printf( "Message logging menu\n");
    printf( "\n");
    printf( "  <a> Toggle exception autologging (now %s)\n",
            LOW_exception::getLogOnCreation()==true?"on":"off");
    printf( "\n");
    printf( "  <1> Toggle debugging of serial port (now %s)\n",
            LOW_helper_msglog::getDebugLevelEnabled( LOW_helper_msglog::portSerial_dl)==true?"on":"off");
    printf( "  <2> Toggle debugging of successfull locking in LOW_ObjectSynchronizer (now %s)\n",
            LOW_helper_msglog::getDebugLevelEnabled( LOW_helper_msglog::objSync_getLock_dl)==true?"on":"off");
    printf( "  <3> Toggle debugging of failed locking in LOW_ObjectSynchronizer (now %s)\n",
            LOW_helper_msglog::getDebugLevelEnabled( LOW_helper_msglog::objSync_lockFailed_dl)==true?"on":"off");
    printf( "\n");
    printf( "  <q> Back to main menu\n");

    int key = hlp_getMenuKey( "a123q");

    try {
      switch( key ) {
        case 'a': LOW_exception::setLogOnCreation( ! LOW_exception::getLogOnCreation());                         break;
        case '1': LOW_helper_msglog::setDebugLevelEnabled( LOW_helper_msglog::portSerial_dl,
                    ! LOW_helper_msglog::getDebugLevelEnabled( LOW_helper_msglog::portSerial_dl));               break;
        case '2': LOW_helper_msglog::setDebugLevelEnabled( LOW_helper_msglog::objSync_getLock_dl,
                    ! LOW_helper_msglog::getDebugLevelEnabled( LOW_helper_msglog::objSync_getLock_dl));          break;
        case '3': LOW_helper_msglog::setDebugLevelEnabled( LOW_helper_msglog::objSync_lockFailed_dl,
                    ! LOW_helper_msglog::getDebugLevelEnabled( LOW_helper_msglog::objSync_lockFailed_dl));       break;
        case 'q': keepLooping=false;                                                                             break;
      }
    }
    catch( LOW_exception & ex) {
      ex.logException();
    }
  }
}
