/***************************************************************************
                          LOW_netSegment.h  -  description
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

#ifndef LOW_NETSEGMENT_H
#define LOW_NETSEGMENT_H


#include <algorithm>

#include "LOW_types.h"
#include "LOW_objectIDFactory.h"
#include "LOW_device.h"
#include "LOW_link.h"
#include "LOW_objectSynchronizer.h"
  

class LOW_device; // Forward declaration to avoid loops.


/** Single segment of the 1-Wire net.

    The class models a single segment reachable by a concrete link 
    of the 1-Wire network.

    Objects for devices are automatically instanciated via LOW_deviceFactory.
    The objects are cached in two maps:
    
      - aliveDevMap: Contains currently present devices on the segment.

      - graveyardMap: Contains formerly present devices on the segment.

    This means, that once a device is not visible any more, the device object
    is not deleted but burried on the graveyard. Once the device is visible again,
    it is revitalized.

    This class is thread-safe.
    
    @author Harald Roelle
    @author Parts of the documentation by Dallas Semiconductors / Maxim Integrated Products
  */
class LOW_netSegment : public LOW_objectSynchronizer
{

//=======================================================================================
public: 
  
  //=====================================================================================
  //
  // exceptions
  //
 
  /** Exception base class for all exceptions thrown by LOW_netSegment. */
  class_DERIVE_FROM_EXCEPTION( netSegment_error, LOW_exception);
  
  /** Exception to indicate that one or more expeced devices were not found. */
  class_DERIVE_FROM_EXCEPTION( noDevice_error, netSegment_error);

  
  //=====================================================================================
  //
  // type definitions
  //
      
  typedef LOW_objectIDFactory::objectID_t segmentID_t;  /**< segment ID type */
  
  typedef std::vector<LOW_netSegment*> netSegPtrVec_t;  /**< Vector type of class pointers. */
  
  
  //=====================================================================================
  //
  // methods
  //
  
  /** Get the LOW_link instance associated with this segment.
      @return  Reference to the link instance.
   */
  virtual LOW_link& getLink();
  
  
  /** Get wether there is an external power line on the segment.
      @returns  Wether there is an external power line on the segment.
   */
  virtual bool getHasExternalPower() const;

  
  /** Get a specific device.
  
      Devices are searched in the internal device list only. No bus actions are performed.
      
      <B>Note:</B>:The device type to look for is selected by the template parameter. 
      
      @param  inDevID  ID of the device to get.
      @return Requested device.
      @throw LOW_device::familyMismatch_error  Thrown when family code of selected class
                                               and the one in the ID don't match.
      @throw noDevice_error  Thrown when requested device is not present.
   */
  template<class devType> devType* getDevice( const LOW_deviceID inDevID);

        
  /** Get devices of a specific type.
  
      Devices are searched in the internal device list only. No bus actions are performed.
      Selecting any device type will result in the complete list of devices 
      known to be present on the segment.
  
      <B>Note:</B>: The device type to look for is selected by the template parameter. 
                    To select any device type use LOW_device.
      
      @return Vector of found devices.
   */
  template<class devType> std::vector<devType*> getDevices() const;

    
  /** Search for devices on the segment.
  
      Selecting any device type will result in searching the whole segment.
      
      <B>Note:</B>: The device type to look for is selected by the template parameter.
                    To select any device type use LOW_device.
      
      <B>Note:</B>: The bus will be actively searched. Newly found devices will be
                    added to the internal device list.
                   
      @param  inOnlyAlarm  Determines whether to look only for alarming devices of the
                           requested type.   
      @return Vector of found devices.
   */
  template<class devType> std::vector<devType*> searchDevices( const bool inOnlyAlarm = false);

    
  /** Verify existance of a specific device on the segment.
  
      <B>Note:</B>: In case you already own a reference to the device, use the 
                    corresponding method from LOW_device.
      
      <B>Note:</B>: The bus will be actively searched. Newly found devices will be
                    added to the internal device list.
                   
      @param  inDevID      ID of the device to verify.
      @param  inOnlyAlarm  Determines whether to report existance only when
                           the device is alarming.
      @param  inDoReset    Whether to reset the bus afterwards.   
      @return Boolean indicates wheter the device could be found or not.
   */
  virtual bool verifyDevice( const LOW_deviceID inDevID, const bool inOnlyAlarm = false, 
                             const bool inDoReset = true);
  

            
//=======================================================================================
protected:

