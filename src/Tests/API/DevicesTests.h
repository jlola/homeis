/*
 * DevicesTests.h
 *
 *  Created on: 19. 3. 2017
 *      Author: pc
 */

#ifndef SRC_TESTS_API_DEVICESTESTS_H_
#define SRC_TESTS_API_DEVICESTESTS_H_

namespace AF {

class DevicesTests : public testing::Test
{
protected:
	SSerPortConfig modbussim;
	HomeIsServer* server;
	IModbusProvider* modbusProvider;
	Client client;
public:
	DevicesTests();
	void SetUp();
	void TearDown();
	virtual ~DevicesTests();
};

} /* namespace AF */

#endif /* SRC_TESTS_API_DEVICESTESTS_H_ */
