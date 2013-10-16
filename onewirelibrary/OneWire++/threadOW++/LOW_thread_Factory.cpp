/***************************************************************************
                          LOW_thread_Factory.cpp  -  description
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

 
#include "LOW_thread_Factory.h"
#include "LOW_thread_mutex_POSIX.h"
#include "LOW_thread_rwlock_POSIX.h"
#include "LOW_thread_thread_POSIX.h"


//=====================================================================================
//
// constructors
//

LOW_thread_Factory::LOW_thread_Factory()
{
}


LOW_thread_Factory::~LOW_thread_Factory()
{
}


//=====================================================================================
//
// static factory
//

LOW_thread_mutex* LOW_thread_Factory::new_mutex( const LOW_thread_mutex::mutexKind_t inMutexKind)
{
#ifdef __linux__
  return new LOW_thread_mutex_POSIX( inMutexKind);
#endif
}


LOW_thread_rwlock* LOW_thread_Factory::new_rwlock()
{
#ifdef __linux__
  return new LOW_thread_rwlock_POSIX();
#endif
}


LOW_thread_thread* LOW_thread_Factory::new_thread( LOW_thread_runable *inRunable)
{
#ifdef __linux__
  return new LOW_thread_thread_POSIX( inRunable);
#endif
}

