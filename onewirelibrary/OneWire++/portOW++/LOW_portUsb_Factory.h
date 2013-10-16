/***************************************************************************
                          LOW_portUsb_Factory.h  -  description
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

#ifndef LOW_PORTUSB_FACTORY_H
#define LOW_PORTUSB_FACTORY_H


#include <string>


#include "LOW_portUsbDevice.h"


/** Factory class for platform specific objects related to USB devices.

    This class also contains platform specific type definitions.

    This class is thread-safe.
    
    @see LOW_portUsbDevice

    @author Harald Roelle
 */
class LOW_portUsb_Factory {

//=======================================================================================
public:

  //=====================================================================================
  //
  // type definitions
  //

#ifdef __linux__

  /** Linux specific type to identify a specific USB device. */
  typedef std::string   usbDeviceSpecifier_t;

#endif

  /** Vector type of USB device specifiers. */
  typedef std::vector<usbDeviceSpecifier_t>   usbDevSpecVec_t;



  //=====================================================================================
  //
  // static factory
  //

  /** Get a list of port specifiers of devices with a certain vendor and product ID.

      @param inVendorID   Vendor for new USB device instance.
      @param inProductID  Product for new USB device instance.

      @return List of port specifiers.
   */
  static usbDevSpecVec_t  getPortSpecifiers( const LOW_portUsbDevice::usbVendorID_t inVendorID,
                                             const LOW_portUsbDevice::usbProductID_t inProductID);


  /** Platform aware replacement for creating dynamic instances of LOW_portUsbDevice.

      @param inPortSpec  Unique identifier for new USB device instance.

      @return  Pointer to new instance of LOW_portUsbDevice.
   */
  static LOW_portUsbDevice* new_portUsbDevice( const usbDeviceSpecifier_t inUsbDevSpec);

  

//=======================================================================================
private:

  //=====================================================================================
  //
  // constructors
  //

  /** Constructor.
      Private to prevent instaciation as this is a static factory.
   */
  LOW_portUsb_Factory();

  /** Destructor.
      Private to prevent instaciation as this is a static factory.
   */
  virtual ~LOW_portUsb_Factory();

};

#endif

