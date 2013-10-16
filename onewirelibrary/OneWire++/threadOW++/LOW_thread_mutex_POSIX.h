/***************************************************************************
                          LOW_thread_mutex_POSIX.h  -  description
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

#ifndef LOW_THREAD_MUTEX_POSIX_H
#define LOW_THREAD_MUTEX_POSIX_H


#include "LOW_thread_mutex.h"


#include <pthread.h>



/** Platform specific LOW_thread_mutex for POSIX platforms.

    This class is thread-safe.

    @see LOW_thread_mutex
    @see IEEE Standard 1003.1-2003 (http://www.opengroup.org/onlinepubs/007904975/toc.htm)
    
    @author Harald Roelle
    @author Parts of the documentation taken from Linux man pages by Xavier Leroy.
    @author Parts of the documentation taken from IEEE Standard 1003.1-2003.
 */
class LOW_thread_mutex_POSIX : public LOW_thread_mutex {

//=======================================================================================
public:

  //=====================================================================================
  //
  // constructors
  //

  /** Destructor.
      @throw thread_mutex_busy    The mutex is still locked.
      @throw thread_mutex_error   Any other error.
   */
  virtual ~LOW_thread_mutex_POSIX();


  //=====================================================================================
  //
  // methods
  //

  /** Obtain a lock on the mutex (blocking).

      Blocks the calling thread if another thread has already locked the mutex.

      If the mutex is already locked by the calling thread, behaviour depends on the mutex kind:

      - <B>mutexKind_fast:</B> The calling thread is suspended until the mutex is unlocked,
        thus effectively causing the calling thread to deadlock.
      - <B>mutexKind_recursive:</B> The call succeeds and returns immediately, recording
        the number of times the calling thread has locked the mutex. An equal number of
        unlock() calls must be performed before the mutex returns to the unlocked state.
      - <B>mutexKind_errorCheck:</B> A thread_mutex_locked exception is thrown.

      @throw thread_mutex_locked  Calling thread has already locked the mutex and
                                  mutex kind is mutexKind_errorCheck.
      @throw thread_mutex_error   Any other error.
   */
  virtual void lock();


  /** Obtain a lock on the mutex (non-blocking).

      Behaves identically to lock(), except that it does not block the calling thread
      if the mutex is already locked by another thread.
      Instead an thread_mutex_busy exception is thrown.

      @throw thread_mutex_busy    The mutex is already locked by another thread.
      @throw thread_mutex_locked  Calling thread has already locked the mutex and
                                  mutex kind is mutexKind_errorCheck.
      @throw thread_mutex_error   Any other error.
   */
  virtual void tryLock();


  /** Release a lock on the mutex.
      Behaviour depends on the mutex kind:

      - <B>mutexKind_fast:</B> The mutex is assumed to be locked and owned by the calling
        thread on entrance and always returns it to the unlocked state.
      - <B>mutexKind_recursive:</B> The mutex is assumed to be locked and owned by the calling
        thread on entrance. Decrements the locking count of the mutex (number of
        lock() calls performed on it by the calling thread), and only when this count reaches
        zero is the mutex actually unlocked.
      - <B>mutexKind_errorCheck:</B> Actually checks at run-time that the mutex is locked on
        entrance, and that it was locked by the same thread that is now calling.

      @throw thread_mutex_busy    The mutex is already by another thread.
      @throw thread_mutex_error   Any other error.
   */
  virtual void unlock();


//=======================================================================================
protected:

  //=====================================================================================
  //
  // friend classes
  //

  friend class LOW_thread_Factory; /**< To allow construction. */


  //=====================================================================================
  //
  // constructors
  //

  /** Constructor.
      Not publicly constructable. Use LOW_thread_Factory.
      @param  inMutexKind   Kind of the new mutex.
      @throw thread_mutex_error   Any error.
   */
  LOW_thread_mutex_POSIX( const mutexKind_t inMutexKind);


//=======================================================================================
private:

  //=====================================================================================
  //
  // attributes
  //

  pthread_mutexattr_t  mutexAttr;  /**< Initial mutex attributes. */
  pthread_mutex_t      theMutex;   /**< The mutex itself. */

};

#endif
