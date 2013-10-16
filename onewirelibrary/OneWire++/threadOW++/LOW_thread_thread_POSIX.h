/***************************************************************************
                          LOW_thread_thread_POSIX.h  -  description
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

#ifndef LOW_THREAD_THREAD_POSIX_H
#define LOW_THREAD_THREAD_POSIX_H


#include "LOW_exception.h"
#include "LOW_thread_mutex.h"
#include "LOW_thread_runable.h"
#include "LOW_thread_thread.h"



/** Platform specific thread_thread for POSIX platforms.

    This class is thread-safe.

    @todo PROVIDE ASSIGNMENT OPERATOR
    @todo Throw more fine grained exceptions.

    @see LOW_thread_thread
    @see IEEE Standard 1003.1-2003 (http://www.opengroup.org/onlinepubs/007904975/toc.htm)
            
    @author Harald Roelle
    @author Parts of the documentation taken from Linux man pages by Xavier Leroy.
    @author Parts of the documentation taken from IEEE Standard 1003.1-2003.
 */
class LOW_thread_thread_POSIX : public LOW_thread_thread {

//=======================================================================================
public:
               
  //=====================================================================================
  //
  // constants
  //

  static const LOW_thread_Factory::schedPrio_t schedPrio_default = -1;  /**< indicator for default scheduling priority. */


  //=====================================================================================
  //
  // constructors
  //

  /** Destructor.
   */
  virtual ~LOW_thread_thread_POSIX();


  //=====================================================================================
  //
  // methods
  //

  /**
      @name Methods callable by any thread.
   */
  //!@{

  /** Indicates if the thread was started.

      @return  Bool indicating if thread was started.
   */
  virtual bool getIsStarted() const;

  
  /** Indicates if the thread has terminated.

      @return  Bool indicating if thread has terminated.
   */
  virtual bool getIsTerminated() const;

  
  /** Get the return value of the thread.

      @return  Return value of terminated thread.
      @throw  thread_thread_error  When thread has not terminated yet.
   */
  virtual int getReturnValue() const;


  /** Actually create the thread and starts it's runnable's run() method.

      Execution is synchronized with threadRunner() via createSyncMutex.
  
      @throw  thread_thread_error  On any error.
   */
  virtual void create();


  /** Cancel the execution of the thread.

      Note that the thread is not forced to quit.
      Cancellation of POSIX threads depends on cancel state and cancel type.
      The thread can then either ignore the request, honor it immediately, or
      defer it till it reaches a cancellation point.

      @throw  thread_thread_error  On any error.
   */
  virtual void cancel();

  
  /** Wait for the termination of the thread.

      Suspends the execution of the calling thread until the thread terminates, either
      by calling thread_exit() or by being cancelled.

      The thread must be in the joinable state: it must not have been detached using detach()
      or constructed as detached.

      When a joinable thread terminates, its memory resources (thread descriptor and stack) are
      not  deallocated until another thread performs join() on it. Therefore, join() must be
      called once for each joinable thread created to avoid memory leaks. When class is
      destructed the destructor takes care of this.

      At most one thread can wait for the termination of a thread. Calling join() on a thread
      on which another thread is already waiting for termination throws an exception.

      Implementation is mandatory.

      @throw  thread_thread_error  On any error.
      
      @see detach()
   */
  virtual int join();

  
  /** Put a running thread in the detached state.

      Puts the thread in the detached state. This guarantees that the memory resources
      consumed by will be freed immediately when it terminates. This also prevents
      other threads from synchronizing on the termination using join().

      A thread can be created initially in the detached state, appropriate constructor argument.
      In contrast, detach() applies to threads created in the joinable state, and which need to
      be put in the detached state later.

      After detach() completes, subsequent attempts to perform join() on it will fail. If
      another thread is already joining the thread at the time detach() is called, detach() does
      nothing and leaves the thread in the joinable state.

      @throw  thread_thread_error  On any error.
   */
  virtual void detach();


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

  
  /** Get cancelling behaviour.

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

      @param inRetVal  Return value to quit with.

      @throw  thread_thread_permission  Calling thread is not the object's thread.
      @throw  thread_thread_error  On any other error.
   */
  virtual void thread_exit( const int inRetVal);
  

  /** Test on pending cancellation.

      Method does nothing except testing for pending cancellation and executing it.
      Its purpose is to introduce explicit checks for cancellation in long sequences
      of code that do not call cancellation point functions otherwise.

      @throw  thread_thread_permission  Calling thread is not the object's thread.
      @throw  thread_thread_error  On any other error.

      @see cancel()
   */
  virtual void testCancel();


  /** Set cancelling behaviour.

      Changes the cancellation state for the calling thread -- that is, whether
      cancellation requests are ignored or not and when they are handled.

      @param inCancelAttrib  Cancelling handling attribute to set.

      @throw  thread_thread_permission  Calling thread is not the object's thread.
      @throw  thread_thread_error  On any other error.
   */
  virtual void setCancelAttrib( const LOW_thread_Factory::cancelAttrib_t inCancelAttrib);

  //!@}



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
      @param inDetachState
      @param inSchedPolicy
      @param inSchedPrio
   */
  LOW_thread_thread_POSIX( LOW_thread_runable *inRunable,
                           const LOW_thread_Factory::detachState_t inDetachState = LOW_thread_Factory::detachState_default,
                           const LOW_thread_Factory::schedPolicy_t inSchedPolicy = LOW_thread_Factory::schedPolicy_default,
                           const LOW_thread_Factory::schedPrio_t   inSchedPrio   = schedPrio_default);


  /** Constructor.
      Not publicly constructable. Use LOW_thread_Factory.
   */
  LOW_thread_thread_POSIX();

  
    
//=======================================================================================
private:

  //=====================================================================================
  //
  // attributes
  //

  LOW_thread_runable                      *myRunable;           /**< Object providing the run() method.    */
  pthread_attr_t                          initAttr;             /**< Initial thread attributes.            */
  pthread_t                               threadID;             /**< ID of the thread.                     */
  bool                                    isStarted;            /**< Indicates that thread was launched.   */
  bool                                    isTerminated;         /**< Indicated that thread has terminated. */
  int                                     returnValue;          /**< Return Value of run() method.         */
  LOW_thread_Factory::cancelAttrib_t      cancelAttrib;         /**< Current cancel attribute.             */
  LOW_thread_mutex                        *createSyncMutex;     /**< Mutex to synchronize with threadRunner on creation. */

  
  //=====================================================================================
  //
  // static methods
  //

  static void* threadRunner( void *inLaunchMe);
};

#endif
