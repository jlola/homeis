/***************************************************************************
                          LOW_devDS2401.h  -  description
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

#ifndef LOW_DEVDS2401_H
#define LOW_DEVDS2401_H


#include "LOW_device.h"



/** Device class for DS2401 Silicon Serial Number.

    DS2401 features:

    - Upgrade and drop-in replacement for DS2400
    - Extended 2.8 to 6.0 voltage range
    - Multiple DS2401s can reside on a common 1-Wire Net
    - Unique, factory-lasered and tested 64-bit registration number; guaranteed no two parts alike
    - Built-in multidrop controller ensures compatibility with other 1-Wire Net products
    - 8-bit family code specifies DS2401 communications requirements to reader
    - Presence Pulse acknowledges when the reader first applies voltage
    - Low-cost TO-92, SOT-223, and TSOC surface mount packages
    - Reduces control, address, data, and power to a single pin
    - Zero standby power required
    - Directly connects to a single port pin of a microprocessor and communicates at up to 16.3kbits/s
    - TO-92 Tape & Reel version with leads bent to 100mil spacing (default) or with straight leads (DS2401T-SL)
    - Applications: PCB Identification, Network Node ID, Equipment Registration
    - Operates over industrial temperature range of -40°C to +85°C

    This class is thread-safe.

    @author Harald Roelle
    @author  Parts of the documentation by Dallas Semiconductors / Maxim Integrated Products
 */
class LOW_devDS2401 : public LOW_device  {

//=======================================================================================
public:

  //=====================================================================================
  //
  // type definitions
  //

  typedef std::vector<LOW_devDS2401*> devDS2401PtrVec_t;  /**< Vector type of class device pointers. */

  
  //=====================================================================================
  //
  // constants
  //

  /** Family code of this specific device. */
  static const LOW_deviceIDRaw::devFamCode_t  familyCode = 0x01;

  /** Family name of this specific device. */
  static const std::string familyName;


  //=====================================================================================
  //
  // constructors
  //

  /** Real constructor corresponding to static pseudo constructor new_Instance().

      External power supply status is initially detected here.

      @param  inSegment  Reference to network segment the device is on.
      @param  inDevID    Reference to device's ID.
   */
  LOW_devDS2401( LOW_netSegment &inSegment, const LOW_deviceID &inDevID);


  /** Destructor.
   */
  virtual ~LOW_devDS2401();

  
  //=====================================================================================
  //
  // methods
  //

  /** Get the device's family code.
      <B>Note:</B> Subclasses must implement this method to return their specific family code.
      @return Family name of the device.
   */
  virtual const LOW_deviceIDRaw::devFamCode_t getFamilyCode() const { return familyCode; };

  /** Get the device's family name.
      <B>Note:</B> Subclasses must implement this method to return a clear text
                   name of their family.
      @return Family name of the device.
   */
  virtual const std::string getFamilyName() const { return familyName; };



//=======================================================================================
protected:

  //=====================================================================================
  //
  // static methods
  //

  /** Static pseudo constructor for registering with LOW_deviceFactory.
      @param  inSegment  Reference to network segment the device is on.
      @param  inDevID    Reference to device's ID.
      @return  New dynamic instance of specific device class.
   */
  static LOW_device* new_Instance( LOW_netSegment &inNetSegment, const LOW_deviceID &inDevID);



//=======================================================================================
private:

  //=====================================================================================
  //
  // static initializer
  //

  /** Needed for dirty little C++ hack to force static initialization on application start.
      @see initialize()
  */
  static int initHelper;

  /** Static inizializer to register the class with LOW_deviceFactory.
      @see initHelper
  */
  static int initialize();

};

#endif
