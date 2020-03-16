/*
 * OneWireHandlerTests.cpp
 *
 *  Created on: 11. 3. 2018
 *      Author: pc
 */
#ifdef HOMEISTEST



#include "gtest/gtest.h"
#include <stdint.h>
#include <string>
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
	IHisDevFactory* factory;
	STypedef stypedef;
	uint16_t* data;
	vector<IHisBase*> items;
	uint8_t datalen;
	OneWireHandler::SDS18B20* ds18b20;
	HisDevValue<bool>* scantag;
	HisDevValue<double>* tempertag;

	MockHisDevModbus(IHisDevFactory* factory)
	{
		this->factory = factory;
		data = NULL;
		datalen = 0;
		ds18b20  = NULL;
		this->data = new uint16_t[1024];
		scantag = NULL;
	}

	void WriteToDevice(ValueChangedEventArgs & args)
	{

	}

	void SetAddDs18B20(bool add)
	{
		OneWireHandler::SOWHeader* owireheader = (OneWireHandler::SOWHeader*)(this->data);
		if (add)
		{
			owireheader->count = 1;
			ds18b20 = (OneWireHandler::SDS18B20*)(&this->data[OW_DEVICES_OFFSET]);
			ds18b20->error = 1;
			ds18b20->temperature = 2071;
			ds18b20->id1234 = 0x0a9ff628;
			ds18b20->id5678 = 0xcc000006;
		}
		else
		{
			owireheader->count = 0;
			owireheader->scan = 0;
		}

	}

	void Load()
	{
		tempertag = new HisDevValue<double>(
								"10",
								EHisDevDirection::Read,
								EDataType::Double,
								"cc0000060a9ff628",
								0,
								LOADTYPE_ONEWIRE,
								factory,
								this);
		tempertag->SetLoadType(LOADTYPE_ONEWIRE);
		items.push_back(tempertag);
		scantag = new HisDevValue<bool>(
				"10",
				EHisDevDirection::Write,
				EDataType::Bool,
				Converter::itos(OW_SCAN_PINNUMBER),
				false,
				"OneWireHandler",
				factory,
				this);
		scantag->SetName(SCAN_ONEWIRE_NAME);
		items.push_back(scantag);
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
		vector<HisDevValue<bool>*> result;
		for(size_t i=0;i<items.size();i++)
		{
			HisDevValue<bool>* val = dynamic_cast<HisDevValue<bool>*>(items[i]);
			if (val!=NULL)
				result.push_back(val);
		}
		return result;
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

	bool GetData(uint16_t* & pdata, uint8_t & length)
	{
		this->datalen = length = OW_DEVICES_OFFSET+sizeof(OneWireHandler::SDS18B20);

		pdata = this->data;

		return true;
	}

	HisDevValueBase* FindValue(string pinNumber)
	{
		return FindValue(pinNumber,"");
	}

	int GetAddress()
	{
		return 0;
	}

	bool setHolding(uint16_t index, uint16_t val)
	{
		return true;
	}

	HisDevValueBase* FindValue(string pinNumber,string loadType)
	{
		if (pinNumber=="100")
		{
			return scantag;
		}
		if (pinNumber=="cc0000060a9ff628")
		{
			return tempertag;
		}
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

class ModbusMock : public IModbus
{
	STypedef stypedef;

	bool Init() { return true; }
	string GetDriverName()  { return ""; }
	string GetName() { return "name"; }
	string GetPort()  { return "tty1"; }
	bool setBitInput(uint16_t address,uint16_t index, uint16_t state)  { return true; };
	bool getBitInput(uint16_t address,uint16_t index,bool & bit) { return true; }

	bool getShortInput(uint16_t address,uint16_t index,uint16_t & input)  { return true; }

	bool setCoil(uint16_t address,uint16_t index, uint16_t state)  { return true; }
	bool getCoil(uint16_t address,uint16_t index,bool & coil)  { return true; }

	bool setHolding(uint16_t address,uint16_t index, uint16_t val)  { return true; }
	bool getHolding(uint16_t address,uint16_t index,uint16_t* holding)  { return true; }
	bool getHoldings(uint16_t address,uint16_t offset,uint16_t count,uint16_t* target)
	{
		return true;
	}
};


TEST_F(OneWireHandlerTests,TestReconnectOneWireAfterError)
{
	MockHisDevModbus mockdev(&hisDevFactoryMock.get());
	mockdev.Load();

	mockdev.stypedef.Count = 1;
	mockdev.stypedef.Func = EModbusFunc::ReadHoldingRegisters;
	mockdev.stypedef.OffsetOfType = 0;
	mockdev.stypedef.Type = ETypes::DS18B20Temp;

	IHisDevFactory* hisDevFactory = &hisDevFactoryMock.get();
	OneWireHandler handler(&mockdev,hisDevFactory);

	mockdev.SetAddDs18B20(true);
	handler.Scan(true);
	vector<HisDevValue<double>*> ds18b20s = mockdev.GetDoubleItems();
	ASSERT_TRUE(ds18b20s.size()==1);
	handler.Refresh(true);
	ASSERT_FALSE(ds18b20s[0]->GetDeviceError());
}

TEST_F(OneWireHandlerTests,TestReconnectOneWireAfterReset)
{
	MockHisDevModbus mockdev(&hisDevFactoryMock.get());
	ModbusMock modbus;
	HisDevModbus dev(&modbus,5,&hisDevFactoryMock.get());
	mockdev.Load();

	mockdev.stypedef.Count = 1;
	mockdev.stypedef.Func = EModbusFunc::ReadHoldingRegisters;
	mockdev.stypedef.OffsetOfType = 0;
	mockdev.stypedef.Type = ETypes::DS18B20Temp;
	vector<HisDevValue<double>*> ds18b20s = mockdev.GetDoubleItems();

	IHisDevFactory* hisDevFactory = &hisDevFactoryMock.get();
	OneWireHandler handler(&mockdev,hisDevFactory);
	mockdev.SetAddDs18B20(false);
	handler.Load();
	handler.Scan(true);
	mockdev.scantag->SetValue(true);
	handler.RefreshOutputs();
	handler.Refresh(true);
	ASSERT_TRUE(ds18b20s[0]->GetDeviceError());
	mockdev.SetAddDs18B20(true);
	mockdev.scantag->SetValue(true);
	handler.RefreshOutputs();
	handler.Refresh(true);
	ASSERT_TRUE(ds18b20s.size()==1);
	ASSERT_FALSE(ds18b20s[0]->GetDeviceError());
}

OneWireHandlerTests::~OneWireHandlerTests() {
}

}

#endif
