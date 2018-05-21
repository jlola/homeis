/*
 * DS18B20Tests.h
 *
 *  Created on: 4. 12. 2017
 *      Author: pc
 */

#ifndef SRC_TESTS_UNITTESTS_DS18B20TESTS_H_
#define SRC_TESTS_UNITTESTS_DS18B20TESTS_H_

#include "gtest/gtest.h"

namespace AF {


class DS18B20Tests : public testing::Test
{
public:
	DS18B20Tests();
	virtual ~DS18B20Tests();
};

}

#endif /* SRC_TESTS_UNITTESTS_DS18B20TESTS_H_ */
