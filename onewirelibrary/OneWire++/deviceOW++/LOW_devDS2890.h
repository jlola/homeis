/***************************************************************************
                          LOW_devDS2890.h  -  description
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

#ifndef LOW_DEVDS2890_H
#define LOW_DEVDS2890_H


#include "LOW_device.h"
#include "LOW_deviceFactory.h"



/** Device class for DS2890 1-Wire Digital Potentiometer.

    DS2890 features:

    - Single element 256-position linear taper potentiometer
    - Supports potentiometer terminal working voltages up to 11V
    - Potentiometer terminal voltage independent of supply voltage
    - Potentiometer wiper position controlled and read over minimal 1-Wire bus interface
    - 100 kOhm resistor element value
    - Provides 1-Wire and VDD power modes
    - Supports Conditional Search based on power-on default wiper position
    - Multiple DS2890 s can be identified on a common 1-Wire bus and operated independently
    - Unique factory lasered 64-bit registration number assures error free device selection and absolute part identity
    - Built-in multi-drop controller ensures compatibility with other 1-Wire Network products
    - Supports Overdrive mode which boosts communication speed up to 142 kbits per second
    - -40oC to +85oC operating temperature range
    - 2.8V to 6.0V operating voltage range

    This class is thread-safe.

    @author Harald Roelle
    @author  Parts of the documentation by Dallas Semiconductors / Maxim Integrated Products
 */
class LOW_devDS2890 : public LOW_device  {

//=======================================================================================
public:

  //=====================================================================================
  //
  // exceptions
  //

  /** Exception base class for all exceptions thrown by LOW_devDS2890. */
  class_DERIVE_FROM_EXCEPTION( devDS2890_error, LOW_exception);


  //=====================================================================================
  //
  // constants
  //

  /** Family code of this specific device. */
  static const LOW_deviceIDRaw::devFamCode_t  familyCode = 0x2c;

  /** Family name of this specific device. */
  static const std::string familyName;

  static const owCommand_t  ReadPosition_COMMAND         = 0xf0; /**< 1-Wire command byte constant */
  static const owCommand_t  WritePosition_COMMAND        = 0x0f; /**< 1-Wire command byte constant */
  static const owCommand_t  ReadControlRegister_COMMAND  = 0xaa; /**< 1-Wire command byte constant */
  static const owCommand_t  WriteControlRegister_COMMAND = 0x55; /**< 1-Wire command byte constant */
  static const owCommand_t  Increment_COMMAND            = 0xc3; /**< 1-Wire command byte constant */
  static const owCommand_t  Decrement_COMMAND            = 0x99; /**< 1-Wire command byte constant */


  //=====================================================================================
  //
  // type definitions
  //

  typedef std::vector<LOW_devDS2890*> devDS2890PtrVec_t;  /**< Vector type of class device pointers. */

  typedef uint16_t wiperPosition_t;  /**< Type for position setting of wiper. */
  
  /** DS2890 internal control register as defined by Dallas. */
  typedef struct  {
    uint8_t  controlledWiperNum;  /**< Currently selected wiper to be manupulated by commands. */
    bool     isChargePumpOn;      /**< Whether charge punp is enabled. */
  } controlRegister_t;
  //typedef struct controlRegister_t controlRegister_t;


  //=====================================================================================
  //
  // constructors
  //

  /** Real constructor corresponding to static pseudo constructor new_Instance().

      Device features are initially detected here.

      @param  inSegment  Reference to network segment the device is on.
      @param  inDevID    Reference to device's ID.
   */
  LOW_devDS2890( LOW_netSegment &inSegment, const LOW_deviceID &inDevID);


  /** Destructor.
   */
  virtual ~LOW_devDS2890();


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

  /** Set whether charge pump is allowed for this device.
      Charge pump is only allowed to be used when external voltage supply is present.
      As this cannot be sensed by the device it has to set manually using this method.
      No real activity on the 1-Wire bus occures.
      @param  inCharpePumpAllowed .
   */
  virtual void setChargePumpAllowed( const bool inCharpePumpAllowed);

  /** Get whether charge pump is allowed for this device.
      No real activity on the 1-Wire bus occures.
      @return Whether charge pump is allowed for this device.
      @see setChargePumpAllowed()
   */
  virtual bool getChargePumpAllowed();

  /** Get whether the device has logarithmic or linear potentiometer elements.
      No real activity on the 1-Wire bus occures.
      @return Boolean value indicating the potentiometer type.
   */
  virtual bool getHasLinearPotElements() const;

  /** Get whether the wiper settings are volatile.
      No real activity on the 1-Wire bus occures.
      @return Boolean value indicating whether wiper settings are volatile.
   */
  virtual bool getIsWiperSettingVolatile() const;

  /** Get number of potentiometers in the device.
      No real activity on the 1-Wire bus occures.
      @return Number of potentiometers in the device.
   */
  virtual uint8_t getPotentiometerCount() const;