  //=====================================================================================
  //
  // friends
  //
  
  friend class LOW_network;  /**< Enable access to static pseudo constructor and destructor */
  friend class LOW_device;   /**< Enable access to unregister a device on destruction. */
  

  
  //=====================================================================================
  //
  // static attributes
  //

  __LOW_SYNCHRONIZE_DEFINE_PROTECTED_LOCK__


  
  //=====================================================================================
  //
  // constructors
  //

  /** Destructor.
      Destroys all objects of devices (in both maps).
   */
  virtual ~LOW_netSegment();


  
  //=====================================================================================
  //
  // static methods
  //

  /** Discovers network segments on a link.
      @warning  By now only a single segment is supported.

      @param  inLink  The link to be searched for network segments.
   */
  static netSegPtrVec_t newSegmentsFromLink( LOW_link &inLink);

  
    
  //=====================================================================================
  //
  // methods
  //
  
  /** Indicate that a device instance is going to be deleted.  
      Function mainly used by the destructor of LOW_device.
  
      @param inDev  Device to unregister.
   */
  virtual void unregisterDevice( const LOW_device *inDev);
  
  
  
  //=====================================================================================
  //
  // protected addressing/searching methods
  //
  
  /** Select a single device on a link.
  
      The matchROM command followed by a 64-bit ROM code sequence 
      allows the bus master to address a specific slave device on a 
      multidrop or single-drop bus. Only the slave that exactly matches 
      the 64-bit ROM code sequence will respond to the function command 
      issued by the master; all other slaves on the bus will wait for a 
      reset pulse.
      
      @param inDevice  Device to select.
      @throw noDevice_error  Thrown when no device was found on the bus.
   */
  virtual void cmd_MatchROM( const LOW_device *inDevice) const;

    
  /** Select all devices on a link.
  
      The master can use this command to address all devices on the bus 
      simultaneously without sending out any ROM code information. 
      For example, the master can make all DS18S20s on the bus perform 
      simultaneous temperature conversions by issuing a Skip ROM command 
      followed by a ConvertT command.

      @throw noDevice_error  Thrown when no device was found on the bus.
   */
  virtual void cmd_SkipROM() const;
  
  
  /** Read ID number from bus.
  
      The read ROM command allows the bus master to read the slave's 64-bit ROM 
      code without using the Search ROM procedure. 
      
      <B>Note</B>: The read ROM command can only be used when there is one slave on 
                   the bus. If this command is used when there is more than one slave 
                   present on the bus, a data collision will occur when all the slaves 
                   attempt to respond at the same time.

      @return  ID of the found device.
      @throw noDevice_error  Thrown when no device was found on the bus.
   */
  virtual LOW_deviceID cmd_ReadROM() const;

  
  /** Scan the bus for devices.
  
      The searchROM command allows the master to determine the number of slaves 
      and their device types. The master learns the ROM codes through a process of 
      elimination that requires the master to perform a Search ROM cycle (i.e., 
      Search ROM command followed by data exchange) as many times as necessary to 
      identify all of the slave devices. 
      
      If there is only one slave on the bus, the simpler Read ROM command can be used 
      in place of the Search ROM process. 
    
      The search can be narrowed to devices which their alarm flag set and/or to 
      specific family codes.
      
      @param inOnlyAlarm   Whether to look only for alarming devices.
      @param inFamCode     Specific family code for narrowing search.
      @return  List of found IDs.
      @throw noDevice_error  Thrown when no device was found on the bus.
   */  
  virtual LOW_deviceID::deviceIDVec_t cmd_SearchROM( const bool inOnlyAlarm = false,
                                                     const LOW_deviceIDRaw::devFamCode_t inFamCode = LOW_device::anyDev_famCode) const;
  
  
  /** Scan the bus for a specific device.

      @param inDevID       Device to search for.
      @param inOnlyAlarm   Whether to report as found only when alarming.
      @param inDoReset     Whether to reset the bus afterwards.
      @return   Indicated whether the device was found or not.
      @throw noDevice_error  Thrown when no device was found on the bus.
   */  
  virtual bool cmd_SearchROMVerify( const LOW_deviceID inDevID, const bool inOnlyAlarm = false,
                                    const bool inDoReset = true) const;

  

//=======================================================================================
private:
  
