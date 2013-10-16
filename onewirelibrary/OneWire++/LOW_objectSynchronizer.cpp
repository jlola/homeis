/***************************************************************************
                          LOW_objectSynchronizer.cpp  -  description
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

 
#include "LOW_objectSynchronizer.h"


//=====================================================================================
//
// constructors
//

LOW_objectSynchronizer::LOW_objectSynchronizer()
{
  writeLockHolder  = 0;
  objectSyncRwlock = LOW_thread_Factory::new_rwlock();
}


LOW_objectSynchronizer::LOW_objectSynchronizer( const LOW_objectSynchronizer &/*inObjectSynchronizer*/)
{
  writeLockHolder  = 0;
  objectSyncRwlock = LOW_thread_Factory::new_rwlock();
}


LOW_objectSynchronizer::~LOW_objectSynchronizer()
{
  delete objectSyncRwlock;
}


//=====================================================================================
//
// operators
//


LOW_objectSynchronizer& LOW_objectSynchronizer::operator= ( const LOW_objectSynchronizer &inRightSide)
{
  if ( this == &inRightSide )  // do nothing on self assignment
    return *this;

  delete objectSyncRwlock;
  objectSyncRwlock = LOW_thread_Factory::new_rwlock();

  return *this;
}
