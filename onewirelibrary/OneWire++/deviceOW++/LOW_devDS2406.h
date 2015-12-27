/***************************************************************************
                          LOW_devDS2406.h  -  description
                             -------------------
    begin                : Fri Aug 23 2002
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

#ifndef LOW_DEVDS2406_H
#define LOW_DEVDS2406_H



#include "LOW_device.h"



/** Device class for DS2406 Dual Addressable Switch Plus 1 kbit Memory.

    DS2406 features:

    - Open drain PIO pins are controlled and their logic level can be determined
      over 1-Wire bus for closed-loop control
    - Replaces and is fully compatible with DS2407 but no user-programmable
      power-on settings and no Hidden Mode
    - PIO channel A sink capability of 50mA at 0.4V with soft turn-on; channel B 8mA at 0.4V
    - Maximum operating voltage of 13V at PIO-A, 6.5V at PIO-B
    - 1024 bits user-programmable OTP EPROM
    - User-programmable status memory to control the device
    - Multiple DS2406 can be identified on a common 1-Wire bus and be turned on or off
      independently of other devices on the bus
    - Unique, factory-lasered and tested 64-bit registration number (8-bit family code + 48-bit
      serial number + 8-bit CRC tester) assures error-free selection and absolute identity
      because no two parts are alike
    - On-chip CRC16 generator allows detection of data transfer errors
    - Built-in multidrop controller ensures compatibility with other 1-Wire net products
    - Reduces control, address, data, programming and power to a single data pin
    - Directly connects to a single port pin of a microprocessor and communicates
      at up to 16.3 kbits/s
    - Supports Conditional Search with userselectable condition
    - Vcc bondout for optional external supply to the device (TSOC package only)
    - 1-Wire communication operates over a wide voltage range of 2.8V to 6.0V from -40°C to +85°C
    - Low cost TO-92 or 6-pin TSOC surface mount package

    This class is thread-safe.
    
    @author Harald Roelle
    @author  Parts of the documentation by Dallas Semiconductors / Maxim Integrated Products
 */
class LOW_devDS2406 : public LOW_device  {

//=======================================================================================
public: 

  //=====================================================================================
  //
  // exceptions
  //

  /** Exception base class for all exceptions thrown by LOW_devDS2406. */
  class_DERIVE_FROM_EXCEPTION( devDS2406_error ,LOW_exception);



  //=====================================================================================
  //
  // constants
  //

  /** Family code of this specific device. */
  static const LOW_deviceIDRaw::devFamCode_t  familyCode = 0x12;

  /** Family name of this specific device. */
  static const std::string familyName;

  static const owCommand_t  ReadMemory_COMMAND         = 0xf0; /**< 1-Wire command byte constant */
  static const owCommand_t  ExtendedReadMemory_COMMAND = 0xa5; /**< 1-Wire command byte constant */
  static const owCommand_t  WriteMemory_COMMAND        = 0x0f; /**< 1-Wire command byte constant */
  static const owCommand_t  WriteStatus_COMMAND        = 0x55; /**< 1-Wire command byte constant */
  static const owCommand_t  ReadStatus_COMMAND         = 0xaa; /**< 1-Wire command byte constant */
  static const owCommand_t  ChannelAccess_COMMAND      = 0xf5; /**< 1-Wire command byte constant */


  
  //=====================================================================================
  //
  // type definitions
  //
  
  typedef std::vector<LOW_devDS2406*> devDS2406PtrVec_t;    /**< Vector type of class device pointers. */

  /** Type for PIO channel selection. */
  typedef enum { noneSelect=0, chanASelect=1, chanBSelect=2, chanBothSelect=3}  chanSelect_t;

  /** Type for search source selection. */
  typedef enum { latchSelect=1, flipFlopSelect=2, curStatusSelect=3}            sourceSelect_t;

  /** Type for PIO transistor states. */
  typedef enum { pioTransistorOn=0, pioTransistorOff=1}                         pioTransistor_t;

  /** Type for activity polarity selection. */
  typedef enum { activeLow=0, activeHigh=1}                                     activePolarity_t;

