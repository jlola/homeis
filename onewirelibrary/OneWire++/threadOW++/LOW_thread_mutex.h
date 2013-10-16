/***************************************************************************
                          LOW_thread_mutex.h  -  description
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

#ifndef LOW_THREAD_MUTEX_H
#define LOW_THREAD_MUTEX_H


#include "LOW_exception.h"


/** Abstract base class for thread mutex.
    Each instance represents one thread mutex.

    Specific platforms dereive their implementation classes from this class.

    The instances are created by LOW_thread_Factory, following the factory
    design pattern.

    Implementation classes must at least support three kinds od a mutex as
    defined by mutexKind_t.

    This class is thread-safe.

    @see LOW_thread_Factory

    @author Harald Roelle
    @author Parts of the documentation taken from Linux man pages by Xavier Leroy.
 */
class LOW_thread_mutex {

//=======================================================================================
public:

  //=====================================================================================
  //
  // exceptions
  //

  /** Exception base class for all exceptions thrown by LOW_thread_mutex. */
  class_DERIVE_FROM_EXCEPTION( thread_mutex_error, LOW_exception);

  /** Exception class to indicate a mutex already locked by the calling thread. */
  class_DERIVE_FROM_EXCEPTION( thread_mutex_locked, thread_mutex_error);

  /** Exception class to indicate a mutex locked by another thread. */
  class_DERIVE_FROM_EXCEPTION( thread_mutex_busy, thread_mutex_error);


  //=====================================================================================
  //
  // locks
  //

  /** Locking class for a mutex.

      The class is intended to be used in a "locking is creation" design pattern.
      On creation a lock is optained on the mutex, and on destruction the mutex is released.
   */
  class mutexLock {
    public:
      /** Obtain the lock.
          Inlined for performance reasons.
          @param inMutex  Pointer to the mutex the lock is for.
       */
      inline mutexLock( LOW_thread_mutex &inMutex) :
        mutex( inMutex)
      {
        mutex.lock();
      };

      /** Release the lock.
          Inlined for performance reasons.
       */
      inline ~mutexLock()
      {
        mutex.unlock();
      };

    private:
      LOW_thread_mutex  &mutex;  /**< Pointer to the mutex the lock is for. */
  };


  //=====================================================================================
  //
  // type definitions
  //

  /** Basic type of a mutex. */
  typedef enum {
    mutexKind_fast = 0,   /**< Binary mutex without deadlock prevention when called locked twice by same thread. */
    mutexKind_recursive,  /**< Integer/counting mutex. */
    mutexKind_errorCheck  /**< Binary mutex with deadlock prevention and extended error checking. */
  } mutexKind_t;



  //=====================================================================================
  //
  // constructor
  //

  /** Constructor.
      @param  inMutexKind   Kind of the new mutex.
      @throw thread_mutex_error   Any error.
   */
  LOW_thread_mutex( const mutexKind_t inMutexKind);

  
  /** Destructor.
      @throw thread_mutex_busy    The mutex is still locked.
      @throw thread_mutex_error   Any other error.
   */
  virtual ~LOW_thread_mutex();


  //=====================================================================================
  //
  // methods
  //

  /** Get kind of the mutex.
      @return Kind of the mutex.
   */
  virtual mutexKind_t getKind() const;


  /** Obtain a lock on the mutex (blocking).

      Blocks the calling thread if another thread has already locked the mutex.
  
      If the mutex is already locked by the calling thread, behaviour depends on the mutex kind:

      - <B>mutexKind_fast:</B> The calling thread is suspended until the mutex is unlocked,
        thus effectively causing the calling thread to deadlock.
      - <B>mutexKind_recursive:</B> The call succeeds and returns immediately, recording
        the number of times the calling thread has locked the mutex. An equal number of
        unlock() calls must be performed before the mutex returns to the unlocked state.
      - <B>mutexKind_errorCheck:</B> A thread_mutex_locked exception is thrown.

      Abstract method to be implemented by derived class.

      @throw thread_mutex_locked  Calling thread has already locked the mutex and
                                  mutex kind is mutexKind_errorCheck.
      @throw thread_mutex_error   Any other error.
   */
  virtual void lock() = 0;


  /** Obtain a lock on the mutex (non-blocking).

      Behaves identically to lock(), except that it does not block the calling thread
      if the mutex is already locked by another thread.
      Instead an thread_mutex_busy exception is thrown.

      Abstract method to be implemented by derived class.

      @throw thread_mutex_busy    The mutex is already locked by another thread.
      @throw thread_mutex_locked  Calling thread has already locked the mutex and
                                  mutex kind is mutexKind_errorCheck.
      @throw thread_mutex_error   Any other error.
   */
  virtual void tryLock() = 0;


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

      Abstract method to be implemented by derived class.

      @throw thread_mutex_busy    The mutex is already by another thread.
      @throw thread_mutex_error   Any other error.
   */
  virtual void unlock() = 0;

  
//=======================================================================================
private:

  //=====================================================================================
  //
  // attributes
  //

  const mutexKind_t  mutexKind;
};

#endif
