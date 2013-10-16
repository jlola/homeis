/***************************************************************************
                          LOW_deviceFactory.cpp  -  description
                             -------------------
    begin                : Mon Aug 5 2002
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


#include "LOW_deviceFactory.h"



//=====================================================================================
//
// constructors
//

LOW_deviceFactory::LOW_deviceFactory()
{
}


LOW_deviceFactory::~LOW_deviceFactory()
{
}


//=====================================================================================
//
// static methods
//

LOW_device* LOW_deviceFactory::new_SpecificDevice( LOW_netSegment &inSegment, const LOW_deviceID &inDevID)
{
  ctorFktMap_t::iterator foundCnstr = deviceCtorsSingleton->find( inDevID.getFamilyCode());
  
  if ( foundCnstr == deviceCtorsSingleton->end() )
    if ( unknownDevCtor == 0 )
      throw deviceFactory_error( "Family code unknown and no constructor for unknown devices registered.", __FILE__, __LINE__);
    else
      return unknownDevCtor( inSegment, inDevID);
  else
    return foundCnstr->second( inSegment, inDevID);
}


void LOW_deviceFactory::registerSpecificCtor( LOW_deviceIDRaw::devFamCode_t inFamCode, newInstanceFkt_t inPseudoCnstr)
{
  // singleton design pattern
  if ( deviceCtorsSingleton == 0 )
    deviceCtorsSingleton = new ctorFktMap_t();

  if ( inFamCode == LOW_device::anyDev_famCode )
    throw deviceFactory_error( "Cannot register for anyDev_famCode.", __FILE__, __LINE__);

  if ( inFamCode == LOW_device::unknownDev_famCode ) {
    if ( unknownDevCtor != 0 )
      throw deviceFactory_error( "Constructor for unknown devices already registered.", __FILE__, __LINE__);
    unknownDevCtor = inPseudoCnstr;
  }
  else {
    if ( (*deviceCtorsSingleton)[inFamCode] != 0 )
      throw deviceFactory_error( "Familiy type already registered. Family code: "+
                                 inFamCode, __FILE__, __LINE__);
    (*deviceCtorsSingleton)[inFamCode] = inPseudoCnstr;
  }
}



//=====================================================================================
//
// static attributes
//

// Do not really initialize here! Must use singleton pattern!
LOW_deviceFactory::ctorFktMap_t*  LOW_deviceFactory::deviceCtorsSingleton = 0;

LOW_deviceFactory::newInstanceFkt_t LOW_deviceFactory::unknownDevCtor = 0;

