/***************************************************************************
                          test_thread.cpp  -  description
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


#include "LOW_platformMisc.h"
#include "LOW_thread_runable.h"
#include "LOW_thread_thread.h"
#include "LOW_thread_Factory.h"
 
#include "helper.h"



class tsSimpleThread : public LOW_thread_runable {
public:
  std::string name;
  tsSimpleThread( std::string inMyName)
  {
    name = inMyName;
  };
  virtual ~tsSimpleThread()
  {
  };
protected:
  virtual int run( LOW_thread_thread* /*inMyThread*/)
  {
    for ( int a=0; a<10; a++) {
      printf( "Thread %s output: %d\n", name.c_str(), a);
      LOW_platformMisc::milliSleep( 1000);
    }
    return 0;
  };
};


void doSimpleThreadTest()
{
  tsSimpleThread runner1 = tsSimpleThread( "1");
  tsSimpleThread runner2 = tsSimpleThread( "2");
  
  LOW_thread_thread *thread1 = LOW_thread_Factory::new_thread( &runner1);
  LOW_thread_thread *thread2 = LOW_thread_Factory::new_thread( &runner2);

  thread1->create();
  thread2->create();
  
  thread1->join();
  thread2->join();

  delete thread1;
  delete thread2;
}


void threadMenu( LOW_network& /*inNet*/)
{
  bool keepLooping = true;
  while ( keepLooping) {
    hlp_printDashline();
    printf( "Thread menu\n");
    printf( "\n");
    printf( "  <s> Simple thread test\n");
    printf( "\n");
    printf( "  <q> Back to main menu\n");

    int key = hlp_getMenuKey( "sq");


    try {
      switch( key ) {
        case 's': doSimpleThreadTest();      break;
        case 'q': keepLooping=false;         break;
      }
    }
    catch( LOW_exception ex) {
      ex.logException();
    }
  }
}
