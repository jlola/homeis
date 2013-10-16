/***************************************************************************
                          LOW_objectIDFactory.h  -  description
                             -------------------
    begin                : Sun Oct 26 2003
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

#ifndef LOW_OBJECTIDFACTORY_H
#define LOW_OBJECTIDFACTORY_H


#include "LOW_types.h"
#include "LOW_objectSynchronizer.h"


/** Factory class for individual object IDs.

    This class is thread-safe.

    @author Harald Roelle
 */
class LOW_objectIDFactory {

//=======================================================================================
public: 

  //=====================================================================================
  //
  // type definitions
  //

  typedef uint32_t objectID_t;  /**< object ID type */


  //=====================================================================================
  //
  // static methods
  //

  /** Get a new unique object ID.
      To be precise, the ID is pseudo unique as a simple counter increment is used.
      @return A new unique object ID.
   */
  static objectID_t getNewObjectID();


  
//=======================================================================================
protected:

  //=====================================================================================
  //
  // static attributes
  //

  __LOW_SYNCHRONIZE_DEFINE_PROTECTED_LOCK__

  

//=======================================================================================
private:

  //=====================================================================================
  //
  // constructors
  //

  /** Default constructor.
      It is private to prevent creating objects from this class as
      this is a static factory.
   */
  LOW_objectIDFactory();

  /** Destructor.
      It is private to prevent creating objects from this class as
      this is a static factory.
   */
  ~LOW_objectIDFactory();


  //=====================================================================================
  //
  // static attributes
  //

  /** Counter to get unique object IDs, incremented on instance creation to get individual IDs. */
  static objectID_t objIDCounter;
};

#endif
