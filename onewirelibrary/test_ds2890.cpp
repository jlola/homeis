/***************************************************************************
                          test_ds2890.cpp  -  description
                             -------------------
    begin                : Sun Nov 9 2003
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
#include "LOW_devDS2890.h"


#include "helper.h"


void getInfoDS2890( const LOW_devDS2890::devDS2890PtrVec_t &inPotDevs)
{
  hlp_printDashline();
  for ( unsigned int a=0; a<inPotDevs.size(); ++a) {
    LOW_helper_msglog::printMessage( "Infos about device #%d:\n", a);
    LOW_helper_msglog::printMessage( "  ROM ID:                    %s\n", inPotDevs[a]->getID().getRomIDString().c_str());
    LOW_helper_msglog::printMessage( "  Potentiometer type:        %s\n", inPotDevs[a]->getHasLinearPotElements()==true?"Linear":"Logarithmic");
    LOW_helper_msglog::printMessage( "  Wiper setting persistency: %s\n", inPotDevs[a]->getIsWiperSettingVolatile()==true?"Volatile":"Persistent");
    LOW_helper_msglog::printMessage( "  Potentiometer count:       %d\n", inPotDevs[a]->getPotentiometerCount());
    LOW_helper_msglog::printMessage( "  Wiper positions:           %d\n", inPotDevs[a]->getWiperPositionsCount());
    LOW_helper_msglog::printMessage( "  Potentiometer resistance:  %d Ohm\n", inPotDevs[a]->getPotentiometerResistance());
    LOW_helper_msglog::printMessage( "  Charge pump allowed:       %s\n", inPotDevs[a]->getChargePumpAllowed()==true?"True":"False");
    LOW_helper_msglog::printMessage( "  Cached selected wiper:     %d\n", inPotDevs[a]->getCachedWiperNumber());
    LOW_helper_msglog::printMessage( "  Cached wiper position:     %d\n", inPotDevs[a]->getCachedWiperPosition( inPotDevs[a]->getCachedWiperNumber()));

    LOW_devDS2890::controlRegister_t ctlReg = inPotDevs[a]->cmd_ReadControlRegister();
    LOW_helper_msglog::printMessage( "  Currently selected wiper:  %d\n", ctlReg.controlledWiperNum);
    LOW_helper_msglog::printMessage( "  Charge pump enabled:       %s\n", ctlReg.isChargePumpOn==true?"True":"False");
    LOW_helper_msglog::printMessage( "  Current wiper position:    %d\n", inPotDevs[a]->cmd_ReadPosition());
    LOW_helper_msglog::printMessage( " \n");
  }

}


void enableChargePump( LOW_devDS2890* inPotDev)
{
  inPotDev->setChargePumpAllowed( hlp_selectBoolOption( "Charge pump allowed", false));
}


void selectWiper( LOW_devDS2890* inPotDev)
{
  std::vector<std::string>  options;
  unsigned int              result;
  uint8_t                   selectedWiper;
  bool                      doChargePump;
  
  options.clear();
  for( unsigned int a=0; a<inPotDev->getPotentiometerCount(); ++a) {
    options.push_back( "Wiper number " + a);
  }
  selectedWiper = hlp_selectSingleOption( "Active wiper", options, 0);
  
  options.clear();
  options.push_back( "off");
  options.push_back( "on");
  result = hlp_selectSingleOption( "Charge pump", options, 0);
  switch( result ) {
    case 0:  doChargePump = false;  break;
    case 1:  doChargePump = true;   break;
    default: doChargePump = false;  break;
  }

  //hlp_printDashline();
  inPotDev->cmd_WriteControlRegister( selectedWiper, doChargePump);
}


void setWiperPos( LOW_devDS2890* inPotDev)
{
  unsigned int  wantedPos;
  
  hlp_printDashline();
  printf( "Enter new wiper position -> ");
  scanf( "%u", &wantedPos);

  inPotDev->cmd_WritePosition( wantedPos);
}


void incWiper( const LOW_devDS2890::devDS2890PtrVec_t &inPotDevs)
{
  hlp_printDashline();
  for ( unsigned int a=0; a<inPotDevs.size(); ++a) {
    uint8_t newPos = inPotDevs[a]->cmd_Increment();
    LOW_helper_msglog::printMessage( "New wiper position of device %s: %3d\n", inPotDevs[a]->getID().getRomIDString().c_str(), newPos);
  }
}


void decWiper( const LOW_devDS2890::devDS2890PtrVec_t &inPotDevs)
{
  hlp_printDashline();
  for ( unsigned int a=0; a<inPotDevs.size(); ++a) {
    uint8_t newPos = inPotDevs[a]->cmd_Decrement();
    LOW_helper_msglog::printMessage( "New wiper position of device %s: %3d\n", inPotDevs[a]->getID().getRomIDString().c_str(), newPos);
  }
}


void ds2890Menu( LOW_network &inNet)
{
  bool keepLooping = true;
  while ( keepLooping) {
    hlp_printDashline();
    printf( "Digital potentiometer DS2890 device menu\n");
    printf( "\n");
    printf( "  <g> Get DS2890 devices\n");
    printf( "  <s> Search all DS2890 devices\n");
    printf( "  <a> Search alarming DS2890 devices\n");
    printf( "\n");
    printf( "  <i> Info of DS2890 devices\n");
    printf( "  <c> Set charge pump allowed\n");
    printf( "  <w> Select wiper and charge pump\n");
    printf( "  <p> Set wiper position\n");
    printf( "\n");
    printf( "  <u> Increment wiper\n");
    printf( "  <d> Decrement wiper\n");
    printf( "\n");
    printf( "  <q> Back to main menu\n");

    int key = hlp_getMenuKey( "gsaicwpudq");

    LOW_devDS2890::devDS2890PtrVec_t allPotDevs = inNet.getDevices<LOW_devDS2890>();

    try {
      switch( key ) {
        case 'g': hlp_getDevices<LOW_devDS2890>( inNet);                           break;
        case 's': hlp_searchDevices<LOW_devDS2890>( inNet, false);                 break;
        case 'a': hlp_searchDevices<LOW_devDS2890>( inNet, true);                  break;
        case 'i': getInfoDS2890( hlp_selectDevices( allPotDevs));                  break;
        case 'c': enableChargePump( hlp_selectSingleDevice( allPotDevs));          break;
        case 'w': selectWiper( hlp_selectSingleDevice( allPotDevs));               break;
        case 'p': setWiperPos( hlp_selectSingleDevice( allPotDevs));               break;
        case 'u': incWiper( hlp_selectDevices( allPotDevs));                       break;
        case 'd': decWiper( hlp_selectDevices( allPotDevs));                       break;
        case 'q': keepLooping=false;                                               break;
      }
    }
    catch( LOW_exception ex) {
      ex.logException();
    }
  }
}
