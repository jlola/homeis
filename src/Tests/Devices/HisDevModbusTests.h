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

	template<class T>
	uint16_t CountOfType(vector<T> values, EHisDevDirection direction)
	{
		uint8_t count = 0;
		for(size_t i=0;i<values.size();i++)
		{
			if (values[i]->GetDirection()==direction)
				count++;
		}
		return count;
	}
};

} /* namespace AF */

#endif /* SRC_TESTS_DEVICES_HISDEVMODBUSTESTS_H_ */
