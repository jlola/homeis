/***************************************************************************
                          LOW_platformMisc.h  -  description
                             -------------------
    begin                : Thu Aug 1 2002
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

#ifndef LOW_PLATFORMMISC_H
#define LOW_PLATFORMMISC_H


#include "OneWire++/LOW_exception.h"
#include "LOW_platformMiscFactory.h"


/** Abstract base class for simple opering system dependent functions.

    The methods are accessed via the public and static access functions.
    This class itself takes care of creating an appropiate instance.

    Specific platforms dereive their implementation classes from this class.

    The instances are created by LOW_platformMiscFactory, following the factory
    design pattern.

    <B>Note:</B> There is no prescribed constructor. A class deriving from this
                 one should only provide the default constructor.<BR>

    This class is thread-safe.

    @see LOW_platformMiscFactory

    @author Harald Roelle
 */
class LOW_platformMisc {

//=======================================================================================
public: 

  //=====================================================================================
  //
  // exceptions
  //

  /** Exception base class for all exceptions thrown by LOW_platformMisc. */
  class_DERIVE_FROM_EXCEPTION( platformMisc_error, LOW_exception);


  //=====================================================================================
  //
  // type definitions
  //

  /** Data structure for timestamps. */
  typedef struct timestamp_t {
    long sec;
    int  milSec;
  } timestamp_t;

  

  //=====================================================================================
  //
  // static methods
  //

  /** Sleep for a specified number of seconds.
      @param  inSeconds  Number of seconds to sleep.
   */
  static const void secSleep( const unsigned int inSeconds);

  /** Sleep for a specified number of milli seconds.
      @param  inMilliSeconds  Number of milli seconds to sleep.
   */
  static const void milliSleep( const unsigned long inMilliSeconds);

  /** Sleep for a specified number of micro seconds.
      @param  inMicroSeconds  Number of micro seconds to sleep.
   */
  static const void microSleep( const unsigned long inMicroSeconds);

  /** Sleep for a specified number of nano seconds.
      @param   inNanoSeconds  Number of nano seconds to sleep.
   */
  static const void nanoSleep( const unsigned long inNanoSeconds);

  /** Sleep for a specified number of nano seconds.
      @param  inSeconds      Number of seconds to sleep.
      @param  inNanoSeconds  Number of additional nano seconds to sleep.
   */
  static const void nanoSleep( const unsigned int inSeconds, const unsigned long inNanoSeconds);


  /** Get a timestamp.
      @param  outTimestamp   Where timestamp should be written to.
   */
  static const void getTimestamp( timestamp_t &outTimestamp);


  /** Calculate difference of two timestamps.
      @param  inT1       A timestamp.
      @param  inT2       A timestamp.
      @param  outResult  T1-T2.
   */
  static const void diffTimestamp( const timestamp_t &inT1, const timestamp_t &inT2, timestamp_t &outResult);


  /** Get identifier for current thread.
      @return  Unique identifier for current thread.
   */
  static const LOW_platformMiscFactory::threadIdent_t  getThreadID();


  /** Get the host's name.
      @return  Host's name.
   */
  static const std::string getHostname();
  
  
//=======================================================================================
protected: 

  //=====================================================================================
  //
  // methods
  //
  
  /** Sleep for a specified number of seconds.
      Virtual method to be implemented by OS specific subclasses.
      @param  inSeconds  Number of seconds to sleep.
   */
  virtual const void p_secSleep( const unsigned int inSeconds) const = 0;

  /** Sleep for a specified number of milli seconds.
      Virtual method to be implemented by OS specific subclasses.
      @param  inMilliSeconds  Number of milli seconds to sleep.
   */
  virtual const void p_milliSleep( const unsigned long inMilliSeconds) const = 0;

  /** Sleep for a specified number of micro seconds.
      Virtual method to be implemented by OS specific subclasses.
      @param  inMicroSeconds  Number of micro seconds to sleep.
   */
  virtual const void p_microSleep( const unsigned long inMicroSeconds) const = 0;

  /** Sleep for a specified number of nano seconds.
      Virtual method to be implemented by OS specific subclasses.
      @param   inNanoSeconds  Number of nano seconds to sleep.
   */
  virtual const void p_nanoSleep( const unsigned long inNanoSeconds) const = 0;

  /** Sleep for a specified number of nano seconds.
      Virtual method to be implemented by OS specific subclasses.
      @param  inSeconds      Number of seconds to sleep.
      @param  inNanoSeconds  Number of additional nano seconds to sleep.
   */
  virtual const void p_nanoSleep( const unsigned int inSeconds, const unsigned long inNanoSeconds) const = 0;
  

  /** Get a timestamp.
      Virtual method to be implemented by OS specific subclasses.
      @param  outTimestamp   Where timestamp should written to.
   */
  virtual const void p_getTimestamp( timestamp_t &outTimestamp) const = 0;


  /** Get identifier for current thread.
      Virtual method to be implemented by OS specific subclasses.
      @return  Unique identifier for current thread.
   */
  virtual const LOW_platformMiscFactory::threadIdent_t  p_getThreadID() const = 0;


  /** Get the host's name.
      Virtual method to be implemented by OS specific subclasses.
      @return  Host's name.
   */
  virtual const std::string p_getHostname() const = 0;


//=======================================================================================
private: 

  //=====================================================================================
  //
  // attributes
  //

  /** Runtime instance of platform specific implementation of this class.
      Having created this instance relieves the burden of explicitely
      creating/accessing an instance by users of methods in this class.
   */
  static const LOW_platformMisc  *runtimeInstance;
  
};    

#endif
