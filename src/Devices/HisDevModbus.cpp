/*
 * HisModbusDevice.cpp
 *
 *  Created on: 21. 10. 2016
 *      Author: pc
 */

#include <HisDevModbus.h>
#include <StringBuilder.h>
#include "Modbus/IModbus.h"
#include <unistd.h>

HisDevModbus::HisDevModbus(IModbus* connection,int address)
	: HisDevBase::HisDevBase(),
	  data(NULL),
	  size(0),
	  typesdefs(NULL),
	  sbinInputstypesdefsIndex(0),
	  sbininputs(NULL),
	  modbusProvider(NULL),
	  sbinoutputs(NULL),
	  sbinOutputstypesdefsIndex(0),
	  refreshOutputs(false),
	  scanRequest(0),
	  owheader(NULL),
	  sowiretypesdefsIndex(0),
	  sds18b20s(NULL),
	  scantag(NULL)
{
	refreshscanmutex = HisLock::CreateMutex();
	this->connection = connection;
	this->address = address;
	SetError(true);
}

HisDevModbus::HisDevModbus(xmlNodePtr node,IModbusProvider* modbusProvider)
	: HisDevBase::HisDevBase(node),
	  data(NULL),
	  size(0),
	  typesdefs(NULL),
	  sbinInputstypesdefsIndex(0),
	  sbininputs(NULL),
	  sbinoutputs(NULL),
	  sbinOutputstypesdefsIndex(0),
	  refreshOutputs(false),
	  scanRequest(0),
	  owheader(NULL),
	  sowiretypesdefsIndex(0),
	  sds18b20s(NULL),
	  scantag(NULL)
{
	refreshscanmutex = HisLock::CreateMutex();
	this->modbusProvider = modbusProvider;
	this->connection = NULL;
	this->address = 0;
	SetError(true);
}

bool HisDevModbus::Resolve(xmlNodePtr pnode)
{
	STACK
	if (xmlStrEqual(pnode->name,DEVMODBUS))
	{
		return true;
	}
	return false;
}

const xmlChar* HisDevModbus::GetNodeNameInternal()
{
	return DEVMODBUS;
}

std::string HisDevModbus::GetConnectionName()
{
	if (connection!=NULL)
		return connection->GetName();
	return NULL;
}

int HisDevModbus::GetAddress()
{
	return address;
}

void HisDevModbus::ReleaseResources()
{
	STACK

	if (data!=NULL)
	{
		delete[] data;
		data = NULL;
	}

	typesdefs = NULL;
	sbininputs = NULL;
	sbinoutputs = NULL;
	sds18b20s = NULL;
	owheader = NULL;
	sowiretypesdefsIndex = 0;
	sbinInputstypesdefsIndex = 0;
	sbinOutputstypesdefsIndex = 0;
}

void HisDevModbus::CreateOrValidOneWire(bool addnew)
{

	for(int i=0;i<owheader->count;i++)
	{
		SDS18B20 mds18b20 = sds18b20s[i];
		uint32_t lowword = mds18b20.id1234;
		uint32_t hiword = mds18b20.id5678;
		LOW_deviceID id(hiword,lowword);
		string strid = id.getRomIDString();
		HisDevValue<double>* tempertag = dynamic_cast<HisDevValue<double>*>(FindValue(strid));
		if ((tempertag==NULL && addnew) || tempertag!=NULL)
		{
			if (tempertag==NULL)
			{
				tempertag = new HisDevValue<double>(Converter::itos(address,10),EHisDevDirection::Read,EDataType::Double,strid,0);
				tempertag->SetLoadType(LOADTYPE_ONEWIRE);
				Add(tempertag);
				//ds18b20s.push_back(tempertag);
			}
			double temper = (double)mds18b20.temperature / 100.0;
			tempertag->ReadedValueFromDevice(temper,false);
		}
	}
}

