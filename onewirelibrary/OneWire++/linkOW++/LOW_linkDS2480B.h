/***************************************************************************
                          LOW_linkDS2480B.h  -  description
                             -------------------
    begin                : Sat Jul 13 2002
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

#ifndef LOW_LINKDS2480B_H
#define LOW_LINKDS2480B_H



#include "LOW_link.h"
#include "LOW_linkFlexibleSpeed.h"
#include "LOW_portSerialFactory.h"


  
/** Link class for DS2480B Serial 1-Wire Line Driver with Load Sensor.

    DS2480B features:

    - Universal, common-ground serial port to 1-Wire line driver for MicroLAN applications
    - Works with all iButtons and MicroLANcompatible 1-Wire slave devices
    - Communicates at regular and Overdrive 1- Wire speed and serial port data rates of
      9600 (default), 19200, 57600 and 115200 bps
    - Supports 12V EPROM programming and stiff 5V pullup for crypto iButton, sensors and EEPROM
    - Load sensor to terminate the stiff pullup as the energy demand of the crypto iButton drops
    - Self-calibrating time base with � 5% tolerance for serial and 1-Wire communication
    - Slew rate controlled 1-Wire pulldown and active pullup to accommodate long lines and
      reduce radiation
    - User-selectable RXD/TXD polarity minimizes component count when interfacing to 5V based
      RS232 systems or directly to UARTs
    - Programmable 1-Wire timing and driver characteristics accommodate a wide range of MicroLAN
      configurations at regular speed
    - Smart protocol combines data and control information without requiring extra pins
    - Compatible with optical, IR and RF to RS232 converters
    - Low cost 8-pin SOIC surface mount package
    - Operates over 4.5V to 5.5V from -40캜 to +85캜

    The DS2480B is a serial port to 1-Wire interface chip that supports regular and Overdrive speeds.
    It connects directly to UARTs and 5V RS232 systems. Interfacing to RS232C (� 12V levels) requires
    a passive clamping circuit and one 5V to �12V level translator. Internal timers relieve the host
    of the burden of generating the time-critical 1-Wire communication waveforms. In contrast to the
    DS9097(E) where a full character must be sent by the host for each 1-Wire time slot, the DS2480B
    can translate each character into eight 1-Wire time slots, thereby increasing the data throughput
    significantly. In addition, the DS2480B can be set to communicate at four different data rates,
    including 115.2 kbps, 57.6 kbps and 19.2 kbps with 9.6 kbps being the power-on default.
    Command codes received from the host s crystal controlled UART serve as a reference to continuously
    calibrate the on-chip timing generator. The DS2480B uses a unique protocol that merges data and
    control information without requiring control pins. This approach maintains compatibility to
    off-the-shelf serial to wireless converters, allowing easy realization of 1-Wire media jumpers.
    The various control functions of the DS2480B are optimized for MicroLAN 1-Wire networks and support
    the special needs of all current 1-Wire devices including the crypto iButton, EPROM-based Add-Only
    Memories, EEPROM devices and 1-Wire Thermometers.

    This class is thread-safe.
            
    @todo Provide assignment operator and copy constructor
    @todo Complete documentation of exceptions thrown.
        
    @author  Harald Roelle
    @author  Parts of the documentation by Dallas Semiconductors / Maxim Integrated Products
 */
class LOW_linkDS2480B : public LOW_link, public LOW_linkFlexibleSpeed {

//=======================================================================================
public: 

  //=====================================================================================
  //
  // type definitions
  //
  
  typedef enum { RXPOL_NORM=0x00, RXPOL_INV} RXPOL_val_t;   /**< RS232 RXD polarity */

    
  //=====================================================================================
  //
  // constructors
  //

