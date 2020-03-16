/*
 * HisDevModbusTests.cpp
 *
 *  Created on: 19. 3. 2017
 *      Author: pc
 */

#ifdef HOMEISTEST

#include "gtest/gtest.h"
#include <stdint.h>
#include "Modbus/ModbusSimulator.h"
#include "HisDevModbus.h"
#include "OneWireHandler.h"
#include "HisDevModbusTests.h"


namespace AF {

uint16_t registers[] = { 5,3,10,5120,32,0,0,0,0,0,1,3,23,3,2,1,26,3,5,1,27,3,0,769,770,771,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint16_t* registersi = (uint16_t*)registers;

HisDevModbusTests::HisDevModbusTests() {
	hisDevFactory = NULL;
	modbus = NULL;
	modbussim.Driver = "modbussimulator";
	modbussim.Name = "modbussimulator";
	modbussim.Port = "";
}

void HisDevModbusTests::SetUp()
{
	modbus = new ModbusSimulator(modbussim,registers,sizeof(registers)/sizeof(uint16_t));

	When(Method(hisDevFactoryMock,Create)).AlwaysReturn(NULL);
	hisDevFactory = &hisDevFactoryMock.get();
}


TEST_F(HisDevModbusTests,Resolve_PASS)
{
	HisDevModbus* devmodbus = new HisDevModbus(modbus,1,hisDevFactory);
	xmlNode node;
	node.name = DEVMODBUS;
	xmlNodePtr nodeptr = &node;
	bool result = devmodbus->Resolve(nodeptr);
	ASSERT_TRUE(result);
	delete devmodbus;
	devmodbus = NULL;
}

TEST_F(HisDevModbusTests,Resolve_FAIL)
{
	HisDevModbus* devmodbus = new HisDevModbus(modbus,1,hisDevFactory);
	xmlNode node;
	node.name = BAD_CAST "wrongname";
	xmlNodePtr nodeptr = &node;
	bool result = devmodbus->Resolve(nodeptr);
	ASSERT_FALSE(result);
	delete devmodbus;
	devmodbus = NULL;
}

TEST_F(HisDevModbusTests,ScanTest)
{
	IModbus* modbus = new ModbusSimulator(modbussim,registers,sizeof(registers)/sizeof(uint16_t));
	Mock<IHisDevFactory> hisDevFactoryMock;
	When(Method(hisDevFactoryMock,Create)).AlwaysReturn(NULL);
	IHisDevFactory &hisDevFactory = hisDevFactoryMock.get();
	HisDevModbus* devmodbus = new HisDevModbus(modbus,1,&hisDevFactory);
	devmodbus->Scan(true);

	vector<HisDevValue<bool>*> values = devmodbus->GetItems<HisDevValue<bool>>();
	uint16_t count = CountOfType<HisDevValue<bool>*>(values,EHisDevDirection::Read);
	ASSERT_EQ(count,3);
	count = CountOfType<HisDevValue<bool>*>(values,EHisDevDirection::Write);
	ASSERT_EQ(count,2);
	IHisBase* value = devmodbus->FindByName(SCAN_ONEWIRE_NAME);
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
