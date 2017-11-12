/*
 * HisDevModbusTests.cpp
 *
 *  Created on: 19. 3. 2017
 *      Author: pc
 */

#ifdef HOMEISTEST

#include "gtest/gtest.h"
#include <stdint.h>
#include "fakeit.hpp"
#include "Modbus/ModbusSimulator.h"
#include "HisDevModbus.h"
#include <Tests/Devices/HisDevModbusTests.h>

using namespace fakeit;

namespace AF {

uint16_t registers[] = {
	    5,
	    3,
	    10,
	    5120,
	    92,
	    0,
	    0,
	    0,
	    0,
	    0,
	    1,
	    3,
	    23,
	    3,
	    2,
	    1,
	    26,
	    3,
	    5,
	    1,
	    27,
	    3,
	    0,
	    769,
	    770,
	    771,
	    4,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0
};
uint16_t* registersi = (uint16_t*)registers;

HisDevModbusTests::HisDevModbusTests() {
	// TODO Auto-generated constructor stub
	modbussim.Driver = "modbussimulator";
	modbussim.Name = "modbussimulator";
	modbussim.Port = "";
	std::vector<SSerPortConfig> serports;

}

TEST_F(HisDevModbusTests,ScanTest)
{

	IModbus* modbus = new ModbusSimulator(modbussim,registersi);
	HisDevModbus* devmodbus = new HisDevModbus(modbus,1);
	devmodbus->Scan(true);

	vector<HisDevValue<bool>*> values = devmodbus->GetItems<HisDevValue<bool>>();
	uint16_t count = CountOfType<HisDevValue<bool>*>(values,EHisDevDirection::Read);
	ASSERT_EQ(count,3);
	count = CountOfType<HisDevValue<bool>*>(values,EHisDevDirection::Write);
	ASSERT_EQ(count,2);
	HisBase* value = devmodbus->FindByName(SCAN_ONEWIRE_NAME);
	ASSERT_TRUE(value!=NULL);
}

TEST_F(HisDevModbusTests,RefreshTest)
{
//	IModbus* modbus = new ModbusSimulator(modbussim);
//	HisDevModbus* devmodbus = new HisDevModbus(modbus,1);
//	devmodbus->Scan(true);
//	devmodbus->Refresh(false);
}

HisDevModbusTests::~HisDevModbusTests() {
	// TODO Auto-generated destructor stub
}

} /* namespace AF */

#endif
