/***************************************************************************
                          LOW_link.h  -  description
                             -------------------
    begin                : Sun Jul 7 2002
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

#ifndef LOW_LINK_H
#define LOW_LINK_H


#include <vector>


#include "LOW_types.h"
#include "LOW_exception.h"
#include "LOW_deviceID.h"
#include "LOW_objectIDFactory.h"



/** Abstract base class for 1-Wire link adapters.

    Any link class representing a concrete 1-Wire link adapter must inherit from this class.

    The class provides some generic implementations relying only on touch*() methods.
    Although beeing sufficient for basic functionality, deriving classes are invited to
    overload those methods, especially in case the hardware provides a native implementation.
    Prominent examples for that:

      - LOW_linkDS2480B: doSearchSequence() uses the built-in "search accelerator"

      - LOW_linkDS2490: searchDevices() uses the built-in "SearchAccess" command.

    This class is thread-safe.

    @todo Provide assignment operator and copy constructor
    @todo Provide strongPullup method with specialized type
    
    @author  Harald Roelle
    @author  Parts of the documentation by Dallas Semiconductors / Maxim Integrated Products
 */
class LOW_link {

//=======================================================================================
public:
  
  //=====================================================================================
  //
  // exceptions
  //
 
  /** Exception base class for all exceptions thrown by LOW_link. */
  class_DERIVE_FROM_EXCEPTION( link_error, LOW_exception);

  /** Exception class to be thrown on communication errors. */
  class_DERIVE_FROM_EXCEPTION( comm_error,         link_error);
  /** Exception class to be thrown on pure internal errors. */
  class_DERIVE_FROM_EXCEPTION( internal_error,     link_error);
  /** Exception class to be thrown on errors related to currently selected wire speed. */
  class_DERIVE_FROM_EXCEPTION( illegalSpeed_error, link_error);
  /** Exception class to be thrown on errors related to requested voltage level. */
  class_DERIVE_FROM_EXCEPTION( illegalLevel_error, link_error);
  /** Exception class to be thrown on access violation errors. */
  class_DERIVE_FROM_EXCEPTION( notAllowed_error,   link_error);
  class_DERIVE_FROM_EXCEPTION( sizeMismatch_error, link_error);

    
  //=====================================================================================
  //
  // locks
  //
  
  /** Locking class to ensure exclusive access to a link.

      The class is intended to be used in a "locking is creation" design pattern.
      On creation an exclusive lock is optained for the device, and on destruction
      the lock is released.

      Uses a recursive mutex, i.e. does not distinguish reads/writes but my be called
      multiple times by the same thread without blocking.

      Class is inlined for performance reasons.
      
      @see LOW_thread_mutex
   */
  class commLock {
    public:
      /** Obtain the lock.
          @param inLink  Reference to the link the lock is for.
       */
      inline commLock( LOW_link &inLink) : link( inLink)
      {
        link.__linkRecMutex->lock();
      };

      /** Release the lock.
       */
      inline ~commLock() 
      {
        link.__linkRecMutex->unlock();
      };

    private:
      LOW_link  &link;  /**< Reference to the link the lock is for. */
  };

  
  //=====================================================================================
  //
  // type definitions
  //

  typedef std::vector<LOW_link*> linkPtrVec_t;   /**< Vector type of link class pointers. */
  
  typedef LOW_objectIDFactory::objectID_t linkID_t;                     /**< Type for individual link ID number. */

  /** Type for strong pullup period specification.
      Period may vary a little, depending on actual device. */
  typedef enum { pullUp_16_4=0x00,  /**<   16 /   16.4 ms */
                 pullUp_65_5,       /**<   64 /   65.5 ms */
                 pullUp_131,        /**<  128 /  131   ms */
                 pullUp_262,        /**<  256 /  262   ms */
                 pullUp_524,        /**<  512 /  524   ms */
                 pullUp_1048,       /**< 1024 / 1048   ms */
                 pullUp_NONE=0xff   /**< no pullup */
               }  strongPullup_t;

  /** Type for 12V program pulse period specification.
      Period may vary a little, depending on actual device. */
  typedef enum { progPulse_32=0x00, /**<   32 microsec */
                 progPulse_64,      /**<   64 microsec */
                 progPulse_128,     /**<  128 microsec */
                 progPulse_256,     /**<  256 microsec */
                 progPulse_512,     /**<  512 microsec */
                 progPulse_1024,    /**< 1024 microsec */
                 progPulse_2048     /**< 2032 / 2048 microsec */
               } progPulse_t;


  //=====================================================================================
  //
  // constructors
  //

  /** Destructor.
   */ 
  virtual ~LOW_link();

   
  //=====================================================================================
  //
  // operator overloading
  //
  
  virtual bool operator==(LOW_link &inLink) const;  /**< Comparison based on linkID. */
  
 
  //=====================================================================================
  /**
      @name Bus touch (write/read) methods
   */
  //!@{
 
