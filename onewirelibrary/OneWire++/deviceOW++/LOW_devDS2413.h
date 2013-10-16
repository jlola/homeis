/*
 * LOW_devDS2413.h
 *
 *  Created on: Feb 4, 2013
 *      Author: Josef Lola
 */

#ifndef LOW_DEVDS2413_H_
#define LOW_DEVDS2413_H_

#include "LOW_device.h"
#include <stdint.h>
/**
 * DS2413 1-Wire Dual Channel Addressable Switch
 *
 * The DS2413 is a dual-channel programmable I/O
1-Wire® chip. The PIO outputs are configured as
open-drain and provide up to 20mA continuous sink
capability and off-state operating voltage up to 28V.
Control and sensing of the PIO pins is performed with
a dedicated device-level command protocol. To
provide a high level of fault tolerance in the end
application, the 1-Wire IO and PIO pins are all
capable of withstanding continuous application of
voltages up to 28V max. Communication and
operation of the DS2413 is performed with the single
contact Maxim/Dallas 1-Wire serial interface.
 *
 */

/**    This class is thread-safe.

    @author Harald Roelle
    @author  Parts of the documentation by Dallas Semiconductors / Maxim Integrated Products
 */
class LOW_devDS2413 : public LOW_device  {

//=======================================================================================
public:

  //=====================================================================================
  //
  // exceptions
  //

  /** Exception base class for all exceptions thrown by LOW_devDS2413. */
  class_DERIVE_FROM_EXCEPTION( devDS2413_error ,LOW_exception);



  //=====================================================================================
  //
  // constants
  //

  /** Family code of this specific device. */
  static const LOW_deviceIDRaw::devFamCode_t  familyCode = 0x3A;

  /** Family name of this specific device. */
  static const std::string familyName;

  static const owCommand_t  ChannelWrite_COMMAND	   = 0x5a; /**< 1-Wire command byte constant */
  static const owCommand_t  ChannelRead_COMMAND		   = 0xf5; /**< 1-Wire command byte constant */
  static const owCommand_t  Configmartion_COMMAND	   = 0xaa; /**< 1-Wire command byte constant */



  //=====================================================================================
  //
  // type definitions
  //

  typedef std::vector<LOW_devDS2413*> devDS2413PtrVec_t;    /**< Vector type of class device pointers. */



  /** Type for PIO transistor states. */
  typedef enum { pioTransistorOn=0, pioTransistorOff=1}                         pioTransistor_t;

  /** Type for activity polarity selection. */
  typedef enum { pinStateFalse=0, pinStateTrue=1}                                     pinState_t;

  /** DS2413 internal status register as defined by Dallas. */
	static const uint8_t PIOAPinStateBit		=	((uint8_t)0x01);
	static const uint8_t PIOAOutLatchStateBit 	=	((uint8_t)0x02);
	static const uint8_t PIOBPinStateBit		=	((uint8_t)0x04);
	static const uint8_t PIOBOutLatchStateBit 	=	((uint8_t)0x08);


  /** DS2406 internal write status register as defined by Dallas. */
	#define PIOAOutLatchWriteBit	((uint8_t)0x01)
	#define PIOBOutLatchWriteBit	((uint8_t)0x02)



  //=====================================================================================
  //
  // constructors
  //

  /** Real constructor corresponding to static pseudo constructor new_Instance().
      @param  inSegment  Reference to network segment the device is on.
      @param  inDevID    Reference to device's ID.
   */
  LOW_devDS2413( LOW_netSegment &inSegment, const LOW_deviceID &inDevID);

  /** Destructor.
   */
  virtual ~LOW_devDS2413();


  //=====================================================================================
  //
  // methods
  //
  /** Check if is val valid readed byte.
        <B>Note: check if is high 4 bits complement to low 4 bits</B>
        @return true if is byte valid else false
  */
  bool IsValid(uint8_t val) const;

  void cmd_ReadMemory( const uint8_t inStartAddr, byteVec_t &outBytes) const;

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




  /** This command reads the PIO logical status and reports it together with the state of the PIO Output Latch in an
	endless loop. A PIO Access Read can be terminated at any time with a 1-Wire Reset.

        @param  inStartAddr 	start address of Status register
        @param  inWriteBytes    byte write
     */
  virtual void cmd_WritePIO(pioTransistor_t PIOA, pioTransistor_t PIOB) const;

  /** The PIO Access Write command writes to the PIO output latches, which control the pulldown transistors of the PIO
	channels. In an endless loop this command first writes new data to the PIO and then reads back the PIO status.
	This implicit read-after-write can be used by the master for status verification. A PIO Access Write can be
	terminated at any time with a 1-Wire Reset.

        @param  inStartAddr 	start address of Status register
        @param  inWriteBytes    byte write
       */
  virtual void cmd_ReadStatus( uint8_t &outByte) const;

//=======================================================================================
protected:

  //=====================================================================================
  //
  // friends
  //




  //=====================================================================================
  //
  // attributes
  //

  bool  isExternalPowered;   /**< External supply indicator */
  bool  hasPioB;             /**< Wether the second PIO channel is present */


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


  //=====================================================================================
  //
  // methods
  //


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

#endif /* LOW_DEVDS2413_H_ */
