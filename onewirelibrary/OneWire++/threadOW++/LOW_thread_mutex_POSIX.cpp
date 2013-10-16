/***************************************************************************
                          LOW_thread_mutex_POSIX.cpp  -  description
                             -------------------
    begin                : Fri Oct 3 2003
    copyright            : (C) 2003 by Harald Roelle
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

 
#include <errno.h>
#include <pthread.h>


#include "LOW_thread_mutex.h"
#include "LOW_thread_mutex_POSIX.h"
#include "LOW_helper_msglog.h"


//=====================================================================================
//
// constructors
//


LOW_thread_mutex_POSIX::LOW_thread_mutex_POSIX( mutexKind_t inMutexType) : LOW_thread_mutex( inMutexType)
{
  if ( int errVal=pthread_mutexattr_init( &mutexAttr) != 0 )
   throw thread_mutex_error( errVal, "pthread_mutexattr_init() failed", __FILE__, __LINE__);

  switch ( inMutexType ) {

    case mutexKind_fast:
      if ( int errVal=pthread_mutexattr_settype( &mutexAttr, PTHREAD_MUTEX_ERRORCHECK_NP) != 0 )
        throw thread_mutex_error( errVal, "pthread_mutexattr_setkind_np() failed", __FILE__, __LINE__);
      break;

    case mutexKind_recursive:
      if ( int errVal=pthread_mutexattr_settype( &mutexAttr, PTHREAD_MUTEX_RECURSIVE_NP) != 0 )
        throw thread_mutex_error( errVal, "pthread_mutexattr_setkind_np() failed", __FILE__, __LINE__);
      break;

    case mutexKind_errorCheck:
      if ( int errVal=pthread_mutexattr_settype( &mutexAttr, PTHREAD_MUTEX_ERRORCHECK_NP) != 0 )
        throw thread_mutex_error( errVal, "pthread_mutexattr_setkind_np() failed", __FILE__, __LINE__);
      break;

    default:
      throw thread_mutex_error( "unknown value of mutexKind_t", __FILE__, __LINE__);
  }
  
  if ( int errVal=pthread_mutex_init( &theMutex, &mutexAttr) != 0 )
    throw thread_mutex_error( errVal, "pthread_mutex_init() failed", __FILE__, __LINE__);
}


LOW_thread_mutex_POSIX::~LOW_thread_mutex_POSIX()
{
  pthread_mutexattr_destroy( &mutexAttr); // ignore errors

  int errVal = pthread_mutex_destroy( &theMutex);
  if ( errVal == EBUSY ) // ignore other errors
    LOW_helper_msglog::printPerror( errVal, "~LOW_thread_mutex_POSIX: pthread_mutex_destroy() failed");
}



//=====================================================================================
//
// public methods
//


void LOW_thread_mutex_POSIX::lock()
{
  int errVal = pthread_mutex_lock( &theMutex);

  if      ( errVal == EDEADLK )
    throw thread_mutex_locked( errVal, "mutex already locked", __FILE__, __LINE__);
  else if ( errVal != 0 )
    throw thread_mutex_error( errVal, "pthread_mutex_lock() failed", __FILE__, __LINE__);
}
  

void LOW_thread_mutex_POSIX::tryLock()
{
  int errVal = pthread_mutex_trylock( &theMutex);

  if      ( errVal == EDEADLK )
    throw thread_mutex_locked( errVal, "mutex already locked", __FILE__, __LINE__);
  else if ( errVal == EBUSY )
    throw thread_mutex_busy( errVal, "mutex already locked by another thread", __FILE__, __LINE__);
  else if ( errVal != 0 )
    throw thread_mutex_error( errVal, "pthread_mutex_lock() failed", __FILE__, __LINE__);
}


void LOW_thread_mutex_POSIX::unlock()
{
  int errVal = pthread_mutex_unlock( &theMutex);

  if      ( errVal == EPERM )
    throw thread_mutex_busy( errVal, "mutex already locked by another thread", __FILE__, __LINE__);
  else if ( errVal != 0 )
    throw thread_mutex_error( errVal, "pthread_mutex_lock() failed", __FILE__, __LINE__);
}