  /** Send 1 bit of communication to the 1-Wire net and return the
      result 1 bit read from the 1-Wire net.

      @param   inSendBit   Bit to send.
      @param   inPullup    Optional strong pullup time following the write/read cycle.
      
      @return  Bit that was reveived.
   */
  virtual bool touchBit( const bool inSendBit, const strongPullup_t inPullup = pullUp_NONE) = 0;
 
 
  /** Send 8 bits of communication to the 1-Wire net and return the
      result 8 bits read from the 1-Wire net. 

      @param   inSendByte  Byte to send.
      @param   inPullup    Optional strong pullup time following the 8 bit write/read cycle.
      
      @return  Byte that was reveived.
   */
  virtual uint8_t touchByte( const uint8_t inSendByte, const strongPullup_t inPullup = pullUp_NONE) = 0;
  
  
  /** Send block of communication to the 1-Wire net and return the
      resulting bytes read from the 1-Wire net. 

      A default implementation relying on the touchByte() method is provided.

      <B>Note:</B> When the strong pullup is selected it will NOT appear after each byte
                   sent, only after the last byte the pullup is done!

      @param   inBytes     Byte block to send.
      @param   inPullup    Optional strong pullup time following each 8 bit write/read cycle.
      
      @return  Byte block that was reveived. Length is equal to number of sent bytes.
   */
  virtual byteVec_t touchBlock( const byteVec_t &inBytes, const strongPullup_t inPullup = pullUp_NONE);

  //!@}
  
  
  //=====================================================================================
  /**
      @name Bus read methods
   */
  //!@{
  
  /** Receive 1 bit from the 1-Wire net by previously sending one bit of 
      read communication to the 1-Wire net.

      A default implementation relying on the touch methods is provided.

      @param   inPullup    Optional strong pullup time following the write/read cycle.
      
      @return  Bit that was reveived.
   */
  virtual bool readDataBit( const strongPullup_t inPullup = pullUp_NONE);

    
  /** Receive 1 byte from the 1-Wire net by previously sending
      8 bits of read communication to the 1-Wire net.

      A default implementation relying on the touch methods is provided.

      @param   inPullup    Optional strong pullup time following the write/read cycle.
      
      @return  Byte that was reveived.
   */
  virtual uint8_t readDataByte( const strongPullup_t inPullup = pullUp_NONE);

    
  /** Receive a block of bytes from the 1-Wire net by previously sending
      a block of bytes of read communication to the 1-Wire Net.

      <B>Note:</B> When the strong pullup is selected it will NOT appear after each byte
                   sent, only after the last byte the pullup is done! 
      
      A default implementation relying on the touch methods is provided.

      @param  outBytes   Values that were reveived. Read length is determined
                         by the preset length of the vector.
      @param  inPullup   Optional strong pullup time following each 8 bit write/read cycle.
   */
  virtual void readData( byteVec_t &outBytes, const strongPullup_t inPullup = pullUp_NONE);

  //!@}

    
  //=====================================================================================
  /**
      @name Bus write methods
   */
  //!@{
  
  /** Send 1 bit to the 1-Wire net and verify that the
      bit read from the 1-Wire net is the same (bus write operation).

      A default implementation relying on the touch methods is provided.

      @param   inSendBit   Bit to send.
      @param   inPullup    Optional strong pullup time following the write/read cycle.
   */
  virtual void writeData( const bool inSendBit, const strongPullup_t inPullup = pullUp_NONE);

    
  /** Send 1 byte to the 1-Wire net and verify that the
      byte read from the 1-Wire net is the same (bus write operation).

      A default implementation relying on the touch methods is provided.

      @param   inSendByte  Byte to send.
      @param   inPullup    Optional strong pullup time following the write/read cycle.
   */
  virtual void writeData( const uint8_t inSendByte, const strongPullup_t inPullup = pullUp_NONE);

    
  /** Send block of bytes to the 1-Wire net and verify that the
      bytes block read from the 1-Wire net are the same (bus write operation).

      A default implementation relying on the touch methods is provided.

      <B>Note:</B> When the strong pullup is selected it will NOT appear after each byte
                   sent, only after the last byte the pullup is done!

      @param  inSendBytes  Block of bytes to send.
      @param  inPullup     Optional strong pullup time following each 8 bit write/read cycle.
   */
  virtual void writeData( const byteVec_t &inSendBytes, const strongPullup_t inPullup = pullUp_NONE);

  //!@}

    
  //=====================================================================================
  /**
      @name Higher level actions
   */
  //!@{

  /** Execute a complete search sequence from preloaded ID vector.

      Search is executed on the currently active bus configuration. No branch selection
      is done in advance.

      A default implementation relying on the doSearchSequence() method is provided.

      @param  inOnlyAlarm  Select normal search or alarmin only search.
      @param  inPreload    The ID vector to start the search from.
      @param  inFamCode    Narrow search to given family code. To not narrow use LOW_device::anyDev_famCode.
      @return  List of devices found on the currently active bus configuration.
   */
  virtual LOW_deviceID::deviceIDVec_t searchDevices( const bool inOnlyAlarm, const LOW_deviceIDRaw inPreload,
                                                     const LOW_deviceIDRaw::devFamCode_t inFamCode, const bool inDoReset);

  
  //!@}

  
  //=====================================================================================
  /**
      @name Misc methods
   */
  //!@{

