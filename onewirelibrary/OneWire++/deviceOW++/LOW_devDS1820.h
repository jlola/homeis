/***************************************************************************
                          LOW_devDS1820.h  -  description
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

#ifndef LOW_DEVDS1820_H
#define LOW_DEVDS1820_H



#include "LOW_device.h"



/** Device class for DS18S20 High-Precision 1-Wire Digital Thermometer.

    DS1820 features:

    - Unique 1-Wire interface requires only one port pin for communication
    - Each device has a unique 64-bit serial code stored in an onboard ROM
    - Multidrop capability simplifies distributed temperature sensing applications
    - Requires no external components
    - Can be powered from data line. Power supply range is 3.0V to 5.5V
    - Measures temperatures from  55°C to +125°C ( 67°F to +257°F)
    - 0.5 C accuracy from  10°C to +85°C
    - 9-bit thermometer resolution
    - Converts temperature in 750ms (max.)
    - User-definable nonvolatile (NV) alarm settings
    - Alarm search command identifies and addresses devices whose temperature is outside
      of programmed limits (temperature alarm condition)
    - Applications include thermostatic controls, industrial systems, consumer products,
      thermometers, or any thermally sensitive system

    This class is thread-safe.

    @author Harald Roelle
    @author  Parts of the documentation by Dallas Semiconductors / Maxim Integrated Products
 */
class LOW_devDS1820 : public LOW_device  {

//=======================================================================================
public: 

  //=====================================================================================
  //
  // exceptions
  //

  /** Exception base class for all exceptions thrown by LOW_devDS1820. */
  class_DERIVE_FROM_EXCEPTION( devDS1820_error, LOW_exception);



  //=====================================================================================
  //
  // constants
  //
  
  /** Family code of this specific device. */
  static const LOW_deviceIDRaw::devFamCode_t  familyCode = 0x28;

  /** Family name of this specific device. */
  static const std::string familyName;

  static const owCommand_t  ConvertT_COMMAND         = 0x44; /**< 1-Wire command byte constant */
  static const owCommand_t  ReadScratchpad_COMMAND   = 0xbe; /**< 1-Wire command byte constant */
  static const owCommand_t  WriteScratchpad_COMMAND  = 0x4e; /**< 1-Wire command byte constant */
  static const owCommand_t  CopyScratchpad_COMMAND   = 0x48; /**< 1-Wire command byte constant */
  static const owCommand_t  RecallE2_COMMAND         = 0xb8; /**< 1-Wire command byte constant */
  static const owCommand_t  ReadPowerSupply_COMMAND  = 0xb4; /**< 1-Wire command byte constant */

  
  //=====================================================================================
  //
  // type definitions
  //
  
  typedef std::vector<LOW_devDS1820*> devDS1820PtrVec_t;  /**< Vector type of class device pointers. */

  /** DS1820 internal scratchpad as defined by Dallas. */
  typedef struct scratchpadDS1820_t {
    uint8_t  tempLSB;
    uint8_t  tempMSB; 
    uint8_t  tH;
    uint8_t  tL; 
    uint8_t  reserved0; 
    uint8_t  reserved1; 
    uint8_t  cntRemain; 
    uint8_t  cntPerCelsius; 
    uint8_t  crc8;
  } scratchpadDS1820_t;

    
  //=====================================================================================
  //
  // constructors
  //
  
  /** Real constructor corresponding to static pseudo constructor new_Instance().

      External power supply status is initially detected here.
  
      @param  inSegment  Reference to network segment the device is on.
      @param  inDevID    Reference to device's ID.
   */
  LOW_devDS1820( LOW_netSegment &inSegment, const LOW_deviceID &inDevID);

  
  /** Destructor.
   */
  virtual ~LOW_devDS1820();

  
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

  /** Get whether the device is externally powered.
      No real activity on the 1-Wire bus occures.
      @return Boolean value indicating status of external power.
   */
  virtual bool getIsExternalPowered() const;
  
  
  /** Initiate a single temperature conversion. 
  
      Following the conversion, the resulting thermal data is stored in the 2-byte 
      temperature register in the scratchpad memory and the DS18S20 returns to its 
      low-power idle state. 
      
      The method uses different techniquies to detect the end of the conversion:
        - If the device is being used in parasite power mode, a strong pullup 
          is enabled on the 1-Wire bus. Conversion ends when strong pullup ends.
      
        - If the device is powered by an external supply, the device is actively
          polled for the end of the conversion.
  */
  virtual void cmd_ConvertT() const;

    
  /** Read the contents of the scratchpad. 
  
      After fully reading the scratchpad a CRC verification is performed.
  
      @param  outScratchpad  Pointer to a scratchpad structure which will be filled in.
      @throw LOW_helper_CRC::crc_error  Thrown on CRC mismatch in transferred scratchpad.
  */
  void cmd_ReadScratchpad( scratchpadDS1820_t *outScratchpad) const;

        
  /** Write 2 bytes of data to the DS18S20 scratchpad. 
  
      @param  inTL  Byte to write to TL byte of scratchpad.
      @param  inTH  Byte to write to TH byte of scratchpad.
  */
  virtual void cmd_WriteScratchpad( const uint8_t inTL, const uint8_t inTH) const;

    
  /** Copy the contents of the scratchpad TH and TL registers to EEPROM.
  
      If the device is being used in parasite power mode, a strong pullup on the 
      1-Wire bus is enabled.
  */
  virtual void cmd_CopyScratchpad() const;

    
  /** Recall the alarm trigger values (TH and TL) from EEPROM and place the data in 
      bytes 2 and 3, respectively, in the scratchpad memory. 
      
      The method uses different techniquies to detect the end of reading:
        - If the device is being used in parasite power mode, a strong pullup 
          is enabled on the 1-Wire bus. Reading ends when strong pullup ends.
      
        - If the device is powered by an external supply, the device is actively
          polled for the end of reading.
  */
  virtual void cmd_RecallE2() const;

          
  /** Determine the device is using parasite power. 
  
      @return   True when device is externally powered. False when on parasite power.
  */
  virtual bool cmd_ReadPowerSupply() const;
  


//=======================================================================================
protected:

  //=====================================================================================
  //
  // attributes
  //
  
  bool  isExternalPowered; /**< Indicating wether the device is externally powered. */

    
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
