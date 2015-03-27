/***************************************************************************
                          LOW_objectSynchronizer.h  -  description
                             -------------------
    begin                : Sat Oct 4 2003
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

#ifndef LOW_OBJECTSYNCHRONIZER_H
#define LOW_OBJECTSYNCHRONIZER_H


#include "platformOW++/LOW_platformMisc.h"
#include "platformOW++/LOW_platformMiscFactory.h"
#include "LOW_helper_msglog.h"
#include "threadOW++/LOW_thread_Factory.h"
#include "threadOW++/LOW_thread_rwlock.h"



#ifdef DEBUG_LOCKING
#include <string>
#endif


#include "LOW_objectSynchronizerMacros.h"


/** Mix-in class to synchronize access to methods on object basis.

    A class which needs synchronization to be thread-save can subclass
    this class. To serialize the access to a method, simply insert one
    of the following macros at the beginning of the method:

      - <b>__LOW_SYNCHRONIZE_METHOD_READ__</b>: Obtains a read lock.
      - <b>__LOW_SYNCHRONIZE_METHOD_WRITE__</b>: Obtains a write lock.
      - <b>__LOW_SYNCHRONIZE_METHOD_READ_WEAK__</b>: Obtains a weak read lock.
      - <b>__LOW_SYNCHRONIZE_METHOD_WRITE_WEAK__</b>: Obtains a weak write lock.

    Note that synchronization is done on object basis.

    The implemtation follows the "locking is creation" design pattern and
    uses a read-write lock. Therefore reentrant calls or successive locking
    in sub-called methods does no harm.

    For locking in static methods some preprocessor macros are provided:
      - <b>__LOW_SYNCHRONIZE_DEFINE_PROTECTED_LOCK__</b>: Insert in protected part to declare mutex.
      - <b>__LOW_SYNCHRONIZE_INIT_PROTECTED_LOCK__</b>: Insert in implementation file to initialize mutex.
      - <b>__LOW_SYNCHRONIZE_STATIC_READ__</b>: Obtains a read lock.
      - <b>__LOW_SYNCHRONIZE_STATIC_WRITE__</b>: Obtains a write lock.
      - <b>__LOW_SYNCHRONIZE_STATIC_READ_WEAK__</b>: Obtains a weak read lock.
      - <b>__LOW_SYNCHRONIZE_STATIC_WRITE_WEAK__</b>: Obtains a weak write lock.

    This class is thread-safe.

    @author Harald Roelle
 */
class LOW_objectSynchronizer {

//=======================================================================================
public:


  //=====================================================================================
  //
  // operators
  //


  LOW_objectSynchronizer& operator= ( const LOW_objectSynchronizer &inRightSide);


  //=====================================================================================
  //
  // static locks
  //

  /** Read locking class to ensure exclusive access.

      The class is intended to be used in a "locking is creation" design pattern.
      On creation a read lock on a rwlock is optained, and on destruction
      the rwlock is released.
   */
  class __synchronizeStaticRead {
    public:

      /** Obtain the read lock.
          Inlined for performance reasons.
          @param inObjectSynchronizer  Reference to the object to synchronize on.
       */
#ifdef DEBUG_LOCKING
      inline __synchronizeStaticRead( LOW_thread_rwlock **inRwlockSingleton, std::string inFile, int inLine) :
#else
      inline __synchronizeStaticRead( LOW_thread_rwlock **inRwlockSingleton) :
#endif
        staticRwLock( inRwlockSingleton)
      {
        if ( *staticRwLock == 0 )
          *staticRwLock = LOW_thread_Factory::new_rwlock();
#ifdef DEBUG_LOCKING
        try {
          (*staticRwLock)->tryLockRead(); // try to obtain the lock, if it fail we get a logged message
          LOW_helper_msglog::printDebug( LOW_helper_msglog::objSync_getLock_dl,
            "__synchronizeStaticRead: obtained lock from file %s, line %d\n", inFile.c_str(), inLine);
        }
        catch ( LOW_thread_rwlock::thread_rwlock_error & ex) {
          LOW_helper_msglog::printDebug( LOW_helper_msglog::objSync_lockFailed_dl,
            "__synchronizeStaticRead: failed to obtain lock from file %s, line %d\n", inFile.c_str(), inLine);
          (*staticRwLock)->lockRead();  // now really block in the lock
        }
#else
        (*staticRwLock)->lockRead();
#endif
      };

