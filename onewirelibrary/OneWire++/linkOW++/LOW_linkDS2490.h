/***************************************************************************
                          LOW_linkDS2490.h  -  description
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

#ifndef LOW_LINKDS2490_H
#define LOW_LINKDS2490_H


#include "LOW_link.h"
#include "LOW_linkFlexibleSpeed.h"
#include "LOW_portUsb_Factory.h"


/** Link class for DS2490 USB to 1-Wire Bridge Chip.

    DS2490 features:

      - Communicates at regular and overdrive 1- Wire® speeds
      - Supports 12V EPROM programming and stiff 5V pullup for EEPROM, sensors, and crypto iButton
      - Slew rate controlled 1-Wire timing and active pullup to accommodate long
        1-Wire network lines and reduce radiation
      - Programmable 1-Wire timing and driver characteristics accommodate a wide range of 1-Wire
        network configurations
      - Low- to high-level command types, including macros, for generating 1-Wire communication
      - Crystal oscillator timebase provides precision timed 1-Wire waveforms
      - High-speed 12Mbps Universal Serial Bus (USB) interface
      - Integrated USB-compliant transceiver
      - Supports USB remote wake-up from a 1-Wire device event to resume a suspended host system
      - 0oC to +70oC operating temperature range

    The DS2490 is a bridge chip that enables communication between a USB host system and a 1-Wire bus.
    It provides regular, overdrive, and flexible 1-Wire communication speeds and a full-speed 12Mbps
    connection to USB. USB vendor-specific commands defined in this specification are used to control
    the DS2490 and communicate with attached 1-Wire devices.

    This class is thread-safe.
    
    @todo Provide assignment operator and copy constructor
    @todo Finish documentation

    @author  Harald Roelle
    @author  Parts of the documentation by Dallas Semiconductors / Maxim Integrated Products
 */
class LOW_linkDS2490 : public LOW_link, public LOW_linkFlexibleSpeed {

//=======================================================================================
public:

  //=====================================================================================
  //
  // constants
  //

  static const LOW_portUsbDevice::usbVendorID_t   usbVendorID  = 0x04fa;        /**< Dallas/Maxim USB vendor ID. */
  static const LOW_portUsbDevice::usbProductID_t  usbProductID = 0x2490;        /**< Dallas/Maxim USB DS2490 product ID. */


  //=====================================================================================
  //
  // constructors
  //

  /** Constructor.

      Timings for flexible speed is configured to the recommeded optimal parameters
      as of app note #148.

      @param  inUsbDevSpec        USB device specifier of the DS2490.
      @param  inHasExternalPower  Whether the attached bus supplies external power.
      @param  inAllowProgPulse    Whether the program pulse should be allowed.
   */
  LOW_linkDS2490( const LOW_portUsb_Factory::usbDeviceSpecifier_t inUsbDevSpec,
                  const bool inHasExternalPower, const bool inAllowProgPulse = false);

                  
  /** Destructor.
   */
  virtual ~LOW_linkDS2490();


  //=====================================================================================
  //
  // Standard methods required by LOW_link
  //

  virtual std::string getLinkFamily() const { return "DS2490 USB link"; };

  //! @name Bus touch (write/read) methods required by LOW_link
  //!@{
  virtual bool touchBit( const bool inSendBit, const strongPullup_t inPullup = pullUp_NONE);
  virtual uint8_t touchByte( const uint8_t inSendByte, const strongPullup_t inPullup = pullUp_NONE);
  virtual byteVec_t touchBlock( const byteVec_t &inBytes, const strongPullup_t inPullup = pullUp_NONE);
  //!@}

  //! @name Higher level methods required by LOW_link
  //!@{
  virtual LOW_deviceID::deviceIDVec_t searchDevices( const bool inOnlyAlarm, const LOW_deviceIDRaw inPreload,
                                                     const LOW_deviceIDRaw::devFamCode_t inFamCode, const bool inDoReset);
  //!@}

  //! @name Misc methods required by LOW_link
  //!@{
  virtual void resetLinkAdapter();
  virtual bool resetBus();
  virtual void strongPullup( const unsigned long inMicroSecs);
  virtual void strongPullup( const strongPullup_t inPullupTime);
  virtual void programPulse( const unsigned long inMicroSecs);
  virtual void programPulse( const progPulse_t inPulseTime);
  //!@}


  //=====================================================================================
  //
  // Standard methods required by LOW_linkFlexibleSpeed
  //

  //! @name Methods required by LOW_linkFlexibleSpeed
  //!@{
  virtual void setWireSpeed( const wireSpeed_t inWireSpeed);
  virtual wireSpeed_t getWireSpeed();
  virtual void setPullDownSlewRate( const pdSlewRate_t inPDSR);
  virtual pdSlewRate_t getPullDownSlewRate();
  virtual void setWrite1LowTime( const w1LowTime_t inW1LT);
  virtual w1LowTime_t getWrite1LowTime();
  virtual void setSampleOffsetWrite0Rec( const soW0RecTime_t inSOW0RT);
  virtual soW0RecTime_t getSampleOffsetWrite0Rec();
  //!@}



//=======================================================================================
protected:


  //=====================================================================================
  //
  // Standard methods required by LOW_link
  //

  virtual void doSearchSequence( const LOW_deviceIDRaw &inBranchVector,
                                 LOW_deviceIDRaw &outFoundID, LOW_deviceIDRaw &outDiscrVec);

                                 
                                 
//=======================================================================================
private:

  //=====================================================================================
  //
  // constants
  //

  //! @name USB related constants.
  //!@{
  static const LOW_portUsbDevice::usbInterface_t  usbDefaultInterface = 0;      /**< Default and only USB interface of DS2490. */
  static const LOW_portUsbDevice::bmRequestType_t vendorCmd_requestType = 0x40; /**< Request type of DS2490 vendor specific commands. */
  //!@}


  //! @name USB timeout constants.
  //!@{
  static const LOW_portUsbDevice::usbTimeout_t   ctlCmd_usbTimeout  = 1000; /**< Timeout in ms for control commands. */
  static const LOW_portUsbDevice::usbTimeout_t   modCmd_usbTimeout  = 1000; /**< Timeout in ms for mode commands. */
  static const LOW_portUsbDevice::usbTimeout_t   comCmd_usbTimeout  = 1000; /**< Timeout in ms for communicaton commands. */
  //!@}