  /** Get ID of the link.

      @return  ID of the link.
   */
  linkID_t getID() const;


  /** Get the link's family type.
      <B>Note:</B> Subclasses must implement this method to return a clear text
                   name of their kind.
      @return Family type name of the link.
   */
  virtual std::string getLinkFamily() const = 0;


  /** Get wether the adapter is capable of 12V Program pulse.

      @return  Wether the adapter is capable of 12V Program pulse.
  */
  bool getHasProgramPulse() const;


  /** Get whether there is an external power line on the segment.

      @return  Whether there is an external power line on the segment.
  */
  bool getHasExternalPower() const;


  /** Get wether the program pulse should be allowed.

      @return  Wether the program pulse should be allowed.
  */
  bool getAllowProgPulse() const;


  /** Reset the adapter.
      
      <B>Note:</B> This does not necessarily include a reset on the 1-Wire net.
                   Whether this is done or net is left to the concrete implementation. 
   */
  virtual void resetLinkAdapter() = 0;
  
  
  /** Reset all of the devices on the 1-Wire net.

      @return  true:  Presense pulse(s) detected, device(s) reset
               false: No presense pulses detected

      @todo Differentiate simple presence pulse and alarming presence pulse.
   */
  virtual bool resetBus() = 0;
  
  
  /** Set the 1-Wire net line level to strong pullup for a specified time.

      <B>Note:</B> Dependening on the implementation, time may only as accurate
                   as the operating system permits (scheduling!).
  
      @param    inMicroSecs    Pullup time in micro seconds.
   */
  virtual void strongPullup( const unsigned long inMicroSecs) = 0;

  
  /** Set the 1-Wire net line level to strong pullup for a specified time.

      <B>Note:</B> Dependening on the implementation, time may only as accurate
                   as the operating system permits (scheduling!).

      @param    inPullupTime   Pullup time by predefined type.
   */
  virtual void strongPullup( const strongPullup_t inPullupTime) = 0;

  
  /** Create a 12 volt pulse on the 1-Wire net for programming EPROM devices.

      For EPROM programming, only a single slave device should be connected
      to the 1-Wire bus and the cable must be short, not to exceed a few meters.

      <B>Note:</B> One should not attempt generating a programming pulse with
                   a non-EPROM device on the bus; this may damage the device
                   as well as the link controller.

      <B>Note:</B> Dependening on the implementation, time may only as accurate
                   as the operating system permits (scheduling!).

      @param    inMicroSecs    Pulse time in micro seconds.
   */
  virtual void programPulse( const unsigned long inMicroSecs) = 0;


  /** Create a 12 volt pulse on the 1-Wire net for programming EPROM devices.

      For EPROM programming, only a single slave device should be connected
      to the 1-Wire bus and the cable must be short, not to exceed a few meters.

      <B>Note:</B> One should not attempt generating a programming pulse with
                   a non-EPROM device on the bus; this may damage the device
                   as well as the link controller.

      <B>Note:</B> Dependening on the implementation, time may only as accurate
                   as the operating system permits (scheduling!).

      @param    inPulseTime   Pulse time by predefined type.
   */
  virtual void programPulse( const progPulse_t inPulseTime) = 0;


  //!@}


            
//=======================================================================================
protected:

  //=====================================================================================
  //
  // attributes
  //
  
  const linkID_t    linkID;            /**< Individual ID of the link adapter. */
  bool              hasProgramPulse;   /**< Wether the adapter is capable of 12V Program pulse. */
  const bool        hasExternalPower;  /**< Wether the attached bus supplies external power. */
  const bool        allowProgPulse;    /**< Wether the program pulse should be allowed. */

    
  //=====================================================================================
  //
  // constructors
  //
 
  /** Constructor.

      @param  inHasProgramPulse   Whether the adapter is capable of 12V Program pulse.
      @param  inHasExternalPower  Whether the attached bus supplies external power.
      @param  inAllowProgPulse    Whether the program pulse should be allowed.
   */
  LOW_link( const bool inHasProgramPulse, const bool inHasExternalPower, const bool inAllowProgPulse = false);

    
  //=====================================================================================
  //
  // methods
  //
 
  /** Execute a single search sequence on the bus.

      A default implementation relying on bit i/o methods is provided.

      @param  inBranchVector   ID vector to start the search sequence.
      @param  outFoundID       Device ID found.
      @param  outDiscrVec      Discrepancy vector after the search.
   */  
  virtual void doSearchSequence( const LOW_deviceIDRaw &inBranchVector, 
                                 LOW_deviceIDRaw &outFoundID, LOW_deviceIDRaw &outDiscrVec);

  
//=======================================================================================
private:

  //=====================================================================================
  //
  // friends
  //
  
  friend class commLock;  /**< Needed to access the mutex. */
  
  
  //=====================================================================================
  //
  // attributes
  //
  
  LOW_thread_mutex  *__linkRecMutex;  /**< Mutex for exclusive access. */

};
  

#endif
