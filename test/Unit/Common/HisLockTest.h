/*
 * HisLockTest.h
 *
 *  Created on: 22. 12. 2017
 *      Author: pc
 */

#ifndef SRC_TESTS_COMMON_HISLOCKTEST_H_
#define SRC_TESTS_COMMON_HISLOCKTEST_H_

#include <pthread.h>
#include "gtest/gtest.h"
#include "HisLock.h"

namespace AF {

class HisLockTests : public testing::Test
{
protected:
	LPCRITICAL_SECTION mutex;
	pthread_t thread;
	bool begin;
	bool end;
public:
	void SetUp();
	void TearDown();
	HisLockTests();
	void Start();
	void Stop();
	static void* ThreadFunction(void* obj);
	virtual ~HisLockTests();
};

} /* namespace AF */


#endif /* SRC_TESTS_COMMON_HISLOCKTEST_H_ */