  //! @name USB interface alternate settings.
  //!@{
  static const LOW_portUsbDevice::usbSetting_t usbAltSetting_10_16 = 0; /**< poll interval EP2/EP3 = 10ms; max packet size = 16 bytes;
                                                                             Long interrupt polling interval, small packet size for bulk pipes. */
  static const LOW_portUsbDevice::usbSetting_t usbAltSetting_10_64 = 1; /**< poll interval EP2/EP3 = 10ms; max packet size = 64 bytes;
                                                                             Long interrupt polling interval, large packet size for bulk pipes. */
  static const LOW_portUsbDevice::usbSetting_t usbAltSetting_1_16  = 2; /**< poll interval EP2/EP3 =  1ms; max packet size = 16 bytes;
                                                                             Short interrupt polling interval, small packet size for bulk pipes. */
  static const LOW_portUsbDevice::usbSetting_t usbAltSetting_1_64  = 3; /**< poll interval EP2/EP3 =  1ms; max packet size = 64 bytes;
                                                                             Short interrupt polling interval, large packet size for bulk pipes. */
  //!@}

  
  //! @name USB endpoints.
  //!@{
  /** Endpoint for the bi-directional default control pipe. It is used for the USB enumeration process,
      USB core request communication, and all DS2490 specific command communication.
   */ 
  static const LOW_portUsbDevice::usbEndpoint_t usbControlEP  = 0;
  /** Endpoint for an interrupt pipe (device to host) and is used to relay DS2490 status register data and
      specific command execution completion and/or error information to the host. It is also used to inform
      the host of 1-Wire device-attach detection. The polling period requested for EP1 depends on the
      alternate interface setting of the 1-Wire interface.
   */
  static const LOW_portUsbDevice::usbEndpoint_t usbStatusInEP = 1;
  /** Endpoint for a bulk data out pipe (data from host) and is used to transmit 1-Wire device data from the
      host to the DS2490. The information received at this endpoint will be transmitted as data on the 1-Wire bus.
   */
  static const LOW_portUsbDevice::usbEndpoint_t usbDataOutEP  = 2;
  /** FIFO size in bytes of endpoint usbDataOutEP.
   */
  static const unsigned int usbDataOutEP_FIFOsize             = 128;
  /** Endpoint for a bulk data in pipe (data to host) and is used to send data received by the DS2490 from
      the 1-Wire bus back to the host for processing.
   */
  static const LOW_portUsbDevice::usbEndpoint_t usbDataInEP   = 3;
  /** FIFO size in bytes of endpoint usbDataOutEP.
   */
  static const unsigned int usbDataInEP_FIFOsize              = 128;
  //!@}


  /** @name Command types.
      Three different vendor-specific command types exist to control and communicate with the DS2490:
      
        - Control commands are used to manage various device functions including the processing of
          communication commands, buffer clearing, and SW reset.

        - Communication commands are used for 1-Wire data and command I/O.

        - Mode commands are used to establish the 1-Wire operational characteristics of the DS2490
          such as slew rate, low time, strong pullup, etc.

      Control, Communication and Mode commands, like USB core requests, are communicated over the
      default control pipe at EP0.
   */
  //!@{
  static const LOW_portUsbDevice::bRequest_t control_cmdType = 0x00; //**< 1-Wire interface control commands. */
  static const LOW_portUsbDevice::bRequest_t comm_cmdType    = 0x01; //**< 1-Wire interface communication commands. */
  static const LOW_portUsbDevice::bRequest_t mode_cmdType    = 0x02; //**< 1-Wire interface operational mode commands. */
  //!@}

  
  //! @name Control commands.
  //!@{
  static const LOW_portUsbDevice::wValue_t resetDevice_ctlCmd    = 0x0000;  /**< Control command. */
  static const LOW_portUsbDevice::wValue_t startExe_ctlCmd       = 0x0001;  /**< Control command. */
  static const LOW_portUsbDevice::wValue_t resumeExe_ctlCmd      = 0x0002;  /**< Control command. */
  static const LOW_portUsbDevice::wValue_t haltExeIdle_ctlCmd    = 0x0003;  /**< Control command. */
  static const LOW_portUsbDevice::wValue_t haltExeDone_ctlCmd    = 0x0004;  /**< Control command. */
  static const LOW_portUsbDevice::wValue_t flushCommCmds_ctlCmd  = 0x0007;  /**< Control command. */
  static const LOW_portUsbDevice::wValue_t flushRcvBuffer_ctlCmd = 0x0008;  /**< Control command. */
  static const LOW_portUsbDevice::wValue_t flushXmtBuffer_ctlCmd = 0x0009;  /**< Control command. */
  static const LOW_portUsbDevice::wValue_t getCommCmds_ctlCmd    = 0x000A;  /**< Control command. */
  //!@}

  //! @name Mode commands.
  //!@{
  static const LOW_portUsbDevice::wValue_t pulseEn_modCmd            = 0x0000;  /**< Mode command. */
  static const LOW_portUsbDevice::wValue_t speedChangeEn_modCmd      = 0x0001;  /**< Mode command. */
  static const LOW_portUsbDevice::wValue_t oneWireSpeed_modCmd       = 0x0002;  /**< Mode command. */
  static const LOW_portUsbDevice::wValue_t strongPuDuration_modCmd   = 0x0003;  /**< Mode command. */
  static const LOW_portUsbDevice::wValue_t pulldownSlewRate_modCmd   = 0x0004;  /**< Mode command. */
  static const LOW_portUsbDevice::wValue_t progPulseDuration_modCmd  = 0x0005;  /**< Mode command. */
  static const LOW_portUsbDevice::wValue_t write1LowTime_modCmd      = 0x0006;  /**< Mode command. */
  static const LOW_portUsbDevice::wValue_t dsow0Trec_modCmd          = 0x0007;  /**< Mode command. */
  //!@}

  //! @name Base for communication commands.
  //!@{
  static const LOW_portUsbDevice::wValue_t setDuration_comCmdBase         = 0x0012;  /**< Communication command. */
  static const LOW_portUsbDevice::wValue_t pulse_comCmdBase               = 0x0030;  /**< Communication command. */
  static const LOW_portUsbDevice::wValue_t oneWireReset_comCmdBase        = 0x0042;  /**< Communication command. */
  static const LOW_portUsbDevice::wValue_t bitIO_comCmdBase               = 0x0020;  /**< Communication command. */
  static const LOW_portUsbDevice::wValue_t byteIO_comCmdBase              = 0x0052;  /**< Communication command. */
  static const LOW_portUsbDevice::wValue_t blockIO_comCmdBase             = 0x0074;  /**< Communication command. */
  static const LOW_portUsbDevice::wValue_t matchAccess_comCmdBase         = 0x0064;  /**< Communication command. */
  static const LOW_portUsbDevice::wValue_t readStraight_comCmdBase        = 0x0080;  /**< Communication command. */
  static const LOW_portUsbDevice::wValue_t doAndRelease_comCmdBase        = 0x6092;  /**< Communication command. */
  static const LOW_portUsbDevice::wValue_t setPath_comCmdBase             = 0x00a2;  /**< Communication command. */
  static const LOW_portUsbDevice::wValue_t writeSramPage_comCmdBase       = 0x00b2;  /**< Communication command. */
  static const LOW_portUsbDevice::wValue_t writeEprom_comCmdBase          = 0x00c4;  /**< Communication command. */
  static const LOW_portUsbDevice::wValue_t readCrcProtPage_comCmdBase     = 0x00d4;  /**< Communication command. */
  static const LOW_portUsbDevice::wValue_t readRedirectPageCrc_comCmdBase = 0x21e4;  /**< Communication command. */
  static const LOW_portUsbDevice::wValue_t searchAccess_comCmdBase        = 0x00f4;  /**< Communication command. */
  //!@}

  
  //! @name Communication command bitmasks.
  //!@{

