/*
 * EmailSenderTests.h
 *
 *  Created on: 26. 4. 2018
 *      Author: pc
 */


#ifndef TESTS_DRIVERS_EMAILSENDERTESTS_H_
#define TESTS_DRIVERS_EMAILSENDERTESTS_H_

#include "gtest/gtest.h"
#include <stdint.h>
#include "fakeit.hpp"


using namespace fakeit;
namespace AF {

class EmailSenderTests : public testing::Test
{
public:
	EmailSenderTests();
	virtual ~EmailSenderTests();
};

}

#endif /* TESTS_DRIVERS_EMAILSENDERTESTS_H_ */
