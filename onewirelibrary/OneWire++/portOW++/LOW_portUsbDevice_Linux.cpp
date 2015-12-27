/***************************************************************************
                          LOW_portUsbDevice_Linux.cpp  -  description
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

#include "LOW_portUsbDevice_Linux.h"


#include "usb.h"


//=====================================================================================
//
// static initializer
//

int LOW_portUsbDevice_Linux::initHelper = initialize();
int LOW_portUsbDevice_Linux::initialize()
{
  // init usb library
  usb_init();

  // set debugging
  //usb_set_debug(int level);

  return 0;
}



//=====================================================================================
//
// constructors
//

LOW_portUsbDevice_Linux::LOW_portUsbDevice_Linux( const LOW_portUsb_Factory::usbDeviceSpecifier_t inUsbDevSpec)
{
  rescanBusses();

  usbLibDevice = 0;
  for ( struct usb_bus *bus = usb_busses; bus; bus = bus->next) {
    for ( struct usb_device *dev = bus->devices; dev; dev = dev->next) {
      std::string tmpStr = std::string( bus->dirname);
      tmpStr += "/";
      tmpStr += dev->filename;
      if ( inUsbDevSpec == tmpStr ) {
        usbLibDevice = dev;
        break;
      }
    }
  }

  if ( usbLibDevice == 0 )
    throw noSuchDevice_error( "specified device not found", __FILE__, __LINE__);

  usbLibDevHdl = usb_open( usbLibDevice);
  if ( usbLibDevHdl == 0 )
    throw portUsbDevice_error( "error calling usb_open(): "+libUsbErrMsg(), __FILE__, __LINE__);
    
}


LOW_portUsbDevice_Linux::~LOW_portUsbDevice_Linux()
{
  int errVal = usb_close( usbLibDevHdl);
  if ( errVal != 0 )
    LOW_helper_msglog::printPerror( errVal, "~LOW_portUsbDevice_Linux: error calling usb_close(): %s", libUsbErrMsg().c_str());
}



//=====================================================================================
//
// public methods
//


LOW_portUsbDevice_Linux::usbVendorID_t LOW_portUsbDevice_Linux::getVendorID()
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  return usbLibDevice->descriptor.idVendor;
}


LOW_portUsbDevice_Linux::usbProductID_t LOW_portUsbDevice_Linux::getProductID()
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  return usbLibDevice->descriptor.idProduct;
}



//  /** Resets a device.
//      Resets the specified device by sending a RESET down the port it is connected to.
//
//   */
//LOW_portUsbDevice_Linux::reset()
//{
//  // Causes re-enumeration: After calling usb_reset, the device will need to
//  // re-enumerate and thusly, requires you to find the new device and open a
//  // new handle. The handle used to call usb_reset will no longer work.
//  int errVal = usb_reset( usbLibDevHdl);
//  if ( errVal != 0 )
//    throw portUsbDevice_error( errVal, "error calling usb_reset()", __FILE__, __LINE__);
//  XXXXXXXXXXXXX
//}


void LOW_portUsbDevice_Linux::setConfiguration( const usbConfig_t inConfig)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE__

  int errVal = usb_set_configuration( usbLibDevHdl, inConfig);
  if ( errVal != 0 )
    throw portUsbDevice_error( errVal, "error calling usb_set_configuration(): "+libUsbErrMsg(), __FILE__, __LINE__);
}




void LOW_portUsbDevice_Linux::claimInterface( const usbInterface_t inInterface)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE__

  int errVal = usb_claim_interface( usbLibDevHdl, inInterface);

  if ( errVal != 0 )
    throw portUsbDevice_error( errVal, "error calling usb_claim_interface(): "+libUsbErrMsg(), __FILE__, __LINE__);
}


void LOW_portUsbDevice_Linux::releaseInterface( const usbInterface_t inInterface)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE__

  int errVal = usb_release_interface( usbLibDevHdl, inInterface);
  if ( errVal != 0 )
    throw portUsbDevice_error( errVal, "error calling usb_release_interface(): "+libUsbErrMsg(), __FILE__, __LINE__);
}


void LOW_portUsbDevice_Linux::setIfaceAltSetting( const usbSetting_t inAltSetting)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE__

  int errVal = usb_set_altinterface( usbLibDevHdl, inAltSetting);
  if ( errVal != 0 )
    throw portUsbDevice_error( errVal, "error calling usb_set_altinterface(): "+libUsbErrMsg(), __FILE__, __LINE__);
}




void LOW_portUsbDevice_Linux::controlMsg( const bmRequestType_t inReqType,
                                          const bRequest_t inRequest,
                                          const wValue_t inValue,
                                          const wIndex_t inIndex,
                                          const wLength_t inLength,
                                          msgData_t inOutData,
                                          const usbTimeout_t inTimeout)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE__

  int errVal = usb_control_msg( usbLibDevHdl, inReqType, inRequest, inValue, inIndex, reinterpret_cast<char*>(inOutData), inLength, inTimeout);
  if ( errVal != 0 )
    throw portUsbDevice_error( errVal, "error calling usb_control_msg(): "+libUsbErrMsg(), __FILE__, __LINE__);
}