  /** RTS = 1 returns the discrepancy information to the host if SM = 1 and there are more devices than could
              be discovered in the current pass.
      RTS = 0 does not return discrepancy information.
   */
  static const LOW_portUsbDevice::wValue_t RTS_bitmask   = 0x4000;

  /** CIB = 1 prevents a strong pullup to 5V if SPU = 1 and the bit read back from the 1- Wire bus is 1.
      CIB = 0 generally enables the strong pullup to 5V.
   */
  static const LOW_portUsbDevice::wValue_t CIB_bitmask   = 0x4000;

  /** PST = 1 continuously generate 1-Wire Reset sequences until a presence pulse is discovered.
      PST = 0 generate only one 1-Wire Reset sequence.
   */
  static const LOW_portUsbDevice::wValue_t PST_bitmask   = 0x4000;

  /** PS = 1 reduces the preamble size to 2 bytes (rather than 3).
      PS = 0 sets preamble size to 3 bytes.
   */
  static const LOW_portUsbDevice::wValue_t PS_bitmask    = 0x4000;

  /** DT = 1 activates/selects the CRC16 generator.
      DT = 0 specifies no CRC.
   */
  static const LOW_portUsbDevice::wValue_t DT_bitmask    = 0x2000;

  /** SPU = 1 inserts a strong pullup to 5V after a Bit or Byte or Block I/O or Do & Release command.
      SPU = 0 no strong pullup.
   */
  static const LOW_portUsbDevice::wValue_t SPU_bitmask   = 0x1000;

  /** F = 1 clears the buffers in case an error occurred during the execution of the previous command;
            requires that ICP = 0 in the previous command.
      F = 0 prevents the buffers from being cleared.
   */
  static const LOW_portUsbDevice::wValue_t F_bitmask     = 0x0800;

  /** NTF = 1 always generate communication command processing result feedback if ICP = 0
      NTF = 0 generate communication command processing result feedback only if an error occurs
              and ICP = 0. If ICP = 1 command result feedback is suppressed for either case,
              see the ICP bit above.
   */
  static const LOW_portUsbDevice::wValue_t NTF_bitmask   = 0x0400;

  /** ICP = 1 indicates that the command is not the last one of a macro; as a consequence command processing
              result feedback messages are suppressed.
      ICP = 0 indicates that the command is the last one of a macro or single command operation;
              enables command processing result feedback signaling.
   */
  static const LOW_portUsbDevice::wValue_t ICP_bitmask   = 0x0200;

  /** RST = 1 inserts a 1-Wire Reset before executing the command.
      RST = 0 no 1-Wire Reset inserted.
   */
  static const LOW_portUsbDevice::wValue_t RST_bitmask   = 0x0100;

  /** SE = 1 enable the speed change on the 1-Wire bus.
      SE = 0 disable the speed change on the 1-Wire bus.
   */
  static const LOW_portUsbDevice::wValue_t SE_bitmask    = 0x0008; 

  /** R = 1 performs a read function.
      R = 0 performs a write function.
   */
  static const LOW_portUsbDevice::wValue_t R_bitmask     = 0x0008;

  /** Z = 1 checks if the 0-bits in the byte to be written are 0-bits in the byte read back form the device.
      Z = 0 checks if the byte to be written is identical to the one read back from the device.
   */
  static const LOW_portUsbDevice::wValue_t Z_bitmask     = 0x0008;

  /** SM = 1 searches for and reports ROM Ids without really accessing a particular device.
      SM = 0 makes a  Strong Access  to a particular device.
   */
  static const LOW_portUsbDevice::wValue_t SM_bitmask    = 0x0008;

  /** CH = 1 follows the chain if the page is redirected.
      CH = 0 stops reading if the page is redirected.
   */
  static const LOW_portUsbDevice::wValue_t CH_bitmask    = 0x0008;

  /** D       Data bit value to be written to the 1-Wire bus.
   */
  static const LOW_portUsbDevice::wValue_t D_bitmask     = 0x0008;

  /** TYPE = 1 specifies programming pulse duration.
      TYPE = 0 specifies strong pullup duration.
   */
  static const LOW_portUsbDevice::wValue_t TYPE_bitmask  = 0x0008;

  /** IM = 1 enables immediate execution of the command. Assumes that all 1-Wire device data required by the
             command has been received at EP2.
      IM = 0 prevents immediate execution of the command; execution must be started through a
             control function command.
   */
  static const LOW_portUsbDevice::wValue_t IM_bitmask    = 0x0001;
  //!@}

  
  //! @name State register enable bitmasks.
  //!@{
  /** SPUE If set to 1, the strong pullup to 5V is enabled, if set to 0, it is disabled.
   */
  static const uint8_t SPUE_bitmask = 0x01;
  /** PRGE If set to 1, a 12V programming pulse is enabled, if set to 0, it is disabled.
   */
  static const uint8_t PRGE_bitmask = 0x02;
  /** SPCE If set to 1, a dynamic 1-Wire bus speed change through a Communication
           command is enabled, if set to 0, it is disabled.
   */
  static const uint8_t SPCE_bitmask = 0x04;
  //!@}

  
  //! @name State register device status bitmasks.
  //!@{
  /** SPUA if set to 1, the strong pullup to 5V is currently active, if set to 0, it is inactive.
   */
  static const uint8_t SPUA_bitmask = 0x01;
  /** PRGA if set to 1, a 12V programming pulse is currently being generated, if set to 0, it is not.
   */
  static const uint8_t PRGA_bitmask = 0x02;
  /** 12VP if set to 1, the external 12V programming voltage is present, if set to 0, it is not present.
   */
  static const uint8_t VP12_bitmask = 0x04;
  /** PMOD if set to 1, the DS2490 is powered from USB and external sources,
           if set to 0, all DS2490 power is provided from USB.
   */
  static const uint8_t PMOD_bitmask = 0x08;
  /** HALT if set to 1, the DS2490 is currently halted, if set to 0, the device is not halted.
   */
  static const uint8_t HALT_bitmask = 0x10;
  /** IDLE if set to 1, the DS2490 is currently idle, if set to 0, the device is not idle.
   */
  static const uint8_t IDLE_bitmask = 0x20;
  /** EPOF  Endpoint 0 FIFO status, see Note 1.
   */
  static const uint8_t EP0F_bitmask = 0x80;
  //!@}

  
  //! @name Communication command error bitmasks.
  //!@{

  /** 1-Wire Device Detect Byte
   */
  static const uint8_t devDetect_code = 0xa5;

  /** EOS A value of 1 indicates that a SEARCH ACCESS with SM = 1 ended sooner than expected
          reporting less ROM ID s than specified in the  number of devices  parameter.
   */
  static const uint8_t EOS_bitmask = 0x80;

  /** RDP A value of 1 indicates that a READ REDIRECT PAGE WITH/CRC encountered a page that is redirected.
   */
  static const uint8_t RDP_bitmask = 0x40;

  /** CRC A value of 1 indicates that a CRC error occurred when executing one of the following
         commands: WRITE SRAM PAGE, WRITE EPROM, READ CRC PROT PAGE, or READ REDIRECT PAGE W/CRC.
   */
  static const uint8_t CRC_bitmask = 0x20;

