/***************************************************************************
                          LOW_devDS2405.h  -  description
                             -------------------
    begin                : Tue Aug 20 2002
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

#ifndef LOW_DEVDS2405_H
#define LOW_DEVDS2405_H



#include "LOW_device.h"
#include "LOW_helper_crc.h"



/** Device class for DS2405 Addressable Switch.

    DS2405 features:

    - Open-drain PIO pin is controlled by matching 64-bit, laser-engraved registration number
      associated with each device
    - Logic level of open drain output can be determined over 1-Wire bus for closed-loop control
    - PIO pin sink capability is greater than 4mA at 0.4V
    - Multiple DS2405s can be identified on a common 1-Wire bus and be turned on or off independent
      of other devices on the bus
    - Unique, factory-lasered and tested 64-bit registration number (8-bit family code +48- bit serial
      number +8-bit CRC tester) assures absolute identity because no two parts are alike
    - Built-in multidrop controller ensures compatibility with other MicroLAN products
    - Reduces control, address, data, and power to a single data pin
    - Directly connects to a single port pin of a microprocessor and communicates at up to 16.3kbits/s
    - 8-bit family code specifies DS2405 communications requirements to reader
    - 8-bit cyclic redundancy check ensures errorfree selection
    - Zero standby power required
    - Low cost TO-92, SOT-223, or 6-pin TSOC surface mount package
    - 1-Wire communication operates over a wide voltage range of 2.8V to 6.0V from -40°C to +85°C

    This class is thread-safe.

    @author Harald Roelle
    @author  Parts of the documentation by Dallas Semiconductors / Maxim Integrated Products
 */
class LOW_devDS2405 : public LOW_device  {

//=======================================================================================
public: 

  //=====================================================================================
  //
  // exceptions
  //

  /** Exception base class for all exceptions thrown by LOW_devDS2405. */
  class_DERIVE_FROM_EXCEPTION( devDS2405_error, LOW_exception);


  //=====================================================================================
  //
  // constants
  //

  /** Family code of this specific device. */
  static const LOW_deviceIDRaw::devFamCode_t  familyCode = 0x05;

  /** Family name of this specific device. */
  static const std::string familyName;


  //=====================================================================================
  //
  // type definitions
  //
  
  typedef std::vector<LOW_devDS2405*> devDS2405PtrVec_t;    /**< Vector type of class device pointers. */
  
    
  //=====================================================================================
  //
  // constructors
  //
  
  /** Real constructor corresponding to static pseudo constructor new_Instance().
      @param  inSegment  Reference to network segment the device is on.
      @param  inDevID    Reference to device's ID.
   */
  LOW_devDS2405( LOW_netSegment &inSegment, const LOW_deviceID &inDevID);

  /** Destructor.
   */
  virtual ~LOW_devDS2405();

  
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

  /** Toggle the state of the PIO pin.
   */
  virtual void cmd_Match() const;
 
   
  /** Toggle the state of the PIO pin and read its new state.
      
      <B>Note:</B> The return value is <B>inverted</B> compared to @ref cmd_SearchActive().
      
      @return  false, if the device IS driving the PIO pin low.
               true,  if the device is NOT driving the PIO pin low.
   */
  virtual bool cmd_MatchRead() const;
  
  
  /** Read the logic state of the PIO pin (input).

      The device will not toggle the state of its PIO pin.
      The combination of Match ROM and Search ROM allows the user to change the state
      of the PIO pin and report the current state (Match ROM) or simply report the 
      current state of the PIO pin without changing it (Search ROM).
        
      @return  false, if the pulldown is on,
                      or the pulldown is off and the PIO pin is a logical 0.<BR>
               true,  if the pulldown is off and the PIO pin is a logical 1 (i.e. external pullup).
  */
  virtual bool cmd_SearchRead() const;

    
  /** Read if the output pulldown is turned on.
      
      The device will not toggle the state of its PIO pin.
 
      <B>Note:</B> The return value is <B>inverted</B> compared to @ref cmd_MatchActive().
      
      @return  false, if the device is NOT driving the PIO pin low.
               true,  if the device IS driving the PIO pin low.
 
  */
  virtual bool cmd_SearchActive() const;

  

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
