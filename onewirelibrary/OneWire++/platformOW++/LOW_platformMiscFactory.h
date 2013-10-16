/***************************************************************************
                          LOW_platformMiscFactory.h  -  description
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

#ifndef LOW_PLATFORMMISCFACTORY_H
#define LOW_PLATFORMMISCFACTORY_H


#ifdef __linux__
  #include <sys/types.h>
#endif


class LOW_platformMisc; // forward declaration to avoid circular reference


/** Factory class for platform specific objects of LOW_platformMisc.

    This class also contains platform specific type definitions.

    This class is thread-safe.

    @see LOW_platformMisc

    @author Harald Roelle
 */
class LOW_platformMiscFactory {

//=======================================================================================
public: 

  //=====================================================================================
  //
  // type definitions
  //
  
#ifdef __linux__
  /** Linux specific type for thread identifier. */
  typedef pid_t   threadIdent_t;
#endif
  
  //=====================================================================================
  //
  // static factory
  //
  
  /** Platform aware replacement for creating dynamic instances of LOW_platformMisc.
      @return  Pointer to new instance of LOW_platformMisc.
   */
  static LOW_platformMisc* new_platformMisc();


//=======================================================================================
private:

  //=====================================================================================
  //
  // constructors
  //

  /** Constructor.
      Private to prevent instaciation as this is a static factory.
   */
  LOW_platformMiscFactory();

  /** Destructor.
      Private to prevent instaciation as this is a static factory.
   */
  virtual ~LOW_platformMiscFactory();

};

#endif