  /** Constructor.

      Timings for flexible speed is configured to the recommeded optimal parameters
      as of app note #148.

      @param  inSerPortSpec       Serial port the DS2480B is connected to.
      @param  inRXPOL             Whether RX signal polarity is inverted to TX signal.
      @param  inHasExternalPower  Whether the attached bus supplies external power.
      @param  inAllowProgPulse    Whether the program pulse should be allowed.
   */
  LOW_linkDS2480B( const LOW_portSerialFactory::portSpecifier_t &inSerPortSpec, const RXPOL_val_t inRXPOL,
                   const bool inHasExternalPower, const bool inAllowProgPulse = false);
  
  /** Destructor.
   */
  virtual ~LOW_linkDS2480B();
  
  
  //=====================================================================================
  //
  // Standard methods required by LOW_link
  //
  
  virtual std::string getLinkFamily() const { return "DS2480B serial link"; };

  //! @name Bus touch (write/read) methods required by LOW_link
  //!@{
  virtual bool touchBit( const bool inSendBit, const strongPullup_t inPullup = pullUp_NONE);
  virtual uint8_t touchByte( const uint8_t inSendByte, const strongPullup_t inPullup = pullUp_NONE);
  virtual byteVec_t touchBlock( const byteVec_t &inBytes, const strongPullup_t inPullup = pullUp_NONE);
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

  //! @name Communication command bytes
  //!@{
  static const uint8_t  SerialSpeedAdapt_Cmd         = 0xc1;  /**< Communication command byte */
  static const uint8_t  SwitchToDataMode_Cmd         = 0xe1;  /**< Communication command byte */
  static const uint8_t  SwitchToCommandMode_Cmd      = 0xe3;  /**< Communication command byte */
  static const uint8_t  PulseTermination_Cmd         = 0xf1;  /**< Communication command byte */
  static const uint8_t  Reset_Cmd                    = 0xc1;  /**< Communication command byte */
  static const uint8_t  SearchAccel_Cmd              = 0xa1;  /**< Communication command byte */
  static const uint8_t  Pulse_Cmd                    = 0xe1;  /**< Communication command byte */
  static const uint8_t  SingleBit_Cmd                = 0x81;  /**< Communication command byte */
  //!@}

  //! @name Configuration command bytes
  //!@{  
  static const uint8_t  PullDownSlewRate_cfgCmd      = 0x01;  /**< Configuration command byte */
  static const uint8_t  ProgPulseDuration_cfgCmd     = 0x02;  /**< Configuration command byte */
  static const uint8_t  StrongPullupDuration_cfgCmd  = 0x03;  /**< Configuration command byte */
  static const uint8_t  Write1LowTime_cfgCmd         = 0x04;  /**< Configuration command byte */
  static const uint8_t  SampleOffsetWrite0Rec_cfgCmd = 0x05;  /**< Configuration command byte */
  static const uint8_t  LoadSensorThreshold_cfgCmd   = 0x06;  /**< Configuration command byte */
  static const uint8_t  RS232BaudRate_cfgCmd         = 0x07;  /**< Configuration command byte */
  //!@}
  
  //=====================================================================================
  //
  // type definitions
  //
    