  /** DS2406 internal status register as defined by Dallas. */
  typedef struct statusRegister_t {
    activePolarity_t   activePolarity;
    sourceSelect_t     sourceSelect;
    chanSelect_t       channelSelect;
    pioTransistor_t    channelFFQ_pioA;
    pioTransistor_t    channelFFQ_pioB;
    bool               isExternalPowered;   // bit 7
  } statusRegister_t;

    
  //=====================================================================================
  //
  // classes
  //


  /** Class for accessing the PIO channels.
  
      The Channel Access command is used to access the PIO channels to sense the logical status
      of the output node and the output transistor and to change the status of the output transistor.

      As there are many combinations of reading/writing the command is implemented as a class.
      The programmer himself is responsible to match read/write cycles according to prior selected
      options on instance creation.

      For details see the original Dallas documentation.

      <B>Note:</B> As any other command creating an instance of this class locks the device.
                   Remember to destroy the object to release the device.
   */
  class cmd_ChannelAccess : public linkLock {

  //-------------------------------------------------------------------------------------
  public:
    
    //-------------------------------------------------------------------------------------
    // type definitions
    //

    /** Type for CRC cycle selection. */
    typedef enum { CRC_disable=0, CRC_after1Byte=1, CRC_after8Byte=2, CRC_after32Byte=3} CRCtype_t;

    /** Type for interleave mode selection. */
    typedef enum { asyncInterleaveMode=0, syncInterleaveMode=1 }                         interleaveMode_t;

    /** Type for toggle mode selection. */
    typedef enum { noToggleMode=0, toggleMode=1}                                         toggleMode_t;

    /** Type for inition I/O mode selection. */
    typedef enum { writeMode=0, readMode=1}                                              initialMode_t;

    /** Type for latch reset selection. */
    typedef enum { noResetLatches=0, resetLatches=1}                                     activityLatchReset_t;

    /** DS2406 channel info as defined by Dallas. */
    typedef struct channelInfo_t {
      bool channelFFQ_pioA;     // bit 0
      bool channelFFQ_pioB;
      bool sensedLevel_pioA;
      bool sensedLevel_pioB;
      bool activityLatch_pioA;
      bool activityLatch_pioB;
      bool hasPioB;
      bool isExternalPowered;   // bit 7
    } channelInfo_t;
  
    //-------------------------------------------------------------------------------------
    // constructors
    //

    /** Constructor with specification of command options.
        Obtains a lock on the device.
        
        @param  inDevice           Reference to the device the command operates on.
        @param  inCRCtype          CRC cycle selection.
        @param  inChanSelect       PIO channel selection.
        @param  inInterleaveMode   Interleave mode selection.
        @param  inToggleMode       Toggle mode selection.
        @param  inInitialMode      Initial I/O mode selection.
        @param  inALR              Latch reset selection.

        @throw devDS2406_error  Thrown when illegal combination of modes is selected.
     */
    cmd_ChannelAccess( const LOW_devDS2406 &inDevice,
                       const CRCtype_t inCRCtype, const chanSelect_t inChanSelect, 
                       const interleaveMode_t inInterleaveMode, const toggleMode_t inToggleMode, 
                       const initialMode_t inInitialMode, const activityLatchReset_t inALR);

    /** Destructor.
        Releases the lock on the device.
     */
    virtual ~cmd_ChannelAccess();
    
    //-------------------------------------------------------------------------------------
    // methods
    //

    /** Get the channel info read after sending command.
     */
    virtual channelInfo_t& getChannelInfo();

    /** Receive 1 bit from the device.
        @return  Bit that was reveived.
     */
    virtual bool readDataBit() const;

    /** Receive 1 byte from the device.
        @return  Byte that was reveived.
     */
    virtual uint8_t readDataByte() const;

    /** Receive a block of bytes from the device.
        @param  outBytes   Values that were reveived. Read length is determined
                           by the preset length of the vector.
     */
    virtual void readData( byteVec_t &outBytes) const;

    /** Send 1 bit to the device.
        @param   inSendBit   Bit to send.
     */
    virtual void writeData( const bool inSendBit) const;

    /** Send 1 byte to the device.
        @param   inSendByte  Byte to send.
     */
    virtual void writeData( const uint8_t inSendByte) const;

