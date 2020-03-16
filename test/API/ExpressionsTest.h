/*
 * ExpressionsTest.h
 *
 *  Created on: 13. 3. 2017
 *      Author: pc
 */

#ifndef SRC_TEST_EXPRESSIONSTEST_H_
#define SRC_TEST_EXPRESSIONSTEST_H_

#include "gtest_src/gtest/gtest.h"
#include "File.h"
#include "UsersAtom.h"
#include "ExpressionAtom.h"
#include "fakeit.hpp"

using namespace fakeit;

namespace AF {

class ExpressionsTest : public testing::Test
{
protected:
	File file;
	HomeIsStarter* homeisStarter;
	Client* client;
	ExpressionAtom* expressionAtom;
	UsersAtom* usersAtom;
	string hashPassword;
public:
	ExpressionsTest();
	void SetUp();
	void TearDown();
	virtual ~ExpressionsTest();
};

} /* namespace AF */

#endif /* SRC_TEST_EXPRESSIONSTEST_H_ */
