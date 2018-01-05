/*
 * ExpressionsTest.h
 *
 *  Created on: 13. 3. 2017
 *      Author: pc
 */

#ifndef SRC_TEST_EXPRESSIONSTEST_H_
#define SRC_TEST_EXPRESSIONSTEST_H_

#include "gtest_src/gtest/gtest.h"

namespace AF {

class ExpressionsTest : public testing::Test
{
protected:
	SSerPortConfig modbussim;
	Client client;
	HomeIsServer* server;
	IModbusProvider* modbusprovider;
public:
	ExpressionsTest();
	void SetUp();
	void TearDown();
	virtual ~ExpressionsTest();
};

} /* namespace AF */

#endif /* SRC_TEST_EXPRESSIONSTEST_H_ */