bool HisDevModbus::Scan(bool addnew)
{
	STACK
	HisLock lock(refreshscanmutex);
	ReleaseResources();

	if (connection==NULL)
	{
		SetError(true);
		return false;
	}

	uint16_t* pheader = reinterpret_cast<uint16_t*>(&header);
	if (connection->getHoldings(address,0,sizeof(header)/2,pheader))
	{
		SetError(false);
		size = header.lastIndex+1;
		data = new uint16_t[size];
		if (connection->getHoldings(address,0,size,data))
		{
			STACK_VAL(Scan,"File: "+string(__FILE__)+",Line: "+Converter::itos(__LINE__))
			if (header.CountOfTypes>0)
			{
				typesdefs = reinterpret_cast<STypedef*>(&data[header.TypeDefsOffset]);

				for (int i=0;i<header.CountOfTypes;i++)
				{
					switch(typesdefs[i].Type)
					{
						case ETypes::AOutputs:
						break;
						case ETypes::AInputs:
						break;
						case ETypes::BinInputs:
							sbinInputstypesdefsIndex = i;
							if (typesdefs[i].Count>0)
							{
								sbininputs = reinterpret_cast<SBinInput*>(&data[typesdefs[i].OffsetOfType]);
								CreateOrValidInputs(addnew);
							}
						break;
						case ETypes::BinOutputs:
							sbinOutputstypesdefsIndex = i;
							if (typesdefs[i].Count>0)
							{
								sbinoutputs = reinterpret_cast<SBinOutput*>(&data[typesdefs[i].OffsetOfType]);
								CreateOrValidOutputs(addnew);
							}
						break;
						case ETypes::DS18B20:
							sowiretypesdefsIndex = i;
							owheader = reinterpret_cast<SOWHeader*>(&data[typesdefs[i].OffsetOfType]);
							CreateOrValidOneWireHeader(addnew);
							CLogger::Info(StringBuilder::Format("Scanned %d ds18b20",owheader->count).c_str());
							if (owheader->count>0)
							{
								sds18b20s = reinterpret_cast<SDS18B20*>(&data[typesdefs[i].OffsetOfType+OW_DEVICES_OFFSET]);
								CreateOrValidOneWire(addnew);
							}
							//add scan tag
							//add ds18b20 tags
						break;
						default:
							break;
					}
				}
			}
		}
		else
		{
			SetError(true);
			return false;
		}
	}
	else
	{
		SetError(true);
		return false;
	}
	return true;
}

void HisDevModbus::CreateOrValidOneWireHeader(bool addnew)
{
	STACK
	HisDevValueBase* valuebase = FindValue(Converter::itos(OW_SCAN_PINNUMBER));
	if ((addnew && valuebase==NULL)||valuebase!=NULL)
	{
		if (valuebase==NULL)
		{
			scantag = new HisDevValue<bool>(Converter::itos(address,10),EHisDevDirection::Write,EDataType::Bool,Converter::itos(OW_SCAN_PINNUMBER),false);
			scantag->SetName(SCAN_ONEWIRE_NAME);
			Add(scantag);
		}
		WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevModbus, &HisDevModbus::WriteToDevice>(this);
		scantag->delegateWrite = delegate;
		scantag->ReadedValueFromDevice(scantag->GetValue(),false);
	}
}

void HisDevModbus::CreateOrValidOutputs(bool addnew)
{
	STACK
	for(int i=0;i<typesdefs[sbinOutputstypesdefsIndex].Count;i++)
	{
		SBinOutput soutput = sbinoutputs[i];
		HisDevValueBase* valuebase = FindValue(Converter::itos(soutput.PinNumber));

		HisDevValue<bool>* output = NULL;
		output = dynamic_cast<HisDevValue<bool>*>(valuebase);

		if ((output==NULL && addnew)||output!=NULL)
		{
			if (output==NULL)
			{
				output = new HisDevValue<bool>(Converter::itos(address,10),EHisDevDirection::Write,EDataType::Bool,soutput.PinNumber,false);
				Add(output);
				valuesOutput.push_back(output);
			}
			WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevModbus, &HisDevModbus::WriteToDevice>(this);
			output->delegateWrite = delegate;
			output->ReadedValueFromDevice(soutput.Value,false);
		}
	}
}

