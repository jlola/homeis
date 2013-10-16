/***************************************************************************
                          test_ds2406.cpp  -  description
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


#include "LOW_network.h"
#include "LOW_devDS2406.h"


#include "helper.h"


void getSearchCondition( const LOW_devDS2406::devDS2406PtrVec_t &inDevs)
{
  hlp_printDashline();
  for ( unsigned int a=0; a<inDevs.size(); ++a) {
    std::string idStr = inDevs[a]->getID().getRomIDString();
    LOW_helper_msglog::printMessage( "PIO search conditions of DS2406 device: #%d:\n", a);
    LOW_helper_msglog::printMessage( "  ROM ID:            %s\n", idStr.c_str());

    LOW_devDS2406::statusRegister_t statReg;
    inDevs[a]->getSearchCondition( &statReg);

    LOW_helper_msglog::printMessage( "  activePolarity:    %s\n", 
      statReg.activePolarity==LOW_devDS2406::activeLow  ? "active low" :
      statReg.activePolarity==LOW_devDS2406::activeHigh ? "active high":
      "ILLEGAL VALUE"  );

    LOW_helper_msglog::printMessage( "  sourceSelect:      %s\n",
      statReg.sourceSelect==LOW_devDS2406::latchSelect     ? "latch"            :
      statReg.sourceSelect==LOW_devDS2406::flipFlopSelect  ? "flip flop"        :
      statReg.sourceSelect==LOW_devDS2406::curStatusSelect ? "cur. PIO status"  :
      "ILLEGAL VALUE"  );
    
    LOW_helper_msglog::printMessage( "  channelSelect:     %s\n",
      statReg.channelSelect==LOW_devDS2406::noneSelect     ? "none"   :
      statReg.channelSelect==LOW_devDS2406::chanASelect    ? "A"      :
      statReg.channelSelect==LOW_devDS2406::chanBSelect    ? "B"      :
      statReg.channelSelect==LOW_devDS2406::chanBothSelect ? "A or B" :
      "ILLEGAL VALUE"  );
    
    LOW_helper_msglog::printMessage( "  channelFFQ_pioA:   %s\n",
      statReg.channelFFQ_pioA==LOW_devDS2406::pioTransistorOn  ? "on"  :
      statReg.channelFFQ_pioA==LOW_devDS2406::pioTransistorOff ? "off" :
      "ILLEGAL VALUE"  );
    
    LOW_helper_msglog::printMessage( "  channelFFQ_pioB:   %s\n",
      statReg.channelFFQ_pioB==LOW_devDS2406::pioTransistorOn  ? "on"  :
      statReg.channelFFQ_pioB==LOW_devDS2406::pioTransistorOff ? "off" :
      "ILLEGAL VALUE"  );
    
    LOW_helper_msglog::printMessage( "  isExternalPowered: %s\n", statReg.isExternalPowered==true?"true":"false");
    LOW_helper_msglog::printMessage( " \n");
  }
}


void setSearchCondition( const LOW_devDS2406::devDS2406PtrVec_t &inDevs)
{
  LOW_devDS2406::chanSelect_t     chanSelect;
  LOW_devDS2406::sourceSelect_t   sourceSelect;
  LOW_devDS2406::activePolarity_t polaritySelect;
  LOW_devDS2406::pioTransistor_t  pioATrans;
  LOW_devDS2406::pioTransistor_t  pioBTrans;
  std::vector<std::string>        options;
  unsigned int                    result;

  options.clear();
  options.push_back( "active low");
  options.push_back( "active high");
  result = hlp_selectSingleOption( "Active polarity", options, 1);
  switch( result ) {
    case 0:  polaritySelect = LOW_devDS2406::activeLow;   break;
    case 1:  polaritySelect = LOW_devDS2406::activeHigh;  break;
    default: polaritySelect = LOW_devDS2406::activeHigh;  break;
  }

  options.clear();
  options.push_back( "latch");
  options.push_back( "flip flop");
  options.push_back( "cur. PIO status");
  result = hlp_selectSingleOption( "Source select", options, 0);
  switch( result ) {
    case 0:  sourceSelect = LOW_devDS2406::latchSelect;       break;
    case 1:  sourceSelect = LOW_devDS2406::flipFlopSelect;    break;
    case 2:  sourceSelect = LOW_devDS2406::curStatusSelect;   break;
    default: sourceSelect = LOW_devDS2406::latchSelect;       break;
  }

  options.clear();
  options.push_back( "none");
  options.push_back( "A");
  options.push_back( "B");
  options.push_back( "A or B");
  result = hlp_selectSingleOption( "Channel select", options, 3);
  switch( result ) {
    case 0:  chanSelect = LOW_devDS2406::noneSelect;      break;
    case 1:  chanSelect = LOW_devDS2406::chanASelect;     break;
    case 2:  chanSelect = LOW_devDS2406::chanBSelect;     break;
    case 3:  chanSelect = LOW_devDS2406::chanBothSelect;  break;
    default: chanSelect = LOW_devDS2406::chanBothSelect;  break;
  }

  options.clear();
  options.push_back( "transistor off");
  options.push_back( "transistor on");
  result = hlp_selectSingleOption( "Channel FlipFlop PIO A", options, 0);
  switch( result ) {
    case 0:  pioATrans = LOW_devDS2406::pioTransistorOff;  break;
    case 1:  pioATrans = LOW_devDS2406::pioTransistorOn;   break;
    default: pioATrans = LOW_devDS2406::pioTransistorOff;  break;
  }

  options.clear();
  options.push_back( "transistor off");
  options.push_back( "transistor on");
  result = hlp_selectSingleOption( "Channel FlipFlop PIO B", options, 0);
  switch( result ) {
    case 0:  pioBTrans = LOW_devDS2406::pioTransistorOff;  break;
    case 1:  pioBTrans = LOW_devDS2406::pioTransistorOn;   break;
    default: pioBTrans = LOW_devDS2406::pioTransistorOff;  break;
  }

  for ( unsigned int a=0; a<inDevs.size(); ++a) {
    inDevs[a]->setSearchCondition( chanSelect, sourceSelect, polaritySelect, pioATrans, pioBTrans);
  }
}


void getChannelInfo( const LOW_devDS2406::devDS2406PtrVec_t &inDevs, bool inDoLooping)
{
  bool keepLooping = inDoLooping;
  do {
    hlp_printDashline();
    for ( unsigned int a=0; a<inDevs.size(); ++a) {
      std::string idStr = inDevs[a]->getID().getRomIDString();
      LOW_helper_msglog::printMessage( "PIO search conditions of DS2406 device: #%d:\n", a);
      LOW_helper_msglog::printMessage( "  ROM ID:                %s\n", idStr.c_str());

      LOW_devDS2406::cmd_ChannelAccess *chAccess = new LOW_devDS2406::cmd_ChannelAccess(
                                                        *inDevs[a],
                                                        LOW_devDS2406::cmd_ChannelAccess::CRC_disable,
                                                        LOW_devDS2406::chanBothSelect,
                                                        LOW_devDS2406::cmd_ChannelAccess::asyncInterleaveMode,
                                                        LOW_devDS2406::cmd_ChannelAccess::noToggleMode,
                                                        LOW_devDS2406::cmd_ChannelAccess::readMode,
                                                        LOW_devDS2406::cmd_ChannelAccess::noResetLatches);
      LOW_devDS2406::cmd_ChannelAccess::channelInfo_t chanInfo = chAccess->getChannelInfo();
      delete chAccess;

      LOW_helper_msglog::printMessage( "  PIO A output FlipFlop: %d\n", chanInfo.channelFFQ_pioA);
      LOW_helper_msglog::printMessage( "  PIO B output FlipFlop: %d\n", chanInfo.channelFFQ_pioB);
      LOW_helper_msglog::printMessage( "  PIO A current level:   %d\n", chanInfo.sensedLevel_pioA);
      LOW_helper_msglog::printMessage( "  PIO B current level:   %d\n", chanInfo.sensedLevel_pioB);
      LOW_helper_msglog::printMessage( "  PIO A latch level:     %d\n", chanInfo.activityLatch_pioA);
      LOW_helper_msglog::printMessage( "  PIO B latch level:     %d\n", chanInfo.activityLatch_pioB);
      LOW_helper_msglog::printMessage( "  Has PIO B:             %d\n", chanInfo.hasPioB);
      LOW_helper_msglog::printMessage( "  Has external power:    %d\n", chanInfo.isExternalPowered);
      LOW_helper_msglog::printMessage( "  \n");

      if ( keepLooping && hlp_kbhit() ) {
        keepLooping=false;
        break;
      }
    }
  } while ( keepLooping );
}


void resetLatches( const LOW_devDS2406::devDS2406PtrVec_t &inDevs)
{
  for ( unsigned int a=0; a<inDevs.size(); ++a) {
    LOW_devDS2406::cmd_ChannelAccess *clearLatchAccess = new LOW_devDS2406::cmd_ChannelAccess(
                                                      *inDevs[a],
                                                      LOW_devDS2406::cmd_ChannelAccess::CRC_disable,
                                                      LOW_devDS2406::chanBothSelect,
                                                      LOW_devDS2406::cmd_ChannelAccess::asyncInterleaveMode,
                                                      LOW_devDS2406::cmd_ChannelAccess::noToggleMode,
                                                      LOW_devDS2406::cmd_ChannelAccess::readMode,
                                                      LOW_devDS2406::cmd_ChannelAccess::resetLatches);
    delete clearLatchAccess;
  }
}


void ds2406Menu( LOW_network &inNet)
{
  bool keepLooping = true;
  while ( keepLooping) {
    hlp_printDashline();
    printf( "PIO DS2406 device menu\n");
    printf( "\n");
    printf( "  <g> Get DS2406 devices\n");
    printf( "  <s> Search all DS2406 devices\n");
    printf( "  <a> Search alarming DS2406 devices\n");
    printf( "\n");
    printf( "  <c> Get search conditions of DS2406 devices\n");
    printf( "  <v> Set search conditions of DS2406 devices\n");
    printf( "  <i>      Get channel info of DS2406 devices\n");
    printf( "  <l> Loop get channel info of DS2406 devices\n");
    printf( "  <r> Reset latches of DS2406 devices\n");
    printf( "\n");
    printf( "  <q> Back to main menu\n");

    int key = hlp_getMenuKey( "gsacvilrq");

    LOW_devDS2406::devDS2406PtrVec_t allPDevs = inNet.getDevices<LOW_devDS2406>();

    try {
      switch( key ) {
        case 'g': hlp_getDevices<LOW_devDS2406>( inNet);                           break;
        case 's': hlp_searchDevices<LOW_devDS2406>( inNet, false);                 break;
        case 'a': hlp_searchDevices<LOW_devDS2406>( inNet, true);                  break;
        case 'i': getChannelInfo( hlp_selectDevices( allPDevs), false);            break;
        case 'c': getSearchCondition( hlp_selectDevices( allPDevs));               break;
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