  /** CMP A value of 1 indicates an error with one of the following: Error when reading the confirmation
          byte with a SET PATH command. The WRITE EPROM command did not program successfully. There was
          a difference between the byte written and then read back with a BYTE I/O command
   */
  static const uint8_t CMP_bitmask = 0x10;

  /** VPP A value of 1 indicates that during a PULSE with TYPE = 1 or WRITE EPROM command the 12V
          programming pulse was not seen on the 1-Wire bus; this could indicate that there were other
          devices on the bus that clamped the voltage to a value lower than VPP or that the programming
          pulse was disabled with a mode command.
   */
  static const uint8_t VPP_bitmask = 0x08;

  /** APP A value of 1 indicates that a 1-WIRE RESET revealed an Alarming Presence Pulse.
   */
  static const uint8_t APP_bitmask = 0x04;

  /** SH  A value of 1 indicates that a 1-WIRE RESET revealed a short to the 1-Wire bus or the
          SET PATH command could not successfully connect a branch due to a short.
   */
  static const uint8_t SH_bitmask  = 0x02;

  /** NRS A value of 1 indicates an error with one of the following: 1-WIRE RESET did not reveal
          a Presence Pulse. SET PATH command did not get a Presence Pulse from the branch that was
          to be connected. No response from one or more ROM ID bits during a SEARCH ACCESS command.
   */
  static const uint8_t NRS_bitmask = 0x01;

  //!@}

  
  //=====================================================================================
  //
  // type definitions
  //
    
  //! @name DS2490 specific configuration values.
  //!@{
  /** Type for wirespeed */
  typedef enum { /** 16 kbps */ normal_OWSPEED=0x00, flexible_OWSPEED, /** 142 kbps */ overdrive_OWSPEED } OWSPEED_val_t;
  /** Type for flexible speed pulldown slew rate in V/microsec */
  typedef enum { PDSR_15=0x00,    PDSR_2_2,  PDSR_1_65, PDSR_1_37, PDSR_1_1, PDSR_0_83, PDSR_0_7, PDSR_0_55} PDSR_val_t;
  /** Type for flexible speed write-1 low time in microsec */
  typedef enum { W1LT_8=0x00,     W1LT_9,    W1LT_10,   W1LT_11,   W1LT_12,  W1LT_13,   W1LT_14,  W1LT_15}   W1LT_val_t;
  /** Type for flexible speed DSO/ W0R time in microsec */
  typedef enum { SOW0RT_3=0x00,   SOW0RT_4,  SOW0RT_5,  SOW0RT_6,  SOW0RT_7, SOW0RT_8,  SOW0RT_9, SOW0RT_10} SOW0RT_val_t;
  //!@}

  /** Type to define result handling with communication commands. */
  typedef enum {
    noResult_rsltHdl=0x00,    /**< NTF=x  ICP=1, Never generate result register feedback. */
    resultOnError_rsltHdl,    /**< NTF=0  ICP=0, Generate result register on error and leave in EP1 FIFO. */
    resultAlways_rsltHdl      /**< NTF=1  ICP=0, Always generate result register and leave in EP1 FIFO. */
  }  resultHandling_t;

  
  //! @name Status and result code types.
  //!@{

  /** Status information, raw format from device */
  typedef struct deviceFeedbackRaw_t {
    uint8_t  enableFlags;
    uint8_t  oneWireSpeed;
    uint8_t  strongPullupDuration;
    uint8_t  progPulseDuration;
    uint8_t  pulldownSlewRate;
    uint8_t  write1LowTime;
    uint8_t  dsow0RecTime;
    uint8_t  reserved0;
    uint8_t  deviceStatusFlags;
    uint8_t  commCmdLO;
    uint8_t  commCmdHI;
    uint8_t  commCmdBufferStatus;
    uint8_t  dataOutBufferStatus;
    uint8_t  dataInBufferStatus;
    uint8_t  reserved1;
    uint8_t  reserved2;
    uint8_t  resultCode[16];
  } deviceFeedbackRaw_t;

  
  /** Status information, decoded format */
  typedef struct resultCode_t {
    bool deviceDetected;             /**< 1-wire device detect byte found */
    bool searchAccessDeviceUnderrun; /**< EOS bit */
    bool pageIsRedirected;           /**< RDP bit */
    bool crcError;                   /**< CRC bit */
    bool compareError;               /**< CMP bit */
    bool progVoltageMissingOnBus;    /**< VPP bit */
    bool alarmingPresencePulse;      /**< APP bit */
    bool shortToGround;              /**< SH bit  */
    bool noPresencePulse;            /**< NRS bit */
  } resultCode_t;

  typedef std::vector<resultCode_t>   resultCodeVec_t;  /**< Vector type of resultCode_t. */

  /** Status information, decoded format */
  typedef struct deviceFeedback_t {
    bool           isStrongPullupEnabled;       /**< SPUE bit */
    bool           isProgPulseEnabled;          /**< PRGE bit */
    bool           isDynSpeedChangeEnabled;     /**< SPCE bit */
    OWSPEED_val_t  oneWireSpeed;                /**< current 1-Wire bus speed code */
    uint8_t        strongPullupDurationFactor;  /**< current pullup duration */
    uint8_t        progPulseDurationFactor;     /**< current programming pulse duration */
    PDSR_val_t     pulldownSlewRate;            /**< current pulldown slew rate code */
    W1LT_val_t     write1LowTime;               /**< current Write-1 low time code */
    SOW0RT_val_t   dsow0RecTime;                /**< current data sample offset/ Write-0 recovery time code */
    bool           strongPullupCurrentActive;   /**< SPUA bit */
    bool           progPulseCurrentActive;      /**< PRGA bit */
    bool           progVoltagePresent;          /**< 12VP bit */
    bool           deviceExternalPowered;       /**< PMOD bit */
    bool           deviceHalted;                /**< HALT bit */
    bool           deviceIdle;                  /**< IDLE bit */
    bool           ep0FifoOverflowError;        /**< EPOF bit */
    uint16_t       currentCommCmd;              /**< Communication command currently being processed. If the device is idle, a register value of 0x00 is sent. */
    uint8_t        commCmdBufferUsage;          /**< Number of data bytes currently contained in the 16-byte FIFO used to hold communication commands. */
    uint8_t        dataOutBufferUsage;          /**< Number of data bytes currently contained in the 128-byte FIFO used to write data to the 1-Wire bus. */
    uint8_t        dataInBufferUsage;           /**< Number of data bytes currently contained in the 128-byte command FIFO used to read data from the 1-Wire bus. */
    uint8_t        resultCount;                 /**< Number of results received. */
    resultCode_t   compoundResults;             /**< All results ORed together. */
  } deviceFeedback_t;

  //!@}

  

  //=====================================================================================
  //
  // attributes
  //

  LOW_portUsbDevice *usbDevice;  /**< The USB device we use. */



  //=====================================================================================
  //
  // methods
  //

  /** @name Mid-layer methods.
   */
  //!@{

