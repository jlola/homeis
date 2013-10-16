/***************************************************************************
                          LOW_devDS2405.cpp  -  description
                             -------------------
    begin                : Tue Aug 20 2002
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


#include "LOW_devDS2405.h"
#include "LOW_platformMisc.h"
#include "LOW_helper_msglog.h"
#include "LOW_deviceFactory.h"
#include "LOW_netSegment.h"



//=====================================================================================
//
// static initializer
//

const std::string LOW_devDS2405::familyName = "DS2405";

int LOW_devDS2405::initHelper = initialize();
int LOW_devDS2405::initialize()
{
  LOW_deviceFactory::registerSpecificCtor( familyCode, &LOW_devDS2405::new_Instance);
  return 0;
}
        

        
//=====================================================================================
//
// static methods
//
  
LOW_device* LOW_devDS2405::new_Instance( LOW_netSegment &inNetSegment, const LOW_deviceID &inDevID)
{
  return new LOW_devDS2405( inNetSegment, inDevID);
}



//=====================================================================================
//
// constructors
//

LOW_devDS2405::LOW_devDS2405( LOW_netSegment &inSegment, const LOW_deviceID &inDevID) : 
  LOW_device( inSegment, inDevID, familyCode)
{
}


LOW_devDS2405::~LOW_devDS2405()
{
}


//=====================================================================================
//
// methods
//

void LOW_devDS2405::cmd_Match() const
{
  linkLock  lock( *this);
  
  cmd_MatchROM();
}


bool LOW_devDS2405::cmd_MatchRead() const
{ 
  linkLock  lock( *this);

  cmd_Match();
  return getLink().readDataBit();
}


bool LOW_devDS2405::cmd_SearchRead() const
{
  linkLock  lock( *this);
  
  verifyDevice( false, false);
  return getLink().readDataBit();
}


bool LOW_devDS2405::cmd_SearchActive() const
{
  linkLock  lock( *this);
  
  return verifyDevice( true);
}
