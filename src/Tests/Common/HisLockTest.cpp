/*
 * HisLockTest.cpp
 *
 *  Created on: 22. 12. 2017
 *      Author: pc
 */
#ifdef HOMEISTEST

#include "HisLockTest.h"

namespace AF {


HisLockTests::HisLockTests() :
		mutex(NULL),thread(0),begin(false),end(false)
{

}

void HisLockTests::SetUp()
{
	mutex = HisLock::CreateMutex();
}

void HisLockTests::TearDown()
{

}

TEST_F(HisLockTests,HisLockTest_LockedThreadThenUnlocked)
{
	HisLock lock(mutex);
	Start();
	sleep(1);
	ASSERT_TRUE(begin);
	ASSERT_FALSE(end);
	lock.Unlock();
	sleep(1);
	ASSERT_TRUE(end);
	Stop();
}

TEST_F(HisLockTests,HisLockTest_NotLockedThreadNotBlock)
{
	Start();
	sleep(1);
	ASSERT_TRUE(begin);
	ASSERT_TRUE(end);
	Stop();
}

void* HisLockTests::ThreadFunction(void* obj)
{
	HisLockTests* test = static_cast<HisLockTests*>(obj);
	test->begin = true;
	HisLock lock(test->mutex);
	test->end = true;
	return NULL;
}

void HisLockTests::Start()
{
	pthread_create( &thread, NULL, HisLockTests::ThreadFunction, (void*)this );
}

void HisLockTests::Stop()
{
	pthread_join( thread, NULL);
}

HisLockTests::~HisLockTests()
{

}



}

#endif
