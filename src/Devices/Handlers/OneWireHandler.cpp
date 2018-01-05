/*
 * OneWireHandler.cpp
 *
 *  Created on: 16. 12. 2017
 *      Author: pc
 */

#include <string>
#include <stddef.h>
#include "PoppyDebugTools.h"
#include "OneWireHandler.h"
#include "LOW_deviceID.h"
#include "unistd.h"
#include "StringBuilder.h"

using namespace std;

string OneWireHandler::LoadType = "OneWireHandler";

OneWireHandler::OneWireHandler(HisDevModbus* devModbus,IHisDevFactory* factory) :
	scantag(NULL),owheader(NULL),sowiretypesdefsIndex(0),
	sds18b20s(NULL),devModbus(devModbus),scanRequest(false),
	factory(factory)
{
}

void OneWireHandler::Load()
{
	vector<HisDevValue<bool>*> values = devModbus->GetItems<HisDevValue<bool>>();

	for(size_t i=0;i<values.size();i++)
	{
		HisDevValue<bool> *value = values[i];
		switch(value->GetDirection())
		{
			case EHisDevDirection::ReadWrite:
			case EHisDevDirection::Write:
			{
				int pinno = Converter::stoi(value->GetPinNumber());
				if (pinno==OW_SCAN_PINNUMBER && value->GetLoadType()==LoadType)
					scantag = value;
			}
			break;
			default:
				break;
		}
	}
}

bool OneWireHandler::Scan(bool addnew)
{
	STACK
	if (devModbus->GetTypeDef(ETypes::DS18B20Temp,stypedef))
	{
		uint16_t* data;
		uint8_t size;
		devModbus->GetData(data,size);
		owheader = reinterpret_cast<SOWHeader*>(&data[stypedef.OffsetOfType]);
		CreateOrValidOneWireHeader(addnew);
		CLogger::Info(StringBuilder::Format("Scanned %d ds18b20",owheader->count).c_str());
		if (owheader->count>0)
		{
			sds18b20s = reinterpret_cast<SDS18B20*>(&data[stypedef.OffsetOfType+OW_DEVICES_OFFSET]);
			CreateOrValidOneWire(addnew);
			return true;
		}
	}
	return false;
}

void OneWireHandler::CreateOrValidOneWireHeader(bool addnew)
{
	STACK
	HisDevValueBase* valuebase = devModbus->FindValue(Converter::itos(OW_SCAN_PINNUMBER));
	if ((addnew && valuebase==NULL)||valuebase!=NULL)
	{
		if (valuebase==NULL)
		{
			scantag = new HisDevValue<bool>(Converter::itos(devModbus->GetAddress(),10),EHisDevDirection::Write,EDataType::Bool,Converter::itos(OW_SCAN_PINNUMBER),false,LoadType,factory);
			scantag->SetName(SCAN_ONEWIRE_NAME);
			devModbus->Add(scantag);
		}
		WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevModbus, &HisDevModbus::WriteToDevice>(devModbus);
		scantag->delegateWrite = delegate;
		scantag->ReadedValueFromDevice(scantag->GetValue(),false);
	}
}

void OneWireHandler::RefreshOutputs()
{
	STACK

	if (scantag->GetValue()==true)
	{
		CLogger::Info("Write to setHolding OW_SCAN_OFFSET to 1");
		bool success = devModbus->setHolding(stypedef.OffsetOfType+OW_SCAN_OFFSET,scantag->GetValue());
		if (success)
		{
			scanRequest = true;
		}
		else
		{
			scantag->ReadedValueFromDevice(scantag->GetValue(),!success);
		}
	}
}

void OneWireHandler::CreateOrValidOneWire(bool addnew)
{
	STACK

	for(int i=0;i<owheader->count;i++)
	{
		SDS18B20 mds18b20 = sds18b20s[i];
		uint32_t lowword = mds18b20.id1234;
		uint32_t hiword = mds18b20.id5678;
		LOW_deviceID id(hiword,lowword);
		string strid = id.getRomIDString();
		HisDevValue<double>* tempertag = dynamic_cast<HisDevValue<double>*>(devModbus->FindValue(strid));
		if ((tempertag==NULL && addnew) || tempertag!=NULL)
		{
			if (tempertag==NULL)
			{
				tempertag = new HisDevValue<double>(Converter::itos(devModbus->GetAddress(),10),EHisDevDirection::Read,EDataType::Double,strid,0,LoadType,factory);
				tempertag->SetLoadType(LOADTYPE_ONEWIRE);
				devModbus->Add(tempertag);
				//ds18b20s.push_back(tempertag);
			}
			double temper = (double)mds18b20.temperature / 100.0;
			tempertag->ReadedValueFromDevice(temper,false);
		}
	}
}

void OneWireHandler::Refresh(bool modbusSuccess)
{
	STACK

	//uint16_t lscan = data[typesdefs[sowiretypesdefsIndex].OffsetOfType];
	uint16_t owscan = owheader->scan;
	scantag->ReadedValueFromDevice(owscan,!modbusSuccess);
	if (!owscan && scanRequest)
	{
		scanRequest = 0;
		if (!this->Scan(true))
		{
			CLogger::Error("Error scan modbus field with address %d",devModbus->GetAddress());
			return;
		}
	}

	vector<HisDevValue<double>*> ds18b20s = devModbus->GetItems<HisDevValue<double>>();
	for(size_t v=0;v<ds18b20s.size();v++)
	{
		if (sds18b20s!=NULL && modbusSuccess && owheader->count>0)
		{
			for(int i=0;i<owheader->count;i++)
			{
				LOW_deviceID id(sds18b20s[i].id5678,sds18b20s[i].id1234);
				string strid = id.getRomIDString();
				string strpinno = ds18b20s[v]->GetPinNumber();
				if (strpinno==strid)
				{
					bool readedrror = sds18b20s[i].error == 1 ? false : true;
					double value = (double)sds18b20s[i].temperature / 100.0;
					ds18b20s[v]->ReadedValueFromDevice(value,readedrror);
					break;
				}
			}
		}
		else
		{
			ds18b20s[v]->ReadedValueFromDevice(ds18b20s[v]->GetValue(),true);
		}
	}
}

OneWireHandler::~OneWireHandler() {
	// TODO Auto-generated destructor stub
}