  //! @name DS2480B specific configuration values.
  //!@{
  /** Type for wirespeed */
  typedef enum { /** 16 kbps */ normal_OWSPEED=0x00, flexible_OWSPEED, /** 142 kbps */ overdrive_OWSPEED } OWSPEED_val_t;
  /** Type for flexible speed pulldown slew rate in V/microsec */
  typedef enum { PDSR_15=0x00,    PDSR_2_2,  PDSR_1_65, PDSR_1_37, PDSR_1_1, PDSR_0_83, PDSR_0_7, PDSR_0_55} PDSR_val_t;
  /** Type for flexible speed write-1 low time in microsec */
  typedef enum { W1LT_8=0x00,     W1LT_9,    W1LT_10,   W1LT_11,   W1LT_12,  W1LT_13,   W1LT_14,  W1LT_15}   W1LT_val_t;
  /** Type for flexible speed DSO/ W0R time in microsec */
  typedef enum { SOW0RT_3=0x00,   SOW0RT_4,  SOW0RT_5,  SOW0RT_6,  SOW0RT_7, SOW0RT_8,  SOW0RT_9, SOW0RT_10} SOW0RT_val_t;
  /** Programming Pulse Duration values. */
  typedef enum { PPD_32=0x00,     PPD_64,    PPD_128,   PPD_256,   PPD_512,  PPD_1024,  PPD_2048, PPD_inf}   PPD_val_t;
  /** Strong Pullup Duration values. */
  typedef enum { SPUD_16_4=0x00,  SPUD_65_5, SPUD_131,  SPUD_262,  SPUD_524, SPUD_1048, SPUD_dyn, SPUD_inf}  SPUD_val_t;
  /** Load Sensor Threshold values. */
  typedef enum { LST_1_8=0x00,    LST_2_1,   LST_2_4,   LST_2_7,   LST_3_0,  LST_3_3,   LST_3_6,  LST_3_9}   LST_val_t;
  /** RS232 Baud Rate values.*/
  typedef enum { RBR_9_6=0x00,    RBR_19_2,  RBR_57_6,  RBR_115_2}                                           RBR_val_t;
  //!@}

  /** Bus status in reset command reply */
  typedef enum { oneWireShorted_busStat=0x00,
                 presencePulse_busStat,
                 alarmingPresencePulse_busStat,
                 noPresencePulse_busStat
               } busStatus_t;

  /** Reset command reply */
  typedef struct resetAnswer_t {
    busStatus_t       busStatus;
    bool              isVppPresent;
    uint8_t           chipRevision;
  } resetAnswer_t;

  /** Type for internal mode flag. */
  typedef enum { command_mode, data_mode} internalMode_t;

  
    
  //=====================================================================================
  //
  // attributes
  //
  
  LOW_portSerial         *serialPort;      /**< The serial port the adapter is attached to. */
  const RXPOL_val_t      receivePolarity;  /**< Polarity of RXD pin. */
  internalMode_t         internalMode;     /**< Mode (cmd/data) of DS2480B. */
    

        
  //=====================================================================================
  /**
      @name DS2480B communication commands
      The DS2480B supports four communication function commands: Reset, Single Bit, Pulse, and
      Search Accelerator control. The Reset, Search Accelerator Control and Single Bit commands
      include bits to select the 1-Wire communication speed (regular, flexible regular, Overdrive).
      Even if a command does not generate activity on the 1-Wire bus, these bits are latched
      inside the device and will take effect immediately.
  */
  //!@{