      /** Release the lock.
          Inlined for performance reasons.
       */
      inline ~__synchronizeStaticRead()
      {
        (*staticRwLock)->unlock();
      };

    private:
      LOW_thread_rwlock **staticRwLock; /**< Pointer to pointer to the static lock. */
  };


  /** Write locking class to ensure exclusive access.

      The class is intended to be used in a "locking is creation" design pattern.
      On creation a write lock on a rwlock is optained, and on destruction
      the rwlock is released.
   */
  class __synchronizeStaticWrite {
    public:

      /** Obtain the write lock.
          Inlined for performance reasons.
          @param inObjectSynchronizer  Reference to the object to synchronize on.
       */
#ifdef DEBUG_LOCKING
      inline __synchronizeStaticWrite( LOW_thread_rwlock **inRwlockSingleton, LOW_platformMiscFactory::threadIdent_t &inWriteLockHolder, std::string inFile, int inLine) :
#else
      inline __synchronizeStaticWrite( LOW_thread_rwlock **inRwlockSingleton, LOW_platformMiscFactory::threadIdent_t &inWriteLockHolder) :
#endif
        staticRwLock( inRwlockSingleton),
        writeLockHolder(inWriteLockHolder)
      {
        if ( *staticRwLock == 0 )
          *staticRwLock = LOW_thread_Factory::new_rwlock();
#ifdef DEBUG_LOCKING
        try {
          (*staticRwLock)->tryLockWrite(); // try to obtain the lock, if it fail we get a logged message
          LOW_helper_msglog::printDebug( LOW_helper_msglog::objSync_getLock_dl,
            "__synchronizeStaticWrite: obtained lock from file %s, line %d\n", inFile.c_str(), inLine);
        }
        catch ( LOW_thread_rwlock::thread_rwlock_error & ex) {
          LOW_helper_msglog::printDebug( LOW_helper_msglog::objSync_lockFailed_dl,
            "__synchronizeStaticWrite: failed to obtain lock from file %s, line %d\n", inFile.c_str(), inLine);
          (*staticRwLock)->lockWrite();  // now really block in the lock
        }
#else
        (*staticRwLock)->lockWrite();
#endif
        // no need to make that block exclusive:
        //  - a foreign thread will block anyway waiting for a lock
        //  - our own thread cannot check as we are still here
        writeLockHolder = LOW_platformMisc::getThreadID();  // set write lock indicator
      };

      /** Release the lock.
          Inlined for performance reasons.
       */
      inline ~__synchronizeStaticWrite()
      {
        // no need to make that block exclusive:
        //  - a foreign thread will block anyway waiting for a lock
        //  - our own thread cannot check as we are still here
        writeLockHolder = 0;  // clear write lock indicator
        (*staticRwLock)->unlock();
      };

    private:
      LOW_thread_rwlock                       **staticRwLock; /**< Pointer to pointer to the static lock. */
      LOW_platformMiscFactory::threadIdent_t  &writeLockHolder;
  };


  /** Weak read locking class to ensure exclusive access.

      Works similar to __synchronizeMethodRead with one difference in the following situation:
      If the calling thread already has a <b>write</b> lock then <b>no blocking</b>
      will occur and <b>no lock</b> will be obtained.
   */
  class __synchronizeStaticReadWeak {
    public:

