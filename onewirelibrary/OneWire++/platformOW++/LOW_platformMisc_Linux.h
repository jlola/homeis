/***************************************************************************
                          LOW_platformMisc_Linux.h  -  description
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

#ifndef LOW_PLATFORMMISC_LINUX_H
#define LOW_PLATFORMMISC_LINUX_H


#include "LOW_platformMisc.h"


/** Platform specific miscelanious for Linux platforms.

    This class is thread-safe.

    @author Harald Roelle
 */
class LOW_platformMisc_Linux : public LOW_platformMisc {

//=======================================================================================
public: 

  //=====================================================================================
  //
  // constructors
  //

  /** Destructor.
    */
  virtual ~LOW_platformMisc_Linux();


  //=====================================================================================
  //
  // methods
  //
  
  void const p_secSleep( const unsigned int inSeconds) const;
  void const p_milliSleep( const unsigned long inMilliSeconds) const;
  void const p_microSleep( const unsigned long inMicroSeconds) const;
  void const p_nanoSleep( const unsigned long inNanoSeconds) const;
  void const p_nanoSleep( const unsigned int inSeconds, const unsigned long inNanoSeconds) const;

  void const p_getTimestamp( LOW_platformMisc::timestamp_t &outTimestamp) const;

  LOW_platformMiscFactory::threadIdent_t  const p_getThreadID() const;

  std::string const p_getHostname() const;

  
//=======================================================================================
protected:



  //=====================================================================================
  //
  // friend classes
  //

  friend class LOW_platformMiscFactory; /**< To allow construction. */


  //=====================================================================================
  //
  // constructors
  //

  /** Constructor.
      Not publicly constructable. Use LOW_platformMiscFactory.
   */
  LOW_platformMisc_Linux();

};

#endif
