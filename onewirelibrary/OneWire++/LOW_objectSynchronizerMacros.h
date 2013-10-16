/***************************************************************************
                          LOW_objectSynchronizerMacros.h  -  description
                             -------------------
    begin                : Sun Oct 19 2003
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


//
// uncomment the following preprocessor directive to get some debugging
//
#define DEBUG_LOCKING


 
//
// Shortcut macro for lock inside methods
//

#ifdef DEBUG_LOCKING

#define __LOW_SYNCHRONIZE_METHOD_READ__        __synchronizeMethodRead      __methodSyncer = __synchronizeMethodRead( *this, __FILE__, __LINE__);
#define __LOW_SYNCHRONIZE_METHOD_WRITE__       __synchronizeMethodWrite     __methodSyncer = __synchronizeMethodWrite( *this, __FILE__, __LINE__);
#define __LOW_SYNCHRONIZE_METHOD_READ_WEAK__   __synchronizeMethodReadWeak  __methodSyncer = __synchronizeMethodReadWeak( *this, __FILE__, __LINE__);
#define __LOW_SYNCHRONIZE_METHOD_WRITE_WEAK__  __synchronizeMethodWriteWeak __methodSyncer = __synchronizeMethodWriteWeak( *this, __FILE__, __LINE__);

#else

#define __LOW_SYNCHRONIZE_METHOD_READ__        __synchronizeMethodRead      __methodSyncer = __synchronizeMethodRead( *this);
#define __LOW_SYNCHRONIZE_METHOD_WRITE__       __synchronizeMethodWrite     __methodSyncer = __synchronizeMethodWrite( *this);
#define __LOW_SYNCHRONIZE_METHOD_READ_WEAK__   __synchronizeMethodReadWeak  __methodSyncer = __synchronizeMethodReadWeak( *this);
#define __LOW_SYNCHRONIZE_METHOD_WRITE_WEAK__  __synchronizeMethodWriteWeak __methodSyncer = __synchronizeMethodWriteWeak( *this);

#endif


//
// Macros for static synchronization
//
#define __LOW_SYNCHRONIZE_DEFINE_PROTECTED_LOCK__    static LOW_thread_rwlock *__staticSyncRwlockSingleton; \
                                                     static LOW_platformMiscFactory::threadIdent_t  __writeLockHolder;

#define __LOW_SYNCHRONIZE_INIT_PROTECTED_LOCK__(X)   LOW_thread_rwlock* X::__staticSyncRwlockSingleton = 0; \
                                                     LOW_platformMiscFactory::threadIdent_t  X::__writeLockHolder = 0;

#ifdef DEBUG_LOCKING

#define __LOW_SYNCHRONIZE_STATIC_READ__        LOW_objectSynchronizer::__synchronizeStaticRead __staticSyncer = \
                                                 LOW_objectSynchronizer::__synchronizeStaticRead( &__staticSyncRwlockSingleton, __FILE__, __LINE__);
#define __LOW_SYNCHRONIZE_STATIC_WRITE__       LOW_objectSynchronizer::__synchronizeStaticWrite __staticSyncer = \
                                                 LOW_objectSynchronizer::__synchronizeStaticWrite( &__staticSyncRwlockSingleton, __writeLockHolder, __FILE__, __LINE__);
#define __LOW_SYNCHRONIZE_STATIC_READ_WEAK__   LOW_objectSynchronizer::__synchronizeStaticReadWeak __staticSyncer = \
                                                 LOW_objectSynchronizer::__synchronizeStaticReadWeak( &__staticSyncRwlockSingleton, __writeLockHolder, __FILE__, __LINE__);
#define __LOW_SYNCHRONIZE_STATIC_WRITE_WEAK__  LOW_objectSynchronizer::__synchronizeStaticWriteWeak __staticSyncer = \
                                                 LOW_objectSynchronizer::__synchronizeStaticWriteWeak( &__staticSyncRwlockSingleton, __writeLockHolder, __FILE__, __LINE__);
#else

#define __LOW_SYNCHRONIZE_STATIC_READ__        LOW_objectSynchronizer::__synchronizeStaticRead __staticSyncer = \
                                                 LOW_objectSynchronizer::__synchronizeStaticRead( &__staticSyncRwlockSingleton);
#define __LOW_SYNCHRONIZE_STATIC_WRITE__       LOW_objectSynchronizer::__synchronizeStaticWrite __staticSyncer = \
                                                 LOW_objectSynchronizer::__synchronizeStaticWrite( &__staticSyncRwlockSingleton, __writeLockHolder);
#define __LOW_SYNCHRONIZE_STATIC_READ_WEAK__   LOW_objectSynchronizer::__synchronizeStaticReadWeak __staticSyncer = \
                                                 LOW_objectSynchronizer::__synchronizeStaticReadWeak( &__staticSyncRwlockSingleton, __writeLockHolder);
#define __LOW_SYNCHRONIZE_STATIC_WRITE_WEAK__  LOW_objectSynchronizer::__synchronizeStaticWriteWeak __staticSyncer = \
                                                 LOW_objectSynchronizer::__synchronizeStaticWriteWeak( &__staticSyncRwlockSingleton, __writeLockHolder);

#endif
