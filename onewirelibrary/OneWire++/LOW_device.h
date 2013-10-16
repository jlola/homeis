/***************************************************************************
                          LOW_device.h  -  description
                             -------------------
    begin                : Sat Jul 6 2002
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

#ifndef LOW_DEVICE_H
#define LOW_DEVICE_H


#include <map>

#include "LOW_link.h"
#include "LOW_deviceIDRaw.h"
#include "LOW_deviceID.h"
#include "LOW_exception.h"
#include "LOW_types.h"

class LOW_netSegment;  // forward declaration needed to resolve circular definitions.


/** Abstract base class for all 1-Wire devices.

    Any device class representing a concrete 1-Wire device must inherit from this class.

    This class is thread-safe.

    @author Harald Roelle
 */
class LOW_device {

//=======================================================================================
public:
  
  //=====================================================================================
  //
  // exceptions
  //
 
  /** Exception base class for all exceptions thrown by LOW_device. */
  class_DERIVE_FROM_EXCEPTION( device_error, LOW_exception);

  /** Exception class indicating a mismatch of expected/required family code. */
  class_DERIVE_FROM_EXCEPTION( familyMismatch_error, device_error);

  /** Exception class indicating that the requested or any device could not be found.
      This exception is also used by many other classes.
   */
  class_DERIVE_FROM_EXCEPTION( noDevice_error, device_error);

  /** Exception class indicating that an illegal dynamic type cast on a device occured.
      This exception is also used by many other classes.
   */
  class_DERIVE_FROM_EXCEPTION( illegalCast_error, device_error);

  
  //=====================================================================================
  //
  // constants
  //

  /** Pseudo family code for selecting any/all device types. */
  static const LOW_deviceIDRaw::devFamCode_t anyDev_famCode = 0x00;

  /** Pseudo family code for unknown device types. */
  static const LOW_deviceIDRaw::devFamCode_t unknownDev_famCode = 0xff;

  /** Family code of this base class equals the one for any device type.
      <B>Note:</B> Subclasses must override this constant to return their specific family code.
   */
  static const LOW_deviceIDRaw::devFamCode_t familyCode = anyDev_famCode;

  /** Family name of this base class.
      <B>Note:</B> Subclasses must override this constant to return their specific family name.
   */
  static const std::string familyName;

  static const owCommand_t  MatchROM_COMMAND; /**< 1-Wire command byte constant */
  static const owCommand_t  ReadROM_COMMAND; /**< 1-Wire command byte constant */
  static const owCommand_t  SkipROM_COMMAND; /**< 1-Wire command byte constant */
  static const owCommand_t  SearchROM_COMMAND; /**< 1-Wire command byte constant */
  static const owCommand_t  SearchAlarmROM_COMMAND; /**< 1-Wire command byte constant */
  
  
  //=====================================================================================
  //
  // type definitions
  //
  
  typedef std::vector<LOW_device*>            devPtrVec_t;  /**< Vector type of class device pointers. */
  typedef std::map<LOW_deviceID,LOW_device*>  deviceMap_t;  /**< Map type of devices with LOW_deviceID as key. */
  
  
  //=====================================================================================
  //
  // classes
  //

  /** Locking class to ensure exclusive access to a device.

      The class is intended to be used in a "locking is creation" design pattern.
      On creation an exclusive loxk is optained for the device, and on destruction
      the lock is released.
      Implemented by obtaining a LOW_link::commLock for the link the device is on.
   */
  class linkLock : public LOW_link::commLock
  {
    public:
      /** Obtain the lock.
          
          @param inDev  Reference to the device to obtain the lock for.
       */
      linkLock( const LOW_device &inDev);

      /** Release the lock. */
      ~linkLock();
  };
  
  
  //=====================================================================================
  //
  // constructors
  //

  /** Destructor.
      Deregisters the device from its LOW_netSegment.
   */
  virtual ~LOW_device();

      
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

  /** Get the device's ROM ID.
      @return ROM ID of the device.
   */
  virtual LOW_deviceID getID() const;

  /** Get the network segment the device is on.
      @return Network segment the device is on.
   */
  virtual LOW_netSegment& getNetSegment() const;

  /** Shortcut method to verify the presence of the device on it's network segment.
      @see LOW_netSegment::verifyDevice()
   */
  virtual bool verifyDevice( const bool inOnlyAlarm = false, const bool inDoReset = true) const;

  
  
//=======================================================================================
protected:
  
  //=====================================================================================
  //
  // friends
  //
  
  friend class linkLock;  /**< Needed to grant access to the protected getLink() method. */

  
  //=====================================================================================
  //
  // attributes
  //
     
  const LOW_deviceID     ID;           /**< 1-Wire ROM ID of the device. */
  LOW_netSegment         &netSegment;  /**< Network segment where the device is located. */

    
  //=====================================================================================
  //
  // constructors
  //

  /** Base constructor for devices.

      <B>Note:</B> In asymmetry to the destructor no action regarding the
                   network segment's device maps is done here. This is already
                   performed by LOW_netSegment.
  
      @param inSegment  Reference to the network segment the device is on.
      @param inDevID    Reference to device's ID.
      @param inFamCode  Expected familiy code for the device.
      @throw familyMismatch_error  Thrown when <I>inFamCode</I> and familiy code
                                   of <I>inDevID</I> don't match.
   */
  LOW_device( LOW_netSegment &inSegment, const LOW_deviceID &inDevID,
              const LOW_deviceIDRaw::devFamCode_t inFamCode);

      
  //=====================================================================================
  //
  // static methods
  //

  /** Static method for creating new concrete device objects.
      This method is de-facto virtual as no implementation is given here
      and so it must be implemented individually by every subclass.
      The returned object must be dynamically allocated (i.e. it must
      be disposable by the delete operator).

      @param inNetSegment  Reference to the network segment the device is on.
      @param inDevID       Reference to the device's ID.
      @returns  The new dynamically created device instance. 
   */
  static LOW_device* new_Instance( LOW_netSegment &inNetSegment, const LOW_deviceID &inDevID);

        
  //=====================================================================================
  //
  // methods
  //

  /** Get the link the device is on.
      @return Link the device is on.
   */    
  virtual LOW_link& getLink() const;

  /** Shortcut for issuing a matchROM command for a device.
      Calls the corresponding method in LOW_netSegment.
   */
  virtual void cmd_MatchROM() const;

};



#endif
