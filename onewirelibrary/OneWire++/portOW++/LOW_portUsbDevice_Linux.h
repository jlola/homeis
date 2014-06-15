/***************************************************************************
                          LOW_portUsbDevice_Linux.h  -  description
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

#ifndef LOW_PORTUSBDEVICE_LINUX_H
#define LOW_PORTUSBDEVICE_LINUX_H


#include "LOW_portUsbDevice.h"
#include "LOW_portUsb_Factory.h"
#include "LOW_objectSynchronizer.h"


#include "usb.h"



/** Linux specific implementation of LOW_portUsbDevice.

    Requires libusb version 0.1.6a or higher.
    
    @see http://libusb.sourceforge.net/
    @see USB Specification Version 1.1
    
    @author Harald Roelle
    @author Parts of the documentation taken from libusb.
 */
class LOW_portUsbDevice_Linux : public LOW_portUsbDevice, public LOW_objectSynchronizer  {

//=======================================================================================
public:

  //=====================================================================================
  //
  // constructors
  //

  /** Destructor.
    */
  virtual ~LOW_portUsbDevice_Linux();



  //=====================================================================================
  //
  // methods required by LOW_portUsbDevice
  //

  virtual usbVendorID_t  getVendorID();

  virtual usbProductID_t getProductID();

  virtual void setConfiguration( const usbConfig_t inConfig);

  virtual void claimInterface( const usbInterface_t inInterface);
  
  virtual void releaseInterface( const usbInterface_t inInterface);

  virtual void setIfaceAltSetting( const usbSetting_t inAltSetting);

  virtual void controlMsg( const bmRequestType_t inReqType,
                           const bRequest_t inRequest,
                           const wValue_t inValue,
                           const wIndex_t inIndex,
                           const wLength_t inLength,
                           msgData_t inOutData,
                           const usbTimeout_t inTimeout);
  
  virtual void controlMsg( const bmRequestType_t inReqType,
                           const bRequest_t inRequest,
                           const wValue_t inValue,
                           const wIndex_t inIndex,
                           byteVec_t &inOutData,
                           const usbTimeout_t inTimeout);

  virtual void clearHalt( const usbEndpoint_t inEP);

  virtual unsigned int bulkWrite( const usbEndpoint_t inEP, const wLength_t inLength,
                                  const msgData_t inData, const usbTimeout_t inTimeout);
                          
  virtual unsigned int bulkWrite( const usbEndpoint_t inEP,
                                  const byteVec_t &inData, const usbTimeout_t inTimeout);

  virtual unsigned int bulkRead( const usbEndpoint_t inEP, const wLength_t inLength,
                                 msgData_t outData, const usbTimeout_t inTimeout);
                             
  virtual unsigned int bulkRead( const usbEndpoint_t inEP, 
                                 byteVec_t &outData, const usbTimeout_t inTimeout);
                             
  void reset();
                             
//=======================================================================================
protected:


  //=====================================================================================
  //
  // friend classes
  //

  friend class LOW_portUsb_Factory; /**< To allow construction. */

  
  //=====================================================================================
  //
  // constructors
  //

  /** Constructor.
      Not publicly constructable. Use LOW_portUsb_Factory.
  
      @param inPortSpec  Unique identifier for new USB device instance.

      @throw noSuchDevice_error  Device was not found.
   */
  LOW_portUsbDevice_Linux( const LOW_portUsb_Factory::usbDeviceSpecifier_t inUsbDevSpec);


  //=====================================================================================
  //
  // static methods
  //

  /** Get a list of port specifiers of devices with a certain vendor and product ID.

      @param inVendorID   Vendor for new USB device instance.
      @param inProductID  Product for new USB device instance.

      @return List of port specifiers.
   */
  static LOW_portUsb_Factory::usbDevSpecVec_t  getPortSpecifiers( const usbVendorID_t inVendorID,
                                                                  const usbProductID_t inProductID);



//=======================================================================================
private:

  //=====================================================================================
  //
  // attributes
  //

  struct usb_device *usbLibDevice; /**< libusb's device structure. */
  usb_dev_handle    *usbLibDevHdl; /**< libusb's handle to USB device. */


  //=====================================================================================
  //
  // static methods
  //

  /** Rescan all USB busses.
   */
  static void rescanBusses();


  //=====================================================================================
  //
  // methods
  //

  /** Get libusb's error message as C++ string.
   */
  virtual std::string libUsbErrMsg();


  //=====================================================================================
  //
  // static initializer
  //

  /** Needed for dirty little C++ hack to force static initialization on application start.
      @see initialize()
  */
  static int initHelper;

  /** Static inizializer.
      @see initHelper
  */
  static int initialize();

};

#endif
