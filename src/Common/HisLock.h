/*
 * HisLock.h
 *
 *  Created on: Nov 9, 2013
 *      Author: Josef Lola
 */

#ifndef HISLOCK_H_
#define HISLOCK_H_

#include "LOW_thread_mutex.h"
#include "LOW_thread_Factory.h"

class HisLock {
	LOW_thread_mutex* mutex;
public:
	static LOW_thread_mutex* CreateMutex();
	HisLock(LOW_thread_mutex* pmutex);
	virtual ~HisLock();
};

#endif /* HISLOCK_H_ */
