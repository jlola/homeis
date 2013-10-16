/***************************************************************************
                          LOW_thread_Factory.h  -  description
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

#ifndef LOW_THREAD_FACTORY_H
#define LOW_THREAD_FACTORY_H


#include "LOW_thread_mutex.h"
#include "LOW_thread_rwlock.h"


#ifdef __linux__

#include <pthread.h>

#endif


class LOW_thread_thread;
class LOW_thread_runable;


/** Factory class for platform specific objects related to threads.

    This class also contains platform specific type definitions.

    This class is thread-safe.

    @see LOW_thread_mutex
    @see LOW_thread_thread

    @author Harald Roelle
 */
class LOW_thread_Factory {

//=======================================================================================
public: 

  //=====================================================================================
  //
  // type definitions
  //

#ifdef __linux__

  /** Type for thread detach state. */
  typedef enum { detachState_joinable = PTHREAD_CREATE_JOINABLE,  /**< Thread can be joined.                    */
                 detachState_detached = PTHREAD_CREATE_DETACHED,  /**< Thread is not joinable.                  */
                 detachState_default  = -1                        /**< System default state (only for setting). */
               } detachState_t;

  /** Type for thread scheduling policies. */
  typedef enum { schedPolicy_regular    = SCHED_OTHER,  /**< Non realtime scheduling.                       */
                 schedPolicy_roundRobin = SCHED_RR,     /**< Realtime scheduling, round robin variant.      */
                 schedPolicy_fifo       = SCHED_FIFO,   /**< Realtime scheduling, fifo variant.             */
                 schedPolicy_default    = -1            /**< System default scheduling (only for setting).  */
               } schedPolicy_t;

  typedef int schedPrio_t;  /**< Type for scheduling priority. */

  /** Type for thread cancellation attributes. */
  typedef enum { cancelAttrib_disable  = 0,   /**< Canceling disabled */
                 cancelAttrib_defered,        /**< Canceling enabled, canceling is deferred until a cancellation point is reached. */
                 cancelAttrib_async           /**< Canceling enabled, thread is immediately canceled */
               } cancelAttrib_t;

#endif

  
  //=====================================================================================
  //
  // static factory
  //

  /** Platform aware replacement for creating dynamic instances of LOW_thread_mutex.

      @param inMutexKind  Kind of the new mutex.

      @return  Pointer to new instance of LOW_thread_mutex.

      @see LOW_thread_mutex
   */
  static LOW_thread_mutex* new_mutex( const LOW_thread_mutex::mutexKind_t inMutexKind);


  /** Platform aware replacement for creating dynamic instances of LOW_thread_rwlock.

      @return  Pointer to new instance of LOW_thread_rwlock.

      @see LOW_thread_rwlock
   */
  static LOW_thread_rwlock* new_rwlock();


  /** Platform aware replacement for creating dynamic instances of LOW_thread_thread.

      @param inRunable  Object to be executed in the new thread.

      @return  Pointer to new instance of LOW_thread_thread.

      @see LOW_thread_thread
   */
  static LOW_thread_thread* new_thread( LOW_thread_runable *inRunable);


//=======================================================================================
private:

  //=====================================================================================
  //
  // constructors
  //

  /** Constructor.
      Private to prevent instaciation as this is a static factory.
   */
  LOW_thread_Factory();

  /** Destructor.
      Private to prevent instaciation as this is a static factory.
   */
  ~LOW_thread_Factory();

};

#endif
