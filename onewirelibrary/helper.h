/***************************************************************************
                          helper.h  -  description
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

#include <vector>
#include <string.h>

#include <LOW_exception.h>
#include <LOW_device.h>
#include <LOW_network.h>
#include <LOW_helper_msglog.h>


#define MAX_MENU_ENTRIES  62

extern char         menuKeys[];


class_DERIVE_FROM_EXCEPTION( hlp_userAbort, LOW_exception);

   
int hlp_kbhit();

int hlp_getch( bool inEcho, bool inFlush);

void hlp_keyToCont();

int hlp_getMenuKey( const char *inAcceptChars);

unsigned int hlp_selectSingleOption( std::string inMsg, std::vector<std::string> inOptions, unsigned int inDefault);

bool hlp_selectBoolOption( std::string inMsg, bool inDefault);

template<class devType> void hlp_printDevList( std::vector<devType*> &inDevs);

template<class devType> std::vector<devType*> hlp_selectDevices( std::vector<devType*> &inDevs);

template<class devType> devType* hlp_selectSingleDevice( std::vector<devType*> &inDevs);

template<class devType> void hlp_getDevices( LOW_network &inNet);

template<class devType> void hlp_searchDevices( LOW_network &inNet, bool inAlarm);

unsigned int hlp_getTermLineLength();

void hlp_printDashline();


//=====================================================================================
// DEFINITIONS COMPILERS CANNOT HANDLE TO GO DIRECTLY INTO THE SOURCE
//=====================================================================================


template<class devType> void hlp_printDevList( std::vector<devType*> &inDevs)
{
  for ( unsigned int a=0; a<inDevs.size(); ++a) {
    std::string idStr  = inDevs[a]->getID().getRomIDString();
    std::string famStr = inDevs[a]->getFamilyName();
    LOW_helper_msglog::printMessage( "  Device #%d: %s (%s)\n", a, idStr.c_str(), famStr.c_str());
  }
  LOW_helper_msglog::printMessage( "Total %d devices.\n", inDevs.size());
}


template<class devType> std::vector<devType*> hlp_selectDevices( std::vector<devType*> &inDevs)
{
  if ( inDevs.size() > MAX_MENU_ENTRIES ) {
    LOW_helper_msglog::printError( "Too may device to handle in menu. Exiting...");
    exit( 1);
  }

  if ( inDevs.size() == 0 ) {
    LOW_helper_msglog::printMessage( "No devices available.\n");
    throw hlp_userAbort( "Device selection aborted.", __FILE__, __LINE__);
  }

  std::vector<bool> selected = std::vector<bool>( inDevs.size(), true);

  char acceptChars[MAX_MENU_ENTRIES+1];
  strncpy( acceptChars, menuKeys, inDevs.size());
  acceptChars[inDevs.size()] = 0;
  strcat( acceptChars, "\r ");
  
  //bool keepLooping = true;
//  while ( keepLooping ) {
//    hlp_printDashline();
//    for ( unsigned int a=0; a<inDevs.size(); ++a) {
//      std::string idStr  = inDevs[a]->getID().getRomIDString();
//      std::string famStr = inDevs[a]->getFamilyName();
//      printf( "  [%s]  <%c> Device #%d: %s (%s)\n", selected[a]==true?"*":" ", menuKeys[a], a, idStr.c_str(), famStr.c_str());
//    }
//    printf( "\n");
//    printf( "De/select, <space> to invert, <return> to finish -> ");
//
//    int key;
//    while( true ) {
//      key = hlp_getch( false, true);
//      if ( index( acceptChars, key) != 0 ) {
//        break;
//      }
//      if ( key == 27 ) // ESC
//        throw hlp_userAbort( "Selection aborted by user.", __FILE__, __LINE__);
//    }
//
//    if      ( key==' ' ) {
//      printf( "<space>\n");
//      for ( unsigned int a=0; a<selected.size(); ++a)
//        selected[a] = ! selected[a];
//    }
//    else if ( key=='\r' ) {
//      printf( "\n");
//      keepLooping = false;
//    }
//    else {
//      printf( "%c\n", key);
//      unsigned int idx = index( menuKeys, key) - menuKeys;
//      selected[idx] = ! selected[idx];
//    }
//  }


//  std::vector<devType*> retVal;
//  for ( unsigned int a=0; a<selected.size(); ++a)
//    if ( selected[a] )
//      retVal.push_back( inDevs[a]);
//
//  return retVal;
  return inDevs;
}


template<class devType> devType* hlp_selectSingleDevice( std::vector<devType*> &inDevs)
{
  if ( inDevs.size() > MAX_MENU_ENTRIES ) {
    LOW_helper_msglog::printError( "Too may device to handle in menu. Exiting...");
    exit( 1);
  }

  if ( inDevs.size() == 0 ) {
    LOW_helper_msglog::printMessage( "No devices available.\n");
    throw hlp_userAbort( "Device selection aborted.", __FILE__, __LINE__);
  }

  char acceptChars[MAX_MENU_ENTRIES+1];
  strncpy( acceptChars, menuKeys, inDevs.size());
  acceptChars[inDevs.size()] = 0;

  hlp_printDashline();
  for ( unsigned int a=0; a<inDevs.size(); ++a) {
    std::string idStr  = inDevs[a]->getID().getRomIDString();
    std::string famStr = inDevs[a]->getFamilyName();
    printf( "  <%c> Device #%d: %s (%s)\n", menuKeys[a], a, idStr.c_str(), famStr.c_str());
  }

  printf( "\n");
  printf( "Select device -> ");

  int key;
  while( true ) {
    key = hlp_getch( false, true);
    if ( index( acceptChars, key) != 0 ) {
      break;
    }
    if ( key == 27 ) // ESC
      throw hlp_userAbort( "Selection aborted by user.", __FILE__, __LINE__);
  }

  printf( "%c\n", key);
  unsigned int selected = index( menuKeys, key) - menuKeys;

  return inDevs[selected];
}


template<class devType> void hlp_getDevices( LOW_network &inNet)
{
  std::vector<devType*> allDevs = inNet.template getDevices<devType>();

  hlp_printDashline();
  LOW_helper_msglog::printMessage( "GET %s devices on the whole network:\n", devType::familyName.c_str());
  hlp_printDevList( allDevs);
}


template<class devType> void hlp_searchDevices( LOW_network &inNet, bool inAlarm)
{
  std::vector<devType*> allDevs = inNet.template searchDevices<devType>( inAlarm);

  hlp_printDashline();
  LOW_helper_msglog::printMessage( "SEARCH %s %s devices on the whole network:\n", inAlarm==true?"ALARMING":"ALL", devType::familyName.c_str());
  hlp_printDevList( allDevs);
}


template<class linkType_t> linkType_t* hlp_selectSingleLink( LOW_link::linkPtrVec_t &inLinks)
{
  std::vector<linkType_t*> selectList;
  for ( unsigned int a=0; a<inLinks.size(); ++a) {
    linkType_t* tmpPtr = dynamic_cast<linkType_t*>(inLinks[a]);
    if ( tmpPtr != 0 )
      selectList.push_back( tmpPtr);
  }
  
  if ( selectList.size() > MAX_MENU_ENTRIES ) {
    LOW_helper_msglog::printError( "Too may links to handle in menu. Exiting...");
    exit( 1);
  }

  if ( selectList.size() == 0 ) {
    LOW_helper_msglog::printMessage( "No links available.\n");
    throw hlp_userAbort( "Link selection aborted.", __FILE__, __LINE__);
  }

  char acceptChars[MAX_MENU_ENTRIES+1];
  strncpy( acceptChars, menuKeys, selectList.size());
  acceptChars[selectList.size()] = 0;

  hlp_printDashline();
  for ( unsigned int a=0; a<selectList.size(); ++a) {
    printf( "  <%c> Link ID %d: %s\n", menuKeys[a], dynamic_cast<LOW_link*>(selectList[a])->getID(),
                                                    dynamic_cast<LOW_link*>(selectList[a])->getLinkFamily().c_str());
  }

  printf( "\n");
  printf( "Select link -> ");

  int key;
  while( true ) {
    key = hlp_getch( false, true);
    if ( index( acceptChars, key) != 0 ) {
      break;
    }
    if ( key == 27 ) // ESC
      throw hlp_userAbort( "Selection aborted by user.", __FILE__, __LINE__);
  }

  printf( "%c\n", key);
  unsigned int selected = index( menuKeys, key) - menuKeys;

  return selectList[selected];
}