  /** Get number of positions selectable for each wiper.
      No real activity on the 1-Wire bus occures.
      @return Number of positions selectable for each wiper.
   */
  virtual wiperPosition_t getWiperPositionsCount() const;

  /** Get potentiometer resistance in ohm.
      No real activity on the 1-Wire bus occures.
      @return Potentiometer resistance in ohm.
   */
  virtual uint32_t getPotentiometerResistance() const;

  /** Get the cached value of the currently activated wiper.
      No real activity on the 1-Wire bus occures.
      @return  Cached wiper number.
   */
  virtual uint8_t getCachedWiperNumber();

  /** Get the cached value of a wiper position.
      No real activity on the 1-Wire bus occures.
      @param inControlledWiperNum  Wiper number to get value from.
      @return  Cached wiper position.
   */
  virtual wiperPosition_t getCachedWiperPosition( const uint8_t inControlledWiperNum = 0);

  /** The Read Position command is used to obtain the wiper setting of the potentiometer currently
      addressed by the Control Register. Although the DS2890 is a single element potentiometer, wiper
      addressing still applies and the Control Register wiper number used for addressing must be set
      accordingly.

      @return Current position of active wiper.
   */
  virtual wiperPosition_t cmd_ReadPosition();
  
  /** The Write Position command is used to set the position of the currently addressed potentiometer
      wiper. Although the DS2890 is a single element potentiometer, wiper addressing still applies
      and the Control Register wiper number used for addressing must be set accordingly.

      @param inNewPosition  New position of the wiper.
   */
  virtual void cmd_WritePosition( const wiperPosition_t inNewPosition);

  /** The Read Control Register command is used to obtain both the Control Register and potentiometer
      Feature Register. The Feature Register is not returned directly, but stored in internal attributes
      which are accessible via the various get*() methods.

      @return The control register value.
   */
  virtual controlRegister_t cmd_ReadControlRegister();

  /** The Write Control Register command is used to manipulate DS2890 state bits located in the
      Control Register. This command is used to set the potentiometer wiper address and charge pump state.
      Note that for enabling the charge pump this feature must explicitely enabled
      by setChargePumpAllowed().
      
      @param inControlledWiperNum   The wiper to be controlled.
      @param inChargePumpOn         Whether to turn on charge pump.
   */
  virtual void cmd_WriteControlRegister( const uint8_t inControlledWiperNum = 0, const bool inChargePumpOn = false);

  /** The Increment command is used for a one step position increase of the currently addressed
      potentiometer wiper. Although the DS2890 is a single element potentiometer, wiper addressing still
      applies and the Control Register wiper number used for addressing must be set accordingly.
      No position change is made if the DS2890 wiper is at the maximum position and
      an Increment command is received.

      @return New position of the wiper.
   */
  virtual wiperPosition_t cmd_Increment();

  /** The Decrement command is used for a one step position decrease of the currently addressed
      potentiometer wiper. Although the DS2890 is a single element potentiometer, wiper addressing still
      applies and the Control Register wiper number used for addressing must be set accordingly.
      No position change is made if the DS2890 wiper is at the minimum position (00h) and
      a Decrement command is received.

      @return New position of the wiper.
   */
  virtual wiperPosition_t cmd_Decrement();


  
//=======================================================================================
protected:


  //=====================================================================================
  //
  // constants
  //

  static const uint8_t releaseCode_subcommand = 0x96;  /**< Special byte to activate new values. */

  //! @name Feature register masks.
  //!@{
  static const uint8_t potentiometerType_featRegMask       = 0x01;
  static const uint8_t wiperSetVolatility_featRegMask      = 0x02;
  static const uint8_t potentiometerCount_featRegMask      = 0x0c;
  static const uint8_t wiperPositions_featRegMask          = 0x30;
  static const uint8_t potentiometerResistance_featRegMask = 0xc0;
  //!@}

  //! @name Control register masks.
  //!@{
  static const uint8_t wiperNum_ctlRegMsk     = 0x03;
  static const uint8_t invWiperNum_ctlRegMsk  = 0x0c;
  static const uint8_t chargePump_ctlRegMsk   = 0x40;
  //!@}


  //=====================================================================================
  //
  // attributes
  //

  bool              chargePumpAllowed;        /**< Enables setting chage pump option. */
  bool              hasLinearPotElements;     /**< Feature register value. */
  bool              isWiperSettingVolatile;   /**< Feature register value. */
  uint8_t           potentiometerCount;       /**< Feature register value. */
  wiperPosition_t   wiperPositionsCount;      /**< Feature register value. */
  uint32_t          potentiometerResistance;  /**< Feature register value. */
  wiperPosition_t   *cachedWiperPos;          /**< Cached wiper positions. */
  uint8_t           cachedCurrentWiper;       /**< Wiper which was selected. */


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