void HisDevModbus::WriteToDevice(ValueChangedEventArgs args)
{
	if (args.GetValue()->GetDirection()==EHisDevDirection::ReadWrite ||
		args.GetValue()->GetDirection()==EHisDevDirection::Write)
	{
		refreshOutputs = true;
	}
	SetChanged();
	NeedRefresh();
}


void HisDevModbus::CreateOrValidInputs(bool addnew)
{
	STACK
	for(int i=0;i<typesdefs[sbinInputstypesdefsIndex].Count;i++)
	{
		SBinInput sinput = sbininputs[i];
		HisDevValueBase* valuebase = FindValue(Converter::itos(sinput.PinNumber));
		HisDevValue<bool>* input = NULL;
		input = dynamic_cast<HisDevValue<bool>*>(valuebase);
		if ((input==NULL && addnew) || input!=NULL)
		{
			if (input==NULL)
			{
				input = new HisDevValue<bool>(Converter::itos(address,10),EHisDevDirection::Read,EDataType::Bool,Converter::itos(sinput.PinNumber),false);
				Add(input);
			}
			input->SetValue(sinput.Value);
			valuesInput.push_back(input);
		}
	}
}

HisDevValueBase* HisDevModbus::FindValue(string pinNumber)
{
	vector<HisDevValueBase*> values = GetItems<HisDevValueBase>();
	for(size_t i = 0; i< values.size(); i++)
	{
		if (values[i]->GetPinNumber()==pinNumber)
			return values[i];
	}
	return NULL;
}

void HisDevModbus::RefreshOutputs()
{
	refreshOutputs = false;
	if (typesdefs==NULL || sbinoutputs==NULL) return;
	bool resultok = false;

	if (scantag->GetValue()==true)
	{
		CLogger::Info("Write to setHolding OW_SCAN_OFFSET to 1");
		resultok = connection->setHolding(address,typesdefs[sowiretypesdefsIndex].OffsetOfType+OW_SCAN_OFFSET,scantag->GetValue());
		if (!resultok)
		{
			scantag->ReadedValueFromDevice(scantag->GetValue(),!resultok);
			SetError(true);
		}
		else
		{
			sleep(1);
			scanRequest = 100;
		}
	}

	for(size_t v=0;v<valuesOutput.size();v++)
	{
		int pinno = Converter::stoi(valuesOutput[v]->GetPinNumber());

		for(int i=0;i<typesdefs[sbinOutputstypesdefsIndex].Count;i++)
		{
			if (pinno==sbinoutputs[i].PinNumber)
			{
				//bool readedrror = sbinoutputs[i].Quality == 1 ? false : true;
				sbinoutputs[i].Value = valuesOutput[v]->GetValue();
				uint16_t writevalue = *((uint16_t*)&sbinoutputs[i]);
				resultok = connection->setHolding(address,typesdefs[sbinOutputstypesdefsIndex].OffsetOfType+i,writevalue);
				valuesOutput[v]->ReadedValueFromDevice(valuesOutput[v]->GetValue(),!resultok);
				break;
			}
		}
	}
}

void HisDevModbus::DoInternalRefresh(bool alarm)
{
	HisLock lock(refreshscanmutex);
	STACK

	if (GetError() || data==NULL)
	{
		if (Scan(false))
		{
			//set outputs
			RefreshOutputs();
		}
		else
		{
			SetError(true);
		}
	}
	else
	{
		if (refreshOutputs)
		{
			RefreshOutputs();
		}

		bool modbusok = connection->getHoldings(address,0,size,data);
		if (!modbusok)
		{
			SetError(true);
			return;
		}
		else
		{
			if (typesdefs==NULL)
			{
				CLogger::Info("Wrong data scanned. Var typesdefs is NULL.");
				return;
			}

			RefreshInputs(modbusok);
			RefreshOneWire(modbusok);
		}
	}
}

