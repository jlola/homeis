/***************************************************************************
                          LOW_network.cpp  -  description
                             -------------------
    begin                : Tue Jul 23 2002
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

#include "LOW_network.h"


//=====================================================================================
//
// constructors
//
  
LOW_network::LOW_network()
{
}


LOW_network::~LOW_network()
{
  for( unsigned int a=0; a<segmentsList.size(); a++)
    delete segmentsList[a];
}



//=====================================================================================
//
// public methods
//
  
void LOW_network::addLink( LOW_link *inLink)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE_WEAK__
  
  for( unsigned int a=0; a<linkList.size(); a++) {
    if ( *(linkList[a]) == *inLink )
      return;
  }

  linkList.push_back( inLink);
  addSegments( *inLink);
}


void LOW_network::removeLink( LOW_link *inLink)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE__

  for( unsigned int a=0; a<linkList.size(); a++) {
    if ( *(linkList[a]) == *inLink ) {

      // remove all segments on this link
      for( LOW_netSegment::netSegPtrVec_t::iterator b=segmentsList.begin(); b!=segmentsList.end();) {
        if ( (*b)->getLink() == *inLink ) {
          delete (*b);
          b = segmentsList.erase( b);
        }
        else
          ++b;
      }

      // remove the link from the list
      linkList.erase( linkList.begin()+a);
      return;
    }
  }

  throw network_error( "Link is not registered.", __FILE__, __LINE__);
}


LOW_netSegment::netSegPtrVec_t  LOW_network::getSegments() const
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  return segmentsList;
}



bool LOW_network::verifyDevice( const LOW_deviceID inDevID, const bool inOnlyAlarm) const
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  LOW_device* theDev;
  try {
    theDev = getDevice<LOW_device>( inDevID);
  }
  catch ( LOW_exception & ex) {
    return false;
  }

  return theDev->verifyDevice( inOnlyAlarm);
}



//=====================================================================================
//
// private methods
//

void LOW_network::addSegments( LOW_link &inLink)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE_WEAK__

  LOW_netSegment::netSegPtrVec_t  newSegments;
  
  newSegments = LOW_netSegment::newSegmentsFromLink( inLink);

  for( unsigned int a=0; a<newSegments.size(); a++)
    segmentsList.push_back( newSegments[a]);
}