  /** Send Reset command.
      
      The Reset command must be used to begin all 1-Wire communication. The speed selection
      included in the command code immediately takes effect. The response byte includes a code
      for the reaction on the 1  Wire bus (bits 0 and 1) and a code for the chip revision
      (bits 2 to 4). If bit 5 of the response byte reads  1,  a programming voltage is present
      on the VPP pin, indicating that one may try programming EPROM devices.

      <pre>
                     | BIT 7 | BIT 6 | BIT 5     | BIT 4 | BIT 3 | BIT 2  | BIT 1 | BIT 0
      ===============+=======+=======+===========+=======+=======+========+=======+====================
      Send Reset     | 1     | 1     | 0         | 0     | 00 reg. speed  | 0     | 1
                     |       |       |           |       | 01 flex. speed |       |
                     |       |       |           |       | 10 OD. speed   |       |
                     |       |       |           |       | 11 reg. speed  |       |
      ---------------+-------+-------+-----------+-------+-------+--------+-------+--------------------
      Response Reset | 1     | 1     | 0 no Vpp  | chip revision          | 00 1-Wire shorted
                     |       |       | 1 Vpp     |                        | 01 presence pulse
                     |       |       |   present |                        | 10 alarming presence pulse
                     |       |       |           |                        | 11 no presence pulse
      </pre>
   */
  virtual void reset_cmd( resetAnswer_t *outAnswer);

  
  /** Send Single Bit command.
  
      The Single Bit command is used to generate a single time slot on the 1-Wire bus at the speed
      indicated by bits 2 and 3. The type of the time slot (Write-0 or Write-1) is determined by the
      logic value of bit 4. A Read Data time slot is identical to the Write-1 time slot. Bits 0 and 1
      of the response byte transmitted by the DS2480B at the end of the time slot reveal the value
      found on the 1-Wire bus when reading.

      For a time slot without a subsequent strong pull up, bit 1
      of the command must be set to 0. For a time slot immediately followed by a strong pullup bit 1
      must be set to 1. As soon as the strong pullup is over, the device will send a second response
      byte, code EFh (read 1) or ECh (read 0), depending on the value found on the 1 Wire bus when
      reading. The strong pullup directly following the single bit is used in conjunction with the
      crypto iButton.

      <pre>
                          | BIT 7 | BIT 6 | BIT 5 | BIT 4     | BIT 3 | BIT 2  | BIT 1           | BIT 0
      ====================+=======+=======+=======+===========+=======+========+=================+=======
      Send Single Bit     | 1     | 0     | 0     | 0=write 0 | 00 reg. speed  | 0 norm pullup   | 1
                          |       |       |       | 1=write 1 | 01 flex. speed | 1 strong pullup |
                          |       |       |       |           | 10 OD. speed   |   SECOND RESP.  |
                          |       |       |       |           | 11 reg. speed  |   BYTE FOLLOWS  |
      --------------------+-------+-------+-------+-----------+-------+--------+-----------------+-------
      Response Single Bit | 1     | 0     | 0     | same as sent               | 1-Wire read back
                          |       |       |       |                            | both bits same value
      --------------------+-------+-------+-------+-----------+-------+--------+-----------------+-------
      Second response Bit | 1     | 1     | 1     | 0         | 1     | 1      | 00 = read 0
       when pullup        |       |       |       |           |       |        | 11 = read 1
      </pre>
   */
  virtual bool singleBit_cmd( const bool inBitVal, const bool inStrongPullup = false);

