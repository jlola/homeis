/*
 * DevicesTests.h
 *
 *  Created on: 19. 3. 2017
 *      Author: pc
 */

#ifndef SRC_TESTS_API_DEVICESTESTS_H_
#define SRC_TESTS_API_DEVICESTESTS_H_


#include "HomeIsStarter.h"

class DevicesTests : public testing::Test
{
protected:
	HomeIsStarter homeisStarter;
public:
	DevicesTests();
	void SetUp();
	void TearDown();
	virtual ~DevicesTests();
};


#endif /* SRC_TESTS_API_DEVICESTESTS_H_ */
