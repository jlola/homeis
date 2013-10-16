/***************************************************************************
                          test_temp.cpp  -  description
                             -------------------
    begin                : Thu Jan 2 2003
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
#include "LOW_devDS1820.h"
#include "LOW_compTempSensor.h"


#include "helper.h"


void getInfoDS1820( const LOW_devDS1820::devDS1820PtrVec_t &inTDevs)
{
  hlp_printDashline();
  for ( unsigned int a=0; a<inTDevs.size(); ++a) {
    std::string idStr = inTDevs[a]->getID().getRomIDString();
    LOW_helper_msglog::printMessage( "Infos about device #%d:\n", a);
    LOW_helper_msglog::printMessage( "  ROM ID:                  %s\n", idStr.c_str());

    LOW_compTempSensor tempComp = LOW_compTempSensor( *inTDevs[a]);

    LOW_helper_msglog::printMessage( "  Power mode:              %s power\n", tempComp.getIsExternalPowered()==true?"External":"Parasite");
    LOW_helper_msglog::printMessage( "  Current temperature:     %f �C\n", tempComp.getTemperature());
    printf("  Current temperature:     %f �C\n", tempComp.getTemperature());

    int8_t  lowBound, highBound;
    tempComp.getAlarmLevels( lowBound, highBound);
    LOW_helper_msglog::printMessage( "  Lower bound alarm level: %d �C\n", lowBound);
    LOW_helper_msglog::printMessage( "  Upper bound alarm level: %d �C\n", highBound);
    LOW_helper_msglog::printMessage( "  Current alarm status:    %s\n", tempComp.checkAlarm( false)==true?"ON ALARM":"idle");
    LOW_helper_msglog::printMessage( " \n");
  }
  
}


void tempCompLoop( const LOW_devDS1820::devDS1820PtrVec_t &inTDevs)
{
  std::vector<LOW_compTempSensor*> allTempComp(inTDevs.size());
  for ( unsigned int a=0; a<inTDevs.size(); ++a) {
    allTempComp[a] = new LOW_compTempSensor( *inTDevs[a]);
  }

  bool keepLooping = true;
  while ( keepLooping ) {
    hlp_printDashline();
    for ( unsigned int a=0; a<allTempComp.size(); ++a) {
      float  temp = allTempComp[a]->getTemperature();
      std::string idStr = inTDevs[a]->getID().getRomIDString();
      LOW_helper_msglog::printMessage( "  Temperature Component %s: Temparature: %f\n", idStr.c_str(), temp);
      if ( hlp_kbhit() ) {
        keepLooping=false;
        break;
      }
    }
  }
}


void tempMenu( LOW_network &inNet)
{
  bool keepLooping = true;
  while ( keepLooping) {
    hlp_printDashline();
    printf( "Temperature device menu\n");
    printf( "\n");
    printf( "  <g> Get DS1820 devices\n");
    printf( "  <s> Search all DS1820 devices\n");
    printf( "  <a> Search alarming DS1820 devices\n");
    printf( "\n");
    printf( "  <i> Infos of DS1820 devices\n");
    //printf( "  <v> Set alarming values of DS1820 device\n");
    printf( "  <t> Temperature readout loop\n");
    //printf( "  <u> Temperature alarming loop\n");
    printf( "\n");
    printf( "  <q> Back to main menu\n");

    int key = hlp_getMenuKey( "gsaitq");

    LOW_devDS1820::devDS1820PtrVec_t allTDevs = inNet.getDevices<LOW_devDS1820>();
    //getInfoDS1820( hlp_selectDevices( allTDevs));

    try {
      switch( key ) {
        case 'g': hlp_getDevices<LOW_devDS1820>( inNet);            break;
        case 's': hlp_searchDevices<LOW_devDS1820>( inNet, false);  break;
        case 'a': hlp_searchDevices<LOW_devDS1820>( inNet, true);   break;
        case 'i': getInfoDS1820( hlp_selectDevices( allTDevs));     break;
        //case 'v': break;
        case 't': tempCompLoop( hlp_selectDevices( allTDevs));      break;
        //case 'u': break;
        case 'q': keepLooping=false;                                break;
      }
    }
    catch( LOW_exception ex) {
      ex.logException();
    }
  }
}
