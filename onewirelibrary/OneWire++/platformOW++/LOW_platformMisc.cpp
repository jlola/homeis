/***************************************************************************
                          LOW_platformMisc.cpp  -  description
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

 
#include "LOW_platformMisc.h"


const LOW_platformMisc*  LOW_platformMisc::runtimeInstance = LOW_platformMiscFactory::new_platformMisc();


const void LOW_platformMisc::secSleep( const unsigned int inSeconds)
{
  runtimeInstance->p_secSleep( inSeconds);
}


const void LOW_platformMisc::milliSleep( const unsigned long inMilliSeconds)
{
  runtimeInstance->p_milliSleep( inMilliSeconds);
}


const void LOW_platformMisc::microSleep( const unsigned long inMicroSeconds)
{
  runtimeInstance->p_microSleep( inMicroSeconds);
}


const void LOW_platformMisc::nanoSleep( const unsigned long inNanoSeconds)
{
  runtimeInstance->p_nanoSleep( inNanoSeconds);
}


const void LOW_platformMisc::nanoSleep( const unsigned int inSeconds, const unsigned long inNanoSeconds)
{
  runtimeInstance->p_nanoSleep( inSeconds, inNanoSeconds);
}


const void LOW_platformMisc::getTimestamp( timestamp_t &outTimestamp)
{
  runtimeInstance->p_getTimestamp( outTimestamp);
}


const void LOW_platformMisc::diffTimestamp( const timestamp_t &inT1, const timestamp_t &inT2, timestamp_t &outResult)
{
  outResult.sec  = inT1.sec - inT2.sec;
  outResult.milSec = inT1.milSec - inT2.milSec;

  if      ( outResult.sec<0 && outResult.milSec>0 ) {
    outResult.sec++;
    outResult.milSec = 1000-outResult.milSec;
  }
  else if ( outResult.sec>0 && outResult.milSec<0 ) {
    outResult.sec--;
    outResult.milSec += 1000;
  }
}


const LOW_platformMiscFactory::threadIdent_t  LOW_platformMisc::getThreadID()
{
  return runtimeInstance->p_getThreadID();
}


const std::string LOW_platformMisc::getHostname()
{
  return runtimeInstance->p_getHostname();
}


