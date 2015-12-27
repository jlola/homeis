/***************************************************************************
                          LOW_platformMisc_Linux.cpp  -  description
                             -------------------
    begin                : Thu Aug 1 2002
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


#define _POSIX_C_SOURCE 199309 

#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/time.h>

#include "LOW_platformMisc_Linux.h"



//=====================================================================================
//
// constructors
//

LOW_platformMisc_Linux::LOW_platformMisc_Linux()
{
}


LOW_platformMisc_Linux::~LOW_platformMisc_Linux()
{
}


//=====================================================================================
//
// time methods
//

const void LOW_platformMisc_Linux::p_secSleep( const unsigned int inSeconds) const
{
  p_nanoSleep( inSeconds, 0);
}


const void LOW_platformMisc_Linux::p_milliSleep( const unsigned long inMilliSeconds) const
{
  unsigned int  sec  = inMilliSeconds/1000;
  unsigned long nsec = (inMilliSeconds%1000)*1000000;
  p_nanoSleep( sec, nsec);
}


const void LOW_platformMisc_Linux::p_microSleep( const unsigned long inMicroSeconds) const
{
  unsigned int  sec  = inMicroSeconds/1000000;
  unsigned long nsec = (inMicroSeconds%1000000)*1000;
  p_nanoSleep( sec, nsec);
}


const void LOW_platformMisc_Linux::p_nanoSleep( const unsigned long inNanoSeconds) const
{
  p_nanoSleep( 0, inNanoSeconds);
}


const void LOW_platformMisc_Linux::p_nanoSleep( const unsigned int inSeconds, const unsigned long inNanoSeconds) const
{
  struct timespec  sleepTime, remainTime;
  
  sleepTime.tv_sec  = inSeconds + inNanoSeconds/1000000000;
  sleepTime.tv_nsec = inNanoSeconds % 1000000000;
  
  while( ::nanosleep( &sleepTime, &remainTime) != 0 ) {
    sleepTime.tv_sec  = remainTime.tv_sec + remainTime.tv_nsec/1000000000;
    sleepTime.tv_nsec = remainTime.tv_nsec % 1000000000;
  }
}


const void LOW_platformMisc_Linux::p_getTimestamp( LOW_platformMisc::timestamp_t &outTimestamp) const
{
  struct timeval  tStamp;
  gettimeofday( &tStamp, 0);
  outTimestamp.sec    = tStamp.tv_sec;
  outTimestamp.milSec = tStamp.tv_usec / 1000;
}


const LOW_platformMiscFactory::threadIdent_t  LOW_platformMisc_Linux::p_getThreadID() const
{
  return getpid();
}


const std::string LOW_platformMisc_Linux::p_getHostname() const
{
  char name[1025];

  gethostname( name, 1024);

  return std::string( name);
}

