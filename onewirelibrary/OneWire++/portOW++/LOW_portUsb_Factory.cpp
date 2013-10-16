/***************************************************************************
                          LOW_portUsb_Factory.cpp  -  description
                             -------------------
    begin                : Sun Oct 12 2003
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

#include "LOW_portUsb_Factory.h"
#include "LOW_portUsbDevice_Linux.h"


//=====================================================================================
//
// constructors
//

LOW_portUsb_Factory::LOW_portUsb_Factory()
{
}


LOW_portUsb_Factory::~LOW_portUsb_Factory()
{
}


//=====================================================================================
//
// static factory
//

LOW_portUsb_Factory::usbDevSpecVec_t  LOW_portUsb_Factory::getPortSpecifiers( const LOW_portUsbDevice::usbVendorID_t inVendorID,
                                                                              const LOW_portUsbDevice::usbProductID_t inProductID)
{
#ifdef __linux__
  return LOW_portUsbDevice_Linux::getPortSpecifiers( inVendorID, inProductID);
#endif
}


LOW_portUsbDevice* LOW_portUsb_Factory::new_portUsbDevice( const usbDeviceSpecifier_t inUsbDevSpec)
{
#ifdef __linux__
  return new LOW_portUsbDevice_Linux( inUsbDevSpec);
#endif
}
