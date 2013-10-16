/***************************************************************************
                          LOW_deviceID.h  -  description
                             -------------------
    begin                : Sat Jul 6 2002
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

#ifndef LOW_DEVICEID_H
#define LOW_DEVICEID_H


#include "LOW_types.h"
#include "LOW_deviceIDRaw.h"


/** Class for immutable 1-Wire ROM IDs.

    In contrast to LOW_deviceIDRaw this class prevents manipulation of the ID
    and a CRC validation is done on creation.

    This class is thread-safe.

    @author Harald Roelle
*/
class LOW_deviceID : public LOW_deviceIDRaw {

//=======================================================================================
public:
  
  //=====================================================================================
  //
  // type definitions
  //
  
  typedef std::vector<LOW_deviceID> deviceIDVec_t;  /**< Vector type of class LOW_deviceID */
  
  
  //=====================================================================================
  //
  // constructors
  //
  
  /** Default constructor. */
  LOW_deviceID();

  /** Copy constructor. */
  LOW_deviceID( const LOW_deviceID &inDeviceID);

  /** Constructor from ROM ID as our own data type.
      @param  inRomID  Reference to ROM ID.
      @throw LOW_helper_CRC::crc_error  Thrown when CRC validation fails.
   */
  LOW_deviceID( const devRomID_t &inRomID);

  /** Constructor from two 32 bit values.
      @param inHighInt  Upper 32 bits of ROM ID.
      @param inLowInt   Lower 32 bits of ROM ID.
      @throw LOW_helper_CRC::crc_error  Thrown when CRC validation fails.
   */
  LOW_deviceID( uint32_t inHighInt, uint32_t inLowInt);

  /** Constructor from byte vector.
      @param inRomID  Reference to byte vector of exactly 8 bytes.
      @throw sizeMismatch_error  Thrown when <I>inRomID</I> has other size than 8.
      @throw LOW_helper_CRC::crc_error  Thrown when CRC validation fails.
   */
  LOW_deviceID( const byteVec_t &inRomID);

  /** Constructor from LOW_deviceIDRaw.
      @param  inDevIDRaw  Reference to a raw ROM ID.
      @throw LOW_helper_CRC::crc_error  Thrown when CRC validation fails.
   */
  LOW_deviceID( const LOW_deviceIDRaw &inDevIDRaw);
  
  /** Destructor. */
  virtual ~LOW_deviceID();


//=======================================================================================
private:

  //=====================================================================================
  //
  // methods
  //
  
  /** Validate CRC checksum of the current ROM ID.
      Returns silently when CRC is OK, else an exception is thrown.
      @throw LOW_helper_CRC::crc_error  Thrown when CRC validation fails.
   */
  virtual void checkCRC();

  /** Hidden method inherited from LOW_deviceIDRaw.
      Declared as private to prevent changes to ROM ID.
   */
  virtual void setBit( const uint8_t inBitNum, const bool inValue);
  
  /** Hidden method inherited from LOW_deviceIDRaw.
      Declared as private to prevent changes to ROM ID.
   */
  virtual void setFamilyCode( const devFamCode_t inFamCode);
};

#endif
