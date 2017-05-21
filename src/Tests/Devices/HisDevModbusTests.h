/*
 * HisDevModbusTests.h
 *
 *  Created on: 19. 3. 2017
 *      Author: pc
 */

#ifndef SRC_TESTS_DEVICES_HISDEVMODBUSTESTS_H_
#define SRC_TESTS_DEVICES_HISDEVMODBUSTESTS_H_

namespace AF {

class HisDevModbusTests : public testing::Test
{
protected:
	SSerPortConfig modbussim;
public:
	HisDevModbusTests();
	virtual ~HisDevModbusTests();
};

} /* namespace AF */

#endif /* SRC_TESTS_DEVICES_HISDEVMODBUSTESTS_H_ */
