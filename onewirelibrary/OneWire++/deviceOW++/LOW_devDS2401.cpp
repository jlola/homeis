/***************************************************************************
                          LOW_devDS2401.cpp  -  description
                             -------------------
    begin                : Sat Nov 8 2003
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

#include "LOW_devDS2401.h"
#include "LOW_deviceFactory.h"



//=====================================================================================
//
// static initializer
//

const std::string LOW_devDS2401::familyName = "DS2401";

int LOW_devDS2401::initHelper = initialize();
int LOW_devDS2401::initialize()
{
  LOW_deviceFactory::registerSpecificCtor( familyCode, &LOW_devDS2401::new_Instance);
  return 0;
}



//=====================================================================================
//
// static methods
//

LOW_device* LOW_devDS2401::new_Instance( LOW_netSegment &inNetSegment, const LOW_deviceID &inDevID)
{
  return new LOW_devDS2401( inNetSegment, inDevID);
}



//=====================================================================================
//
// public constructors
//

LOW_devDS2401::LOW_devDS2401( LOW_netSegment &inSegment, const LOW_deviceID &inDevID) :
  LOW_device( inSegment, inDevID, familyCode)
{
}


LOW_devDS2401::~LOW_devDS2401()
{
}
