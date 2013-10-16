/***************************************************************************
                          LOW_netSegment.cpp  -  description
                             -------------------
    begin                : Sun Jul 7 2002
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

#include "LOW_netSegment.h"
#include "LOW_deviceFactory.h"



//=====================================================================================
//
// static attributes
//

__LOW_SYNCHRONIZE_INIT_PROTECTED_LOCK__(LOW_netSegment)


//=====================================================================================
//
// constructors
//

LOW_netSegment::LOW_netSegment( LOW_link &inLink) :
  segmentID( LOW_objectIDFactory::getNewObjectID()), 
  link( inLink)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE__

  hasExternalPower = link.getHasExternalPower();

  searchDevices<LOW_device>();  //find all devices on bus
}


LOW_netSegment::~LOW_netSegment()
{
  for( LOW_device::deviceMap_t::const_iterator a=aliveDevMap.begin(); a!=aliveDevMap.end(); ++a)
    delete a->second;

  for( LOW_device::deviceMap_t::const_iterator a=graveyardMap.begin(); a!=graveyardMap.end(); ++a)
    delete a->second;
}



//=====================================================================================
//
// static methods
//

LOW_netSegment::netSegPtrVec_t LOW_netSegment::newSegmentsFromLink( LOW_link &inLink)
{
  __LOW_SYNCHRONIZE_STATIC_WRITE__
  
  netSegPtrVec_t  segments;
  
  /** @warning  By now only a single segment is supported
      @todo Add support for multi-segment links
   */
  segments.push_back( new LOW_netSegment( inLink));

  return segments;
}


//=====================================================================================
//
// operator overloading
//

bool LOW_netSegment::operator==(LOW_netSegment &inSegment) const
{
  __LOW_SYNCHRONIZE_METHOD_READ_WEAK__

  return (segmentID==inSegment.segmentID);
}



//=====================================================================================
//
// public methods
//

LOW_link& LOW_netSegment::getLink()
{
  __LOW_SYNCHRONIZE_METHOD_READ_WEAK__

  return link;
}


bool LOW_netSegment::getHasExternalPower() const
{
  __LOW_SYNCHRONIZE_METHOD_READ_WEAK__

  return hasExternalPower;
}


bool LOW_netSegment::verifyDevice( const LOW_deviceID inDevID, const bool inOnlyAlarm, const bool inDoReset)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE_WEAK__

  bool isPresent = cmd_SearchROMVerify( inDevID, inOnlyAlarm, inDoReset);
  
  if ( !inOnlyAlarm ) {
    if ( isPresent ) {
      addDevice( inDevID);
    }
    else { 
      addDevice( inDevID);
      try {
        LOW_device *theDev = getDevice<LOW_device>( inDevID);
        revitalizeDevice( theDev);
      }
      catch ( LOW_exception ex) {
        return false;
      }
    }
  }

  return isPresent;
}



//=====================================================================================
//
// protected methods
//
  
void LOW_netSegment::unregisterDevice( const LOW_device *inDev)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE_WEAK__

  removeDevice( inDev);
}


                              
//=====================================================================================
//
// protected addressing/searching methods
//

void LOW_netSegment::cmd_MatchROM( const LOW_device *inDevice) const
{
  __LOW_SYNCHRONIZE_METHOD_READ_WEAK__

  LOW_link::commLock lock( link);
  
  if ( link.resetBus() == false )
    throw noDevice_error( "Reset indicated no devices", __FILE__, __LINE__);
  
  byteVec_t outVec;
    
  outVec.push_back( static_cast<uint8_t>( LOW_device::MatchROM_COMMAND));
  
  byteVec_t id = inDevice->getID().getRomIDVec();  
  outVec.insert( outVec.end(), id.begin(), id.end());
    
  link.writeData( outVec);
}


LOW_deviceID LOW_netSegment::cmd_ReadROM() const
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  LOW_link::commLock lock( link);
  
  if ( link.resetBus() == false )
    throw noDevice_error( "Reset indicated no devices", __FILE__, __LINE__);
      
  link.writeData( LOW_device::ReadROM_COMMAND);

  byteVec_t idBytes = byteVec_t( sizeof( LOW_deviceIDRaw::devRomID_t));
  
  link.readData( idBytes);

  return LOW_deviceID( idBytes);
}


