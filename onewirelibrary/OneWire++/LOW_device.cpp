/***************************************************************************
                          LOW_device.cpp  -  description
                             -------------------
    begin                : Sat Jul 6 2002
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

 
#include "LOW_device.h"
#include "LOW_netSegment.h"
#include "PoppyDebugTools.h"
//=====================================================================================
//
// static constant init
//

const std::string LOW_device::familyName = "any";



//=====================================================================================
//
// classes
//
  
LOW_device::linkLock::linkLock( const LOW_device &inDev) : LOW_link::commLock( inDev.getLink())
{
}


LOW_device::linkLock::~linkLock()
{
}

    
  
//=====================================================================================
//
// constructors
//

LOW_device::LOW_device( LOW_netSegment &inSegment, const LOW_deviceID &inDevID,
                        const LOW_deviceIDRaw::devFamCode_t inFamCode) :
  ID( inDevID),
  netSegment( inSegment)
{
  if ( ID.getFamilyCode() != inFamCode)
    throw familyMismatch_error( "Family code mismatch", __FILE__, __LINE__);
}


LOW_device::~LOW_device()
{
  netSegment.unregisterDevice( this);
}


//=====================================================================================
//
// public methods
//

LOW_deviceID LOW_device::getID() const
{
  return ID;
}


LOW_netSegment& LOW_device::getNetSegment() const
{
  return netSegment;
}


bool LOW_device::verifyDevice( const bool inOnlyAlarm, const bool inDoReset) const
{
  return netSegment.verifyDevice( ID, inOnlyAlarm, inDoReset);
}


//=====================================================================================
//
// protected methods
//

LOW_link& LOW_device::getLink() const
{
  return netSegment.getLink();
}


void LOW_device::cmd_MatchROM() const
{
	STACK
  netSegment.cmd_MatchROM( this);
}
