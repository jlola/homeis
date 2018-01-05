/*
 * HisLock.h
 *
 *  Created on: Nov 9, 2013
 *      Author: Josef Lola
 */

#ifndef HISLOCK_H_
#define HISLOCK_H_

#if defined (__cplusplus)
extern "C" {
#endif

#include "linuxcs.h"

#if defined (__cplusplus)
}
#endif

class HisLock {
	bool selfCreated;
	LPCRITICAL_SECTION csLock;
public:
	static LPCRITICAL_SECTION CreateMutex();
	static void DeleteMutex(LPCRITICAL_SECTION & pmutex);
	HisLock();
	HisLock(LPCRITICAL_SECTION & pmutex);
	void Lock();
	void Unlock();
	virtual ~HisLock();
};

#endif /* HISLOCK_H_ */