  /** Actions common to all constructors.
   */
  void commonConstructorActions();
  
  
  /** Fetch device status and all available result codes.
      @param outDevFeedback    Device status read from the device.
      @param outResultCodeVec  All result codes which were available.
   */
  void readDeviceStatus( deviceFeedback_t &outDevFeedback, resultCodeVec_t &outResultCodeVec);

  
  /** Periodically poll the device status until the it's status is idle.
      @param outDeviceFeedback  Feedback status of the last poll.
      @param outResultCodeVec   All result codes being available during polling.
      @param inTimeout          Maximum time to wait for the device to become idle.
   */
  void waitUntilIdle( deviceFeedback_t &outDeviceFeedback, resultCodeVec_t &outResultCodeVec, 
                      const LOW_portUsbDevice::usbTimeout_t inTimeout);

  /** Universal and internal funtion for single strong pullups.
      @param inPullupFactor  Factor of pulse length. Length will be factor times 16ms.
   */
  void strongPullupInternal( const unsigned int inPullupFactor);

  /** Universal and internal funtion for program pulses.
      @param inPullupFactor  Factor of pulse length. Length will be factor times 8us.
   */
  void progPulseInternal( const unsigned int inPulseFactor);
  
  //!@}


  
  /** @name Control command methods.
      Control commands are used to control the processing of Communication commands/data and can also be used
      to issue a device Reset. There are 9 Control commands. Like Mode commands, Control commands are
      immediately processed by the DS2490 when they are received.
   */
  //!@{

  /** Performs a hardware reset equivalent to the power-on reset.
      This includes clearing all endpoint buffers and loading the Mode control registers
      with their default values.
   */
  virtual void ctlCmd_resetDevice();

  /** Starts execution of Communication commands.
      This command is also required to start the execution of Communication commands with an
      IM (immediate execution control) bit set to logic 0.
   */
  virtual void ctlCmd_startExecution();


  /** Used to resume execution of a Communication command that was halted with either of
      the HALT EXECUTION commands.
   */
  virtual void ctlCmd_resumeExecution();


  /** Used to halt the execution of the current Communication command after the 1-Wire bus
      has returned to the idle state.
      Further Communication command processing is stopped until a RESUME EXECUTION command is received.
      This command, or the HALT EXECUTION WHEN DONE command, is also used to terminate a strong pullup
      or programming pulse of semi-infinite or infinite duration.
   */
  virtual void ctlCmd_haltExecutionWhenIdle();


  /** Used to halt the execution of a Communication command after the current command execution
      is complete.
      Further Communication command processing is stopped until a RESUME EXECUTION command is received.
      This command, or the HALT EXECUTION WHEN IDLE command, is also used to terminate a strong pullup
      or programming pulse of semi-infinite or infinite duration.
   */
  virtual void ctlCmd_haltExecutionWhenDone();


  /** Used to clear all unexecuted Communication commands from the command FIFO.
      The DS2490 must be in a halted state before the FLUSH COMM CMDS command can be processed.
   */
  virtual void ctlCmd_flushCommCmds();


  /** Used to clear EP3 receive data FIFO (data from 1- Wire device).
      The DS2490 must be in a halted state before the FLUSH DATA RCV BUFFER command can be processed.
   */
  virtual void ctlCmd_flushDataRcvBuffer();


  /** Used to clear EP2 transmit data FIFO (data to 1- Wire device).
      The DS2490 must be in a halted state before the FLUSH DATA XMT BUFFER command can be processed.
   */
  virtual void ctlCmd_flushDataXmtBuffer();


  /** Used to retrieve unexecuted Communication commands and parameters from the command FIFO.

      The DS2490 must be in a halted state before the GET COMM CMDS command can be processed.

      Unexecuted commands are returned over EP0 in the control transfer data phase.
      Host software is responsible for determining the number of command/parameter bytes to be returned and
      specifying the value in the wLength field of the control transfer setup packet.

      Commands/parameters are deleted from the FIFO as they are transmitted to the host; the command
      pointer used with the FIFO is updated as values are read. Any commands/parameters that are not
      transferred remain in the FIFO and will be processed when command execution resumes.

      If the wLength value passed is larger than the number of command/parameter bytes, the DS2490 will
      terminate the control transfer with a short data packet.
   */
  virtual void ctlCmd_getCommCmds( byteVec_t &outBytes);
  
  //!@}


  
  /** @name Mode command methods.
      1-Wire characteristics and features (speed, durations, slew rate, etc.) are controlled with discrete 
      Mode commands and/or embedded Communication command parameters. Parameter values and enable/disable 
      settings are used for characteristic control. Mode settings are stored in the DS2490 State Registers 
      and can be read at any time or at the polling interval of EP1. Enable/disable settings are used to 
      control three global features:   
      
        - Strong pullup to +5V   
        
        - +12V EPROM programming pulse   
        
        - Dynamic 1-Wire bus communication speed change through a communication command 
        
      The settings for these three global controls are accessible only through Mode commands; the features 
      are either enabled or disabled with the appropriate Mode command. Specific Communication commands 
      exist to issue a strong pullup, a programming pulse, or speed change. When the corresponding setting 
      is enabled the feature is usable as part of a communication command when disabled the feature is not usable. 
      Parameter values and codes are used to specify and control 1-Wire bus characteristics:   
      
        - 1-Wire bus communication speed   
        
        - +5V strong pullup duration   
        
        - +12V programming pulse duration   
        
        - Pulldown slew rate   
        
        - Write-1 low time   
        
        - Data sample offset/Write-0 Recovery time

      These six settings are controllable with discrete Mode commands or as embedded command/parameter values 
      in Communications commands. Mode commands are immediately processed by the DS2490 when they are received. 
   */
  //!@{
  /** Used to enable or disable a 1-Wire strong pullup pulse to 5V and/or +12V EPROM programming pulse.
      Two bit positions in the parameter byte are used to control the enabled/disabled state for each pulse type. 
      The specific pulse is enabled when the respective bit is set to a 1 and disabled when set to a 0. 
      The DS2490 power-up default state for both strong pullup and programming pulse is disabled. 
   */
  virtual void modCmd_setEnablePulse( const bool inEnableStrongPullup, const bool inEnableProgPulse);
  
  
  /** Used to enable or disable a 1-Wire communication speed change. 
      Enabled when a TRUE parameter value is passed with the command, disabled with a FALSE. 
      The DS2490 power-up default state for speed change is disabled. 
   */
  virtual void modCmd_setEnableSpeedChange( const bool inEnableSpeedChange);
  
  
  /** Used to set the speed of 1-Wire communication; three settings are possible. 
      The DS2490 power-up default communication speed is regular.
   */
  virtual void modCmd_setOneWireSpeed( const OWSPEED_val_t inWireSpeed);

  
  /** Used to set the time duration of a 1-Wire strong pullup. 
      The time is controlled with an unsigned 8-bit binary number between 0x00 and 0xFE which specifies 
      the duration in multiples of 16ms. A value of 0x01 specifies 16ms, 0x02 equals 32ms, etc. 
      A value of 0x00 specifies infinite duration. Parameter value 0xFF is reserved and will cause 
      the device to deliver a pullup duration of <1µs. 
      To terminate an infinite duration pullup use either the HALT EXECUTION WHEN DONE or 
      HALT EXECUTION WHEN IDLE Control commands as described in Appendix 1. 
      The DS2490 power-up default strong pullup duration register value is 512ms.
   */
  virtual void modCmd_setStrongPullupDuration( const uint8_t inSpuDurationFactor);

  
  /** Used to set the time duration of a 1-Wire Programming Pulse. 
      The time is controlled with a an unsigned 8-bit binary number between 0x00 and 0xFE specifying 
      the duration in multiples of 8µs. A value of 0x00 stands for infinite duration. Parameter value 0xFF 
      is reserved and will cause the device to deliver a pulse duration of <1µs. To terminate an infinite 
      duration programming pulse use the HALT EXECUTION WHEN DONE or HALT EXECUTION WHEN IDLE command.
      The DS2490 power-up default strong pullup duration register value is 512µS.
   */
  virtual void modCmd_setProgPulseDuration( const uint8_t inPpDurationFactor);

  
  /** Used to select the pulldown slew rate for 1-Wire bus Flexible Speed operation. 
      Eight pulldown slew rates are possible. The nominal pulldown slew rate for Regular speed is 0.83V/s and 
      for Overdrive speeds it is 15V/s.
      The DS2490 pulldown slew rate power-up default value for Flexible speed is 0.83V/  s.
   */
  virtual void modCmd_setPulldownSlewRate( const PDSR_val_t inPDSR);
  
  
  /** Used to select the Write-1 low time for 1-Wire bus Flexible speed operation.
      Eight Write-1 low time durations are possible. The nominal Write-1 Low Time for Regular speed is 8us, at 
      Overdrive speed it is 1us. 
      The DS2490 Write-1 Low Time power-up default value for Flexible speed is 12us.
   */
  virtual void modCmd_setWrite1LowTime( const W1LT_val_t inW1LT);
  
  
  /** Used to select the Data Sample Offset (tDSO) / Write-0 recovery (tW0R) time (DSO/W0R) for 1-Wire 
      bus Flexible Speed operation. 
      Eight DSO/W0R times are possible. The nominal DSO/W0R time for Regular speed is 3us, for Overdrive 
      speed the Data Sample Offset is 1us and the Write-0 Recovery Time is 3 s. 
      The DS2490 DSO/W0R power-up default value for Flexible speed is 7us.
   */
  virtual void modCmd_setDsoW0RecoveryTime( const SOW0RT_val_t inSOW0RT);
  
