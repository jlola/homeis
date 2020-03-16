/*
 * UserManagerTests.h
 *
 *  Created on: 19. 10. 2018
 *      Author: pc
 */

#ifndef TESTS_UNIT_AUTH_USERMANAGERTESTS_H_
#define TESTS_UNIT_AUTH_USERMANAGERTESTS_H_

#include "gtest/gtest.h"
#include <stdint.h>
#include <string>
#include "fakeit.hpp"
#include <string>

class UserManagerTests : public testing::Test
{
public:
	void SetUp();
	void TearDown();
	std::string usersxml;
	UserManagerTests();
	virtual ~UserManagerTests();
};

#endif /* TESTS_UNIT_AUTH_USERMANAGERTESTS_H_ */
