/***************************************************************************
                          LOW_thread_thread.h  -  description
                             -------------------
    begin                : Mon Sep 29 2003
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

#ifndef LOW_THREAD_THREAD_H
#define LOW_THREAD_THREAD_H


#include "LOW_exception.h"
#include "LOW_thread_Factory.h"



/** Abstract base class for threads.
    Each instance represents one thread.

    Specific platforms dereive their implementation classes from this class.
    This interface is strongly inspired by the abilities of POSIX threads. As not all
    OS specific threads have the same abilities, implementation of some methods are
    optional. Methods are marked accordingly. The default implementation of optional
    methods just throw a thread_thread_notAvailable exception.

    Optional methods are mainly related to the following features:
      - Scheduling
      - Detaching
      - Cancellation
    Avaliability of optional methods can be determined by corresponding methods.
            
    The instances are created by LOW_thread_Factory, following the factory
    design pattern.

    This class is thread-safe.

    @see LOW_thread_Factory

    @author Harald Roelle
    @author Parts of the documentation taken from Linux man pages by Xavier Leroy.
    @author Parts of the documentation taken from IEEE Standard 1003.1-2003.
 */
class LOW_thread_thread {

//=======================================================================================
public:

  //=====================================================================================
  //
  // exceptions
  //

  /** Exception base class for all exceptions thrown by LOW_thread_thread. */
  class_DERIVE_FROM_EXCEPTION( thread_thread_error, LOW_exception);

  /** Exception class indicating insufficient access rights. */
  class_DERIVE_FROM_EXCEPTION( thread_thread_permission, thread_thread_error);

  /** Exception class indicating that an option method is not implemented. */
  class_DERIVE_FROM_EXCEPTION( thread_thread_notAvailable, thread_thread_error);
  
  
  //=====================================================================================
  //
  // constructors
  //

  /** Destructor.
   */
  virtual ~LOW_thread_thread();


  //=====================================================================================
  //
  // methods
  //

  /**
      @name Methods callable by any thread.
   */
  //!@{

  /** Indicates if the thread was started.

      Implementation is mandatory.
      
      @return  Bool indicating if thread was started.
   */
  virtual bool getIsStarted() const = 0;

  
  /** Indicates if the thread has terminated.

      Implementation is mandatory.

      @return  Bool indicating if thread has terminated.
   */
  virtual bool getIsTerminated() const = 0;

  
  /** Get the return value of the thread.

      Implementation is mandatory.

      @return  Return value of terminated thread.
      @throw  thread_thread_error  When thread has not terminated yet.
   */
  virtual int getReturnValue() const = 0;

  
  /** Actually create the thread and starts it's runnable's run() method.

      The new thread terminates either explicitly, by calling thread_exit(), or
      implicitly, by returning  from the run() method. The latter case is equivalent
      to calling thread_exit() with the result returned by run() as exit code.
      
      Implementation is mandatory.

      @throw  thread_thread_error  On any error.
   */
  virtual void create() = 0;

  
  /** Cancel the execution of the thread.

      Note that the thread is not forced to quit. It depends on the thread implementation
      when or or if anyway a cancellation request is honored.

      Implementation is mandatory.

      @throw  thread_thread_error  On any error.
   */
  virtual void cancel() = 0;

  
  /** Wait for the termination of the thread.
  
      Suspends the execution of the calling thread until the thread terminates, either
      by calling thread_exit() or by being cancelled.

      Implementation is mandatory.

      @throw  thread_thread_error  On any error.
   */
  virtual int join() = 0;

  
  /** Indicate if thread detaching is available.
   */
  virtual bool getDetachAvialable() const;


  /** Put a running thread in the detached state.

      Implementation is optional.

      @throw  thread_thread_error  On any error.

      @see getDetachAvialable()
   */
  virtual void detach();


  /** Indicate if thread scheduling modification is available.
   */
  virtual bool getScheduleModifyAvailable() const;


  /** Set policy and parameter of scheduling.

      @param inPolicy  Scheduling policy to apply.
      @param inPolicy  Scheduling parameter to apply.

      @throw  thread_thread_error  On any error.
   */
  virtual void setScheduling( const LOW_thread_Factory::schedPolicy_t inPolicy, const LOW_thread_Factory::schedPrio_t inPrio);

  
  /** Get the scheduling policy.

      @throw  thread_thread_error  On any error.
   */
  virtual LOW_thread_Factory::schedPolicy_t getSchedPolicy() const;


  /** Get the scheduling parameter.

      @throw  thread_thread_error  On any error.
   */
  virtual LOW_thread_Factory::schedPrio_t getSchedPrio() const;


  /** Indicate if thread cancelling attributes are available.
   */
  virtual bool getCancelAttribsAvialable() const;


  /** Get cancelling behaviour.

      Implementation is optional.

      @throw  thread_thread_error  On any error.
   */
  virtual LOW_thread_Factory::cancelAttrib_t getCancelAttrib() const;

  //!@}

    
  /**
      @name Methods only callable by the own thread.
   */
  //!@{

  /** Terminate the calling thread.
  
      Method can only be called by the own thread.

      Implementation is mandatory.

      @param inRetVal  Return value to quit with.

      @throw  thread_thread_permission  Calling thread is not the object's thread.
      @throw  thread_thread_error  On any other error.
   */
  virtual void thread_exit( const int inRetVal) = 0;


  /** Test on pending cancellation.

      Method does nothing except testing for pending cancellation and executing it.
      Its purpose is to introduce explicit checks for cancellation in long sequences
      of code that do not call cancellation point functions otherwise.

      Implementation is mandatory.

      @throw  thread_thread_permission  Calling thread is not the object's thread.
      @throw  thread_thread_error  On any other error.

      @see cancel()
   */
  virtual void testCancel() = 0;

  
  /** Set cancelling behaviour.

      Changes the cancellation state for the calling thread -- that is, whether
      cancellation requests are ignored or not and when they are handled.

      Implementation is optional.

      @param inCancelAttrib  Cancelling handling attribute to set.

      @throw  thread_thread_permission  Calling thread is not the object's thread.
      @throw  thread_thread_error  On any other error.

      @see getCancelAttribsAvialable()
   */
  virtual void setCancelAttrib( const LOW_thread_Factory::cancelAttrib_t inCancelAttrib);

  //!@}

  
//=======================================================================================
public:

  /** Helper method for thread launching.

      This method gives the possibility to interface with OS-specific calls which require
      C-style linkage conventions.

      Note that when imlementing such a method, special care
      about thread-safety must be taken.
   */
  static void* threadRunner( void*);  
};

#endif
