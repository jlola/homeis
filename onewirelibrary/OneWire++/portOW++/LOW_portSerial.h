/***************************************************************************
                          LOW_portSerial.h  -  description
                             -------------------
    begin                : Mon Jul 29 2002
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

#ifndef LOW_PORTSERIAL_H
#define LOW_PORTSERIAL_H


#include "LOW_types.h"
#include "LOW_exception.h"


/** Abstract base class for serial ports.
    Each instance represents one serial port.

    Specific platforms dereive their implementation classes from this class.

    The instances are created by LOW_portSerialFactory, following the factory
    design pattern.

    <B>Note:</B> There is no prescribed constructor. A class deriving from this
                 one should have a constructor which only requires to specify the
                 port, but no extra setup parameters for it.<BR>
                 This enables parts of your software to specify a serial port by
                 creating an object, but without knowing the specific setup for it.
                 This setup might be known then by other parts, which receive the
                 object as port specification.                

    This class is thread-safe.

    @see LOW_portSerialFactory

    @author Harald Roelle
 */
class LOW_portSerial {

//=======================================================================================
public: 
  
  //=====================================================================================
  //
  // exceptions
  //
 
  /** Exception base class for all exceptions thrown by LOW_portSerial. */
  class_DERIVE_FROM_EXCEPTION( portSerial_error, LOW_exception);

    
  //=====================================================================================
  //
  // constants
  //

  static const unsigned int defaultTimeout = 5;

  
  //=====================================================================================
  //
  // type definitions
  //

  /** Serial flow control type. */
  typedef enum { none_flowControl, xonxoff_flowControl, rtscts_flowControl} flowControl_t;

  /** Number of data bits type. */
  typedef enum { bit5_size, bit6_size, bit7_size, bit8_size} dataBitsSite_t;

  /** Parity control type. */
  typedef enum { no_parity, odd_parity, even_parity} parity_t;

  /** Number of stop bits type. */
  typedef enum { bit1_stopBit, bit2_stopBit} stopBits_t;

  /** Serial speed control type */
  typedef enum { B50_speed, B75_speed, B110_speed, B134_speed, B150_speed, B200_speed,
                 B300_speed, B600_speed, B1200_speed, B1800_speed, B2400_speed, B4800_speed,
                 B9600_speed, B19200_speed, B38400_speed, B57600_speed, B115200_speed,
                 B10472_speed} speed_t;
  
  
  //=====================================================================================
  //
  // constructors
  //

  /** Destructor.
   */
  virtual ~LOW_portSerial();


  //=====================================================================================
  //
  // methods
  //

  /** Configure the serial port.
      Abstract method to be implemented by derived class.

      @param  inFlowCtl   Flow control.
      @param  inDataBits  Number of data bits.
      @param  inParity    Parity control.
      @param  inStopBits  Number of stop bits.
      @param  inSpeed     Port speed.
   */
  virtual void tty_configure( const flowControl_t inFlowCtl, const dataBitsSite_t inDataBits,
                              const parity_t inParity, const stopBits_t inStopBits, const speed_t inSpeed) = 0;

  
  /** Flushs serial input and/or output buffers.
      Abstract method to be implemented by derived class.

      @param  inFlushIn   If set to true input buffer is flushed.
      @param  inFlushOut  If set to true output buffer is flushed.
   */
  virtual void tty_flush( const bool inFlushIn = true, const bool inFlushOut = true) = 0;

  
  /** Sends break signal.
      Abstract method to be implemented by derived class.
   */
  virtual void tty_break() = 0;

  
  /** Reads on byte from serial port.
      Abstract method to be implemented by derived class.

      @param inTrashExtraReply  If true one extra byte is read from serial port and trashed.
      @param inSecTimeout       Timeout in seconds.
      @return  Byte read from serial port.
   */
  virtual uint8_t tty_readByte( const bool inTrashExtraReply = false, const unsigned int inSecTimeout = defaultTimeout) = 0;

  
  /** Reads multiple bytes from serial port.
      The desired number of bytes to read is specified by the preset length
      of the vector parameter.

      Abstract method to be implemented by derived class.
      
      @param  outReadBytes       Reference to byte vector, where read bytes are stored in.
      @param  inTrashExtraReply  If true one extra byte is read from serial port at the end
                                 and trashed.
      @param inSecTimeout        Timeout in seconds.
   */
  virtual void tty_read( byteVec_t &outReadBytes, const bool inTrashExtraReply = false, const unsigned int inSecTimeout = defaultTimeout) = 0;

  
  /** Writes one byte to serial port.
      Abstract method to be implemented by derived class.

      @param inWriteByte  Byte to write.
   */
  virtual void tty_write( const uint8_t inWriteByte) = 0;

  
  /** Writes multiple bytes to serial port.
      The desired number of bytes to write is specified by the preset length
      of the vector parameter.

      Abstract method to be implemented by derived class.

      @param inWriteBytes  Reference to byte vector which contains bytes to write.
   */
  virtual void tty_write( const byteVec_t &inWriteBytes) = 0;

};

#endif