void HisDevModbus::RefreshInputs(bool modbusok)
{
	STACK
	for(size_t v=0;v<valuesInput.size();v++)
	{
		for(int i=0;i<typesdefs[sbinInputstypesdefsIndex].Count;i++)
		{
			int strpinno = Converter::stoi(valuesInput[v]->GetPinNumber());
			if (strpinno==sbininputs[i].PinNumber)
			{
				bool readedrror = sbininputs[i].Quality == 1 ? false : true;
				bool value = sbininputs[i].Value == 1 ? true : false;
				valuesInput[v]->ReadedValueFromDevice(value,readedrror && !modbusok);
				break;
			}
		}
	}
}

void HisDevModbus::RefreshOneWire(bool modbusok)
{
	STACK
	if (scantag!=NULL)
	{
		//uint16_t lscan = data[typesdefs[sowiretypesdefsIndex].OffsetOfType];
		uint16_t lscan = owheader->scan;
		scantag->ReadedValueFromDevice(lscan,!modbusok);
		if (!lscan && scanRequest)
		{
			scanRequest = 0;
			if (!this->Scan(true))
			{
				SetError(true);
				CLogger::Error("Error scan modbus field with address %d",this->address);
				return;
			}
		}
	}

	vector<HisDevValue<double>*> ds18b20s = GetItems<HisDevValue<double>>();
	for(size_t v=0;v<ds18b20s.size();v++)
	{
		if (sds18b20s!=NULL && modbusok && owheader->count>0)
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

void HisDevModbus::DoInternalSave(xmlNodePtr & node)
{
	HisDevBase::DoInternalSave(node);

	xmlSetProp(node,PROP_ADDRESS,(const xmlChar*)Converter::itos(address,10).c_str());

	if (connection!=NULL)
	{
		xmlSetProp(node,PROP_CONNECTION_NAME,(const xmlChar*)connection->GetName().c_str());
	}
}

void HisDevModbus::DoInternalLoad(xmlNodePtr & node)
{
	HisDevBase::DoInternalLoad(node);

	xmlChar* prop = NULL;
	if (xmlHasProp(node,PROP_ADDRESS))
	{
		prop = xmlGetProp(node,PROP_ADDRESS);
		string strAddress = (const char*)prop;
		address = Converter::stoui(strAddress,10);
		xmlFree(prop);
		prop = NULL;
	}

	if (xmlHasProp(node,PROP_CONNECTION_NAME))
	{
		prop = xmlGetProp(node,PROP_CONNECTION_NAME);
		string strConnectionName = (const char*)prop;

		connection = modbusProvider->Find(strConnectionName);

		if (connection==NULL)
		{
			CLogger::Error("HisDevModbus::Load | Name: %s Connection: %s not found",
					GetName().c_str(),strConnectionName.c_str());
		}

		xmlFree(prop);
		prop = NULL;
	}

	vector<HisDevValue<bool>*> values = GetItems<HisDevValue<bool>>();

	for(size_t i=0;i<values.size();i++)
	{
		HisDevValue<bool> *value = values[i];
		switch(value->GetDirection())
		{
			case EHisDevDirection::Read:
				valuesInput.push_back(value);
				break;
			case EHisDevDirection::ReadWrite:
			case EHisDevDirection::Write:
				int pinno = Converter::stoi(value->GetPinNumber());
				if (pinno==OW_SCAN_PINNUMBER)
					scantag = value;
				else
					valuesOutput.push_back(value);
				break;
		}
	}
}

void HisDevModbus::OnError() {

}


HisDevModbus::~HisDevModbus() {
}