      /** Obtain the weak read lock.
          Inlined for performance reasons.
          @param inObjectSynchronizer  Reference to the object to synchronize on.
       */
#ifdef DEBUG_LOCKING
      inline __synchronizeStaticReadWeak( LOW_thread_rwlock **inRwlockSingleton, LOW_platformMiscFactory::threadIdent_t &inWriteLockHolder, std::string inFile, int inLine) :
#else
      inline __synchronizeStaticReadWeak( LOW_thread_rwlock **inRwlockSingleton, LOW_platformMiscFactory::threadIdent_t &inWriteLockHolder) :
#endif
        staticRwLock( inRwlockSingleton)
      {
        if ( *staticRwLock == 0 )
          *staticRwLock = LOW_thread_Factory::new_rwlock();
        wasObtainedWeakly = false;
        try {
          (*staticRwLock)->tryLockRead();
          LOW_helper_msglog::printDebug( LOW_helper_msglog::objSync_getLock_dl,
            "__synchronizeStaticReadWeak: obtained lock from file %s, line %d\n", inFile.c_str(), inLine);
        }
        catch ( LOW_thread_rwlock::thread_rwlock_error & ex) {
          if ( LOW_platformMisc::getThreadID() == inWriteLockHolder )
            wasObtainedWeakly = true;  // if it was me who has the lock, "obtain" it weakly
          else {
#ifdef DEBUG_LOCKING
            LOW_helper_msglog::printDebug( LOW_helper_msglog::objSync_lockFailed_dl,
              "__synchronizeStaticReadWeak: failed to obtain lock from file %s, line %d\n", inFile.c_str(), inLine);
#endif
            (*staticRwLock)->lockRead();  // if it wasn't me who obtained the lock, really block
          }
        }
      };

      /** Release the weak lock.
          Inlined for performance reasons.
       */
      inline ~__synchronizeStaticReadWeak()
      {
        if ( ! wasObtainedWeakly )
          (*staticRwLock)->unlock();
      };

    private:
      LOW_thread_rwlock  **staticRwLock;      /**< Pointer to pointer to the static lock. */
      bool               wasObtainedWeakly;   /**< Indicate if lock was really obtained */
  };


  /** Weak write locking class to ensure exclusive access.

      Works similar to __synchronizeMethodRead with one difference in the following situation:
      If the calling thread already has a <b>write</b> lock then <b>no blocking</b>
      will occur and <b>no lock</b> will be obtained.
   */
  class __synchronizeStaticWriteWeak {
    public:

      /** Obtain the weak write lock.
          Inlined for performance reasons.
          @param inObjectSynchronizer  Reference to the object to synchronize on.
       */
#ifdef DEBUG_LOCKING
      inline __synchronizeStaticWriteWeak( LOW_thread_rwlock **inRwlockSingleton, LOW_platformMiscFactory::threadIdent_t &inWriteLockHolder, std::string inFile, int inLine) :
#else
      inline __synchronizeStaticWriteWeak( LOW_thread_rwlock **inRwlockSingleton, LOW_platformMiscFactory::threadIdent_t &inWriteLockHolder) :
#endif
        staticRwLock( inRwlockSingleton),
        writeLockHolder(inWriteLockHolder)
      {
        if ( *staticRwLock == 0 )
          *staticRwLock = LOW_thread_Factory::new_rwlock();
        wasObtainedWeakly = false;
        try {
          (*staticRwLock)->tryLockWrite();
          // no need to make that block exclusive:
          //  - a foreign thread will block anyway waiting for a lock
          //  - our own thread cannot check as we are still here
          writeLockHolder = LOW_platformMisc::getThreadID();  // set write lock indicator
          LOW_helper_msglog::printDebug( LOW_helper_msglog::objSync_getLock_dl,
            "__synchronizeStaticWriteWeak: obtained lock from file %s, line %d\n", inFile.c_str(), inLine);
        }
        catch ( LOW_thread_rwlock::thread_rwlock_error & ex) {
          if ( LOW_platformMisc::getThreadID() == writeLockHolder )
            wasObtainedWeakly = true;  // if it was me who has the lock, "obtain" it weakly
          else {
#ifdef DEBUG_LOCKING
            LOW_helper_msglog::printDebug( LOW_helper_msglog::objSync_lockFailed_dl,
              "__synchronizeStaticWriteWeak: failed to obtain lock from file %s, line %d\n", inFile.c_str(), inLine);
#endif
            (*staticRwLock)->lockWrite();  // if it wasn't me who obtained the lock, really block
          }
        }
      };