  /** Send Pulse command.
  
      The Pulse command serves several functions that are selected by the contents of bit 1 and
      bit 4 of the command code. The main functions are generating a strong pullup to 5V and
      generating 12V programming pulses for EPROM devices (if the 12V are available at the VPP pin).
      The secondary function of the pulse command is arming and disarming a strong pullup after every
      subsequent byte in Data Mode.

      The arm/disarm function is controlled by bit 1 of the command code. Bit 4 determines whether
      the device will generate a strong pullup to 5V or a 12V programming pulse. The table below
      summarizes these options.

      <pre>
      BIT 4 | BIT 1 | FUNCTION
      ======+=======+==================================
       0    | 0     | strong pullup to 5V and disarm
      ------+-------+----------------------------------
       1    | 0     | 12V programming pulse and disarm
      ------+-------+----------------------------------
       0    | 1     | strong pullup to 5V and arm
      ------+-------+----------------------------------
       1    | 1     | 12V programming pulse and arm
      ------+-------+----------------------------------
      </pre>
      
      The strong pullup to 5V is required to program EEPROM devices or to operate special function
      devices that require a higher current for a limited time after having received a  go and convert
      command. Therefore and because it significantly reduces the effective data throughput on the
      1-wire bus, the strong pullup is disarmed most of the time. Although arming or disarming is
      simultaneously possible while generating a programming pulse, this is not recommended since
      it is likely to destroy the DS2480B if non EPROM devices are connected to the 1 Wire bus.

      The duration of the strong pullup or programming pulse is determined by configuration parameters
      and ranges from a few microseconds over  dynamic  duration (strong pullup only) up to unlimited
      (see section Configuration Commands). However, unlimited duration is not allowed in conjunction
      with arming the strong pullup after every byte. As long as the DS2480B is in Command Mode the
      host may terminate a strong pullup or programming pulse prematurely at any time by sending the
      command code F1h.

      The response byte is generated as soon as the strong pullup or programming pulse is over (either
      because the predefined time has elapsed, the high current demand is over, or due to termination
      by the host). The response byte mainly returns the command code as sent by the host, but the 2
      least significant bits are undefined.

      If the strong pullup is armed and the device is in Data Mode, the end of the strong pullup will
      be signaled as code F6h if the most significant bit of the preceding data byte on the 1 Wire bus
      is a 1 and 76h otherwise. The host will see this response byte in addition to the response on the
      data byte sent.
          
      <pre>
                     | BIT 7 | BIT 6 | BIT 5 | BIT 4              | BIT 3 | BIT 2  | BIT 1    | BIT 0
      ===============+=======+=======+=======+====================+=======+========+==========+=======
      Send Pulse     | 1     | 1     |1      | 0 5V strong pullup | 11 pulse       | 0 disarm | 1
                     |       |       |       | 1 12V prog. pulse  |                | 1 arm    |
      ---------------+-------+-------+-------+--------------------+-------+--------+----------+-------
      Response Pulse | 1     | 1     |1      | same as sent                        | undefined
      </pre>
   */
  virtual void pulse_cmd( const bool inArm, const bool inProgPulse = false, const bool inImmidiateTerm = false,
                          const unsigned int inSecTimeout = 5);

                          
  /** Generate a pulse by manually sleeping.

      @param  inSleepTime  If it's a strong pullup sleep time in ms,
                           if it's a program pulse sleep time in microseconds
      @param  inProgPulse  Whether it's a strong pullup or a program pulse.
   */
  virtual void pulse_cmd_manual( const unsigned long inSleepTime, const bool inProgPulse = false);

  
  /**
      The Search Accelerator Control command is used to set or reset the Search Accelerator control
      flag. Bit 4 of the command code contains the state to which the accelerator control flag is
      to be set. If the flag is set to a 1 (on) the device translates every byte received in Data Mode
      into a 12 bit sequence on the 1-Wire bus. Before activating the Search Accelerator, one must
      make sure that the strong pullup after every byte is disarmed (see Pulse Command). The Search
      Accelerator command does not generate a command response byte.

      Although the Search Accelerator Control command itself does not generate any 1-Wire activity,
      it can be used to select the communication speed on the 1-Wire bus. The speed selection
      (if different from the previous setting, e.g., from a Reset command) will take effect immediately.

      Command code description:
      <pre>
                    | BIT 7 | BIT 6 | BIT 5 | BIT 4              | BIT 3 | BIT 2  | BIT 1 | BIT 0
      ==============+=======+=======+=======+====================+=======+========+=======+=======
      Search Accel. | 1     | 0     | 1     | 0 accelerator off  | 00 reg. speed  | 0     | 1
      Control       |       |       |       | 1 = accelerator on | 01 flex. speed |       |
                    |       |       |       |                    | 10 OD. speed   |       |
                    |       |       |       |                    | 11 reg. speed  |       |
      </pre>
   */
  virtual void searchAccelCtrl_cmd( const bool inAccelOn);

  //@}


