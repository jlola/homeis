/***************************************************************************
                          LOW_portSerialLinux.h  -  description
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

#ifndef LOW_PORTSERIAL_LINUX_H
#define LOW_PORTSERIAL_LINUX_H


#include <string>

#include "LOW_portSerial.h"
#include "LOW_portSerialFactory.h"
#include "LOW_objectSynchronizer.h"



/** Platform specific portSerial for Linux platforms.

    This class is thread-safe.

    @todo Add OS-level locking of serial port (e.g. uucp style).
    
    @author Harald Roelle
 */
class LOW_portSerial_Linux : public LOW_portSerial, LOW_objectSynchronizer {

//=======================================================================================
public: 
  
  /** Destructor.
   */  
  virtual ~LOW_portSerial_Linux();


  //=====================================================================================
  //
  // methods
  //
  
  virtual void tty_configure( const flowControl_t inFlowCtl, const dataBitsSite_t inDataBits,
                              const parity_t inParity, const stopBits_t inStopBits, const speed_t inSpeed);

  virtual void tty_flush( const bool inFlushIn = true, const bool inFlushOut = true);
  
  virtual void tty_break();
  
  virtual uint8_t tty_readByte( const bool inTrashExtraReply = false, const unsigned int inSecTimeout = defaultTimeout);
  
  virtual void tty_read( byteVec_t &outReadBytes, const bool inTrashExtraReply = false, const unsigned int inSecTimeout = defaultTimeout);
  
  virtual void tty_write( const uint8_t inWriteByte);
  
  virtual void tty_write( const byteVec_t &inWriteBytes);



//=======================================================================================
protected:

  //=====================================================================================
  //
  // friend classes
  //

  friend class LOW_portSerialFactory; /**< To allow construction. */


  //=====================================================================================
  //
  // constructors
  //

  /** Constructor.
      Not publicly constructable. Use LOW_portSerialFactory.

      @param inPortSpec  Path to serial port device file.
   */
  LOW_portSerial_Linux( const LOW_portSerialFactory::portSpecifier_t inSerialPort);

    
  
//=======================================================================================
private:

  //=====================================================================================
  //
  // constants
  //
  
  /** Timeout of serial operations in seconds */
  //static const int serialTimeout = 5;
  
  
  //=====================================================================================
  //
  // attributes
  //

  const std::string  serialPortPath;  /**< Path to device file */
  int                serialFD;        /**< File descriptor of serial port. */
};

#endif
