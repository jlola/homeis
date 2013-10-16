/***************************************************************************
                          LOW_component.h  -  description
                             -------------------
    begin                : Fri Jul 12 2002
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

#ifndef LOW_COMPONENT_H
#define LOW_COMPONENT_H


#include "LOW_exception.h"
#include "LOW_objectSynchronizer.h"


/** Abstract base class for 1-Wire components.

    Any component class representing a concrete 1-Wire based component
    must inherit from this class.

    This class is thread-safe.

    @todo Add multi-link lock for simultanously lock devices on different 1-Wire links.

    @author Harald Roelle
 */
class LOW_component {

//=======================================================================================
public: 

  //=====================================================================================
  //
  // exceptions
  //
 
  /** Exception base class for all exceptions thrown by LOW_component. */
  class_DERIVE_FROM_EXCEPTION( component_error, LOW_exception);
  
  
  //=====================================================================================
  //
  // constructor
  //

  /** Destructor.
   */
  virtual ~LOW_component();

  
//=======================================================================================
protected:

  //=====================================================================================
  //
  // constructor
  //

  /** Constructor.
   */
  LOW_component();

};

#endif