  //=====================================================================================
  /**
      @name DS2480B configuration commands
      The DS2480B is designed to be configurable for the varying requirements of its application.
      When the device powers up and/or performs a master reset cycle, the hard wired default
      configuration settings take effect. These settings will work on a short 1-Wire bus and assume
      regular 1-Wire communication speed. To change these default settings and to verify the current
      settings, the logic of the DS2480B supports configuration commands. A summary of the available
      configuration parameters, their default settings at regular and Overdrive speed and their
      applicability is shown in Table 3.

      Parameters not related to the communication speed on the 1-Wire bus specify the duration of the
      12V programming pulse, the duration of the strong pull up to 5V, the threshold current of the
      load sensor for  dynamic  strong pullup duration, and the baud rate on the interface that
      connects the DS2480B to the host.

      The remaining three parameters are used to modify the 1-Wire communication wave forms if one
      selects  Flexible Speed.

      Configuration parameter overview:
      <pre>
                                  |        | Confgrable at| Default
      Parameter Description       |Par.Code|Reg.|Flex|Ovrd| Reg/Flex| Overdrive
      ============================+========+====+====+====+=========+==========
      Pull-Down Slew Rate Control | 001  1 |    | X  |    | 15 V/탎 | 15 V/탎
      ----------------------------+--------+----+----+----+---------+----------
      Programming Pulse Duration  | 010  2 | X  | X  | X  | 512 탎  | 512 탎
      ----------------------------+--------+----+----+----+---------+----------
      Strong Pullup Duration      | 011  3 | X  | X  | X  | 524 ms  | 524 ms
      ----------------------------+--------+----+----+----+---------+----------
      Write-1 Low Time            | 100  4 |    | X  |    | 8 탎    | 1 탎
      ----------------------------+--------+----+----+----+---------+----------
      Sample Offset               | 101  5 |    | X  |    | 3 탎    | 1 탎 
        and Write 0 Recovery Time |        |    |    |    | 3 탎    | 3 탎
      ----------------------------+--------+----+----+----+---------+----------
      Load Sensor Threshold       | 110  6 | X  | X  | X  | 3 mA    | 3 mA
      ----------------------------+--------+----+----+----+---------+----------
      RS232 Baud Rate             | 111  7 | X  | X  | X  | 9.6 kbps| 9.6 kbps 
      </pre>

      Configuration parameter value codes:
      <pre>
                     |                  Value Codes                  |
      Parameter Code | 000 | 001 | 010 | 011 | 100 | 101 | 110 | 111 | Unit
      ===============+=====+=====+=====+=====+=====+=====+=====+=====+========
      001 (PDSR)     | 15  | 2.2 | 1.65| 1.37| 1.1 | 0.83| 0.7 | 0.55| V/탎
      ---------------+-----+-----+-----+-----+-----+-----+-----+-----+--------
      010 (PPD)      | 32  | 64  | 128 | 256 | 512 | 1024| 2048| inf.| 탎
      ---------------+-----+-----+-----+-----+-----+-----+-----+-----+--------
      011 (SPUD)     | 16.4| 65.5| 131 | 262 | 524 | 1048| dyn.| inf.| ms
      ---------------+-----+-----+-----+-----+-----+-----+-----+-----+--------
      100 (W1LT)     | 8   | 9   | 10  | 11  | 12  | 13  | 14  | 15  | 탎
      ---------------+-----+-----+-----+-----+-----+-----+-----+-----+--------
      101 (SOW0RT)   | 3   | 4   | 5   | 6   | 7   | 8   | 9   | 10  | 탎
      ---------------+-----+-----+-----+-----+-----+-----+-----+-----+--------
      110 (LST)      | 1.8 | 2.1 | 2.4 | 2.7 | 3.0 | 3.3 | 3.6 | 3.9 | mA
      ---------------+-----+-----+-----+-----+-----+-----+-----+-----+--------
      111 (RBR)      | 9.6 | 19.2| 57.6|115.2| 9.6 | 19.2| 57.6|115.2| kbits/s
      </pre>
  */
  //!@{


  /** Set the Pulldown Slew Rate Control.
      
      The numbers given Pulldown Slew Rate Control are nominal values. They may vary to
      some extent and are almost independent of the load on the 1-Wire bus.

      Whenever the DS2480B begins pulling the 1-Wire bus low to initiate a time slot,
      for example, it first turns off the weak pullup current IWEAKPU. Then, at regular
      and Overdrive speeds it will generate a falling edge at a slew rate of typically
      15V/ms. This value is acceptable for short 1-Wire busses and adequate for
      communication at Overdrive speed. For MicroLAN networks of more than roughly
      30 meters length one should always use flexible speed. One of the parameters that
      is adjustable at flexible speed is the slew rate of DS2480B initiated falling
      edges.

      As extensive tests have shown, MicroLAN networks at a length of up to 300 meters
      will perform best if the fall time tF is in the range of 4 � 0.5 ms. This translates
      into a slew rate of approximately 1V/ms. If the actual measured fall time is longer
      than the target value, one should use a value of 1.37 V/탎 or higher. If the fall
      time is shorter, one should use a value of 0.83 V/탎 or lower.

      <B>Note:</B> This is the raw command version. No checking if the parameter is
                   selectable in the current 1-Wire speed mode is done.
   */
  virtual void setPullDownSlewRate_cmd( const PDSR_val_t inPDSR);

