/***************************************************************************
                          LOW_deviceIDRaw.h  -  description
                             -------------------
    begin                : Thu Jul 18 2002
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

#ifndef LOW_DEVICEIDRAW_H
#define LOW_DEVICEIDRAW_H

  
#include "LOW_types.h"
#include "LOW_exception.h"
#include "LOW_objectSynchronizer.h"



/** Base class for 1-Wire ROM IDs.
  
    In contrast to LOW_deviceID this class allows manipulation of the ID.
    Furthermore no CRC validation is done on creation.
    
    This class is thread-safe.

    @author Harald Roelle
*/
class LOW_deviceIDRaw : public LOW_objectSynchronizer {

//=======================================================================================
public: 
  
  //=====================================================================================
  //
  // exceptions
  //

  /** Exception base class for all exceptions thrown by LOW_deviceIDRaw. */
  class_DERIVE_FROM_EXCEPTION( deviceIDRaw_error  , LOW_exception);

  /** Exception class indicating a mismatch of expected/required
      size in dynamic sized data types.
   */
  class_DERIVE_FROM_EXCEPTION( sizeMismatch_error , deviceIDRaw_error);
  
  /** Exception class indicating an illegal or out of range parameter. */
  class_DERIVE_FROM_EXCEPTION( range_error        , deviceIDRaw_error);

    
  //=====================================================================================
  //
  // type definitions
  //
  
  typedef std::vector<LOW_deviceIDRaw> deviceIDRawVec_t;  /**< Vector type of class LOW_deviceIDRaw */

  typedef uint8_t  devRomID_t[8];   /**< Type of the whole 64 bit ID. */
  typedef uint8_t  devCRC_t;        /**< Type of 8 bit CRC checksum of the whole ID. */
  typedef uint8_t  devSerNum_t[6];  /**< Type of device individual serial number. */
  typedef uint8_t  devFamCode_t;    /**< Type of device's family code. */

    
  //=====================================================================================
  //
  // constructors
  //
  
  /** Default constructor. */
  LOW_deviceIDRaw();

  /** Copy constructor. */
  LOW_deviceIDRaw( const LOW_deviceIDRaw &inDeviceIDRaw);

  /** Constructor from ROM ID as our own data type.
      <B>Note:</B> No CRC validation is done.
      @param  inRomID  Reference to ROM ID.
   */
  LOW_deviceIDRaw( const devRomID_t &inRomID);

  /** Constructor from two 32 bit values.
      <B>Note:</B> No CRC validation is done.
      @param inHighInt  Upper 32 bits of ROM ID.
      @param inLowInt   Lower 32 bits of ROM ID.
   */
  LOW_deviceIDRaw( uint32_t inHighInt, uint32_t inLowInt);

  /** Constructor from byte vector.
      <B>Note:</B> No CRC validation is done.
      @param inRomID  Reference to byte vector of exactly 8 bytes.
      @throw sizeMismatch_error  Thrown when <I>inRomID</I> has other size than 8.
   */
  LOW_deviceIDRaw( const byteVec_t &inRomID);

  /** Destructor. */
  virtual ~LOW_deviceIDRaw();

  
  //=====================================================================================
  //
  // operators
  //
  
  virtual bool operator==(const LOW_deviceIDRaw &inDID) const;  /**< Comparison on the whole 64 bits of ID. */
  virtual bool operator!=(const LOW_deviceIDRaw &inDID) const;  /**< Comparison on the whole 64 bits of ID. */
  virtual bool operator<(const LOW_deviceIDRaw &inDID) const;   /**< Comparison on the whole 64 bits of ID. */
  
  
  //=====================================================================================
  //
  // methods
  //
  
  /** Get 64 bit ROM ID as our data type.
      @param  outID  Reference where ID is written to.
   */
  virtual void getRomID( devRomID_t &outID) const;
  
  /** Get 64 bit ROM ID as byte vector.
      @return  The ROM ID as byte vector.
   */
  virtual byteVec_t getRomIDVec() const;

  /** Get 64 bit ROM ID as hex C++ string.
      @return  The ROM ID as hex string.
   */
  virtual std::string getRomIDString() const;

  /** Get 8 bit CRC.
      @return  The ID's CRC part.
   */
  virtual devCRC_t getCRC() const;

  /** Get 48 bit serial number.
      @param  outSerNum  Reference where rhe ID's serial number part is written to.
   */
  virtual void getSerialNum( devSerNum_t &outSerNum) const;

  /** Get 8 bit family code.
      @return  The ID's family code part.
   */
  virtual devFamCode_t getFamilyCode() const;

  /** Set 8 bit family code.
      @param  inFamCode  Value of family code to set.
   */
  virtual void setFamilyCode( const devFamCode_t inFamCode);

  /** Get 1 bit from the ID.
      @param inBitNum            Number of the bit to get (0-63).
      @return                    Value of requested bit.
      @throw range_error         Thrown when <I>inBitNum</I> is out of range.
   */
  virtual bool getBit( uint8_t inBitNum) const;

  /** Set 1 bit in the ID.
      @param inBitNum            Number of the bit to set (0-63).
      @param inValue             Value to set the bit to.
      @throw range_error         Thrown when <I>inBitNum</I> is out of range.
   */
  virtual void setBit( const uint8_t inBitNum, const bool inValue);

  
//=======================================================================================
protected:

  //=====================================================================================
  //
  // attributes
  //
  
  devRomID_t romID;  /**< The lasered ROM ID. */

};

#endif