void LOW_netSegment::cmd_SkipROM() const
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  LOW_link::commLock lock( link);
  
  if ( link.resetBus() == false )
    throw noDevice_error( "Reset indicated no devices", __FILE__, __LINE__);
      
  link.writeData( LOW_device::SkipROM_COMMAND);
}
    

LOW_deviceID::deviceIDVec_t LOW_netSegment::cmd_SearchROM( const bool inOnlyAlarm,
                                                           const LOW_deviceIDRaw::devFamCode_t inFamCode) const
{
  __LOW_SYNCHRONIZE_METHOD_READ_WEAK__

  return link.searchDevices( inOnlyAlarm, LOW_deviceIDRaw() /* init with 0 */, inFamCode, true);
}
    

bool LOW_netSegment::cmd_SearchROMVerify( const LOW_deviceID inDevID, const bool inOnlyAlarm, const bool inDoReset) const
{
  __LOW_SYNCHRONIZE_METHOD_READ_WEAK__

  //@todo CHECK NEW IMPLEMENTATION
  LOW_deviceID::deviceIDVec_t results = link.searchDevices( inOnlyAlarm, inDevID, LOW_device::anyDev_famCode, inDoReset);
      
  if ( results.size()>0 && results[0]==inDevID )
    return true;
  else
    return false;
}



//=====================================================================================
//
// private active map / graveyard management
//

void LOW_netSegment::buryDevice( const LOW_device *inDev)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE_WEAK__

  // look in alive map, if not present simply return
  LOW_device::deviceMap_t::iterator foundAlive = aliveDevMap.find( inDev->getID());
  if ( foundAlive == aliveDevMap.end() ) {
    return;
  }
  
  LOW_device *theDev = foundAlive->second;
  aliveDevMap.erase( foundAlive);
  
  graveyardMap[theDev->getID()] = theDev;
  // theDev.bury();
}


void LOW_netSegment::revitalizeDevice( const LOW_device *inDev)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE_WEAK__

  // look in graveyard, if not present simply return
  LOW_device::deviceMap_t::iterator foundGraveyard = graveyardMap.find( inDev->getID());
  if ( foundGraveyard == graveyardMap.end() ) {
    return;
  }
  
  LOW_device *theDev = foundGraveyard->second;
  graveyardMap.erase( foundGraveyard);
  
  aliveDevMap[theDev->getID()] = theDev;
  // theDev.revitalize();
}


LOW_device* LOW_netSegment::addDevice( const LOW_deviceID inDevID)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE_WEAK__

  try
  {

	  // look in alive map, if present simply return device
	  LOW_device::deviceMap_t::iterator foundAlive = aliveDevMap.find( inDevID);
	  if ( foundAlive != aliveDevMap.end() ) {
		return foundAlive->second;
	  }

	  // look in graveyard, if present revitalize it and return it
	  LOW_device::deviceMap_t::iterator foundGraveyard = graveyardMap.find( inDevID);
	  if ( foundGraveyard != graveyardMap.end() ) {
		LOW_device *theDev = foundGraveyard->second;
		revitalizeDevice( theDev);
		return theDev;
	  }

	  // now it must be really new, so create it, add it to alive map and return it
	  LOW_device* newDev = LOW_deviceFactory::new_SpecificDevice( *this, inDevID);
	  aliveDevMap[inDevID] = newDev;
	  return newDev;
  }
  catch( LOW_exception &ex) {
	ex.logException();
	return NULL;
  }

}


void LOW_netSegment::removeDevice( const LOW_device *inDev)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE_WEAK__

  const LOW_deviceID devID = inDev->getID();
  
  // look in alive map, if present delete it and return
  LOW_device::deviceMap_t::iterator foundAlive = aliveDevMap.find( inDev->getID());
  if ( foundAlive != aliveDevMap.end() ) {
    aliveDevMap.erase( foundAlive);
    return;
  }
    
  // look in graveyard, if present delete it and return
  LOW_device::deviceMap_t::iterator foundGraveyard = graveyardMap.find( inDev->getID());
  if ( foundGraveyard != graveyardMap.end() ) {
    graveyardMap.erase( foundGraveyard);
    return;
  }
}