  /** Get the Pulldown Slew Rate Control.
      See setPullDownSlewRate_cmd()
   */
  virtual PDSR_val_t getPullDownSlewRate_cmd();

  /** Set the Programming Pulse Duration.

      For parameter Programming Pulse Duration one may select indefinite duration. This value,
      however, should only be selected if one is not going to switch the device to Data Mode.
      As long as the device stays in Command Mode, any pulse function (programming or
      strong pullup) that uses one of these parameters can be terminated by sending the
      command code F1h. Termination is not possible if the device is in Data Mode.

      <B>Note:</B> This is the raw command version. No checking if the parameter is
                   selectable in the current 1-Wire speed mode is done.
   */
  virtual void setProgPulseDuration_cmd( const PPD_val_t inPPD);

  /** Get the Programming Pulse Duration.
      See setProgPulseDuration_cmd()
   */
  virtual PPD_val_t getProgPulseDuration_cmd();

  
  /** Set the Strong Pullup Duration.

      For parameter Strong Pullup Duration one may select indefinite duration. This value,
      however, should only be selected if one is not going to switch the device to Data Mode.
      As long as the device stays in Command Mode, any pulse function (programming or
      strong pullup) that uses one of these parameters can be terminated by sending the
      command code F1h. Termination is not possible if the device is in Data Mode.

      <B>Note:</B> This is the raw command version. No checking if the parameter is
                   selectable in the current 1-Wire speed mode is done.
   */
  virtual void setStrongPullupDuration_cmd( const SPUD_val_t inSPUD);

  /** Get the Strong Pullup Duration.
      See setProgPulseDuration_cmd()

      <B>Note:</B> This is the raw command version. No checking if the parameter is
                   selectable in the current 1-Wire speed mode is done.
   */
  virtual SPUD_val_t getStrongPullupDuration_cmd();


  /** Set the Write 1 Low Time.

      <B>Note:</B> This is the raw command version. No checking if the parameter is
                   selectable in the current 1-Wire speed mode is done.
   */
  virtual void setWrite1LowTime_cmd( const W1LT_val_t inW1LT);

  /** Get the Write 1 Low Time.
      See setWrite1LowTime_cmd()
   */
  virtual W1LT_val_t getWrite1LowTime_cmd();

  
  /** Set the Sample Offset / Write 0 Recovery time.

      <B>Note:</B> This is the raw command version. No checking if the parameter is
                   selectable in the current 1-Wire speed mode is done.
   */
  virtual void setSampleOffsetWrite0Rec_cmd( const SOW0RT_val_t inSOW0RT);

  /** Get the Sample Offset / Write 0 Recovery time.
      See setSampleOffsetWrite0Rec_cmd()
   */
  virtual SOW0RT_val_t getSampleOffsetWrite0Rec_cmd();


  /** Set the Load Sensor Threshold.

      Parameter Load Sensor Threshold has been implemented in order to more efficiently
      support the high current demand of the crypto iButton. The load sensor is only active
      if  "dynamic"  for the Strong Pullup Duration has been selected. The nominal and
      default value for the load sensor threshold is 3.0 mA with a tolerance band of
      -25% to +80%. The sensor threshold should be left at its default value; changes should
      only be made to compensate for tolerances. Dynamic duration should only be used when
      operating crypto iButtons and not for gang-programming 1-Wire EEPROMs or measuring
      temperature with multiple temperature sensors converting simultaneously.

      <B>Note:</B> This is the raw command version. No checking if the parameter is
                   selectable in the current 1-Wire speed mode is done.
  */
  virtual void setLoadSensorThreshold_cmd( const LST_val_t inLST);
  
