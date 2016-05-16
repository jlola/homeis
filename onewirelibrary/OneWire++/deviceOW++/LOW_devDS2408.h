/*
 * LOWdevDS2408.h
 *
 *  Created on: 14. 10. 2015
 *      Author: pc
 */

#ifndef ONEWIRELIBRARY_ONEWIRE___DEVICEOW___LOW_DEVDS2408_H_
#define ONEWIRELIBRARY_ONEWIRE___DEVICEOW___LOW_DEVDS2408_H_

#include "LOW_device.h"

class LOW_devDS2408 : public LOW_device
{
	  //=====================================================================================
	  //
	  // exceptions
	  //

	  /** Exception base class for all exceptions thrown by LOW_devDS2408. */
	  class_DERIVE_FROM_EXCEPTION( devDS2408_error ,LOW_exception);



	  //=====================================================================================
	  //
	  // constants
	  //
public:
	  /** Family code of this specific device. */
	  static const LOW_deviceIDRaw::devFamCode_t  familyCode = 0x29;

	  /** Family name of this specific device. */
	  static const std::string familyName;

	  static const owCommand_t  ReadMemory_COMMAND         = 0xf0; /**< 1-Wire command byte constant */
	  static const owCommand_t  ExtendedReadMemory_COMMAND = 0xa5; /**< 1-Wire command byte constant */
	  static const owCommand_t  WriteMemory_COMMAND        = 0x0f; /**< 1-Wire command byte constant */
	  static const owCommand_t  WriteStatus_COMMAND        = 0x55; /**< 1-Wire command byte constant */
	  static const owCommand_t  ReadStatus_COMMAND         = 0xaa; /**< 1-Wire command byte constant */

	  static const owCommand_t W1_F29_REG_LOGIG_STATE       = 0x88; /* R */

	  static const owCommand_t W1_F29_REG_OUTPUT_LATCH_STATE   = 0x89; /* R */

	  static const owCommand_t W1_F29_REG_ACTIVITY_LATCH_STATE  = 0x8A; /* R */

	  static const owCommand_t W1_F29_REG_COND_SEARCH_SELECT_MASK = 0x8B; /* RW */

	  static const owCommand_t W1_F29_REG_COND_SEARCH_POL_SELECT = 0x8C; /* RW */

	  static const owCommand_t W1_F29_REG_CONTROL_AND_STATUS  = 0x8D; /* RW */

	  static const owCommand_t W1_F29_FUNC_READ_PIO_REGS = 0xF0;

	  static const owCommand_t W1_F29_FUNC_CHANN_ACCESS_READ  = 0xF5;

	  static const owCommand_t W1_F29_FUNC_CHANN_ACCESS_WRITE = 0x5A;

	/* also used to write the control/status reg (0x8D): */

	  static const owCommand_t W1_F29_FUNC_WRITE_COND_SEARCH_REG = 0xCC;

	  static const owCommand_t W1_F29_FUNC_RESET_ACTIVITY_LATCHES = 0xC3;

	  static const owCommand_t W1_F29_SUCCESS_CONFIRM_BYTE   = 0xAA;

	  //=====================================================================================
	  //
	  // constructors
	  //

	  /** Real constructor corresponding to static pseudo constructor new_Instance().
	      @param  inSegment  Reference to network segment the device is on.
	      @param  inDevID    Reference to device's ID.
	   */
	  LOW_devDS2408( LOW_netSegment &inSegment, const LOW_deviceID &inDevID);

	  /** Destructor.
	   */
	  virtual ~LOW_devDS2408();

	  bool WriteControlStatus(uint8_t status);

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

	  /** Get wether the device is externally powered.
	      @return  Boolean indicates external power.
	   */
	  virtual bool getIsExternalPowered() const;

	  //=======================================================================================
	  private:

	    //=====================================================================================
	    //
	    // static initializer
	    //
	  /** Static pseudo constructor for registering with LOW_deviceFactory.
	        @param  inSegment  Reference to network segment the device is on.
	        @param  inDevID    Reference to device's ID.
	        @return  New dynamic instance of specific device class.
	     */
	    static LOW_device* new_Instance( LOW_netSegment &inNetSegment, const LOW_deviceID &inDevID);

	    /** Needed for dirty little C++ hack to force static initialization on application start.
	        @see initialize()
	    */
	    static int initHelper;

	    /** Static inizializer to register the class with LOW_deviceFactory.
	        @see initHelper
	    */
	    static int initialize();
	  public:

	    bool WritePIO(uint8_t byte);

	    bool WritePIO(uint8_t pin, bool value);

	    bool ReadPIO(uint8_t & value);
};

#endif /* ONEWIRELIBRARY_ONEWIRE___DEVICEOW___LOW_DEVDS2408_H_ */
