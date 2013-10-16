/***************************************************************************
                          LOW_linkFlexibleSpeed.h  -  description
                             -------------------
    begin                : Sat Oct 11 2003
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

#ifndef LOW_LINKFLEXIBLESPEED_H
#define LOW_LINKFLEXIBLESPEED_H


#include <vector>


#include "LOW_exception.h"


/** Mix-in class for 1-Wire link adapters capable of flexible speed.

    Any link class representing a concrete 1-Wire link adapter can inherit from this class.

    Values are stored by the methods in this class. For covenience they may called by
    overriding methods. See also description of individual methods.

    Note that deriving classes should also override the get*() methods to reflect actual
    values of the device.
    
    <b>Note:</b> This class is intentionally NOT thread-safe. This has to be accomplished
    by deriving classes
            
    @author Harald Roelle
 */
class LOW_linkFlexibleSpeed {

//=======================================================================================
public:


  //=====================================================================================
  //
  // exceptions
  //

  /** Exception base class for all exceptions thrown by LOW_link. */
  class_DERIVE_FROM_EXCEPTION( incompatibleSpeed_error, LOW_exception);


  
  //=====================================================================================
  //
  // type definitions
  //

  typedef std::vector<LOW_linkFlexibleSpeed*> flexLinkPtrVec_t;   /**< Vector type of link class pointers. */

  /** 1-Wire communication speeds. */
  typedef enum { normal_speed=0,    /**< Normal 1-Wire speed, 16 kbps. */
                 flexible_speed=1,  /**< Flexible 1-Wire speed. */
                 overdrive_speed=2  /**< Overdrive 1-Wire speed, 142 kbps. */
               } wireSpeed_t;

  //! @name Configuration values.
  //!@{
    
  /** Type for flexible speed pulldown slew rate in V/microsec. */
  typedef enum { pdSlewRate_15=0x00,  /**< Pulldown slew rate 15    V/microsec. */
                 pdSlewRate_2_2,      /**< Pulldown slew rate  2.2  V/microsec. */
                 pdSlewRate_1_65,     /**< Pulldown slew rate  1.65 V/microsec. */
                 pdSlewRate_1_37,     /**< Pulldown slew rate  1.37 V/microsec. */
                 pdSlewRate_1_1,      /**< Pulldown slew rate  1.1  V/microsec. */
                 pdSlewRate_0_83,     /**< Pulldown slew rate  0.83 V/microsec. */
                 pdSlewRate_0_7,      /**< Pulldown slew rate  0.7  V/microsec. */
                 pdSlewRate_0_55      /**< Pulldown slew rate  0.55 V/microsec. */
               } pdSlewRate_t;

  /** Type for flexible speed write-1 low time in microsec. */
  typedef enum { w1LowTime_8=0x00,  /**< Write-1 low time  8 microsec. */
                 w1LowTime_9,       /**< Write-1 low time  9 microsec. */
                 w1LowTime_10,      /**< Write-1 low time 10 microsec. */
                 w1LowTime_11,      /**< Write-1 low time 11 microsec. */
                 w1LowTime_12,      /**< Write-1 low time 12 microsec. */
                 w1LowTime_13,      /**< Write-1 low time 13 microsec. */
                 w1LowTime_14,      /**< Write-1 low time 14 microsec. */
                 w1LowTime_15       /**< Write-1 low time 15 microsec. */
               } w1LowTime_t;

  /** Type for flexible speed DSO/ W0R time in microsec. */
  typedef enum { soW0RecTime_3=0x00,  /**< DSO/w0R time  3 microsec. */
                 soW0RecTime_4,       /**< DSO/w0R time  4 microsec. */
                 soW0RecTime_5,       /**< DSO/w0R time  5 microsec. */
                 soW0RecTime_6,       /**< DSO/w0R time  6 microsec. */
                 soW0RecTime_7,       /**< DSO/w0R time  7 microsec. */
                 soW0RecTime_8,       /**< DSO/w0R time  8 microsec. */
                 soW0RecTime_9,       /**< DSO/w0R time  9 microsec. */
                 soW0RecTime_10       /**< DSO/w0R time 10 microsec. */
               } soW0RecTime_t;

  //!@}


  
  //=====================================================================================
  //
  // methods
  //

  /** Set the 1-Wire speed mode.
   */
  virtual void setWireSpeed( const wireSpeed_t inWireSpeed);

  /** Get the 1-Wire speed mode.
   */
  virtual wireSpeed_t getWireSpeed() = 0;

  
  /** Set the Pulldown Slew Rate Control.
      Current speed checking and remembering the value is done by provided implementation.
      @throw  incompatibleSpeed_error  Thrown when not in flexible speed mode.
   */
  virtual void setPullDownSlewRate( const pdSlewRate_t inPDSR);

  /** Get the Pulldown Slew Rate Control.
      Current speed checking is done by provided implementation.

      <b>Note:</b>Implementations should NOT simply return the value saved in
                  this class, but read the actual values from the device.
      
      @throw  incompatibleSpeed_error  Thrown when not in flexible speed mode.
   */
  virtual pdSlewRate_t getPullDownSlewRate();


  /** Set the Write 1 Low Time.
      Current speed checking and remembering the value is done by provided implementation.
      @throw  incompatibleSpeed_error  Thrown when not in flexible speed mode.
   */
  virtual void setWrite1LowTime( const w1LowTime_t inW1LT);

  /** Get the Write 1 Low Time.
      Current speed checking is done by provided implementation.

      <b>Note:</b>Implementations should NOT simply return the value saved in
                  this class, but read the actual values from the device.

      @throw  incompatibleSpeed_error  Thrown when not in flexible speed mode.
   */
  virtual w1LowTime_t getWrite1LowTime();


  /** Set the Sample Offset / Write 0 Recovery time.
      Current speed checking and remembering the value is done by provided implementation.
      @throw  incompatibleSpeed_error  Thrown when not in flexible speed mode.
   */
  virtual void setSampleOffsetWrite0Rec( const soW0RecTime_t inSOW0RT);

  /** Get the Sample Offset / Write 0 Recovery time.
      Current speed checking is done by provided implementation.

      <b>Note:</b>Implementations should NOT simply return the value saved in
                  this class, but read the actual values from the device.

      @throw  incompatibleSpeed_error  Thrown when not in flexible speed mode.
   */
  virtual soW0RecTime_t getSampleOffsetWrite0Rec();


  
//=======================================================================================
protected:
  
  //=====================================================================================
  //
  // attributes
  //
  
  wireSpeed_t    wireSpeed;    /**< Saved value of wire speed. */
  pdSlewRate_t   pdSlewRate;   /**< Saved value of pulldown slew rate. */
  w1LowTime_t    w1LowTime;    /**< Saved value of write-1 low time. */
  soW0RecTime_t  soW0RecTime;  /**< Saved value of DSO/W0R time. */


  
  //=====================================================================================
  //
  // constructors
  //

  /** Constructor.
      Protected because should only be callable from deriving classes.

      @param  inWireSpeed    Initial value for 1-Wire speed.
      @param  inPdSlewRate   Initial value for pulldown slew rate.
      @param  inW1LowTime    Initial value for write-1 low time.
      @param  inSoW0RecTime  Initial value for DSO/W0R time.
   */
  LOW_linkFlexibleSpeed( const wireSpeed_t inWireSpeed, const pdSlewRate_t inPdSlewRate,
                         const w1LowTime_t inW1LowTime, const soW0RecTime_t inSoW0RecTime);


  /** Destructor.
   */
  virtual ~LOW_linkFlexibleSpeed();

};

#endif
