/***************************************************************************
                          LOW_portSerialFactory.h  -  description
                             -------------------
    begin                : Tue Jul 30 2002
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

#ifndef LOW_PORTSERIALFACTORY_H
#define LOW_PORTSERIALFACTORY_H


#include <string>


#include "LOW_portSerial.h"


/** Factory class for platform specific objects of LOW_portSerial.

    This class also contains platform specific type definitions.

    This class is thread-safe.

    @see LOW_portSerial

    @author Harald Roelle
 */
class LOW_portSerialFactory {

//=======================================================================================
public:

  //=====================================================================================
  //
  // type definitions
  //

#ifdef __linux__
  /** Linux specific type for serial port unique identifier. */
  typedef std::string   portSpecifier_t;
#endif

  //=====================================================================================
  //
  // static factory
  //

  /** Platform aware replacement for creating dynamic instances of LOW_portSerial.
      @param inPortSpec  Unique identifier for new serial port instance.
      @return  Pointer to new instance of LOW_portSerial.
   */
  static LOW_portSerial* new_portSerial( const portSpecifier_t inPortSpec);


//=======================================================================================
private:

  //=====================================================================================
  //
  // constructors
  //

  /** Constructor.
      Private to prevent instaciation as this is a static factory.
   */
  LOW_portSerialFactory();

  /** Destructor.
      Private to prevent instaciation as this is a static factory.
   */
  virtual ~LOW_portSerialFactory();

};

#endif
