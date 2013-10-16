/***************************************************************************
                          LOW_deviceFactory.h  -  description
                             -------------------
    begin                : Mon Aug 5 2002
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

#ifndef LOW_DEVICEFACTORY_H
#define LOW_DEVICEFACTORY_H


#include "LOW_exception.h"
#include "LOW_device.h"
#include "LOW_deviceIDRaw.h"


/** Factory class for creating instances of classes derived from LOW_device.

    Any class derived from LOW_device must register with this factory.
    After that, this factory can create "the right" instance of LOW_device subclasses
    on the fly.

    To learn how to statically register at program initialization, see an already implemented
    subclass, e.g. LOW_devDS1820.

    @author Harald Roelle
 */
class LOW_deviceFactory {

//=======================================================================================
public:
  
  //=====================================================================================
  //
  // exceptions
  //
 
  /** Exception base class for all exceptions thrown by LOW_deviceFactory. */
  class_DERIVE_FROM_EXCEPTION( deviceFactory_error, LOW_exception);


  //=====================================================================================
  //
  // type definitions
  //

  /** Function type for static pseudo constructors. */
  typedef LOW_device* (*newInstanceFkt_t)( LOW_netSegment&, const LOW_deviceID&);
     
  
  //=====================================================================================
  //
  // static methods
  //

  /** Create new dynamic instance of specific device class.
      The created instance is disposable with the "delete" operator.
    
      @param  inSegment  Reference to network segment the device is on.
      @param  inDevID    Reference to device's ID.
      @return  New dynamic instance of specific device class.
      @throw  deviceFactory_error  Thrown when device type is not registered.
   */   
  static LOW_device* new_SpecificDevice( LOW_netSegment &inSegment, const LOW_deviceID &inDevID);

  
  /** Register a static pseudo constructor.

      @param  inFamCode      Family code of the specific class.
      @param  inPseudoCnstr  Static pseudo constructor to register.
      @throw  deviceFactory_error  Thrown when family type is already registered.
   */
  static void registerSpecificCtor( const LOW_deviceIDRaw::devFamCode_t inFamCode, newInstanceFkt_t inPseudoCnstr);

  
        
//=======================================================================================
private:
  
  //=====================================================================================
  //
  // type definitions
  //

  /** Map type for registered family types and constructors. */
  typedef std::map< LOW_deviceIDRaw::devFamCode_t, newInstanceFkt_t>  ctorFktMap_t;
  
  
  //=====================================================================================
  //
  // static attributes
  //

  /** Map holding registered family types and associated constructors.
      <B>Note:</B> Singleton design pattern is important here, because order of static
                   initialization produced by the compiler cannot be assured.
   */
  static ctorFktMap_t  *deviceCtorsSingleton;

  /** Pseudo constructor for unknown device types.
      This constructor is used when no specific constructor is found.
   */
  static newInstanceFkt_t unknownDevCtor;


  //=====================================================================================
  //
  // constructors
  //

  /** Constructor.
      Private to prevent instaciation as this is a static factory.
   */
  LOW_deviceFactory();

  /** Destructor.
      Private to prevent instaciation as this is a static factory.
   */
  virtual ~LOW_deviceFactory();
  
};

#endif
