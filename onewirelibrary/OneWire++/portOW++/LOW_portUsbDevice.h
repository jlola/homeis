/***************************************************************************
                          LOW_portUsbDevice.h  -  description
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

#ifndef LOW_PORTUSBDEVICE_H
#define LOW_PORTUSBDEVICE_H


#include "LOW_types.h"
#include "LOW_exception.h"


/** Abstract base class for a device on a USB port.
    Each instance represents one USB device.

    Specific platforms dereive their implementation classes from this class.

    The instances are created by LOW_portUsb_Factory, following the factory
    design pattern.

    <B>Note:</B> There is no prescribed constructor. A class deriving from this
                 one should have two constructors. One which requires to specify
                 some kind of address identifying the device, the other a USB
                 vendor/product ID pair returning the first device of that kind
                 found.

    This class is far away from beeing generic, it just contains stuff needed for the DS2490.

    This class is thread-safe.

    @see LOW_portUsb_Factory
    @see USB Specification Version 1.1

    @author Harald Roelle
    @author Parts of the documentation taken from libusb.
 */
class LOW_portUsbDevice {

//=======================================================================================
public:

  //=====================================================================================
  //
  // exceptions
  //

  /** Exception base class for all exceptions thrown by LOW_portUsbDevice. */
  class_DERIVE_FROM_EXCEPTION( portUsbDevice_error, LOW_exception);

  /** Exception class when a device could not be found. */
  class_DERIVE_FROM_EXCEPTION( noSuchDevice_error, portUsbDevice_error);



  //=====================================================================================
  //
  // type definitions
  //

  typedef uint16_t   usbVendorID_t;   /**< Type for USB vendor ID. */
  typedef uint16_t   usbProductID_t;  /**< Type for USB product ID. */
  typedef uint8_t    usbConfig_t;     /**< Type for USB configuration. */
  typedef uint8_t    usbInterface_t;  /**< Type for USB interface. */
  typedef uint8_t    usbSetting_t;    /**< Type for USB setting. */
  typedef uint8_t    bmRequestType_t; /**< Corresponds to type defined in USB 1.1 specification. */
  typedef uint8_t    bRequest_t;      /**< Corresponds to type defined in USB 1.1 specification. */
  typedef uint16_t   wValue_t;        /**< Corresponds to type defined in USB 1.1 specification. */
  typedef uint16_t   wIndex_t;        /**< Corresponds to type defined in USB 1.1 specification. */
  typedef uint16_t   wLength_t;       /**< Corresponds to type defined in USB 1.1 specification. */
  typedef uint8_t    *msgData_t;      /**< Pointer type for I/O methods. */
  typedef uint16_t   usbTimeout_t;    /**< Type for timeout on USB operations in ms. */
  typedef uint8_t    usbEndpoint_t;   /**< Type for USB endpoint. */


  
  //=====================================================================================
  //
  // constructors
  //

  /** Destructor.
   */
  virtual ~LOW_portUsbDevice();



  //=====================================================================================
  //
  // methods
  //

  /** Get vendor ID of USB device.
   */
  virtual usbVendorID_t  getVendorID() = 0;

  
  /** Get product ID of USB device.
   */
  virtual usbProductID_t getProductID() = 0;
  

  /**  Sets the active configuration of a device.
       @param inConfig  The value as specified in the USB descriptor field bConfigurationValue.
   */
  virtual void setConfiguration( const usbConfig_t inConfig) = 0;

  
  /** Claim an interface of a device.
      Claims the interface with the Operating System.

      <b>Note</b>: The method must be called before you perform any operations related
      to this interface (like setIfaceAltSetting(), bulkWrite(), etc).

      @param inInterface The value as specified in the USB descriptor field bInterfaceNumber.
   */
  virtual void claimInterface( const usbInterface_t inInterface) = 0;

  
  /** Releases a previously claimed interface.
      Releases an interface previously claimed with claimInterface().

      @param inInterface  The value as specified in the USB descriptor field bInterfaceNumber.
   */
  virtual void releaseInterface( const usbInterface_t inInterface) = 0;

  
  /** Sets the active alternate setting of the current interface.
      @param inAltInterface   The value as specified in the USB descriptor field bAlternateSetting.
   */
  virtual void setIfaceAltSetting( const usbSetting_t inAltSetting) = 0;


  /** Send a control message to a device.
      Performs a control request to the default control pipe on a device.

      The parameters mirror the types of the same name in the USB specification.
   */
  virtual void controlMsg( const bmRequestType_t inReqType,
                           const bRequest_t inRequest,
                           const wValue_t inValue,
                           const wIndex_t inIndex,
                           const wLength_t inLength,
                           msgData_t inOutData,
                           const usbTimeout_t inTimeout) = 0;

  /** Send a control message to a device.
      Performs a control request to the default control pipe on a device.

      The parameters mirror the types of the same name in the USB specification.
   */
  virtual void controlMsg( const bmRequestType_t inReqType,
                           const bRequest_t inRequest,
                           const wValue_t inValue,
                           const wIndex_t inIndex,
                           byteVec_t &inOutData,
                           const usbTimeout_t inTimeout) = 0;

                           
  /** Clears any halt status on an endpoint.

      @param inEOP  The value specified in the USB descriptor field bEndpointAddress.
   */
  virtual void clearHalt( const usbEndpoint_t inEP) = 0;


  
  /** Write data to a bulk endpoint.
      @param inEP       The endpoint.
      @param inLength   Number of bytes to write.
      @param inData     Pointer to data to write.
      @param inTimeout  Timeout to wait for completion.
      @return Number of bytes actually written.
   */
  virtual unsigned int bulkWrite( const usbEndpoint_t inEP, const wLength_t inLength,
                                  const msgData_t inData, const usbTimeout_t inTimeout) = 0;

  /** Write data to a bulk endpoint.
      @param inEP       The endpoint.
      @param inData     Data to write.
      @param inTimeout  Timeout to wait for completion.
      @return Number of bytes actually written.
   */
  virtual unsigned int bulkWrite( const usbEndpoint_t inEP,
                                  const byteVec_t &inData, const usbTimeout_t inTimeout) = 0;

                          
  /** Read data from a bulk endpoint.
      @param inEP       The endpoint.
      @param inLength   Number of bytes to read.
      @param outData    Pointer to memory to write data to. Memory must be already allocated!
      @param inTimeout  Timeout to wait for completion.
      @return Number of bytes actually read.
   */
  virtual unsigned int bulkRead( const usbEndpoint_t inEP, const wLength_t inLength,
                                 msgData_t outData, const usbTimeout_t inTimeout) = 0;

  /** Read data from a bulk endpoint.
      @param inEP       The endpoint.
      @param outData    Data that was read. Preset length of the array determines amount of bytes to read.
      @param inTimeout  Timeout to wait for completion.
      @return Number of bytes actually read.
   */
  virtual unsigned int bulkRead( const usbEndpoint_t inEP,
                                 byteVec_t &outData, const usbTimeout_t inTimeout) = 0;

};

#endif
