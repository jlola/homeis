/***************************************************************************
                          LOW_thread_rwlock_POSIX.cpp  -  description
                             -------------------
    begin                : Sun Oct 5 2003
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


#include "LOW_thread_rwlock.h"
#include "LOW_thread_rwlock_POSIX.h"
#include "LOW_helper_msglog.h"



//=====================================================================================
//
// constructors
//


LOW_thread_rwlock_POSIX::LOW_thread_rwlock_POSIX()
{
  if ( int errVal=pthread_rwlock_init( &theRwlock, NULL) < 0 )
   throw thread_rwlock_error( errVal, "pthread_rwlock_init() failed", __FILE__, __LINE__);
}


LOW_thread_rwlock_POSIX::~LOW_thread_rwlock_POSIX()
{
  int errVal = pthread_rwlock_destroy( &theRwlock);
  if ( errVal == EBUSY ) // ignore other errors
    LOW_helper_msglog::printPerror( errVal, "~LOW_thread_rwlock_POSIX: pthread_rwlock_destroy() failed");
}



//=====================================================================================
//
// public methods
//


void LOW_thread_rwlock_POSIX::lockRead()
{
  int errVal = pthread_rwlock_rdlock( &theRwlock);

  if      ( errVal == EDEADLK )
    throw thread_rwlock_locked( errVal, "current thread already owns rwlock for writing", __FILE__, __LINE__);
  else if ( errVal != 0 )
    throw thread_rwlock_error( errVal, "pthread_rwlock_rdlock() failed", __FILE__, __LINE__);
}


void LOW_thread_rwlock_POSIX::tryLockRead()
{
  int errVal = pthread_rwlock_tryrdlock( &theRwlock);

  if      ( errVal == EDEADLK )
    throw thread_rwlock_locked( errVal, "current thread already owns rwlock for writing", __FILE__, __LINE__);
  else if ( errVal == EBUSY )
    throw thread_rwlock_busy( errVal, "already locked for writing by another thread", __FILE__, __LINE__);
  else if ( errVal != 0 )
    throw thread_rwlock_error( errVal, "pthread_rwlock_tryrdlock() failed", __FILE__, __LINE__);
}


void LOW_thread_rwlock_POSIX::lockWrite()
{
  int errVal = pthread_rwlock_wrlock( &theRwlock);

  if      ( errVal == EDEADLK )
    throw thread_rwlock_locked( errVal, "current thread already owns rwlock for reading or writing", __FILE__, __LINE__);
  else if ( errVal != 0 )
    throw thread_rwlock_error( errVal, "pthread_rwlock_wrlock() failed", __FILE__, __LINE__);
}


void LOW_thread_rwlock_POSIX::tryLockWrite()
{
  int errVal = pthread_rwlock_trywrlock( &theRwlock);

  if      ( errVal == EDEADLK )
    throw thread_rwlock_locked( errVal, "current thread already owns rwlock for reading or writing", __FILE__, __LINE__);
  else if ( errVal == EBUSY )
    throw thread_rwlock_busy( errVal, "already locked for reading or writing", __FILE__, __LINE__);
  else if ( errVal != 0 )
    throw thread_rwlock_error( errVal, "pthread_rwlock_trywrlock() failed", __FILE__, __LINE__);
}


void LOW_thread_rwlock_POSIX::unlock()
{
  int errVal = pthread_rwlock_unlock( &theRwlock);

  if      ( errVal == EPERM )
    throw thread_rwlock_busy( errVal, "Calling thread does not hold a lock", __FILE__, __LINE__);
  else if ( errVal != 0 )
    throw thread_rwlock_error( errVal, "pthread_rwlock_unlock() failed", __FILE__, __LINE__);
}