  //!@}

  
  
  /** @name Communication command methods.
  
      Communication commands are used to communicate with and control an attached 1-Wire device(s). 
      Communication commands also contain embedded command bits for conditional control or additional 
      functionality. 
      
      Unlike Mode and Control commands, Communication command processing is controlled by host software 
      by setting embedded command bits within a Communication command. For example, a command macro form 
      is supported in which several Communication commands and parameter data are sent/buffered in the 
      DS2490 and then processed as a group. 
      
      Embedded command bit ICP is used to create macro command sets. Also, unlike Mode and Control commands, 
      several forms of Communication command monitoring exist for host software to track processing progress. 
      Depending on the settings of embedded communication command bits ICP and NTF, a Result Register value 
      may be generated to provide command processing feedback. 
      
      Communication command buffer (FIFO) status and 1-Wire data transmit/received buffers (EP2/3 FIFOs) 
      status can also be monitored via the State Registers.
   */
  //!@{
  
  /** Changes the State Register pulse duration value for either the +12V programming pulse or strong pullup.
      The new duration value is loaded into the duration register specified. The duration values can alternatively
      be changed using a Mode command. The new duration setting remains effective until changed by another 
      SET DURATION or MODE Command. See section  MODE COMMANDS  for duration parameter value setting.
   */
  virtual void comCmd_setDuration( const uint8_t inTimeFactor,
                                   const bool inSpecifyProgPulse,
                                   const resultHandling_t inResultHandling = resultOnError_rsltHdl,
                                   const bool inImmediateExec = true);
  
  
  /** Used to temporarily pull the 1-Wire bus to +12V in order to program an EPROM device or 
      to generate a strong pullup to 5V in order to provide extra power for an attached iButton device, 
      e.g., temperature sensor or crypto iButton. 
      
      The pulse duration is determined by the value in the respective mode register, programming pulse or 
      strong pullup. The 12VP bit in the Device Status Flags (bit2) is used to determine whether +12V VPP 
      programming voltage is present.
   */
  virtual void comCmd_pulse( const bool inSpecifyProgPulse,
                             const bool inFlushBuffersOnErr,
                             const resultHandling_t inResultHandling = resultOnError_rsltHdl,
                             const bool inImmediateExec = true);

  
  /** Used to generate a reset pulse on the 1-Wire bus and to optionally change the 1-Wire speed. 
      The new speed will take effect only if the embedded command bit SE is set to 1. When the 1-WIRE RESET is 
      sent after an Overdrive Skip ROM command, SE must be 1 and the new speed must be 0x02. 
      See MODE CONTROL section for additional communication speed information. To switch back to regular speed, 
      set SE = 1 and the new speed parameter to 0x00 (regular) or 0x01 (flexible).
   */
  virtual void comCmd_oneWireReset( const bool inLoopUntilPresenceDetect,
                                    const bool inEnableSpeedChange,
                                    const OWSPEED_val_t inSpeedSelector,
                                    const bool inFlushBuffersOnErr,
                                    const resultHandling_t inResultHandling = resultOnError_rsltHdl,
                                    const bool inImmediateExec = true);

  
  /** Generates a single time slot on the 1-Wire bus and reads back the response. 
      This time slot may optionally be followed by a strong pullup using embedded command bits SPU and CIB. 
      With CIB = 1, a requested strong pullup will only occur if the read-back revealed a 0. 
      Data is returned to the host only if the embedded command bit ICP = 0. If ICP = 0, the bit read from 
      the 1-Wire device is stored in the EP3 FIFO and is read by the host using an EP3 bulk transaction.
   */
  virtual void comCmd_bitIO( const bool inWriteBit,
                             const bool inDoStrongPullup,
                             const bool inSuppressPullupOnRead1,
                             const resultHandling_t inResultHandling = resultOnError_rsltHdl,
                             const bool inImmediateExec = true);

  
  /** Accomplishes a direct 1-Wire write and read with optional strong pullup after the last bit of the byte. 
      The optional strong pullup is controlled using embedded command bit SPU. For a write sequence, the data 
      byte to be written is included in the command setup packet as shown in Appendix 2.
      For a read sequence, the setup packet data byte value is set to 0xFF. 
      Data is returned to the host only if the embedded command bit ICP = 0. If ICP = 0, the byte read from 
      the 1-Wire device is stored in the EP3 FIFO and is read by the host using an EP3 bulk transaction.
   */
  virtual void comCmd_byteIO( const uint8_t inWriteByte,
                              const bool inDoStrongPullup,
                              const resultHandling_t inResultHandling = resultOnError_rsltHdl,
                              const bool inImmediateExec = true);

  
  /** Accomplishes a direct 1-Wire write or read with optional strong pullup after the last byte of the block. 
      The optional strong pullup is controlled using embedded command bit SPU. Embedded command bit RST enables 
      a 1-Wire reset before the command executes. To accomplish a READ function all input data should be 0xFF, 
      otherwise the data read from the 1-Wire bus will be masked. For a block write sequence the EP2 FIFO must 
      be pre-filled with data before command execution. Additionally, for block sizes greater then the FIFO 
      size, the FIFO content status must be monitored by host SW so that additional data can be sent to the 
      FIFO when necessary. A similar EP3 FIFO content monitoring requirement exists for block read sequences. 
      During a block read the number of bytes loaded into the EP3 FIFO must be monitored so that the data can 
      be read before the FIFO overflows.
   */
  virtual void comCmd_blockIO( const uint16_t inWriteSize,
                               const bool inBusResetBefore,
                               const bool inDoStrongPullup,
                               const resultHandling_t inResultHandling = resultOnError_rsltHdl,
                               const bool inImmediateExec = true);

  
  /** Is used to address a device on the active section of the 1-Wire bus using the Match ROM or Overdrive Match 
      command code. 
      The EP2 FIFO must be pre-filled with the 8 bytes target ROM ID before command execution. Embedded command 
      bit RST enables a 1-Wire reset before the command executes, and embedded command bit SE enables a 1-Wire 
      speed change that takes effect before the command executes.
   */
  virtual void comCmd_matchAccess( const uint8_t inMatchCommand,
                                   const bool inBusResetBefore,
                                   const bool inEnableSpeedChange,
                                   const OWSPEED_val_t inSpeedSelector,
                                   const resultHandling_t inResultHandling = resultOnError_rsltHdl,
                                   const bool inImmediateExec = true);

  
  /** Transmits a user-specified preamble of data to the 1-Wire bus and then reads back as 
      many bytes as specified. 
      The preamble typically consists of a 1-Wire command code followed by TA1 and TA2. It is possible to include 
      a Match ROM command and ROM ID in the preamble to also address a device at the current speed. 
      This command can also be used to copy the scratchpad or disconnect a path. Embedded command bit RST enables 
      a 1-Wire reset before the command executes. The EP2 FIFO must be pre-filled with preamble data before 
      command execution. Additionally, for preamble sizes greater then the FIFO size, the FIFO content status 
      must be monitored by host SW so that additional data can be sent to the FIFO when necessary. A similar 
      EP3 FIFO content monitoring requirement exists for the block read. During a block read, the number of bytes 
      loaded into the EP3 FIFO must be monitored so that the data can be read before the FIFO overflows.
   */
  virtual void comCmd_readStraight( const uint8_t inWritePreambleSize,
                                    const uint16_t inReadSize,
                                    const bool inBusResetBefore,
                                    const resultHandling_t inResultHandling = resultOnError_rsltHdl,
                                    const bool inImmediateExec = true);

  
  /** Typically used to control the activity of the CPU of the crypto iButton, which requires a release sequence. 
      This command can also be used to read from or write to the I/O buffer or Status Register of the crypto 
      iButton. A short preamble (3 bytes) consists of a 1-Wire command code followed by the release sequence. 
      A four-byte preamble with embedded command bit R = 1 consists of a 1-Wire command code followed by length 
      byte and the release sequence. A 4-byte preamble with embedded command bit R = 0 consists of a 1-Wire command 
      code followed by a status byte and the release sequence. The long preamble (minimum 5 bytes, embedded command 
      bit R = 0) consists of a 1-Wire command, a length byte, data bytes and the release sequence. In this case, 
      the length byte indicates the total number of bytes between the length byte and release sequence. The least 
      significant byte of the release sequence is transmitted first. Embedded command bit SPU must be 1 when trying 
      to run (start, continue) the CPU of the crypto iButton. In all other cases, SPU should be 0. Additionally, 
      embedded command bit F can be used to clear the communication command buffer and EP2/3 FIFOs if an error 
      occurs during execution of the command. The EP2 FIFO must be pre-filled with preamble data before command 
      execution. Additionally, for preamble sizes greater then the FIFO size, the FIFO content status must be 
      monitored by host SW so that additional data can be sent to the FIFO when necessary. A similar EP3 FIFO 
      content monitoring requirement exists for the block read. During a block read the number of bytes loaded 
      into the EP3 FIFO must be monitored so that the data can be read before the FIFO overflows.
   */
  virtual void comCmd_doAndRelease( const uint8_t inWritePreambleSize,
                                    const bool inDoReadOperation,
                                    const bool inDoStrongPullup,
                                    const bool inFlushBuffersOnErr,
                                    const resultHandling_t inResultHandling = resultOnError_rsltHdl,
                                    const bool inImmediateExec = true);

  
  /** Used to activate a series of couplers that finally provide the access to the target 1-Wire device. 
      Embedded command bit RST enables a 1-Wire reset before the command executes. Additionally, embedded 
      command bit F can be used to clear the communication command buffer and EP2/3 FIFOs if an error occurs 
      during execution of the command. The EP2 FIFO must be pre-filled with coupler data before command execution. 
      Additionally, for coupler data sizes greater then the EP2 FIFO size, the FIFO content status must be 
      monitored by host SW so that additional data can be sent to the FIFO when necessary. Upon completion of 
      command execution, the EP3 FIFO is loaded with a 1-byte value that indicates the number of couplers 
      activated. An NRS error code is an indication that there was no presence pulse on the branch that was 
      to be connected.
   */
  virtual void comCmd_setPath( const uint8_t inPreloadPathSize,
                               const bool inBusResetBefore,
                               const bool inFlushBuffersOnErr,
                               const resultHandling_t inResultHandling = resultOnError_rsltHdl,
                               const bool inImmediateExec = true);

  
  /** Used to write data to the scratchpad of an SRAM device and optionally check the CRC. To copy the data 
      to its final memory location, it is required to send the READ STRAIGHT command with a  copy scratchpad  
      preamble and a 0 block size. Embedded command bit DT = 1 activates the CRC16 generator. This command is 
      also applicable to the Write IPR function of the Crypto iButton if embedded command bit CIB = 1. To write 
      a partial page the page size parameter and the target address need to be set accordingly. The EP2 FIFO must 
      be pre-filled with preamble and SRAM data before command execution. Additionally, for data page sizes 
      greater then the EP2 FIFO size, the FIFO content status must be monitored by host SW so that additional data 
      can be sent to the FIFO when necessary.
   */
  virtual void comCmd_writeSramPage( const uint8_t inWriteSize,
                                     const bool inShortPreambleSize,
                                     const bool inActivateCrc16,
                                     const bool inFlushBuffersOnErr,
                                     const resultHandling_t inResultHandling = resultOnError_rsltHdl,
                                     const bool inImmediateExec = true);

  
  /** Used to write data directly to the data or status memory of a 1-Wire EPROM device. 
      If embedded command bit DT = 1, the CRC16 generator is selected; if DT = 0, CRC8 is used. Embedded command 
      bit Z controls the method used for comparison of the data written to the 1-Wire device. Additionally, embedded 
      command bit F can be used to clear the Communication command buffer and EP2/3 FIFOs if an error occurs during 
      execution of the command. The EP2 FIFO must be pre-filled with preamble and EPROM data before command execution. 
      Additionally, for data block sizes greater then the EP2 FIFO size, the FIFO content status must be monitored by 
      host SW so that additional data can be sent to the FIFO when necessary.
   */
  virtual void comCmd_writeEprom( const uint16_t inWriteSize,
                                  const bool inActivateCrc16,
                                  const bool inCheck0BitsOnly,
                                  const bool inFlushBuffersOnErr,
                                  const resultHandling_t inResultHandling = resultOnError_rsltHdl,
                                  const bool inImmediateExec = true);

  
  /** Used to read one or multiple CRC-protected pages of EPROM devices and to read tamper-detect bytes and counter 
      value of Monetary iButtons. 
      If embedded command bit DT = 1, the CRC16 generator is selected; if DT = 0, CRC8 is used. Embedded command 
      bit CIB is used to specify either a 2- or 3-byte read preamble. Additionally, embedded command bit F can be 
      used to clear the Communication command buffer and EP2/3 FIFOs if an error occurs during execution of the 
      command. The command can also be used to read a single partial page up to the end of that page and reading the 
      PIOs of a DS2406. This command is also applicable to the Read IPR function of the crypto iButton if CIB = 1. 
      The EP2 FIFO must be pre-filled with preamble data before command execution. To prevent overflow, the EP3 FIFO 
      must be monitored (and read if necessary) during command execution if reading more data than the size of the 
      EP3 FIFO.
   */
  virtual void comCmd_readCrcProtPage( const uint8_t inPageCount,
                                       const uint8_t inPageSizeLog2,
                                       const bool inShortPreambleSize,
                                       const bool inActivateCrc16,
                                       const bool inFlushBuffersOnErr,
                                       const resultHandling_t inResultHandling = resultOnError_rsltHdl,
                                       const bool inImmediateExec = true);

  
  /** Used to read a single page of data and check the CRC. 
      If the page is not redirected, the host will receive the page number and all its data. If the page is 
      redirected and embedded command bit CH = 1 the device will follow the trail of redirections until a page is 
      found that is not redirected. When found, the host will receive the page number and all data of that 
      particular page. If CH = 0 and the first read attempt reveals a redirection the host will only receive the 
      number of the page the addressed page is redirected to. Reading starts at a page boundary. Additionally, 
      embedded command bit F can be used to clear the communication command buffer and EP2/3 FIFOs if an error occurs 
      during execution of the command. An NRS error code is an indication of an infinite loop. The EP2 FIFO must be
      pre-filled with ROM ID and command code data before command execution. To prevent overflow, the EP3 FIFO must 
      be monitored (and read if necessary) during command execution if reading more data than the size of the EP3 FIFO.
   */
  virtual void comCmd_readRedirectPageCrc( const uint8_t inPageNumber,
                                           const uint8_t inPageSize,
                                           const bool inFollowRedirect,
                                           const bool inFlushBuffersOnErr,
                                           const resultHandling_t inResultHandling = resultOnError_rsltHdl,
                                           const bool inImmediateExec = true);
  
  
  /** Is used to either access a device using the Search ROM command or to identify the devices on 
      the active segments of a 1-Wire Network. 
      Embedded command bit SM is used to specify the type of search. Embedded command bit RTS is used to control 
      discrepancy information reporting. Additionally, embedded command bit F can be used to clear the Communication 
      command buffer and EP2/3 FIFOs if an error occurs during execution of the command. An NRS error code indicates 
      that there was no response on at least one of the 64 bits of the ROM search. The EP2 FIFO must be pre-filled 
      with ROM ID data before command execution. To prevent overflow, the EP3 FIFO must be monitored (and read if 
      necessary) during command execution if returning more data than the size of the EP3 FIFO.
   */
  virtual void comCmd_searchAccess( const uint8_t inMaxDevNum,
                                    const uint8_t inSearchCommand,
                                    const bool inSearchWithoutFullAccess,
                                    const bool inReturnDiscrepancyInfo,
                                    const bool inBusResetBefore,
                                    const bool inFlushBuffersOnErr,
                                    const resultHandling_t inResultHandling = resultOnError_rsltHdl,
                                    const bool inImmediateExec = true);

                                    
  /** Handle common bits of all communication commands.
   */
  void handleCommonComCmdBits( LOW_portUsbDevice::wValue_t &inOutValueCmd,
                               const resultHandling_t inResultHandling, const bool inImmediateExec);


