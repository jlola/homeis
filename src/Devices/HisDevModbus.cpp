/*
 * HisModbusDevice.cpp
 *
 *  Created on: 21. 10. 2016
 *      Author: pc
 */

#include <unistd.h>
#include "Modbus/IModbus.h"
#include "StringBuilder.h"
#include "HisDevModbus.h"
#include "ModbusHandlers.h"

using namespace std;

HisDevModbus::HisDevModbus(IModbus* connection,int address,IHisDevFactory* factory)
	: logger(CLogger::GetLogger()),
	  HisDevBase::HisDevBase(factory),
	  data(NULL),
	  size(0),
	  typesdefs(NULL),
	  modbusProvider(NULL),
	  refreshOutputs(false),
	  scanRequest(0)
{
	handlers = new ModbusHandlers(this,factory);
	refreshscanmutex = HisLock::CreateMutex();
	this->connection = connection;
	this->address = address;
	SetError(true);
}

HisDevModbus::HisDevModbus(xmlNodePtr node,IModbusProvider* modbusProvider,IHisDevFactory* factory)
	: HisDevBase::HisDevBase(node,factory),
	  logger(CLogger::GetLogger()),
	  data(NULL),
	  size(0),
	  typesdefs(NULL),
	  refreshOutputs(false),
	  scanRequest(0)
{
	handlers = new ModbusHandlers(this,factory);
	refreshscanmutex = HisLock::CreateMutex();
	this->modbusProvider = modbusProvider;
	this->connection = NULL;
	this->address = 0;
	SetError(true);
}

void HisDevModbus::Add(IHisBase *pitem)
{
	HisBase::Add(pitem);
}

IHisBase* HisDevModbus::Remove(CUUID puuid)
{
	HisLock lock(refreshscanmutex);

	if (handlers->Remove(puuid))
		return HisBase::Remove(puuid);
	return NULL;
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

bool HisDevModbus::setHolding(uint16_t index, uint16_t val)
{
	bool success = connection->setHolding(address,index,val);
	SetError(!success);
	return success;
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
}

bool HisDevModbus::GetTypeDef(ETypes type,STypedef * stypedef)
{
	if (header.CountOfTypes>0)
	{
		typesdefs = reinterpret_cast<STypedef*>(&data[header.TypeDefsOffset]);

		for (int i=0;i<header.CountOfTypes;i++)
		{
			if (typesdefs[i].Type==type)
			{
				*stypedef = typesdefs[i];
				return true;
			}
		}
	}
	return false;
}
vector<HisDevValue<double>*> HisDevModbus::GetDoubleItems()
{
	return GetItems<HisDevValue<double>>();
}

vector<HisDevValue<bool>*> HisDevModbus::GetBoolItems()
{
	return GetItems<HisDevValue<bool>>();
}

bool HisDevModbus::Scan(bool addnew)
{
	STACK
	HisLock lock(refreshscanmutex);
//	ReleaseResources();
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
			if (handlers->Scan(addnew))
			{
				CLogger::GetLogger().Info(StringBuilder::Format("Dev %s: Scan was not success",this->GetAddress()).c_str());
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

IModbus* HisDevModbus::GetModbus()
{
	return connection;
}

bool HisDevModbus::GetData(uint16_t* & data, uint8_t & length)
{
	if (this->data!=NULL)
	{
		data = this->data;
		length = this->header.lastIndex+1;
		return true;
	}
	return false;
}

void HisDevModbus::WriteToDevice(ValueChangedEventArgs & args)
{
	if (args.GetValue()->GetDirection()==EHisDevDirection::ReadWrite ||
		args.GetValue()->GetDirection()==EHisDevDirection::Write)
	{
		logger.Trace("Dev %d WriteToDevice method called from %s",GetAddress(),args.GetValue()->GetName().c_str());
		refreshOutputs = true;
	}
	SetChanged();
	NeedRefresh();
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

HisDevValueBase* HisDevModbus::FindValue(string pinNumber,string loadType)
{
	vector<HisDevValueBase*> values = GetItems<HisDevValueBase>();
	for(size_t i = 0; i< values.size(); i++)
	{
		if (values[i]->GetPinNumber()==pinNumber && values[i]->GetLoadType()==loadType)
			return values[i];
	}
	return NULL;
}

HisDevValueBase* HisDevModbus::FindLoadType(string loadType)
{
	vector<HisDevValueBase*> values = GetItems<HisDevValueBase>();
	for(size_t i = 0; i< values.size(); i++)
	{
		if (values[i]->GetLoadType()==loadType)
			return values[i];
	}
	return NULL;
}

void HisDevModbus::DoInternalRefresh(bool alarm)
{
	STACK

	HisLock lock(refreshscanmutex);

	if (GetError() || data==NULL)
	{
		if (Scan(false))
		{
			//set outputs
			handlers->RefreshOutputs();
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
			refreshOutputs = false;
			handlers->RefreshOutputs();
		}

		bool modbusok = connection->getHoldings(address,0,size,data);
		if (!modbusok)
		{
			SetError(true);
			return;
		}
		else
		{
			SHeader* h = (SHeader*)data;
			header.lastIndex = h->lastIndex;
			size = header.lastIndex+1;

			if (typesdefs==NULL)
			{
				logger.Info("Wrong data scanned. Var typesdefs is NULL.");
				return;
			}

			handlers->Refresh(modbusok);
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
			logger.Error("HisDevModbus::Load | Name: %s Connection: %s not found",
					GetName().c_str(),strConnectionName.c_str());
		}

		xmlFree(prop);
		prop = NULL;
	}

	handlers->Load();
}

void HisDevModbus::OnError() {

}


HisDevModbus::~HisDevModbus() {
	delete handlers;
	handlers = NULL;
}