    /** Send block of bytes to the device.
        @param  inSendBytes  Block of bytes to send.
     */
    virtual void writeData( const byteVec_t &inSendBytes) const;
  
  //-------------------------------------------------------------------------------------
  private:
    const LOW_devDS2406  &device;      /**< Device the command is operating on */
    channelInfo_t        channelInfo;  /**< Channel info received after issuing the command */
    
  };
  
  
  //=====================================================================================
  //
  // constructors
  //
  
  /** Real constructor corresponding to static pseudo constructor new_Instance().
      @param  inSegment  Reference to network segment the device is on.
      @param  inDevID    Reference to device's ID.
   */
  LOW_devDS2406( LOW_netSegment &inSegment, const LOW_deviceID &inDevID);

  /** Destructor.
   */
  virtual ~LOW_devDS2406();

  
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

  
  /** Get wether the second PIO channel is present.
      @return  Boolean indicates presence of channel B.
   */
  virtual bool getHasPioB() const;

  
  /** Get the current search condition.
      See also setSearchCondition().

      @param  outStatusRegister  Pointer to status register struture. Results
                                 are filled in there.
   */
  virtual void getSearchCondition( LOW_devDS2406::statusRegister_t *outStatusRegister) const;

    
  /** Set the search condition and status of the PIO transistors.
  
      The condition is specified by the bit functions CSS0 to CSS4 in Status Memory location 7.
      At power-on all these bits are 1s. As long as the device remains powered up, the
      modified search conditions are available for use at any time.
      For the conditional search, one can specify
        - the polarity (HIGH or LOW; CSS0)
        - the source (PIO-pin, channel flip flop or activity latch; CSS1, CSS2)
        - the channel of interest (A, B or the logical OR of A, B; CSS3, CSS4)

      The table shows all qualifying conditions and the required settings for CSS0 to CSS4:
      <pre>
              DESCRIPTION        |   CONDITIONAL SEARCH SELECT CODE
      -----------------+---------+--------------+-------------+---------
                       |         |CHANNEL SELECT|SOURCE SELECT|POLARITY
                       |         +------+-------+------+------+---------
       CONDITION       | CHANNEL | CSS4 | CSS3  | CSS2 | CSS1 | CSS0
      =================+=========+======+=======+======+======+=========
      RESERVED         |         |  Don't care  | 0    | 0    | 0/1
      -----------------+---------+------+-------+------+------+---------
      Unconditional    |neither  | 0    | 0     | At least one| 0
                       |         |      |       | must be 1   |
      -----------------+---------+------+-------+------+------+---------
      Activity Latch=0 | A       | 0    | 1     | 0    | 1    | 0
      -----------------+---------+------+-------+------+------+---------
      Activity Latch=1 | A       | 0    | 1     | 0    | 1    | 1
      -----------------+---------+------+-------+------+------+---------
      Channel FF = 0   | A       | 0    | 1     | 1    | 0    | 0
      (transistor on)  |         |      |       |      |      |
      -----------------+---------+------+-------+------+------+---------
      Channel FF = 1   | A       | 0    | 1     | 1    | 0    | 1
      (transistor off) |         |      |       |      |      |
      -----------------+---------+------+-------+------+------+---------
      PIO Low          | A       | 0    | 1     | 1    | 1    | 0
      -----------------+---------+------+-------+------+------+---------
      PIO High         | A       | 0    | 1     | 1    | 1    | 1
      -----------------+---------+------+-------+------+------+---------
      Activity Latch=0 | B       | 1    | 0     | 0    | 1    | 0
      -----------------+---------+------+-------+------+------+---------
      Activity Latch=1 | B       | 1    | 0     | 0    | 1    | 1
      -----------------+---------+------+-------+------+------+---------
      Channel FF = 0   | B       | 1    | 0     | 1    | 0    | 0
      (transistor on)  |         |      |       |      |      |
      -----------------+---------+------+-------+------+------+---------
      Channel FF = 1   | B       | 1    | 0     | 1    | 0    | 1
      (transistor off) |         |      |       |      |      |
      -----------------+---------+------+-------+------+------+---------
      PIO Low          | B       | 1    | 0     | 1    | 1    | 0
      -----------------+---------+------+-------+------+------+---------
      PIO High         | B       | 1    | 0     | 1    | 1    | 1
      -----------------+---------+------+-------+------+------+---------
      Activity Latch=0 | A or B  | 1    | 1     | 0    | 1    | 0
      -----------------+---------+------+-------+------+------+---------
      Activity Latch=1 | A or B  | 1    | 1     | 0    | 1    | 1
      -----------------+---------+------+-------+------+------+---------
      Channel FF = 0   | A or B  | 1    | 1     | 1    | 0    | 0
      (transistor on)  |         |      |       |      |      |
      -----------------+---------+------+-------+------+------+---------
      Channel FF = 1   | A or B  | 1    | 1     | 1    | 0    | 1
      (transistor off) |         |      |       |      |      |
      -----------------+---------+------+-------+------+------+---------
      PIO Low          | A or B  | 1    | 1     | 1    | 1    | 0
      -----------------+---------+------+-------+------+------+---------
      PIO High         | A or B  | 1    | 1     | 1    | 1    | 1
      -----------------+---------+------+-------+------+------+---------
      </pre>

      @param inChanSelect      Select channel search condition.
      @param inSourceSelect    Select source search condition.
      @param inPolaritySelect  Select polarity search condition.
      @param inPioATrans       Status of PIO transistor A.
      @param inPioBTrans       Status of PIO transistor B.

      @throw devDS2406_error  Thrown when channel B is selected without being present.
   */
  virtual void setSearchCondition( const chanSelect_t inChanSelect, const sourceSelect_t inSourceSelect,
                                   const activePolarity_t inPolaritySelect,
                                   const pioTransistor_t inPioATrans, const pioTransistor_t inPioBTrans) const;


