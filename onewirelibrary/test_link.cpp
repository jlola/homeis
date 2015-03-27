/***************************************************************************
                          test_link.cpp  -  description
                             -------------------
    begin                : Fri Jan 10 2003
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

#include "LOW_portSerial.h"
#include "LOW_link.h"
#include "LOW_linkFlexibleSpeed.h"
#include "LOW_linkPassiveSerial.h"
#include "LOW_linkDS2480B.h"
#include "LOW_linkDS2490.h"
#include "LOW_network.h"

#include "helper.h"



LOW_portSerialFactory::portSpecifier_t getSerialPortSpecifier()
{
  char port[256];
  hlp_printDashline();
  printf( "Enter full path to serial port device -> ");
  scanf( "%255s", port);
  return std::string( port);
}


LOW_portUsb_Factory::usbDeviceSpecifier_t getUsbDevSpecifier()
{
  char dev[256];
  hlp_printDashline();
  printf( "Enter path of usb device relative to /proc/bus/usb/ -> ");
  scanf( "%255s", dev);
  return std::string( dev);
}


void addActiveLink( LOW_link::linkPtrVec_t &inLinkList, LOW_network &inNet)
{
  LOW_linkDS2480B  *activeLink = 0;
  try {
    LOW_portSerialFactory::portSpecifier_t  tty = getSerialPortSpecifier();

    LOW_linkDS2480B::RXPOL_val_t  rxPol;
    std::vector<std::string> optRxPol;
    optRxPol.push_back( "RX polarity normal");
    optRxPol.push_back( "RX polarity inverted");
    unsigned int result = hlp_selectSingleOption( "RX pin polarity", optRxPol, 1);
    switch( result ) {
      case 0: rxPol = LOW_linkDS2480B::RXPOL_NORM;  break;
      case 1: rxPol = LOW_linkDS2480B::RXPOL_INV;   break;
    }

    bool extPower      = hlp_selectBoolOption( "Supplies external power", true);
    bool allowPrgPulse = hlp_selectBoolOption( "Allow EPROM program pulse", false);

    activeLink = new LOW_linkDS2480B( tty, LOW_linkDS2480B::RXPOL_INV, extPower, allowPrgPulse);
    inNet.addLink( activeLink);
    inLinkList.push_back( activeLink);
  }
  catch( LOW_exception & ex) {
    ex.logException();
    if ( activeLink ) delete activeLink;
  }
}


void addPassiveLink( LOW_link::linkPtrVec_t &inLinkList, LOW_network &inNet)
{
  LOW_linkPassiveSerial  *passiveLink = 0;
  try {
    LOW_portSerialFactory::portSpecifier_t  tty = getSerialPortSpecifier();

    passiveLink = new LOW_linkPassiveSerial( tty);
    inNet.addLink( passiveLink);
    inLinkList.push_back( passiveLink);
  }
  catch( LOW_exception & ex) {
    ex.logException();
    if ( passiveLink ) delete passiveLink;
  }
}


void addUsbLinkAutoAll( LOW_link::linkPtrVec_t &inLinkList, LOW_network &inNet)
{
  bool extPower      = hlp_selectBoolOption( "Supplies external power", true);
  bool allowPrgPulse = hlp_selectBoolOption( "Allow EPROM program pulse", false);

  LOW_portUsb_Factory::usbDevSpecVec_t  specifierList = LOW_portUsb_Factory::getPortSpecifiers( LOW_linkDS2490::usbVendorID, LOW_linkDS2490::usbProductID);
  for( unsigned int a=0; a<specifierList.size(); a++) {
    LOW_linkDS2490  *usbLink = 0;
    try {
      usbLink = new LOW_linkDS2490( specifierList[a], extPower, allowPrgPulse);
      inNet.addLink( usbLink);
      inLinkList.push_back( usbLink);
    }
    catch( LOW_exception & ex) {
      ex.logException();
      if ( usbLink ) delete usbLink;
    }
  }
}


void addUsbLinkManual( LOW_link::linkPtrVec_t &inLinkList, LOW_network &inNet)
{
  LOW_linkDS2490  *usbLink = 0;
  try {
    LOW_portUsb_Factory::usbDeviceSpecifier_t dev = getUsbDevSpecifier();
    bool extPower      = hlp_selectBoolOption( "Supplies external power", true);
    bool allowPrgPulse = hlp_selectBoolOption( "Allow EPROM program pulse", false);

    usbLink = new LOW_linkDS2490( dev, extPower, allowPrgPulse);
    inNet.addLink( usbLink);
    inLinkList.push_back( usbLink);
  }
  catch( LOW_exception & ex) {
    ex.logException();
    if ( usbLink ) delete usbLink;
  }
}


void removeLink( LOW_link *inLink, LOW_link::linkPtrVec_t &inLinkList, LOW_network &inNet)
{
  inNet.removeLink( inLink);
  inLinkList.erase( find( inLinkList.begin(), inLinkList.end(), inLink));
  delete inLink;
}


void linkInfo( LOW_link::linkPtrVec_t &inLinkList)
{
  hlp_printDashline();
  for( unsigned int a=0; a<inLinkList.size(); a++) {
    LOW_helper_msglog::printMessage( "Link ID %d:\n", inLinkList[a]->getID());
    LOW_helper_msglog::printMessage( "  Adapter type:            %s\n", inLinkList[a]->getLinkFamily().c_str());
    LOW_helper_msglog::printMessage( "  Has program pulse:       %s\n", (inLinkList[a]->getHasProgramPulse()==true)?"yes":"no");
    LOW_helper_msglog::printMessage( "  Program pulse allowed:   %s\n", (inLinkList[a]->getAllowProgPulse()==true)?"yes":"no");
    LOW_helper_msglog::printMessage( "  Has external power:      %s\n", (inLinkList[a]->getHasExternalPower()==true)?"yes":"no");

    LOW_linkFlexibleSpeed *flexLink = dynamic_cast<LOW_linkFlexibleSpeed*>(inLinkList[a]);
    if ( flexLink == 0 ) {
      LOW_helper_msglog::printMessage( "  Has flexible speed:      no\n");
    }
    else {
      LOW_helper_msglog::printMessage( "  Has flexible speed:      yes\n");

      std::string  tmpStr;
      
      LOW_linkFlexibleSpeed::wireSpeed_t speed = flexLink->getWireSpeed();
      switch ( speed ) {
        case LOW_linkFlexibleSpeed::normal_speed:    tmpStr = "normal";   break;
        case LOW_linkFlexibleSpeed::flexible_speed:  tmpStr = "flexible"; break;
        default:                                     tmpStr = "UNRECOGNIZED VALUE!"; break;
      }
      LOW_helper_msglog::printMessage( "  Current speed:           %s\n", tmpStr.c_str());

      if ( speed == LOW_linkFlexibleSpeed::flexible_speed ) {
        switch ( flexLink->getPullDownSlewRate() ) {
          case LOW_linkFlexibleSpeed::pdSlewRate_15:     tmpStr = "15 V/microsec";   break;
          case LOW_linkFlexibleSpeed::pdSlewRate_2_2:    tmpStr = "2.2 V/microsec";  break;
          case LOW_linkFlexibleSpeed::pdSlewRate_1_65:   tmpStr = "1.65 V/microsec"; break;
          case LOW_linkFlexibleSpeed::pdSlewRate_1_37:   tmpStr = "1.37 V/microsec"; break;
          case LOW_linkFlexibleSpeed::pdSlewRate_1_1:    tmpStr = "1.1 V/microsec";  break;
          case LOW_linkFlexibleSpeed::pdSlewRate_0_83:   tmpStr = "0.83 V/microsec"; break;
          case LOW_linkFlexibleSpeed::pdSlewRate_0_7:    tmpStr = "0.7 V/microsec";  break;
          case LOW_linkFlexibleSpeed::pdSlewRate_0_55:   tmpStr = "0.55 V/microsec"; break;
          default:                                       tmpStr = "UNRECOGNIZED VALUE!"; break;
        }
      }
      else {
        tmpStr = "only in flexible speed";
      }
      LOW_helper_msglog::printMessage( "  Pull down slew rate:     %s\n", tmpStr.c_str());

      if ( speed == LOW_linkFlexibleSpeed::flexible_speed ) {
        switch ( flexLink->getWrite1LowTime() ) {
          case LOW_linkFlexibleSpeed::w1LowTime_8:   tmpStr = "8 microsec";  break;
          case LOW_linkFlexibleSpeed::w1LowTime_9:   tmpStr = "9 microsec";  break;
          case LOW_linkFlexibleSpeed::w1LowTime_10:  tmpStr = "10 microsec"; break;
          case LOW_linkFlexibleSpeed::w1LowTime_11:  tmpStr = "11 microsec"; break;
          case LOW_linkFlexibleSpeed::w1LowTime_12:  tmpStr = "12 microsec"; break;
          case LOW_linkFlexibleSpeed::w1LowTime_13:  tmpStr = "13 microsec"; break;
          case LOW_linkFlexibleSpeed::w1LowTime_14:  tmpStr = "14 microsec"; break;
          case LOW_linkFlexibleSpeed::w1LowTime_15:  tmpStr = "15 microsec"; break;
          default:                                   tmpStr = "UNRECOGNIZED VALUE!"; break;
        }
      }
      else {
        tmpStr = "only in flexible speed";
      }
      LOW_helper_msglog::printMessage( "  Write-1 low time:        %s\n", tmpStr.c_str());

      if ( speed == LOW_linkFlexibleSpeed::flexible_speed ) {
        switch ( flexLink->getSampleOffsetWrite0Rec() ) {
          case LOW_linkFlexibleSpeed::soW0RecTime_3:  tmpStr = "3 microsec";  break;
          case LOW_linkFlexibleSpeed::soW0RecTime_4:  tmpStr = "4 microsec";  break;
          case LOW_linkFlexibleSpeed::soW0RecTime_5:  tmpStr = "5 microsec";  break;
          case LOW_linkFlexibleSpeed::soW0RecTime_6:  tmpStr = "6 microsec";  break;
          case LOW_linkFlexibleSpeed::soW0RecTime_7:  tmpStr = "7 microsec";  break;
          case LOW_linkFlexibleSpeed::soW0RecTime_8:  tmpStr = "8 microsec";  break;
          case LOW_linkFlexibleSpeed::soW0RecTime_9:  tmpStr = "9 microsec";  break;
          case LOW_linkFlexibleSpeed::soW0RecTime_10: tmpStr = "10 microsec"; break;
          default:                                    tmpStr = "UNRECOGNIZED VALUE!"; break;
        }
      }
      else {
        tmpStr = "only in flexible speed";
      }
      LOW_helper_msglog::printMessage( "  Data sample offset / \n");
      LOW_helper_msglog::printMessage( "    Write-0 recovery time: %s\n", tmpStr.c_str());
  }
    
    LOW_helper_msglog::printMessage( "  \n");
  }
  LOW_helper_msglog::printMessage( "Total %d links.\n", inLinkList.size());
}


void setWireSpeed( LOW_linkFlexibleSpeed &inFlexLink)
{
  LOW_linkFlexibleSpeed::wireSpeed_t  newSpeed;
  std::vector<std::string> optSpeed;
  optSpeed.push_back( "Normal speed");
  optSpeed.push_back( "Flexible speed");
  unsigned int result = hlp_selectSingleOption( "1-Wire speed", optSpeed, 1);
  switch( result ) {
    case 0: newSpeed = LOW_linkFlexibleSpeed::normal_speed;   break;
    case 1: newSpeed = LOW_linkFlexibleSpeed::flexible_speed; break;
  }

  inFlexLink.setWireSpeed( newSpeed);
}


void setPulldownSlewRate( LOW_linkFlexibleSpeed &inFlexLink)
{
  LOW_linkFlexibleSpeed::pdSlewRate_t  newRate;
  std::vector<std::string> optRate;
  optRate.push_back( "15    V/microsec");
  optRate.push_back( " 2.2  V/microsec");
  optRate.push_back( " 1.65 V/microsec");
  optRate.push_back( " 1.37 V/microsec");
  optRate.push_back( " 1.1  V/microsec");
  optRate.push_back( " 0.83 V/microsec");
  optRate.push_back( " 0.7  V/microsec");
  optRate.push_back( " 0.55 V/microsec");
  unsigned int result = hlp_selectSingleOption( "Pulldown slew rate", optRate, 5);
  switch( result ) {
    case 0: newRate = LOW_linkFlexibleSpeed::pdSlewRate_15;    break;
    case 1: newRate = LOW_linkFlexibleSpeed::pdSlewRate_2_2;   break;
    case 2: newRate = LOW_linkFlexibleSpeed::pdSlewRate_1_65;  break;
    case 3: newRate = LOW_linkFlexibleSpeed::pdSlewRate_1_37;  break;
    case 4: newRate = LOW_linkFlexibleSpeed::pdSlewRate_1_1;   break;
    case 5: newRate = LOW_linkFlexibleSpeed::pdSlewRate_0_83;  break;
    case 6: newRate = LOW_linkFlexibleSpeed::pdSlewRate_0_7;   break;
    case 7: newRate = LOW_linkFlexibleSpeed::pdSlewRate_0_55;  break;
  }

  inFlexLink.setPullDownSlewRate( newRate);
}


void setWrite1LowTime( LOW_linkFlexibleSpeed &inFlexLink)
{
  LOW_linkFlexibleSpeed::w1LowTime_t  newW1LowTime;
  std::vector<std::string> optW1LowTime;
  optW1LowTime.push_back( " 8 microsec");
  optW1LowTime.push_back( " 9 microsec");
  optW1LowTime.push_back( "10 microsec");
  optW1LowTime.push_back( "11 microsec");
  optW1LowTime.push_back( "12 microsec");
  optW1LowTime.push_back( "13 microsec");
  optW1LowTime.push_back( "14 microsec");
  optW1LowTime.push_back( "15 microsec");
  unsigned int result = hlp_selectSingleOption( "Write-1 low time", optW1LowTime, 4);
  switch( result ) {
    case 0: newW1LowTime = LOW_linkFlexibleSpeed::w1LowTime_8;  break;
    case 1: newW1LowTime = LOW_linkFlexibleSpeed::w1LowTime_9;  break;
    case 2: newW1LowTime = LOW_linkFlexibleSpeed::w1LowTime_10; break;
    case 3: newW1LowTime = LOW_linkFlexibleSpeed::w1LowTime_11; break;
    case 4: newW1LowTime = LOW_linkFlexibleSpeed::w1LowTime_12; break;
    case 5: newW1LowTime = LOW_linkFlexibleSpeed::w1LowTime_13; break;
    case 6: newW1LowTime = LOW_linkFlexibleSpeed::w1LowTime_14; break;
    case 7: newW1LowTime = LOW_linkFlexibleSpeed::w1LowTime_15; break;
  }

  inFlexLink.setWrite1LowTime( newW1LowTime);
}


void setDsoW0rTime( LOW_linkFlexibleSpeed &inFlexLink)
{
  LOW_linkFlexibleSpeed::soW0RecTime_t  newDsoW0Time;
  std::vector<std::string> optDsoW0Time;
  optDsoW0Time.push_back( " 3 microsec");
  optDsoW0Time.push_back( " 4 microsec");
  optDsoW0Time.push_back( " 5 microsec");
  optDsoW0Time.push_back( " 6 microsec");
  optDsoW0Time.push_back( " 7 microsec");
  optDsoW0Time.push_back( " 8 microsec");
  optDsoW0Time.push_back( " 9 microsec");
  optDsoW0Time.push_back( "10 microsec");
  unsigned int result = hlp_selectSingleOption( "Data sample offset / write-0 recovery time", optDsoW0Time, 0);
  switch( result ) {
    case 0: newDsoW0Time = LOW_linkFlexibleSpeed::soW0RecTime_3;  break;
    case 1: newDsoW0Time = LOW_linkFlexibleSpeed::soW0RecTime_4;  break;
    case 2: newDsoW0Time = LOW_linkFlexibleSpeed::soW0RecTime_5;  break;
    case 3: newDsoW0Time = LOW_linkFlexibleSpeed::soW0RecTime_6;  break;
    case 4: newDsoW0Time = LOW_linkFlexibleSpeed::soW0RecTime_7;  break;
    case 5: newDsoW0Time = LOW_linkFlexibleSpeed::soW0RecTime_8;  break;
    case 6: newDsoW0Time = LOW_linkFlexibleSpeed::soW0RecTime_9;  break;
    case 7: newDsoW0Time = LOW_linkFlexibleSpeed::soW0RecTime_10; break;
  }

  inFlexLink.setSampleOffsetWrite0Rec( newDsoW0Time);
}


void resetAdapter( LOW_link &inLink)
{
  inLink.resetLinkAdapter();
}


void resetBus( LOW_link &inLink)
{
  inLink.resetBus();
}


void strongPullupPredef( LOW_link &inLink)
{
  LOW_link::strongPullup_t  pullupTime;
  std::vector<std::string>  pullupTimeMenu;
  pullupTimeMenu.push_back( "  16 millisec");
  pullupTimeMenu.push_back( "  64 millisec");
  pullupTimeMenu.push_back( " 128 millisec");
  pullupTimeMenu.push_back( " 256 millisec");
  pullupTimeMenu.push_back( " 512 millisec");
  pullupTimeMenu.push_back( "1024 millisec");
  unsigned int result = hlp_selectSingleOption( "Strong pullup time", pullupTimeMenu, 0);
  switch( result ) {
    case 0: pullupTime = LOW_link::pullUp_16_4; break;
    case 1: pullupTime = LOW_link::pullUp_65_5; break;
    case 2: pullupTime = LOW_link::pullUp_131;  break;
    case 3: pullupTime = LOW_link::pullUp_262;  break;
    case 4: pullupTime = LOW_link::pullUp_524;  break;
    case 5: pullupTime = LOW_link::pullUp_1048; break;
  }

  inLink.strongPullup( pullupTime);
}


void strongPullupFree( LOW_link &inLink)
{
  unsigned long pulseTime;
  hlp_printDashline();
  printf( "Enter strong pullup time [ms] -> ");
  scanf( "%ld", &pulseTime);
  inLink.strongPullup( pulseTime);
}


void progPulsePredef( LOW_link &inLink)
{
  LOW_link::progPulse_t    pulseTime;
  std::vector<std::string> pulseTimeMenu;
  pulseTimeMenu.push_back( "  32 microsec");
  pulseTimeMenu.push_back( "  64 microsec");
  pulseTimeMenu.push_back( " 128 microsec");
  pulseTimeMenu.push_back( " 256 microsec");
  pulseTimeMenu.push_back( " 512 microsec");
  pulseTimeMenu.push_back( "1024 microsec");
  pulseTimeMenu.push_back( "2048 microsec");
  unsigned int result = hlp_selectSingleOption( "Program pulse time", pulseTimeMenu, 0);
  switch( result ) {
    case 0: pulseTime = LOW_link::progPulse_32;   break;
    case 1: pulseTime = LOW_link::progPulse_64;   break;
    case 2: pulseTime = LOW_link::progPulse_128;  break;
    case 3: pulseTime = LOW_link::progPulse_256;  break;
    case 4: pulseTime = LOW_link::progPulse_512;  break;
    case 5: pulseTime = LOW_link::progPulse_1024; break;
    case 6: pulseTime = LOW_link::progPulse_2048; break;
  }

  inLink.programPulse( pulseTime);
}


void progPulseFree( LOW_link &inLink)
{
  unsigned long pulseTime;
  hlp_printDashline();
  printf( "Enter program pulse time [microsec] -> ");
  scanf( "%ld", &pulseTime);
  inLink.programPulse( pulseTime);
}


void powerMenu( LOW_link::linkPtrVec_t &inLinkList)
{
  bool keepLooping = true;
  while ( keepLooping) {

    hlp_printDashline();
    printf( "Links power menu\n");
    printf( "\n");
    printf( "  Number of links: %d\n", inLinkList.size());
    printf( "\n");
    printf( "  <s> Strong pullup predefined length\n");
    printf( "  <d> Strong pullup free length\n");
    printf( "\n");
    printf( "  <p> Program pulse predefined length (DANGER!)\n");
    printf( "  <o> Program pulse free length (DANGER!)\n");
    printf( "\n");
    printf( "  <q> Back to links menu\n");

    int key = hlp_getMenuKey( "sdpoq");

    try {
      switch( key ) {
        case 's': strongPullupPredef( *hlp_selectSingleLink<LOW_link>( inLinkList));   break;
        case 'd': strongPullupFree( *hlp_selectSingleLink<LOW_link>( inLinkList));     break;
        case 'p': progPulsePredef( *hlp_selectSingleLink<LOW_link>( inLinkList));      break;
        case 'o': progPulseFree( *hlp_selectSingleLink<LOW_link>( inLinkList));        break;
        case 'q': keepLooping=false;                                                   break;
      }
    }
    catch( LOW_exception & ex) {
      ex.logException();
    }
  }
}


void linkMenu( LOW_link::linkPtrVec_t &inLinkList, LOW_network &inNet)
{
  bool keepLooping = true;
  while ( keepLooping) {

    LOW_link::linkPtrVec_t activeFlexModeLinkList;
    for ( unsigned int a=0; a<inLinkList.size(); ++a) {
      LOW_linkFlexibleSpeed *flexLink = dynamic_cast<LOW_linkFlexibleSpeed*>(inLinkList[a]);
      if ( flexLink != 0 ) {
        if ( flexLink->getWireSpeed() == LOW_linkFlexibleSpeed::flexible_speed ) {
          activeFlexModeLinkList.push_back( inLinkList[a]);
        }
      }
    }

    hlp_printDashline();
    printf( "Links menu\n");
    printf( "\n");
    printf( "  Number of links: %d\n", inLinkList.size());
    printf( "\n");
    printf( "  <a> Add active link to network\n");
    printf( "  <p> Add passive link to network\n");
    printf( "  <u> Add all USB links to network (USB bus scan)\n");
    printf( "  <v> Add USB link to network (manually specified)\n");
    printf( "\n");
    printf( "  <i> Infos on link\n");
    printf( "  <s> Set 1-Wire speed\n");
    printf( "  <f> Set pulldown slew rate\n");
    printf( "  <w> Set write-1 low time\n");
    printf( "  <d> Set data sample offset / write-0 recovery time\n");
    printf( "\n");
    printf( "  <n> Reset adapter\n");
    printf( "  <m> Reset 1-Wire bus\n");
    printf( "\n");
    printf( "  <x> Power menu\n");
    printf( "\n");    
    printf( "  <r> Remove link from network\n");
    printf( "\n");
    printf( "  <q> Back to main menu\n");

    int key = hlp_getMenuKey( "apuvisfwdnmxrq");

    try {
      switch( key ) {
        case 'a': addActiveLink( inLinkList, inNet);              break;
        case 'p': addPassiveLink( inLinkList, inNet);             break;
        case 'u': addUsbLinkAutoAll( inLinkList, inNet);          break;
        case 'v': addUsbLinkManual( inLinkList, inNet);           break;
        case 'i': linkInfo( inLinkList);                          break;
        case 's': setWireSpeed( *hlp_selectSingleLink<LOW_linkFlexibleSpeed>( inLinkList));                       break;
        case 'f': setPulldownSlewRate( *hlp_selectSingleLink<LOW_linkFlexibleSpeed>( activeFlexModeLinkList));    break;
        case 'w': setWrite1LowTime( *hlp_selectSingleLink<LOW_linkFlexibleSpeed>( activeFlexModeLinkList));       break;
        case 'd': setDsoW0rTime( *hlp_selectSingleLink<LOW_linkFlexibleSpeed>( activeFlexModeLinkList));          break;
        case 'n': resetAdapter( *hlp_selectSingleLink<LOW_link>( inLinkList));                                    break;
        case 'm': resetBus( *hlp_selectSingleLink<LOW_link>( inLinkList));                                        break;
        case 'x': powerMenu( inLinkList);                                                                         break;
        case 'r': removeLink( hlp_selectSingleLink<LOW_link>( inLinkList), inLinkList, inNet);                    break;
        case 'q': keepLooping=false;                              break;
      }
    }
    catch( LOW_exception & ex) {
      ex.logException();
    }
  }
}
