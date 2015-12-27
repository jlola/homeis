/***************************************************************************
                          LOW_portSerialLinux.cpp  -  description
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

 
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/serial.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include <memory>
#include <new>


#include "LOW_portSerial_Linux.h"
#include "LOW_helper_msglog.h"



//=====================================================================================
//
// constructors
//


LOW_portSerial_Linux::LOW_portSerial_Linux( const LOW_portSerialFactory::portSpecifier_t inSerialPort) :
  serialPortPath( inSerialPort)
{
  // Open the serial port, turning on devices
  if ( (serialFD=open( serialPortPath.c_str(), O_RDWR)) == -1 ) {
    throw portSerial_error( errno, "Error opening tty " + serialPortPath, __FILE__, __LINE__);
  }
}
  

LOW_portSerial_Linux::~LOW_portSerial_Linux()
{
  close( serialFD);
}



//=====================================================================================
//
// methods
//

void LOW_portSerial_Linux::tty_configure( const flowControl_t inFlowCtl, const dataBitsSite_t inDataBits,
                                          const parity_t inParity, const stopBits_t inStopBits, const speed_t inSpeed)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE__

  struct serial_struct   serial;
  struct termios         term;
  
  if ( ioctl( serialFD, TIOCGSERIAL, &serial) < 0 )
    throw portSerial_error( errno, "TIOCGSERIAL failed", __FILE__, __LINE__);
    
  // Get the current device settings
  if( tcgetattr( serialFD, &term) < 0 )
    throw portSerial_error( errno, "Error with tcgetattr", __FILE__, __LINE__);

  // preset settings
  term.c_iflag = IGNBRK;
  term.c_oflag = 0;
  term.c_cflag = CREAD | HUPCL;
  term.c_lflag = 0;

  // set the config values
            
  switch ( inFlowCtl ) {
    
    case none_flowControl:
      term.c_cflag |= CLOCAL;
      break;
    
    case xonxoff_flowControl:
      term.c_iflag |= IXON | IXOFF;
      term.c_cflag |= CLOCAL;
      break;
    
    case rtscts_flowControl:
      term.c_cflag |= CRTSCTS;
      break;
      
    default:
      throw portSerial_error( "Unknown control parameter value", __FILE__, __LINE__);
      break;
  }
  
  switch ( inParity ) {
    
    case no_parity:
      term.c_iflag |= IGNPAR;
      break;
    
    case odd_parity:
      term.c_iflag |= INPCK;
      term.c_cflag |= PARENB | PARODD;
      break;
    
    case even_parity:
      term.c_iflag |= INPCK;
      term.c_cflag |= PARENB;
      break;
      
    default:
      throw portSerial_error( "Unknown control parameter value", __FILE__, __LINE__);
      break;
  }
  
  switch ( inDataBits ) {
    
    case bit5_size:
      term.c_cflag |= CS5;
      break;
  
    case bit6_size:
      term.c_cflag |= CS6;
      break;
  
    case bit7_size:
      term.c_cflag |= CS7;
      break;
  
    case bit8_size:
      term.c_cflag |= CS8;
      break;
      
    default:
      throw portSerial_error( "Unknown control parameter value", __FILE__, __LINE__);
      break;
  }
  
  switch ( inStopBits ) {
    
    case bit1_stopBit:
      break;
  
    case bit2_stopBit:
      term.c_cflag |= CSTOPB;
      break;
      
    default:
      throw portSerial_error( "Unknown control parameter value", __FILE__, __LINE__);
      break;
  }
  
  int speed = 0;
  switch ( inSpeed ) {
    case B50_speed:     speed = B50;      break;
    case B75_speed:     speed = B75;      break;
    case B110_speed:    speed = B110;     break;
    case B134_speed:    speed = B134;     break;
    case B150_speed:    speed = B150;     break;
    case B200_speed:    speed = B200;     break;
    case B300_speed:    speed = B300;     break;
    case B600_speed:    speed = B600;     break;
    case B1200_speed:   speed = B1200;    break;
    case B1800_speed:   speed = B1800;    break;
    case B2400_speed:   speed = B2400;    break;
    case B4800_speed:   speed = B4800;    break;
    case B9600_speed:   speed = B9600;    break;
    case B19200_speed:  speed = B19200;   break;
    case B38400_speed:  speed = B38400;   break;
    case B57600_speed:  speed = B57600;   break;
    case B115200_speed: speed = B115200;  break;
    
    case B10472_speed:  speed = B38400;   break;
    
    default:
      throw portSerial_error( "Unknown control parameter value", __FILE__, __LINE__);
      break;
  }
  cfsetospeed( &term, speed);
  cfsetispeed( &term, speed);
  
  if ( inSpeed == B10472_speed ) {
    serial.flags = (serial.flags & ~ASYNC_SPD_MASK) | ASYNC_SPD_CUST;
    serial.custom_divisor = 11;   // 10472bps
  }
  else {
    serial.flags = (serial.flags & ~ASYNC_SPD_MASK);  // make 38400 really 38400
  }
  
  // read one byte without timeout. timeouts will be done by select
  term.c_cc[VMIN]  = 1;
  term.c_cc[VTIME] = 0;
    
  if ( ioctl( serialFD, TIOCSSERIAL, &serial) < 0 )
    throw portSerial_error( errno, "TIOCSSERIAL failed", __FILE__, __LINE__);
  
  if( tcsetattr( serialFD, TCSAFLUSH, &term ) < 0 )
    throw portSerial_error( errno, "Error with tcsetattr", __FILE__, __LINE__);
}


void LOW_portSerial_Linux::tty_flush( const bool inFlushIn, const bool inFlushOut)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE__

  if      (  inFlushIn && !inFlushOut ) {
    tcflush( serialFD, TCIFLUSH);
  }
  else if ( !inFlushIn &&  inFlushOut ) {
    tcflush( serialFD, TCOFLUSH);
  }
  else if (  inFlushIn &&  inFlushOut ) {
    tcflush( serialFD, TCIOFLUSH);
  }
}


void LOW_portSerial_Linux::tty_break()
{
  __LOW_SYNCHRONIZE_METHOD_WRITE__

  tcsendbreak( serialFD, 20);
}

  
uint8_t LOW_portSerial_Linux::tty_readByte( const bool inTrashExtraReply, const unsigned int inSecTimeout)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE_WEAK__

  fd_set         readset;
  struct timeval wait_tm;
  int            selRet;
  uint8_t        retVal = 0;

  for( int a=0; a<((inTrashExtraReply==true)?2:1); a++) {
    
    while ( true ) {
      
      // Setup for wait for a response or timeout
      wait_tm.tv_usec = 0;
      wait_tm.tv_sec  = inSecTimeout;
      FD_ZERO( &readset);
      FD_SET( serialFD, &readset);
        
      // Read byte if it doesn't timeout first
      selRet = select( serialFD+1, &readset, NULL, NULL, &wait_tm);
      if( selRet > 0 ) {
        
        if( FD_ISSET( serialFD, &readset) ) 
        {
          uint8_t readByte;
  
          if ( read( serialFD, &readByte, 1) != 1 )
            throw portSerial_error( "Unexpected short read", __FILE__, __LINE__);
          
          if ( a == 0 )
            retVal = readByte;
      
          LOW_helper_msglog::printDebug( LOW_helper_msglog::portSerial_dl, "LOW_linkDS2480B: TTY READ: %02x read cycle: %d\n", readByte, a);
          
          break;
        }
       
      }
      else if ( selRet == 0 ) {
        throw portSerial_error( "TTY operation timed out", __FILE__, __LINE__);
      }
      else {
        throw portSerial_error( errno, "Unexpected error in select call", __FILE__, __LINE__);
      }
    
    }
  }
  
  return retVal;
}


void LOW_portSerial_Linux::tty_read( byteVec_t &outReadBytes, const bool inTrashExtraReply, const unsigned int inSecTimeout)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE__

  for( unsigned int a=0; a<outReadBytes.size(); a++) {
    outReadBytes[a] = tty_readByte( inTrashExtraReply, inSecTimeout);
  }
}


void LOW_portSerial_Linux::tty_write( const uint8_t inWriteByte)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE__

  int written;
  
  LOW_helper_msglog::printDebug( LOW_helper_msglog::portSerial_dl, "LOW_linkDS2480B: TTY WRITE: %02x\n", inWriteByte);
  do {
    written = write( serialFD, &inWriteByte, 1);
    if ( written == -1 )
      throw portSerial_error( errno, "Error writing single byte to TTY", __FILE__, __LINE__);
    tcdrain( serialFD);
  }
  while ( written != 1 );
}


void LOW_portSerial_Linux::tty_write( const byteVec_t &inWriteBytes)
{
  __LOW_SYNCHRONIZE_METHOD_WRITE__

  unsigned int  total   = inWriteBytes.size();
  uint8_t       *buffer = new uint8_t[total];
    
  try {
    std::copy( inWriteBytes.begin(), inWriteBytes.end(), buffer);
    
    int           written   = 0;
    unsigned int  remaining = total;
    uint8_t       *writePtr = buffer;
    do {
      // I had troubles with writing to fast, so block the stuff a bit
      written = write( serialFD, writePtr, (remaining<4)?remaining:4);
      if ( written == -1 )
        throw portSerial_error( errno, "Error writing byte block to TTY", __FILE__, __LINE__);
      tcdrain( serialFD);
      remaining -= written;
      writePtr  += written;
    }
    while ( remaining != 0);
  }
  catch( ... ) {
    delete[] buffer;
    throw;
  }
  
  delete[] buffer;
}