  //!@}

  
  
  //=====================================================================================
  //!
  //! @name Enumeration type conversion methods
  //!
  //@{


  /** Convert value of type PDSR_val_t to pdSlewRate_t.
      @throw  internal_error  Thrown when conversion not possible (should not happen).
   */
  virtual const pdSlewRate_t PDSR_val_2_pdSlewRate( const PDSR_val_t inPDSR_val) const;

  /** Convert value of type pdSlewRate_t to PDSR_val_t.
      @throw  internal_error  Thrown when conversion not possible (should not happen).
   */
  virtual const PDSR_val_t pdSlewRate_2_PDSR_val( const pdSlewRate_t inPdSlewRate_t) const;

  /** Convert value of type W1LT_val_t to w1LowTime_t.
      @throw  internal_error  Thrown when conversion not possible (should not happen).
   */
  virtual const w1LowTime_t W1LT_val_2_w1LowTime( const W1LT_val_t inW1LT_val) const;

  /** Convert value of type w1LowTime_t to W1LT_val_t.
      @throw  internal_error  Thrown when conversion not possible (should not happen).
   */
  virtual const W1LT_val_t w1LowTime_2_W1LT_val( const w1LowTime_t inW1LowTime) const;

  /** Convert value of type SOW0RT_val_t to soW0RecTime_t.
      @throw  internal_error  Thrown when conversion not possible (should not happen).
   */
  virtual const soW0RecTime_t SOW0RT_val_2_soW0RecTime( const SOW0RT_val_t inSOW0RT_val) const;

