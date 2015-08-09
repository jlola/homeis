/***************************************************************************
                          LOW_network.h  -  description
                             -------------------
    begin                : Tue Jul 23 2002
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

#ifndef LOW_NETWORK_H
#define LOW_NETWORK_H


#include "LOW_netSegment.h"
#include "LOW_objectSynchronizer.h"


class LOW_link;



/** Class to represent a whole 1-Wire network.

    A network consists of multiple links (LOW_link).
    On a link there is at least one segment (LOW_netSegment).

    This class is thread-safe.

    @todo COPY CONSTRUCTOR AND ASSIGNMENT OPERATOR.
    @todo Add multi-segment support.

    @author Harald Roelle
 */
class LOW_network : public LOW_objectSynchronizer
{

//=======================================================================================
public: 
  
  //=====================================================================================
  //
  // exceptions
  //

  /** Exception base class for all exceptions thrown by LOW_netSegment. */
  class_DERIVE_FROM_EXCEPTION( network_error, LOW_exception);



  //=====================================================================================
  //
  // constructors
  //

  /** Constructor.
   */  
  LOW_network();

  
  /** Destructor.
      Automatically disposes all segments.
   */
  virtual ~LOW_network();
 
  
  //=====================================================================================
  //
  // methods
  //

  /** Add a link to the network.
      Triggers discovery of network segments.
      Returns silently when link is already registered.

      @param  inLink  The link to be added.
   */
  virtual void addLink( LOW_link *inLink);


  /** Remove a link from the network.
      Destroys all segments on this link.

      @param  inLink  The link to be removed.
      @throw  network_error  When link is not registered.
   */
  virtual void removeLink( LOW_link *inLink);


  /** Get list of all network segments on the network.
      @return  List of all segments.
   */  
  virtual LOW_netSegment::netSegPtrVec_t  getSegments() const;

  /** Get a specific device.

	Devices are searched in the internal lists of all segments only.
	No bus actions are performed.

	<B>Note:</B>:The device type to look for is selected by the template parameter.

	@param  inDevID  ID of the device to get.
	@return Requested device.
 */
  template<class devType> bool ContainsDevice( const LOW_deviceID inDevID) const;
  
  /** Get a specific device.

      Devices are searched in the internal lists of all segments only.
      No bus actions are performed.

      <B>Note:</B>:The device type to look for is selected by the template parameter.

      @param  inDevID  ID of the device to get.
      @return Requested device.
   */
  template<class devType> devType* getDevice( const LOW_deviceID inDevID) const;

    
  /** Get devices of a specific type.

      Devices are searched in the internal lists of all segments only.
      No bus actions are performed. Selecting any device type will result
      in the complete list of devices known to be present on the network.

      <B>Note:</B>: The device type to look for is selected by the template parameter.
                    To select any device type use LOW_device.

      @return Vector of found devices.
   */
  template<class devType> std::vector<devType*> getDevices() const;

  
  /** Search for devices on the network.

      Selecting any device type will result in searching the whole network.

      <B>Note:</B>: The device type to look for is selected by the template parameter.
                    To select any device type use LOW_device.

      <B>Note:</B>: The bus will be actively searched. Newly found devices will be
                    added to the internal lists of the segments.

      @param  inOnlyAlarm  Determines whether to look only for alarming devices of the
                           requested type.
      @return Vector of found devices.
   */
  template<class devType> std::vector<devType*> searchDevices( const bool inOnlyAlarm = false) const;
  
  
  /** Verify existance of a specific device on the network.

      <B>Note:</B>: In case you already own a reference to the device, use the
                    corresponding method from LOW_device.

      <B>Note:</B>: The bus will be actively searched. Newly found devices will be
                    added to the internal lists of the segments.

      @param  inDevID      ID of the device to verify.
      @param  inOnlyAlarm  Determines whether to report existance only when
                           the device is alarming.
      @return Boolean indicates wheter the device could be found or not.
   */
  virtual bool verifyDevice( const LOW_deviceID inDevID, const bool inOnlyAlarm = false) const;

  
    
//=======================================================================================
private: 

  //=====================================================================================
  //
  // attributes
  //
  
  LOW_netSegment::netSegPtrVec_t    segmentsList;  /**< List of all segments on the network. */
  LOW_link::linkPtrVec_t            linkList;      /**< List of links the network consts of. */
  
  
  //=====================================================================================
  //
  // methods
  //

  /** Trigger discovery of network segments.
      Found segments are added to the internal segment list.
  
      @param  inLink  The link where segments should be discovered on.
   */
  virtual void addSegments( LOW_link &inLink);
};



//=====================================================================================
// DEFINITIONS COMPILERS CANNOT HANDLE TO GO DIRECTLY INTO THE LIBRARY
//=====================================================================================


//=====================================================================================
//
// template definitions
//

template<class devType> devType* LOW_network::getDevice( const LOW_deviceID inDevID) const
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  for( unsigned int segI=0; segI<segmentsList.size(); segI++) {
    try {
      return segmentsList[segI]->getDevice<devType>( inDevID);
    }
    catch( LOW_device::noDevice_error & ex) {} // for now ignore exception
  }
  throw LOW_device::noDevice_error( "Device not found on whole network", __FILE__, __LINE__);
}
  
template<class devType> bool LOW_network::ContainsDevice( const LOW_deviceID inDevID) const
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  for( unsigned int segI=0; segI<segmentsList.size(); segI++) {
      try {
        if (segmentsList[segI]->containsDevice<LOW_device>(inDevID))
        	return true;
      }
      catch( LOW_device::noDevice_error & ex) {} // for now ignore exception
    }
    return false;

}

template<class devType>
std::vector<devType*> LOW_network::getDevices() const
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  std::vector<devType*>  retVal;
  
  for( unsigned int segI=0; segI<segmentsList.size(); segI++) {
    std::vector<devType*> segDevs = segmentsList[segI]->getDevices<devType>();
    for( unsigned int devI=0; devI<segDevs.size(); devI++)
      retVal.push_back( segDevs[devI]);
  }
  
  return retVal;
}


template<class devType> std::vector<devType*> LOW_network::searchDevices( const bool inOnlyAlarm) const
{
  __LOW_SYNCHRONIZE_METHOD_READ__

  std::vector<devType*>  retVal;
  
  for( unsigned int segI=0; segI<segmentsList.size(); segI++) {
    std::vector<devType*> segDevs = segmentsList[segI]->searchDevices<devType>( inOnlyAlarm);
    for( unsigned int devI=0; devI<segDevs.size(); devI++)
      retVal.push_back( segDevs[devI]);
  }
  
  return retVal;
}


#endif