  //=====================================================================================
  //
  // attributes
  //
  
  const segmentID_t         segmentID;         /**< Identifier of the segment. */
  LOW_link                  &link;             /**< Link this segment is present on */
  bool                      hasExternalPower;  /**< Whether there is an additional line for external power supply available */
  LOW_device::deviceMap_t   aliveDevMap;       /**< Map of currently present devices on the segment */
  LOW_device::deviceMap_t   graveyardMap;      /**< Map of formerly present devices on the segment */
    
  
  //=====================================================================================
  //
  // constructors
  //

  /** Constructor from link.
      Triggers initial search of devices on the segment.
      @param  inLink  The link the segment resides on.
   */
  LOW_netSegment( LOW_link &inLink);


  //=====================================================================================
  //
  // operator overloading
  //

  virtual bool operator==(LOW_netSegment &inSegment) const;  /**< Comparison based on segmentID. */


  //=====================================================================================
  //
  // methods
  //

  /** Safely cast a generic device pointer to a specific one.
      The type to cast to is specified by the template parameter.
      
      @param  inPtr  Generic pointer to be casted.
      @return Casted pointer of the requested type.
      @throw LOW_device::illegalCast_error  Thrown when type cast is not possible.
   */
  template<class castType> inline castType* devicePtr_cast( LOW_device *inPtr) const;

    
  /** Move a device from the alive map to the graveyard.
      If not in alive map, the method returns silently.
      
      @param  inDev  The device to move.
   */
  virtual void buryDevice( const LOW_device *inDev);
  
  
  /** Move a device from the graveyard to the alive map.
      If not on graveyard, the method returns silently.
      
      @param  inDev  The device to move.
   */
  virtual void revitalizeDevice( const LOW_device *inDev);
  
  
  /** Add a device to the alive map.
  
      When the device is already in the alive map nothing is done.
      When it is on the graveyard, it is moved to the alive map.
      When it does not exist at all, a new instance is created.
      
      @param  inDevID  The ID of the device to add.
      @return The added device.
   */
  virtual LOW_device* addDevice( const LOW_deviceID inDevID);
  
  
  /** Removes a device from either the alive map or the graveyard.
      If it does not exist, the method returns silently.
  
      @param  inDev  The device to remove.
   */
  virtual void removeDevice( const LOW_device *inDev);

  template<class devType> bool containsDevice( const LOW_deviceID inDevID);
};



//=====================================================================================
// DEFINITIONS COMPILERS CANNOT HANDLE TO GO DIRECTLY INTO THE LIBRARY
//=====================================================================================


//=====================================================================================
//
// template definitions
//