      /** Release the weak write lock.
          Inlined for performance reasons.
       */
      inline ~__synchronizeStaticWriteWeak()
      {
        if ( ! wasObtainedWeakly ) {
          // no need to make that block exclusive:
          //  - a foreign thread will block anyway waiting for a lock
          //  - our own thread cannot check as we are still here
          writeLockHolder = 0;  // clear write lock indicator
          (*staticRwLock)->unlock();
        }
      };

    private:
      LOW_thread_rwlock                       **staticRwLock;      /**< Pointer to pointer to the static lock. */
      bool                                    wasObtainedWeakly;   /**< Indicate if lock was really obtained */
      LOW_platformMiscFactory::threadIdent_t  &writeLockHolder;
  };



//=======================================================================================
protected: 

  //=====================================================================================
  //
  // locks
  //

  /** Read locking class to ensure exclusive access.

      The class is intended to be used in a "locking is creation" design pattern.
      On creation a read lock on a rwlock is optained, and on destruction
      the rwlock is released.
   */
  class __synchronizeMethodRead {
    public:

      /** Obtain the read lock.
          Inlined for performance reasons.
          @param inObjectSynchronizer  Reference to the object to synchronize on.
       */
#ifdef DEBUG_LOCKING
      inline __synchronizeMethodRead( const LOW_objectSynchronizer &inObjectSynchronizer, std::string inFile, int inLine) :
#else
      inline __synchronizeMethodRead( const LOW_objectSynchronizer &inObjectSynchronizer) :
#endif
        objectSynchronizer( inObjectSynchronizer)
      {
#ifdef DEBUG_LOCKING
        try {
          objectSynchronizer.objectSyncRwlock->tryLockRead(); // try to obtain the lock, if it fail we get a logged message
          LOW_helper_msglog::printDebug( LOW_helper_msglog::objSync_getLock_dl,
            "__synchronizeMethodRead: obtained read lock from file %s, line %d\n", inFile.c_str(), inLine);
        }
        catch ( LOW_thread_rwlock::thread_rwlock_error & ex) {
          LOW_helper_msglog::printDebug( LOW_helper_msglog::objSync_lockFailed_dl,
            "__synchronizeMethodRead: failed to obtain read lock from file %s, line %d\n", inFile.c_str(), inLine);
          objectSynchronizer.objectSyncRwlock->lockRead();  // now really block in the lock
        }
#else
        objectSynchronizer.objectSyncRwlock->lockRead();
#endif
      };

      /** Release the lock.
          Inlined for performance reasons.
       */
      inline ~__synchronizeMethodRead()
      {
        objectSynchronizer.objectSyncRwlock->unlock();
      };

    private:
      const LOW_objectSynchronizer &objectSynchronizer; /**< Reference to the object to lock on. */
  };


  /** Write locking class to ensure exclusive access.

      The class is intended to be used in a "locking is creation" design pattern.
      On creation a write lock on a rwlock is optained, and on destruction
      the rwlock is released.
   */
  class __synchronizeMethodWrite {
    public:

