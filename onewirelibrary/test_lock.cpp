/***************************************************************************
                          test_lock.cpp  -  description
                             -------------------
    begin                : Mon Oct 6 2003
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
 
 
#include "LOW_platformMisc.h"
#include "LOW_thread_Factory.h"
 
#include "helper.h"



//class tsSimpleThread : public LOW_thread_runable {
//public:
//  char* name;
//  tsSimpleThread( char* inMyName)
//  {
//    name = inMyName;
//  };
//protected:
//  int run( LOW_thread_thread *inMyThread)
//  {
//    for ( int a=0; a<10; a++) {
//      printf( "Thread %s output: %d\n", name, a);
//      LOW_platformMisc::milliSleep( 1000);
//    }
//    return 0;
//  };
//};


void doSingleThreadWrWrLock()
{
  LOW_thread_rwlock *theLock = LOW_thread_Factory::new_rwlock();

  theLock->lockWrite();
  theLock->tryLockWrite();
    
  theLock->unlock();
  
  delete theLock;
}


void doSingleThreadWrRdLock()
{
  LOW_thread_rwlock *theLock = LOW_thread_Factory::new_rwlock();

  theLock->lockWrite();
  theLock->tryLockRead();
    
  theLock->unlock();
  
  delete theLock;
}


void doSingleThreadRdWrLock()
{
  LOW_thread_rwlock *theLock = LOW_thread_Factory::new_rwlock();

  theLock->lockRead();
  theLock->tryLockWrite();
    
  theLock->unlock();
  
  delete theLock;
}


void lockMenu( LOW_network& /*inNet*/)
{
  bool keepLooping = true;
  while ( keepLooping) {
    hlp_printDashline();
    printf( "Lock menu\n");
    printf( "\n");
    printf( "  <1> Same thread LockWrite -> TryLockWrite\n");
    printf( "  <2> Same thread LockWrite -> TryLockRead\n");
    printf( "  <3> Same thread LockRead -> TryLockWrite\n");
    printf( "\n");
    printf( "  <q> Back to main menu\n");

    int key = hlp_getMenuKey( "123q");


    try {
      switch( key ) {
        case '1': doSingleThreadWrWrLock();      break;
        case '2': doSingleThreadWrRdLock();      break;
        case '3': doSingleThreadRdWrLock();      break;
        case 'q': keepLooping=false;             break;
      }
    }
    catch( LOW_exception ex) {
      ex.logException();
    }
  }
}
