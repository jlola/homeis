/*
 * OneWireHandlerTests.h
 *
 *  Created on: 11. 3. 2018
 *      Author: pc
 */

#ifndef SRC_TESTS_DEVICES_HANDLERS_ONEWIREHANDLERTESTS_H_
#define SRC_TESTS_DEVICES_HANDLERS_ONEWIREHANDLERTESTS_H_

#include "fakeit.hpp"


using namespace fakeit;
namespace AF {

class OneWireHandlerTests : public testing::Test
{
public:
	Mock<IHisDevFactory> hisDevFactoryMock;

	void SetUp();
	OneWireHandlerTests();
	virtual ~OneWireHandlerTests();
};

}

#endif /* SRC_TESTS_DEVICES_HANDLERS_ONEWIREHANDLERTESTS_H_ */
