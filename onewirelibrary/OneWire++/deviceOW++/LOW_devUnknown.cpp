/***************************************************************************
                          LOW_devUnknown.cpp  -  description
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

 
#include "LOW_devUnknown.h"
#include "LOW_deviceFactory.h"


//=====================================================================================
//
// static initializer
//

const std::string LOW_devUnknown::familyName = "Unknown device type";

int LOW_devUnknown::initHelper = initialize();
int LOW_devUnknown::initialize()
{
  LOW_deviceFactory::registerSpecificCtor( familyCode, &LOW_devUnknown::new_Instance);
  return 0;
}



//=====================================================================================
//
// static methods
//

LOW_device* LOW_devUnknown::new_Instance( LOW_netSegment &inNetSegment, const LOW_deviceID &inDevID)
{
  return new LOW_devUnknown( inNetSegment, inDevID);
}



//=====================================================================================
//
// public constructors
//

LOW_devUnknown::LOW_devUnknown( LOW_netSegment &inSegment, const LOW_deviceID &inDevID) :
  LOW_device( inSegment, inDevID, inDevID.getFamilyCode()) // here we must fake the family code to satisfy the base class
{
}


LOW_devUnknown::~LOW_devUnknown()
{
}
