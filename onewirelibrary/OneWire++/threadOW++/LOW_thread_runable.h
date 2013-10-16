/***************************************************************************
                          LOW_thread_runable.h  -  description
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

#ifndef LOW_THREAD_RUNABLE_H
#define LOW_THREAD_RUNABLE_H


#include "LOW_thread_thread.h"


/** Interface class for objects implementing the "body" of a thread.

    This class is thread-safe.

    @see LOW_thread_thread

    @author Harald Roelle
 */
class LOW_thread_runable {

//=======================================================================================
public:

  //=====================================================================================
  //
  // friends
  //

  /** Needed to grant access to the protected run() method.
      Gives LOW_thread_thread the permission to access the run() method.
   */
  friend void* LOW_thread_thread::threadRunner( void*);

  
  //=====================================================================================
  //
  // methods
  //

  /** Method to be called when thread gets started.
      The thread terminates when this method returns.

      @param inMyThread  Pointer to the thread.

      @return Return value of the thread on termination.
   */
  virtual int run( LOW_thread_thread *inMyThread) = 0;

};

#endif
