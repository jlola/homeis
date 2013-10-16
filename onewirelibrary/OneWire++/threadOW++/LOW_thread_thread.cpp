/***************************************************************************
                          LOW_thread_thread.cpp  -  description
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


#include "LOW_thread_thread.h"


//=====================================================================================
//
// constructors
//

LOW_thread_thread::~LOW_thread_thread()
{
}


//=====================================================================================
//
// methods
//

bool LOW_thread_thread::getDetachAvialable() const
{
  return false;
}


void LOW_thread_thread::detach()
{
  throw thread_thread_notAvailable( "detach() not implemented", __FILE__, __LINE__);
}


bool LOW_thread_thread::getScheduleModifyAvailable() const
{
  return false;
}


void LOW_thread_thread::setScheduling( const LOW_thread_Factory::schedPolicy_t /*inPolicy*/, const LOW_thread_Factory::schedPrio_t /*inPrio*/)
{
  throw thread_thread_notAvailable( "setScheduling() not implemented", __FILE__, __LINE__);
}

  
LOW_thread_Factory::schedPolicy_t LOW_thread_thread::getSchedPolicy() const
{
  throw thread_thread_notAvailable( "getSchedPolicy() not implemented", __FILE__, __LINE__);
}


LOW_thread_Factory::schedPrio_t LOW_thread_thread::getSchedPrio() const
{
  throw thread_thread_notAvailable( "getSchedPrio() not implemented", __FILE__, __LINE__);
}


bool LOW_thread_thread::getCancelAttribsAvialable() const
{
  return false;
}


LOW_thread_Factory::cancelAttrib_t LOW_thread_thread::getCancelAttrib() const
{
  throw thread_thread_notAvailable( "getCancelAttrib() not implemented", __FILE__, __LINE__);
}


void LOW_thread_thread::setCancelAttrib( const LOW_thread_Factory::cancelAttrib_t /*inCancelAttrib*/)
{
  throw thread_thread_notAvailable( "setCancelAttrib() not implemented", __FILE__, __LINE__);
}