  /** Read from EPROM memory.
  
      The Read Memory command is used to read data from the 1024-bit EPROM data memory field.
      The bus master follows the command byte with a two-byte address (TA1=(T7:T0), TA2=(T15:T8))
      that indicates a starting byte location within the data field. With every subsequent read
      data time slot the bus master receives data from the DS2406 starting at the initial address
      and continuing until the end of the 1024-bits data field is reached or until a Reset Pulse
      is issued.

      If reading occurs through the end of memory space, the bus master issues sixteen additional
      read time slots and the DS2406 will respond with a 16-bit CRC of the command, address bytes
      and all data bytes read from the initial starting byte through the last byte of memory.
      This CRC is the result of clearing the CRC generator and then shifting in the command byte
      followed by the two address bytes and the data bytes beginning at the first addressed memory
      location and continuing through to the last byte of the EPROM data
      memory. Any reads ended by a Reset Pulse prior to reaching the end of memory will not have
      the 16-bit CRC available.

      Typically the software controlling the device should store a 16-bit CRC with each page
      of data to insure rapid, error-free data transfers that eliminate having to read a page
      multiple times to determine if the received data is correct or not. (See Book of DS19xx
      iButton Standards, Chapter 7 for the recommended file structure to be used with the 1-Wire
      environment). If CRC values are imbedded within the data it is unnecessary to read the
      end-of-memory CRC. The Read Memory command can be ended at any point by issuing a Reset Pulse.

      @param  inStartAddr  Start address for reading.
      @param  outBytes     Values that were reveived. Read length is determined
                           by the preset length of the vector.
   */
  virtual void cmd_ReadMemory( const uint8_t inStartAddr, byteVec_t &outBytes) const;

  
  //cmd_ExtendedReadMemory();  // Not implemented yet

  
  //cmd_WriteMemory();         // Not implemented yet

  
  /** Read from status memory.
  
      The Read Status command is used to read data from the Status Memory field. The functional flow
      of this command is identical to the Read Memory command. Since the Status Memory is only 8 bytes,
      the DS2406 will send the 16-bit CRC after the last byte of status information has been transmitted.

      DS2406 status memory map:
      <pre>
      ADDRESS   | BIT 7             | BIT 6         | BIT 5         | BIT 4        | BIT 3        | BIT 2       | BIT 1       | BIT 0
      ==========+===================+===============+===============+==============+==============+=============+=============+==========
      0 (EPROM) | BM3               | BM2           | BM1           | BM0          | WP3          | WP2         | WP1         | WP0
      ----------+-------------------+---------------+---------------+--------------+--------------+-------------+-------------+----------
      1 (EPROM) | 1                 | 1             | 1             | 1            | 1            | 1           | Redir. 0    | Redir. 0
      ----------+-------------------+---------------+---------------+--------------+--------------+-------------+-------------+----------
      2 (EPROM) | 1                 | 1             | 1             | 1            | 1            | 1           | Redir. 1    | Redir. 1
      ----------+-------------------+---------------+---------------+--------------+--------------+-------------+-------------+----------
      3 (EPROM) | 1                 | 1             | 1             | 1            | 1            | 1           | Redir. 2    | Redir. 2
      ----------+-------------------+---------------+---------------+--------------+--------------+-------------+-------------+----------
      4 (EPROM) | 1                 | 1             | 1             | 1            | 1            | 1           | Redir. 3    | Redir. 3
      ----------+-------------------+---------------+---------------+--------------+--------------+-------------+-------------+----------
      5 (EPROM) |                                                   EPROM Factory Test byte
      ----------+-------------------+---------------+---------------+--------------+--------------+-------------+-------------+----------
      6 (EPROM) |                                                 Don t care, always reads 00
      ----------+-------------------+---------------+---------------+--------------+--------------+-------------+-------------+----------
      7 (SRAM)  | Supply Indication | PIO-B Channel | PIO-A Channel | CSS4 Channel | CSS3 Channel | CSS2 Source | CSS1 Source | CSS0
                | (read only)       | Flip-flop     | Flip-flop     |      Select  |      Select  |      Select |      Select | Polarity
      ----------+-------------------+---------------+---------------+--------------+--------------+-------------+-------------+----------
      </pre>

      @param  inStartAddr  Start address for reading.
      @param  outBytes     Values that were reveived. Read length is determined
                           by the preset length of the vector.

      @throw devDS2406_error  Thrown when illegal address is selected.
   */  
  virtual void cmd_ReadStatus( const uint8_t inStartAddr, byteVec_t &outBytes) const;

   
  /** Write to status memory.
  
      The Write Status command is used to program the Status Memory, which includes the
      specification of the Conditional Search Settings.

      The Status Memory address range is
      0000h to 0007h. The general programming algorithm is valid for the EPROM section of
      the Status Memory (addresses 0 to 4) only. The Status memory locations 5 and 6 are
      already pre-programmed to 00h and therefore cannot be altered. Status memory
      location 7 consists of static RAM, which can be reprogrammed without limitation.
      The supply indication (bit 7) is read-only; attempts to write to it are ignored. The
      function flow for writing to status memory location 7 is basically the same as for
      the other EPROM Status Memory Bytes. However, instead of a programming pulse the bus
      master sends a FFh byte (equivalent to 8 Write-One Time Slots) to transfer the new
      value from the scratchpad to the status memory.

      See also cmd_ReadStatus().

      @param  inStartAddr    Start address for reading.
      @param  inWriteBytes   Values to write.

      @throw devDS2406_error  Thrown when illegal address is selected.
   */
  virtual void cmd_WriteStatus( const uint8_t inStartAddr, const byteVec_t &inWriteBytes) const;
    

    
//=======================================================================================
protected:

  //=====================================================================================
  //
  // friends
  //
  
  friend class cmd_ChannelAccess;  /**< required for accessing the device's lock */
  
  
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
  
  /** Universal, internal data reading.
      Implements common parts of memory reading.
  
      @param  inStartAddr  Start address for reading.
      @param  outBytes     Values that were reveived. Read length is determined
                           by the preset length of the vector.
      @param  inMaxLen     Maximum readable memory size.
      @param  inCommand    1-Wire command to start the read cycle.

      @throw devDS2406_error  Thrown when illegal address is selected.
   */
  virtual void readMemUniversal( const uint16_t inStartAddr, byteVec_t &outBytes, 
                                 const uint16_t inMaxLen, const owCommand_t inCommand) const;
  
  
  
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
