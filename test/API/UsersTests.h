/*
 * UsersTests.h
 *
 *  Created on: 10. 11. 2018
 *      Author: pc
 */

#ifndef TESTS_API_USERSTESTS_H_
#define TESTS_API_USERSTESTS_H_

#include "HomeIsStarter.h"
#include "UsersAtom.h"

class UsersTests  : public testing::Test
{
	string usersxml;
protected:
	HomeIsStarter* homeisStarter;
	UsersAtom* usersAtom;
public:
	UsersTests();
	void SetUp();
	void TearDown();
	virtual ~UsersTests();
};

#endif /* TESTS_API_USERSTESTS_H_ */