      /** Obtain the write lock.
          Inlined for performance reasons.
          @param inObjectSynchronizer  Reference to the object to synchronize on.
       */
#ifdef DEBUG_LOCKING
      inline __synchronizeMethodWrite( LOW_objectSynchronizer &inObjectSynchronizer, std::string inFile, int inLine) :
#else
      inline __synchronizeMethodWrite( LOW_objectSynchronizer &inObjectSynchronizer) :
#endif
        objectSynchronizer( inObjectSynchronizer)
      {
#ifdef DEBUG_LOCKING
        try {
          objectSynchronizer.objectSyncRwlock->tryLockWrite(); // try to obtain the lock, if it fail we get a logged message
          LOW_helper_msglog::printDebug( LOW_helper_msglog::objSync_getLock_dl,
            "__synchronizeMethodWrite: obtained write lock from file %s, line %d\n", inFile.c_str(), inLine);
        }
        catch ( LOW_thread_rwlock::thread_rwlock_error & ex) {
          LOW_helper_msglog::printDebug( LOW_helper_msglog::objSync_lockFailed_dl,
            "__synchronizeMethodWrite: failed to obtain write lock from file %s, line %d\n", inFile.c_str(), inLine);
          objectSynchronizer.objectSyncRwlock->lockWrite();  // now really block in the lock
        }
#else
        objectSynchronizer.objectSyncRwlock->lockWrite();
#endif
        // no need to make that block exclusive:
        //  - a foreign thread will block anyway waiting for a lock
        //  - our own thread cannot check as we are still here
        objectSynchronizer.writeLockHolder = LOW_platformMisc::getThreadID();  // set write lock indicator
      };

      /** Release the lock.
          Inlined for performance reasons.
       */
      inline ~__synchronizeMethodWrite()
      {
        // no need to make that block exclusive:
        //  - a foreign thread will block anyway waiting for a lock
        //  - our own thread cannot check as we are still here
        objectSynchronizer.writeLockHolder = 0;  // clear write lock indicator
        objectSynchronizer.objectSyncRwlock->unlock();
      };

    private:
      LOW_objectSynchronizer &objectSynchronizer; /**< Reference to the object to lock on. */
  };


  /** Weak read locking class to ensure exclusive access.

      Works similar to __synchronizeMethodRead with one difference in the following situation:
      If the calling thread already has a <b>write</b> lock then <b>no blocking</b>
      will occur and <b>no lock</b> will be obtained.
   */
  class __synchronizeMethodReadWeak {
    public:

      /** Obtain the weak read lock.
          Inlined for performance reasons.
          @param inObjectSynchronizer  Reference to the object to synchronize on.
       */
#ifdef DEBUG_LOCKING
      inline __synchronizeMethodReadWeak( const LOW_objectSynchronizer &inObjectSynchronizer, std::string inFile, int inLine) :
#else
      inline __synchronizeMethodReadWeak( const LOW_objectSynchronizer &inObjectSynchronizer) :
#endif
        objectSynchronizer( inObjectSynchronizer)
      {
        wasObtainedWeakly = false;
        try {
          objectSynchronizer.objectSyncRwlock->tryLockRead();
            LOW_helper_msglog::printDebug( LOW_helper_msglog::objSync_getLock_dl,
              "__synchronizeMethodReadWeak: obtain weak read lock from file %s, line %d\n", inFile.c_str(), inLine);
        }
        catch ( LOW_thread_rwlock::thread_rwlock_error & ex) {
          if ( LOW_platformMisc::getThreadID() == objectSynchronizer.writeLockHolder )
            wasObtainedWeakly = true;  // if it was me who has the lock, "obtain" it weakly
          else {
#ifdef DEBUG_LOCKING
            LOW_helper_msglog::printDebug( LOW_helper_msglog::objSync_lockFailed_dl,
              "__synchronizeMethodReadWeak: failed to obtain lock from file %s, line %d\n", inFile.c_str(), inLine);
#endif
            objectSynchronizer.objectSyncRwlock->lockRead();  // if it wasn't me who obtained the lock, really block
          }
        }
      };

      /** Release the weak lock.
          Inlined for performance reasons.
       */
      inline ~__synchronizeMethodReadWeak()
      {
        if ( ! wasObtainedWeakly )
          objectSynchronizer.objectSyncRwlock->unlock();
      };

