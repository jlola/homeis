/*
 * HisLock.cpp
 *
 *  Created on: Nov 9, 2013
 *      Author: Josef Lola
 */

#include "HisLock.h"

LOW_thread_mutex* HisLock::CreateMutex()
{
	return LOW_thread_Factory::new_mutex(LOW_thread_mutex::mutexKind_recursive);
}

HisLock::HisLock(LOW_thread_mutex* pmutex) :
			mutex(pmutex)
{
	mutex->lock();
}

HisLock::~HisLock()
{
	mutex->unlock();
}