template<class devType> bool LOW_netSegment::containsDevice( const LOW_deviceID inDevID)
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  if (    devType::familyCode != LOW_device::anyDev_famCode
       && devType::familyCode != LOW_device::unknownDev_famCode
       && devType::familyCode != inDevID.getFamilyCode()    )
    throw LOW_device::familyMismatch_error( "Requested type does not match given device ID", __FILE__, __LINE__);

  // look in the map of alive devices
  LOW_device::deviceMap_t::iterator found = aliveDevMap.find( inDevID);
  if ( found == aliveDevMap.end() )
    return false;//throw noDevice_error( "Device not present on segment", __FILE__, __LINE__);

  return true;//devicePtr_cast<devType>(found->second);
}

template<class devType> devType* LOW_netSegment::getDevice( const LOW_deviceID inDevID)
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  if (    devType::familyCode != LOW_device::anyDev_famCode
       && devType::familyCode != LOW_device::unknownDev_famCode
       && devType::familyCode != inDevID.getFamilyCode()    )
    throw LOW_device::familyMismatch_error( "Requested type does not match given device ID", __FILE__, __LINE__);
  
  // look in the map of alive devices
  LOW_device::deviceMap_t::iterator found = aliveDevMap.find( inDevID);
  if ( found == aliveDevMap.end() )
    throw LOW_netSegment::noDevice_error( "Device not present on segment", __FILE__, __LINE__);
  
  return devicePtr_cast<devType>(found->second);
}


template<class devType> std::vector<devType*> LOW_netSegment::getDevices() const
{
  __LOW_SYNCHRONIZE_METHOD_READ_WEAK__

  std::vector<devType*> retVal;

  // look in the map of alive devices
  for( LOW_device::deviceMap_t::const_iterator a=aliveDevMap.begin(); a!=aliveDevMap.end(); ++a) {
    if (    devType::familyCode == LOW_device::anyDev_famCode
         || ( devType::familyCode==LOW_device::unknownDev_famCode && dynamic_cast<devType*>(a->second)!=0 )
         || devType::familyCode == a->second->getID().getFamilyCode())
      retVal.push_back( devicePtr_cast<devType>(a->second));
  }

  return retVal;
}
  
  
template<class devType> std::vector<devType*> LOW_netSegment::searchDevices( const bool inOnlyAlarm)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE_WEAK__
  
  LOW_deviceID::deviceIDVec_t idList = cmd_SearchROM( inOnlyAlarm, devType::familyCode);
  std::vector<devType*>       retVal = std::vector<devType*>( idList.size());

  // in case of NOT alarm search, remember the active devices
  LOW_device::devPtrVec_t   formerlyActiveDevs;
  if ( !inOnlyAlarm ) {
    std::vector<devType*> tmpVec = getDevices<devType>();
    for( unsigned int a=0; a<tmpVec.size(); a++)
      formerlyActiveDevs.push_back( tmpVec[a]);
  }
  
  for( unsigned int a=0; a<idList.size(); a++) {
    LOW_device *newDev = addDevice( idList[a]);
    retVal[a] = devicePtr_cast<devType>(newDev);
      
    // in case it was already present, remove the device from the formerly active devices
    if ( !inOnlyAlarm ) {
      LOW_device::devPtrVec_t::iterator foundFormerlyAlive = find( formerlyActiveDevs.begin(), formerlyActiveDevs.end(), newDev);
      if ( foundFormerlyAlive!=formerlyActiveDevs.end())
        formerlyActiveDevs.erase( foundFormerlyAlive);
    }
  }
  
  // what is now remaining is not present any more and therefore is sent to the graveyard
  if ( !inOnlyAlarm ) {
    for( unsigned int a=0; a<formerlyActiveDevs.size(); a++)
      buryDevice( formerlyActiveDevs[a]);
  }
    
  return retVal;
}


template<class castType> inline castType* LOW_netSegment::devicePtr_cast( LOW_device *inPtr) const
{
  // no locking necessary
  
  castType* retVal = dynamic_cast<castType*>(inPtr);
  
  if ( retVal == 0 )
    throw LOW_device::illegalCast_error( "Unexpected illegal type cast", __FILE__, __LINE__);
    
  return retVal;
}


#endif

