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

HisDevModbusTests::HisDevModbusTests() {
	// TODO Auto-generated constructor stub
	modbussim.Driver = "modbussimulator";
	modbussim.Name = "modbussimulator";
	modbussim.Port = "";
	std::vector<SSerPortConfig> serports;

}

TEST_F(HisDevModbusTests,ScanTest)
{
	IModbus* modbus = new ModbusSimulator(modbussim);
	HisDevModbus* devmodbus = new HisDevModbus(modbus,1);
	devmodbus->Scan(true);

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