void LOW_portUsbDevice_Linux::controlMsg( const bmRequestType_t inReqType,
                                          const bRequest_t inRequest,
                                          const wValue_t inValue,
                                          const wIndex_t inIndex,
                                          byteVec_t &inOutData,
                                          const usbTimeout_t inTimeout)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE__

  msgData_t buffer = new uint8_t[inOutData.size()];
  try {
    std::copy( inOutData.begin(), inOutData.end(), buffer );
    controlMsg( inReqType, inRequest, inValue, inIndex, inOutData.size(), buffer, inTimeout);
    std::copy( buffer, buffer+inOutData.size(), inOutData.begin());
  }
  catch( ... ) {
    delete[] buffer;
    throw;
  }
  delete[] buffer;
}




void LOW_portUsbDevice_Linux::clearHalt( const usbEndpoint_t inEP)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE__

  int errVal = usb_clear_halt( usbLibDevHdl, inEP);
  if ( errVal != 0 )
    throw portUsbDevice_error( errVal, "error calling usb_clear_halt(): "+libUsbErrMsg(), __FILE__, __LINE__);
}




unsigned int LOW_portUsbDevice_Linux::bulkWrite( const usbEndpoint_t inEP, const wLength_t inLength,
                                                 const msgData_t inData, const usbTimeout_t inTimeout)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE_WEAK__
  
  int transferBytes = usb_bulk_write( usbLibDevHdl, inEP, reinterpret_cast<char*>(inData), inLength, inTimeout);
  if ( transferBytes <= 0 )
    throw portUsbDevice_error( "error calling usb_bulk_write(): "+libUsbErrMsg(), __FILE__, __LINE__);
  return transferBytes;
}


unsigned int LOW_portUsbDevice_Linux::bulkWrite( const usbEndpoint_t inEP,
                                                 const byteVec_t &inData, const usbTimeout_t inTimeout)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE__

  unsigned int transferBytes = 0;
  msgData_t buffer = new uint8_t[inData.size()];
  try {
    std::copy( inData.begin(), inData.end(), buffer );
    transferBytes = bulkWrite( inEP, inData.size(), buffer, inTimeout);
  }
  catch( ... ) {
    delete[] buffer;
    throw;
  }
  delete[] buffer;
  return transferBytes;
}


unsigned int LOW_portUsbDevice_Linux::bulkRead( const usbEndpoint_t inEP, const wLength_t inLength,
                                                msgData_t outData, const usbTimeout_t inTimeout)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE_WEAK__

  int transferBytes = usb_bulk_read( usbLibDevHdl, inEP, reinterpret_cast<char*>(outData), inLength, inTimeout);
  if ( transferBytes <= 0 )
    throw portUsbDevice_error( "error calling usb_bulk_read(): "+libUsbErrMsg(), __FILE__, __LINE__);
  return transferBytes;
}


unsigned int LOW_portUsbDevice_Linux::bulkRead( const usbEndpoint_t inEP,
                                                byteVec_t &outData, const usbTimeout_t inTimeout)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE__

  unsigned int transferBytes = 0;
  msgData_t buffer = new uint8_t[outData.size()];
  try {
    transferBytes = bulkRead( inEP, outData.size(), buffer, inTimeout);
    std::copy( buffer, buffer+transferBytes, outData.begin());
  }
  catch( ... ) {
    delete[] buffer;
    throw;
  }
  delete[] buffer;
  return transferBytes;
}




//=====================================================================================
//
// static protected methods
//


LOW_portUsb_Factory::usbDevSpecVec_t  LOW_portUsbDevice_Linux::getPortSpecifiers( const usbVendorID_t inVendorID,
                                                                                  const usbProductID_t inProductID)
{
  rescanBusses();

  LOW_portUsb_Factory::usbDevSpecVec_t specifierVec;

  for ( struct usb_bus *bus = usb_busses; bus; bus = bus->next) {
    for ( struct usb_device *dev = bus->devices; dev; dev = dev->next) {
      if ( dev->descriptor.idVendor==inVendorID && dev->descriptor.idProduct==inProductID ) {
        std::string  specifier = std::string( bus->dirname);
        specifier += "/";
        specifier += dev->filename;
        specifierVec.push_back( specifier);
      }
    }
  }

  return specifierVec;
}




//=====================================================================================
//
// static private methods
//

void LOW_portUsbDevice_Linux::rescanBusses()
{
  // find all/new busses
  usb_find_busses();

  // find all/new devices
  usb_find_devices();
}



//=====================================================================================
//
// private methods
//

std::string LOW_portUsbDevice_Linux::libUsbErrMsg()
{
  std::string tmpStr = std::string( usb_strerror());
  return tmpStr;
}



