/***************************************************************************
                          LOW_thread_mutex.cpp  -  description
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

 
#include "LOW_thread_mutex.h"



//=====================================================================================
//
// constructors
//


LOW_thread_mutex::LOW_thread_mutex( const mutexKind_t inMutexKind) :
  mutexKind( inMutexKind)
{
}


LOW_thread_mutex::~LOW_thread_mutex()
{
}



//=====================================================================================
//
// public methods
//


LOW_thread_mutex::mutexKind_t LOW_thread_mutex::getKind() const
{
  return mutexKind;
}
