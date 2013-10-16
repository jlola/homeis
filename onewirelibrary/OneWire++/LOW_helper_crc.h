/***************************************************************************
                          LOW_helper_crc.h  -  description
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

#ifndef LOW_HELPER_CRC_H
#define LOW_HELPER_CRC_H

  

#include "LOW_types.h"
#include "LOW_exception.h"


/** Static functions for CRC calculation.

    This class is thread-safe.

    @author Harald Roelle
 */
class LOW_helper_CRC {
  
//=======================================================================================
public: 

  //=====================================================================================
  //
  // exceptions
  //
 
  /** Exception base class to indicate CRC errors. */
  class_DERIVE_FROM_EXCEPTION( crc_error, LOW_exception);

    
  //=====================================================================================
  //
  // static methods
  //

  /** Calculate 8 bit CRC from piece of memory.
      @param  inBuf         Pointer to memoty segment.
      @param  inLen         Length of memory segment.
      @param  inPreloadCRC  Initial CRC value.
   */
  static const uint8_t calcCRC8( const uint8_t *inBuf, const unsigned int inLen, const uint8_t inPreloadCRC = 0);

  /** Calculate 8 bit CRC from byte vector.
      @param  inVec         Reference to byte vector.
      @param  inPreloadCRC  Initial CRC value.
   */
  static const uint8_t calcCRC8( const byteVec_t &inVec, const uint8_t inPreloadCRC = 0);

  
  /** Calculate 16 bit CRC from piece of memory.
      @param  inBuf         Pointer to memoty segment.
      @param  inLen         Length of memory segment.
      @param  inPreloadCRC  Initial CRC value.
   */
  static const uint16_t calcCRC16( const uint8_t *inBuf, const unsigned int inLen, const uint16_t inPreloadCRC = 0);

  /** Calculate 16 bit CRC from byte vector.
      @param  inVec         Reference to byte vector.
      @param  inPreloadCRC  Initial CRC value.
   */
  static const uint16_t calcCRC16( const byteVec_t &inVec, const uint16_t inPreloadCRC = 0);


//=======================================================================================
private:

  //=====================================================================================
  //
  // constructors
  //

  /** Default constructor.
      It is private to prevent creating objects from this class as
      this is a static helper class.
   */
  LOW_helper_CRC();

  /** Destructor.
      It is private to prevent creating objects from this class as
      this is a static helper class.
   */
  ~LOW_helper_CRC();


  //=====================================================================================
  //
  // constants
  //

  static const uint8_t  crc8Table[256];   /**< Preinitialize CRC-8 table. */
  static const uint16_t crc16Table[256];  /**< Preinitialize CRC-16 table. */

};

#endif