  /** Set the Load Sensor Threshold.
      See setLoadSensorThreshold_cmd()
   */
  virtual LST_val_t getLoadSensorThreshold_cmd();

  
  /** Set the RS232 Baud Rate and RXD polarity.
  
      Parameter RS232 Baud Rate has two functions. It selects the baud rate and allows
      inversion of the signal at the RXD pin. Note that when changing the baud rate, the
      DS2480B will send the command response byte at the new data rate.

      <B>Note:</B> This is the raw command version. No checking if the parameter is
                   selectable in the current 1-Wire speed mode is done.
   */    
  virtual void setRS232BaudRate_cmd( const RBR_val_t inRBR, const RXPOL_val_t inRXPOL);

  /** Get the RS232 Baud Rate.
      See setRS232BaudRate_cmd()
   */
  virtual RBR_val_t getRS232BaudRate_cmd();

  /** Get the RS232 RXD polarity.
      See setRS232BaudRate_cmd()
   */ 
  virtual RXPOL_val_t getRS232RxPol_cmd();  

  //@}


  //=====================================================================================
  //!
  //! @name Internal methods 
  //!
  //@{  

  /** Write a config value to DS2480B and do sanity checking.
      <pre>
                               | BIT 7 | BIT 6 | BIT 5 | BIT 4 | BIT 3 | BIT 2 | BIT 1 |BIT 0
      =========================+=======+=======+=======+=======+=======+=======+=======+=====
      Send Write Parameter     | 0     |     parameter code    |  parameter value code | 1
      -------------------------+-------+-------+-------+-------+-------+-------+-------+-----
      Response Write Parameter | 0     |     same as sent      |     same as sent      | 0
      </pre>
      @param  inParamCode   Selection of configuration value to write.
      @param  inParamValue  Value to write.
      @throw comm_error  Thrown when sanity check fails.
   */
  virtual void writeConfigValue( const uint8_t inParamCode, const uint8_t inParamValue);

  
  /** Read a config value from DS2480B and do sanity checking.
      <pre>
                               | BIT 7 | BIT 6 | BIT 5 | BIT 4 | BIT 3 | BIT 2 | BIT 1 |BIT 0
      =========================+=======+=======+=======+=======+=======+=======+=======+=====
      Send Read Parameter      | 0     | 0     | 0     | 0     |    parameter code     | 1
      -------------------------+-------+-------+-------+-------+-------+-------+-------+-----
      Response Read Parameter  | 0     |     same as sent      |  parameter value code | 0
      </pre>
      
      @param  inParamCode  Selection of configuration value to read.
      @return Read config value.
      @throw comm_error  Thrown when sanity check fails.
   */
  virtual uint8_t readConfigValue( const uint8_t inParamCode);

  
  /** Switch between DS2480B data/command mode and vice versa.
      It is safe to call the method regardless of current state.
      @param  inMode  Desired DS2480 mode.
      @throw internal_error  Thrown on illegal transition (should never happen).
   */
  virtual void setMode( const internalMode_t inMode);

  //@}

    
  //=====================================================================================
  //!
  //! @name Enumeration type conversion methods
  //!
  //@{

  /** Convert value of type strongPullup_t to SPUD_val_t.
      @throw  internal_error  Thrown when conversion not possible (should not happen).
   */
  virtual const SPUD_val_t strongPullup_2_SPUD_val( const strongPullup_t inStrongPullup) const;

  /** Convert value of type progPulse_t to PPD_val_t.
      @throw  internal_error  Thrown when conversion not possible (should not happen).
   */
  virtual const LOW_linkDS2480B::PPD_val_t progPulse_2_PPD_val( const progPulse_t inProgPulse) const;

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
  
  //@}

};

#endif