  /** Convert value of type soW0RecTime_t to SOW0RT_val_t.
      @throw  internal_error  Thrown when conversion not possible (should not happen).
   */
  virtual const SOW0RT_val_t soW0RecTime_2_SOW0RT_val( const soW0RecTime_t inSoW0RecTime) const;

  /** Convert value of type OWSPEED_val_t to wireSpeed_t.
      @throw  internal_error  Thrown when conversion not possible (should not happen).
   */
  virtual const wireSpeed_t OWSPEED_val_2_wireSpeed( const OWSPEED_val_t inOWSPEED_val) const;
  
  /** Convert value of type wireSpeed_t to OWSPEED_val_t.
      @throw  internal_error  Thrown when conversion not possible (should not happen).
   */
  virtual const OWSPEED_val_t wireSpeed_2_OWSPEED_val( const wireSpeed_t inWireSpeed) const;

  /** Convert value of type strongPullup_t to factor of 16ms.
      @throw  internal_error  Thrown when conversion not possible (should not happen).
   */
  virtual const uint8_t strongPullup_2_spFactor( const strongPullup_t inPullup) const;

  /** Convert value of type progPulse_t to factor of 8us.
      @throw  internal_error  Thrown when conversion not possible (should not happen).
   */
  virtual const uint8_t progPulse_2_ppFactor( const progPulse_t inPulse) const;
  //@}
    
};

#endif
