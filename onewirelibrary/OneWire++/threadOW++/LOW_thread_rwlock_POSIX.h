/***************************************************************************
                          LOW_thread_rwlock_POSIX.h  -  description
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

#ifndef LOW_THREAD_RWLOCK_POSIX_H
#define LOW_THREAD_RWLOCK_POSIX_H


#include "LOW_thread_rwlock.h"


#include <pthread.h>



/** Platform specific LOW_thread_rwlock for POSIX platforms.

    This class is thread-safe.

    @see LOW_thread_rwlock
    @see IEEE Standard 1003.1-2003 (http://www.opengroup.org/onlinepubs/007904975/toc.htm)
  
    @author Harald Roelle
    @author Parts of the documentation taken from Linux man pages by Xavier Leroy.
    @author Parts of the documentation taken from IEEE Standard 1003.1-2003.
 */
class LOW_thread_rwlock_POSIX : public LOW_thread_rwlock {

//=======================================================================================
public:

  //=====================================================================================
  //
  // constructors
  //

  /** Destructor.
      @throw thread_rwlock_busy    The lock is still locked.
      @throw thread_rwlock_error   Any other error.
   */
  virtual ~LOW_thread_rwlock_POSIX();


  //=====================================================================================
  //
  // methods
  //

  /** Obtain a read lock on the rwlock (blocking).

      The method applies a read lock to the read-write lock.
      The calling thread acquires the read lock if a writer does not hold the lock and
      there are no writers blocked on the lock.

      A thread may hold multiple concurrent read locks on rwlock (that is, successfully
      call the lockRead() method n times). If so, the application shall ensure that the
      thread performs matching unlocks (that is, it calls unlock() method n times).

      @throw thread_rwlock_locked  Calling thread already owns rwlock for writing.
      @throw thread_rwlock_error   Any other error.
   */
  virtual void lockRead();


  /** Obtain a read lock on the rwlock (non-blocking).

      The method applies a read lock as in the lockRead() method, with the exception
      that the function throws an exception if the equivalent lockRead() call would
      have blocked the calling thread. The method never blocks; it always either
      acquires the lock or throws a thread_rwlock_busy exception.

      @throw thread_rwlock_busy    The rwlock is already locked for writing by another thread.
      @throw thread_rwlock_locked  Calling thread already owns rwlock for writing.
      @throw thread_rwlock_error   Any other error.
   */
  virtual void tryLockRead();


  /** Obtain a write lock on the rwlock (blocking).

      The method applies a write lock to the read-write lock.
      The calling thread acquires the write lock if no other thread (reader or writer)
      holds the read-write lock rwlock. Otherwise, the thread will block until it can
      acquire the lock. The calling thread may deadlock if at the time the call is made
      it holds the read-write lock (whether a read or write lock).

      @throw thread_rwlock_locked  Calling thread already owns the read-write lock for writing or reading.
      @throw thread_rwlock_error   Any other error.
   */
  virtual void lockWrite();


  /** Obtain a write lock on the rwlock (non-blocking).

      The method applies a write lock like the lockWrite() method, with the exception
      that the function throws an exception if any thread currently holds the
      rwlock (for reading or writing).

      @throw thread_rwlock_busy    The rwlock is already locked for reading or writing.
      @throw thread_rwlock_locked  Calling thread already owns the read-write lock for writing or reading.
      @throw thread_rwlock_error   Any other error.
   */
  virtual void tryLockWrite();


  /** Release a lock on the rwlock.

      The method releasees a lock held on the read-write lock. Results are undefined if the read-write
      lock rwlock is not held by the calling thread.

      If this method is called to release a read lock from the read-write lock object and
      there are other read locks currently held on this read-write lock object, the read-write
      lock object remains in the read locked state. If this method releases the last read lock
      for this read-write lock object, the read-write lock object is put in the unlocked
      state with no owners.

      If this method is called to release a write lock for this read-write lock object,
      the read-write lock object is put in the unlocked state.

      @throw thread_rwlock_busy    calling thread does not hold a lock.
      @throw thread_rwlock_error   Any other error.
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
      @throw thread_rwlock_error   Any error.
   */
  LOW_thread_rwlock_POSIX();


  
//=======================================================================================
private:

  //=====================================================================================
  //
  // attributes
  //

  pthread_rwlock_t      theRwlock;   /**< The rwlock itself. */

};

#endif
