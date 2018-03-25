/*
 * OneWireHandlerTests.cpp
 *
 *  Created on: 11. 3. 2018
 *      Author: pc
 */
#ifdef HOMEISTEST



#include "gtest/gtest.h"
#include <stdint.h>
#include "Modbus/ModbusSimulator.h"
#include "HisDevModbus.h"
#include "OneWireHandler.h"

#include "OneWireHandlerTests.h"


namespace AF {

OneWireHandlerTests::OneWireHandlerTests() {
}

void OneWireHandlerTests::SetUp()
{

	When(Method(hisDevFactoryMock,Create)).AlwaysReturn(NULL);
}


class MockHisDevModbus : public IHisDevModbus
{
public:
	STypedef stypedef;
	uint16_t* data;
	vector<IHisBase*> items;
	uint8_t datalen;
	OneWireHandler::SDS18B20* ds18b20;

	MockHisDevModbus()
	{
		data = NULL;
		datalen = 0;
	}

	void WriteToDevice(ValueChangedEventArgs args)
	{

	}

	vector<HisDevValue<double>*> GetDoubleItems()
	{
		vector<HisDevValue<double>*> result;
		for(size_t i=0;i<items.size();i++)
		{
			HisDevValue<double>* val = dynamic_cast<HisDevValue<double>*>(items[i]);
			if (val!=NULL)
				result.push_back(val);
		}
		return result;
	}

	vector<HisDevValue<bool>*> GetBoolItems()
	{

	}

	void Add(IHisBase* item)
	{
		items.push_back(item);
	}

	bool GetTypeDef(ETypes type,STypedef * stypedef)
	{
		*stypedef = this->stypedef;
		return true;
	}

	bool GetData(uint16_t** pdata, uint8_t & length)
	{
		this->datalen = length = OW_DEVICES_OFFSET+sizeof(OneWireHandler::SDS18B20);
		this->data = new uint16_t[length];
		OneWireHandler::SOWHeader* owireheader = (OneWireHandler::SOWHeader*)(this->data);
		owireheader->count = 1;
		owireheader->scan = 0;
		ds18b20 = (OneWireHandler::SDS18B20*)(&this->data[OW_DEVICES_OFFSET]);
		ds18b20->error = 1;
		ds18b20->temperature = 2071;
		ds18b20->id1234 = 0x0a9ff628;
		ds18b20->id5678 = 0xcc000006;

		*pdata = this->data;

		return true;
	}

	HisDevValueBase* FindValue(string pinNumber)
	{
		return NULL;
	}

	int GetAddress()
	{
		return 0;
	}

	bool setHolding(uint16_t index, uint16_t val)
	{

	}

	HisDevValueBase* FindValue(string pinNumber,string loadType)
	{
		return NULL;
	}

	~MockHisDevModbus()
	{
		if (data!=NULL)
		{
			delete[] data;
			data = NULL;
		}
		for(size_t i=0;i<items.size();i++)
		{
			 delete items[i];
		}
	}
};

TEST_F(OneWireHandlerTests,TestReconnectOneWireAfterError)
{
	MockHisDevModbus mockdev;
	mockdev.stypedef.Count = 1;
	mockdev.stypedef.Func = EModbusFunc::ReadHoldingRegisters;
	mockdev.stypedef.OffsetOfType = 0;
	mockdev.stypedef.Type = ETypes::DS18B20Temp;

	IHisDevFactory* hisDevFactory = &hisDevFactoryMock.get();

	OneWireHandler handler(&mockdev,hisDevFactory);

	handler.Scan(true);
	vector<HisDevValue<double>*> ds18b20s = mockdev.GetDoubleItems();
	handler.Refresh(true);
	ASSERT_FALSE(ds18b20s[0]->GetDeviceError());
	mockdev.ds18b20->error = 0;
	handler.Refresh(true);
	ASSERT_TRUE(ds18b20s[0]->GetDeviceError());
	mockdev.ds18b20->error = 1;
	handler.Refresh(true);
	ASSERT_FALSE(ds18b20s[0]->GetDeviceError());
}

OneWireHandlerTests::~OneWireHandlerTests() {
}

}

#endif
