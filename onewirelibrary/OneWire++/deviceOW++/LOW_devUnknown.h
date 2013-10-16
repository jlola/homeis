/***************************************************************************
                          LOW_devUnknown.h  -  description
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

#ifndef LOW_DEVUNKNOWN_H
#define LOW_DEVUNKNOWN_H


#include "LOW_device.h"



/** Device class for (yet) unknown 1-Wire devices.

    This class is thread-safe.

    @author Harald Roelle
 */
class LOW_devUnknown : public LOW_device  {

//=======================================================================================
public:

  //=====================================================================================
  //
  // type definitions
  //

  typedef std::vector<LOW_devUnknown*> devUnknownPtrVec_t;  /**< Vector type of class device pointers. */


  //=====================================================================================
  //
  // constants
  //

  /** Family code of this specific device. */
  static const LOW_deviceIDRaw::devFamCode_t  familyCode = LOW_device::unknownDev_famCode;

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
  LOW_devUnknown( LOW_netSegment &inSegment, const LOW_deviceID &inDevID);


  /** Destructor.
   */
  virtual ~LOW_devUnknown();


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
