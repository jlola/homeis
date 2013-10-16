/***************************************************************************
                          test_ds2405.cpp  -  description
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


#include "helper.h"


void outputToogleDS2405( const LOW_devDS2405::devDS2405PtrVec_t &inDevs, bool inDoLooping)
{
  bool keepLooping = inDoLooping;
  do {
    hlp_printDashline();
    for ( unsigned int a=0; a<inDevs.size(); ++a) {
      inDevs[a]->cmd_Match();
      std::string idStr = inDevs[a]->getID().getRomIDString();
      LOW_helper_msglog::printMessage( "  PIO device %s: output toogled\n", idStr.c_str());
      if ( keepLooping && hlp_kbhit() ) {
        keepLooping=false;
        break;
      }
    } 
  } while ( keepLooping );
}


void outputToogleStatusDS2405( const LOW_devDS2405::devDS2405PtrVec_t &inDevs, bool inDoLooping)
{
  bool keepLooping = inDoLooping;
  do {
    hlp_printDashline();
    for ( unsigned int a=0; a<inDevs.size(); ++a) {
      bool pioStat = inDevs[a]->cmd_MatchRead();
      std::string idStr = inDevs[a]->getID().getRomIDString();
      LOW_helper_msglog::printMessage( "  PIO device %s: output toogled, new PIO status: %s\n", idStr.c_str(), pioStat==true?"open":"driving LOW");
      if ( keepLooping && hlp_kbhit() ) {
        keepLooping=false;
        break;
      }
    }
  } while ( keepLooping );
}


void inputReadDS2405( const LOW_devDS2405::devDS2405PtrVec_t &inDevs, bool inDoLooping)
{
  bool keepLooping = inDoLooping;
  do {
    hlp_printDashline();
    for ( unsigned int a=0; a<inDevs.size(); ++a) {
      bool pioStat = inDevs[a]->cmd_SearchRead();
      std::string idStr = inDevs[a]->getID().getRomIDString();
      LOW_helper_msglog::printMessage( "  PIO device %s: input PIO status: %s\n", idStr.c_str(), pioStat==true?"inp. high":"inp. low _OR_ outp. low");
      if ( keepLooping && hlp_kbhit() ) {
        keepLooping=false;
        break;
      }
    }
  } while ( keepLooping );
}


void outputStatusDS2405( const LOW_devDS2405::devDS2405PtrVec_t &inDevs)
{
  hlp_printDashline();
  LOW_helper_msglog::printMessage( "Output driving status (cmd_SearchActive()):\n");
  for ( unsigned int a=0; a<inDevs.size(); ++a) {
    bool pioStat = inDevs[a]->cmd_SearchActive();
    std::string idStr = inDevs[a]->getID().getRomIDString();
    LOW_helper_msglog::printMessage( "  PIO device %s: output PIO status: %s\n", idStr.c_str(), pioStat==true?"driving LOW":"open");
  }
}


void ds2405Menu( LOW_network &inNet)
{
  bool keepLooping = true;
  while ( keepLooping) {
    hlp_printDashline();
    printf( "PIO DS2405 device menu\n");
    printf( "\n");
    printf( "  <g> Get DS2405 devices\n");
    printf( "  <s> Search all DS2405 devices\n");
    printf( "  <a> Search alarming DS2405 devices\n");
    printf( "\n");
    printf( "  <t>      Output toogle of DS2405 devices\n");
    printf( "  <z> Loop output toogle of DS2405 devices\n");
    printf( "  <b>      Output toogle/status of DS2405 devices\n");
    printf( "  <n> Loop output toogle/status of DS2405 devices\n");
    printf( "  <i>      Input status of DS2405 devices\n");
    printf( "  <o> Loop input status of DS2405 devices\n");
    printf( "  <p> Output status of DS2405 devices\n");
    printf( "\n");
    printf( "  <q> Back to main menu\n");

    int key = hlp_getMenuKey( "gsatzbniopq");

    LOW_devDS2405::devDS2405PtrVec_t allPDevs = inNet.getDevices<LOW_devDS2405>();

    try {
      switch( key ) {
        case 'g': hlp_getDevices<LOW_devDS2405>( inNet);                           break;
        case 's': hlp_searchDevices<LOW_devDS2405>( inNet, false);                 break;
        case 'a': hlp_searchDevices<LOW_devDS2405>( inNet, true);                  break;
        case 't': outputToogleDS2405( hlp_selectDevices( allPDevs), false);        break;
        case 'z': outputToogleDS2405( hlp_selectDevices( allPDevs), true);         break;
        case 'b': outputToogleStatusDS2405( hlp_selectDevices( allPDevs), false);  break;
        case 'n': outputToogleStatusDS2405( hlp_selectDevices( allPDevs), true);   break;
        case 'i': inputReadDS2405( hlp_selectDevices( allPDevs), false);           break;
        case 'o': inputReadDS2405( hlp_selectDevices( allPDevs), true);            break;
        case 'p': outputStatusDS2405( hlp_selectDevices( allPDevs));               break;
        case 'q': keepLooping=false;                                               break;
      }
    }
    catch( LOW_exception ex) {
      ex.logException();
    }
  }
}
