/***************************************************************************
                          LOW_thread_thread_POSIX.cpp  -  description
                             -------------------
    begin                : Thu Oct 2 2003
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

#include "LOW_thread_Factory.h"
#include "LOW_thread_thread_POSIX.h"
#include "LOW_helper_msglog.h"


#include <errno.h>
#include <pthread.h>
#include <signal.h>


//=====================================================================================
//
// constructors
//

LOW_thread_thread_POSIX::LOW_thread_thread_POSIX( LOW_thread_runable *inRunable,
                                                  const LOW_thread_Factory::detachState_t inDetachState,
                                                  const LOW_thread_Factory::schedPolicy_t inSchedPolicy,
                                                  const LOW_thread_Factory::schedPrio_t inSchedPrio)
{
  createSyncMutex = LOW_thread_Factory::new_mutex( LOW_thread_mutex::mutexKind_errorCheck);
  
  myRunable = inRunable;
  
  if ( int errVal=pthread_attr_init( &initAttr) != 0 ) // fill initial attributes with default values
    throw thread_thread_error( errVal, "pthread_attr_init() failed", __FILE__, __LINE__);

  switch ( inDetachState ) {
    case LOW_thread_Factory::detachState_joinable:
      if ( int errVal=pthread_attr_setdetachstate( &initAttr, PTHREAD_CREATE_JOINABLE) != 0 )
        throw thread_thread_error( errVal, "pthread_attr_setdetachstate() failed", __FILE__, __LINE__);
      break;
    case LOW_thread_Factory::detachState_detached:
      if ( int errVal=pthread_attr_setdetachstate( &initAttr, PTHREAD_CREATE_DETACHED) != 0 )
        throw thread_thread_error( errVal, "pthread_attr_setdetachstate() failed", __FILE__, __LINE__);
      break;
    case LOW_thread_Factory::detachState_default:
      break;
    default:
      throw thread_thread_error( "internal error: unknown value of detachState_t", __FILE__, __LINE__);
  }

  switch ( inSchedPolicy ) {
    case LOW_thread_Factory::schedPolicy_regular:
      if ( int errVal=pthread_attr_setschedpolicy( &initAttr, SCHED_OTHER) != 0 )
        throw thread_thread_error( errVal, "pthread_attr_setschedpolicy() failed", __FILE__, __LINE__);
      break;
    case LOW_thread_Factory::schedPolicy_roundRobin:
      if ( int errVal=pthread_attr_setschedpolicy( &initAttr, SCHED_RR) != 0 )
        throw thread_thread_error( errVal, "pthread_attr_setschedpolicy() failed", __FILE__, __LINE__);
      break;
    case LOW_thread_Factory::schedPolicy_fifo:
      if ( int errVal=pthread_attr_setschedpolicy( &initAttr, SCHED_FIFO) != 0 )
        throw thread_thread_error( errVal, "pthread_attr_setschedpolicy() failed", __FILE__, __LINE__);
      break;
    case LOW_thread_Factory::schedPolicy_default:
      break;
    default:
      throw thread_thread_error( "internal error: unknown value of schedPolicy_t", __FILE__, __LINE__);
  }

  if ( inSchedPrio != schedPrio_default ) {
    struct sched_param newSchedParam;
    newSchedParam.sched_priority = inSchedPrio;
    if ( int errVal=pthread_attr_setschedparam( &initAttr, &newSchedParam) != 0 )
      throw thread_thread_error( errVal, "pthread_attr_setschedparam() failed", __FILE__, __LINE__);
  }
  
  isStarted = false;
  isTerminated = false;
}


LOW_thread_thread_POSIX::~LOW_thread_thread_POSIX()
{
  pthread_cancel( threadID);     // ignore errors
  pthread_join( threadID, NULL); // ignore errors

  delete createSyncMutex;
  
  if ( int errVal=pthread_attr_destroy( &initAttr) != 0 )
    LOW_helper_msglog::printPerror( errVal, "~LOW_thread_thread_POSIX: pthread_attr_destroy() failed");
}



//=====================================================================================
//
// methods
//

bool LOW_thread_thread_POSIX::getIsStarted() const
{
  return isStarted;
}


bool LOW_thread_thread_POSIX::getIsTerminated() const
{
  return isTerminated;
}


int LOW_thread_thread_POSIX::getReturnValue() const 
{
  if ( ! getIsStarted() )
    throw thread_thread_error( "thread was not started", __FILE__, __LINE__);

  if ( ! getIsTerminated() )
    throw thread_thread_error( "thread not terminated yet", __FILE__, __LINE__);

  return returnValue;
}


void LOW_thread_thread_POSIX::create()
{
  if ( getIsStarted() )
    throw thread_thread_error( "thread already started", __FILE__, __LINE__);

  LOW_thread_mutex::mutexLock theLock = LOW_thread_mutex::mutexLock( *createSyncMutex); // lock until return

  if ( int errVal=pthread_create( &threadID, &initAttr, threadRunner, this) != 0 )
    throw thread_thread_error( errVal, "pthread_create() failed", __FILE__, __LINE__);

  isStarted     = true;
}


void LOW_thread_thread_POSIX::cancel()
{
  if ( int errVal=pthread_cancel( threadID) != 0 )
    throw thread_thread_error( errVal, "pthread_cancel() failed", __FILE__, __LINE__);
}


int LOW_thread_thread_POSIX::join()
{
  if ( int errVal=pthread_join( threadID, reinterpret_cast<void**>(&returnValue)) != 0 )
    throw thread_thread_error( errVal, "pthread_join() failed", __FILE__, __LINE__);

  return returnValue;
}


void LOW_thread_thread_POSIX::detach()
{
  if ( int errVal=pthread_detach( threadID) != 0 )
    throw thread_thread_error( errVal, "pthread_detach() failed", __FILE__, __LINE__);
}


void LOW_thread_thread_POSIX::setScheduling( const LOW_thread_Factory::schedPolicy_t inPolicy, const LOW_thread_Factory::schedPrio_t inPrio)
{
  struct sched_param  schedParams;
  
  schedParams.sched_priority = inPrio;
  if ( int errVal=pthread_setschedparam( threadID, inPolicy, &schedParams) != 0 )
    throw thread_thread_error( errVal, "pthread_setschedparam() failed", __FILE__, __LINE__);
}


LOW_thread_Factory::schedPolicy_t LOW_thread_thread_POSIX::getSchedPolicy() const
{
  int                 schedPolicy;
  struct sched_param  schedParams;
  
  if ( int errVal=pthread_getschedparam( threadID, &schedPolicy, &schedParams) != 0 )
    throw thread_thread_error( errVal, "pthread_getschedparam() failed", __FILE__, __LINE__);
    
  return static_cast<LOW_thread_Factory::schedPolicy_t>(schedPolicy);
}


LOW_thread_Factory::schedPrio_t LOW_thread_thread_POSIX::getSchedPrio() const
{
  int                 schedPolicy;
  struct sched_param  schedParams;
  
  if ( int errVal=pthread_getschedparam( threadID, &schedPolicy, &schedParams) != 0 )
    throw thread_thread_error( errVal, "pthread_getschedparam() failed", __FILE__, __LINE__);
    
  return schedParams.sched_priority;
}


LOW_thread_Factory::cancelAttrib_t LOW_thread_thread_POSIX::getCancelAttrib() const 
{
  if ( ! getIsStarted() || getIsTerminated() )
    throw thread_thread_error( "thread not running", __FILE__, __LINE__);
  
  return cancelAttrib;
}




void LOW_thread_thread_POSIX::thread_exit( const int inRetVal)
{
  if ( ! pthread_equal( threadID, pthread_self()) )
    throw thread_thread_permission( "only the current thread is allowed to use thread_exit()", __FILE__, __LINE__);
  
  pthread_exit( reinterpret_cast<void*>(inRetVal));
}


void LOW_thread_thread_POSIX::testCancel()
{
  if ( ! pthread_equal( threadID, pthread_self()) )
    throw thread_thread_permission( "only the current thread is allowed to use testCancel()", __FILE__, __LINE__);
  
  pthread_testcancel();
}


void LOW_thread_thread_POSIX::setCancelAttrib( const LOW_thread_Factory::cancelAttrib_t inCancelAttrib)
{
  if ( ! pthread_equal( threadID, pthread_self()) )
    throw thread_thread_permission( "only the current thread is allowed to use setCancelState()", __FILE__, __LINE__);

  cancelAttrib = inCancelAttrib;

  switch ( inCancelAttrib ) {
    case LOW_thread_Factory::cancelAttrib_disable:
      if ( int errVal=pthread_setcancelstate( PTHREAD_CANCEL_DISABLE, NULL) != 0 )
        throw thread_thread_error( errVal, "pthread_setcancelstate() failed", __FILE__, __LINE__);
      break;

    case LOW_thread_Factory::cancelAttrib_defered:
      if ( int errVal=pthread_setcanceltype( PTHREAD_CANCEL_DEFERRED, NULL) != 0 )
        throw thread_thread_error( errVal, "pthread_setcanceltype() failed", __FILE__, __LINE__);
      if ( int errVal=pthread_setcancelstate( PTHREAD_CANCEL_ENABLE, NULL) != 0 )
        throw thread_thread_error( errVal, "pthread_setcancelstate() failed", __FILE__, __LINE__);
      break;

    case LOW_thread_Factory::cancelAttrib_async:
      if ( int errVal=pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS, NULL) != 0 )
        throw thread_thread_error( errVal, "pthread_setcanceltype() failed", __FILE__, __LINE__);
      if ( int errVal=pthread_setcancelstate( PTHREAD_CANCEL_ENABLE, NULL) != 0 )
        throw thread_thread_error( errVal, "pthread_setcancelstate() failed", __FILE__, __LINE__);
      break;

    default:
      throw thread_thread_error( "internal error: unknown value of cancelAttrib_t", __FILE__, __LINE__);
  }
}


//=====================================================================================
//
// private static methods
//

void* LOW_thread_thread_POSIX::threadRunner( void *inLaunchMe)
{
  LOW_thread_thread_POSIX *myThreadObj = static_cast<LOW_thread_thread_POSIX*>(inLaunchMe);

  // synchronize with create() method to ensure everything is set up
  LOW_thread_mutex::mutexLock theLock = LOW_thread_mutex::mutexLock( *myThreadObj->createSyncMutex); // lock until return

  // set cancellation parameters
  myThreadObj->setCancelAttrib( LOW_thread_Factory::cancelAttrib_async);

  // block all signals in the new thread
  sigset_t noSignals;
  sigemptyset( &noSignals);
  if ( int errVal=pthread_sigmask( SIG_SETMASK, &noSignals, NULL) != 0 )
    throw thread_thread_error( errVal, "pthread_sigmask() failed", __FILE__, __LINE__);

  myThreadObj->returnValue   = myThreadObj->myRunable->run( myThreadObj);
  myThreadObj->isTerminated  = true;

  return reinterpret_cast<void*>(myThreadObj->returnValue);
}


