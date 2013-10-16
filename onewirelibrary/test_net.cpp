/***************************************************************************
                          test_net.cpp  -  description
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
#include "LOW_devUnknown.h"

#include "helper.h"



void netMenu( LOW_network &inNet)
{
  bool keepLooping = true;
  while ( keepLooping) {
    hlp_printDashline();
    printf( "Network menu\n");
    printf( "\n");
    printf( "  Number of segments: %d\n", inNet.getSegments().size());
    printf( "\n");
    printf( "  <g> Get all devices\n");
    printf( "  <s> Search all devices\n");
    printf( "  <a> Search alarming devices\n");
    printf( "\n");
    printf( "  <u> Get unknown devices\n");
    printf( "\n");
    printf( "  <q> Back to main menu\n");

    int key = hlp_getMenuKey( "gsauq");

    try {
      switch( key ) {
        case 'g': hlp_getDevices<LOW_device>( inNet);             break;
        case 's': hlp_searchDevices<LOW_device>( inNet, false);   break;
        case 'a': hlp_searchDevices<LOW_device>( inNet, true);    break;
        case 'u': hlp_getDevices<LOW_devUnknown>( inNet);         break;
        case 'q': keepLooping=false;                              break;
      }
    }
    catch( LOW_exception ex) {
      ex.logException();
    }
  }
}