    private:
      const LOW_objectSynchronizer  &objectSynchronizer; /**< Reference to the object to lock on. */
      bool                          wasObtainedWeakly;   /**< Indicate if lock was really obtained */
  };


  /** Weak write locking class to ensure exclusive access.

      Works similar to __synchronizeMethodRead with one difference in the following situation:
      If the calling thread already has a <b>write</b> lock then <b>no blocking</b>
      will occur and <b>no lock</b> will be obtained.
   */
  class __synchronizeMethodWriteWeak {
    public:

      /** Obtain the weak write lock.
          Inlined for performance reasons.
          @param inObjectSynchronizer  Reference to the object to synchronize on.
       */
#ifdef DEBUG_LOCKING
      inline __synchronizeMethodWriteWeak( LOW_objectSynchronizer &inObjectSynchronizer, std::string inFile, int inLine) :
#else
      inline __synchronizeMethodWriteWeak( LOW_objectSynchronizer &inObjectSynchronizer) :
#endif
        objectSynchronizer( inObjectSynchronizer)
      {
        wasObtainedWeakly = false;
        try {
          objectSynchronizer.objectSyncRwlock->tryLockWrite();
          LOW_helper_msglog::printDebug( LOW_helper_msglog::objSync_getLock_dl,
            "__synchronizeMethodWriteWeak: obtained weak write lock from file %s, line %d\n", inFile.c_str(), inLine);
          // no need to make that block exclusive:
          //  - a foreign thread will block anyway waiting for a lock
          //  - our own thread cannot check as we are still here
          objectSynchronizer.writeLockHolder = LOW_platformMisc::getThreadID();  // set write lock indicator
        }
        catch ( LOW_thread_rwlock::thread_rwlock_error & ex) {
          if ( LOW_platformMisc::getThreadID() == objectSynchronizer.writeLockHolder )
            wasObtainedWeakly = true;  // if it was me who has the lock, "obtain" it weakly
          else {
#ifdef DEBUG_LOCKING
            LOW_helper_msglog::printDebug( LOW_helper_msglog::objSync_lockFailed_dl,
              "__synchronizeMethodWriteWeak: failed to obtain lock from file %s, line %d\n", inFile.c_str(), inLine);
#endif
            objectSynchronizer.objectSyncRwlock->lockWrite();  // if it wasn't me who obtained the lock, really block
          }
        }
      };

      /** Release the weak write lock.
          Inlined for performance reasons.
       */
      inline ~__synchronizeMethodWriteWeak()
      {
        if ( ! wasObtainedWeakly ) {
          // no need to make that block exclusive:
          //  - a foreign thread will block anyway waiting for a lock
          //  - our own thread cannot check as we are still here
          objectSynchronizer.writeLockHolder = 0;  // clear write lock indicator
          objectSynchronizer.objectSyncRwlock->unlock();
        }
      };

    private:
      LOW_objectSynchronizer  &objectSynchronizer;      /**< Reference to the object to lock on. */
      bool                          wasObtainedWeakly;  /**< Indicate if lock was really obtained */
  };


  //=====================================================================================
  //
  // constructors
  //

  
  /** Constructor.
   */
  LOW_objectSynchronizer();


  /** Copy constructor.
   */
  LOW_objectSynchronizer( const LOW_objectSynchronizer &inObjectSynchronizer);


  /** Destructor.
   */
  virtual ~LOW_objectSynchronizer();
  
  
//=======================================================================================
private:

  //=====================================================================================
  //
  // friends
  //

  friend class __synchronizeMethodRead;
  friend class __synchronizeMethodWrite;
  friend class __synchronizeMethodReadWeak;
  friend class __synchronizeMethodWriteWeak;

  
  //=====================================================================================
  //
  // attributes
  //

  LOW_thread_rwlock                       *objectSyncRwlock; /**< Rwlock used for locking. */
  LOW_platformMiscFactory::threadIdent_t  writeLockHolder;   /**< Thread id of the one who's holding a write lock */
};

#endif
