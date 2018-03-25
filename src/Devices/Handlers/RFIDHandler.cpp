/*
 * RFIDHandler.cpp
 *
 *  Created on: 4. 1. 2018
 *      Author: pc
 */

#include "RFIDHandler.h"
#include "StringBuilder.h"
#include "HisException.h"

string RFIDHandler::LoadType = "RFIDHandler";

RFIDHandler::RFIDHandler(IHisDevModbus* devModbus,IHisDevFactory* factory) {
	if (devModbus==NULL)
		throw ArgumentNullException(string("devModbus"));
	if (factory==NULL)
		throw ArgumentNullException(string("factory"));

	this->srfidregs = NULL;
	this->newDataFlag = NULL;
	this->rfiddataflag = NULL;
	this->dev = devModbus;
	this->factory = factory;
}

bool RFIDHandler::Scan(bool addnew)
{
	STACK

	if (dev->GetTypeDef(ETypes::RFID,&stypedef))
	{
		uint16_t** data;
		uint8_t size;
		dev->GetData(data,size);
		srfidregs = reinterpret_cast<SRFIDRegs*>(&data[stypedef.OffsetOfType]);
		CreateOrValidTags(addnew);
	}

	return true;
}

bool RFIDHandler::Remove(CUUID id)
{
	if (rfiddataflag!=NULL && rfiddataflag->GetRecordId()==id)
	{
		rfiddataflag = NULL;
		return true;
	}
	if (newDataFlag!=NULL && newDataFlag->GetRecordId()==id)
	{
		newDataFlag = NULL;
		return true;
	}
	return false;
}

void RFIDHandler::CreateOrValidTags(bool addnew)
{
	HisDevValueBase* valuebase = dev->FindValue(NEWDATAFLAG,LoadType);
	newDataFlag = dynamic_cast<HisDevValue<bool>*>(valuebase);
	if (newDataFlag!=NULL || addnew)
	{
		if (newDataFlag==NULL)
		{
			newDataFlag = new HisDevValue<bool>(Converter::itos(dev->GetAddress(),10),
					EHisDevDirection::ReadWrite,
					EDataType::Bool,
					NEWDATAFLAG,
					false,
					LoadType,
					factory,
					dev);
			newDataFlag->SetName("NewRFIDDataFlag");
			dev->Add(newDataFlag);
		}
		newDataFlag->SetValue(srfidregs->NewDataFlag ? true : false);
	}

	HisDevValueBase* rfiddatabase = dev->FindValue(RFIDDATATAG,LoadType);
	rfiddataflag = dynamic_cast<HisDevValue<string>*>(rfiddatabase);
	if (rfiddataflag!=NULL || addnew)
	{
		if (rfiddataflag==NULL)
		{
			rfiddataflag = new HisDevValue<string>(Converter::itos(dev->GetAddress(),10),
					EHisDevDirection::Read,
					EDataType::String,
					RFIDDATATAG,
					"",
					LoadType,
					factory,
					dev);
			rfiddataflag->SetName("RFIDData");
			dev->Add(rfiddataflag);
		}
		string data = (const char*)srfidregs->buffer;
		rfiddataflag->SetValue(data);
	}
}

void RFIDHandler::RefreshOutputs()
{
	STACK
	if (newDataFlag!=NULL && srfidregs!=NULL)
	{
		bool value = newDataFlag->GetValue();
		uint16_t regvalue = srfidregs->NewDataFlag = value ? 1 : 0;
		bool success = dev->setHolding(stypedef.OffsetOfType+NEWDATAFLAG_OFFSET, regvalue);
		newDataFlag->ReadedValueFromDevice(value,!success);
	}
}

void RFIDHandler::Refresh(bool modbusSuccess)
{
	if ( srfidregs != NULL )
	{
		if (newDataFlag!=NULL)
		{
			newDataFlag->ReadedValueFromDevice(srfidregs->NewDataFlag ? true : false,!modbusSuccess);
		}
		if (rfiddataflag!=NULL)
		{
			rfiddataflag->ReadedValueFromDevice((const char*)srfidregs->buffer,!modbusSuccess);
		}
	}
}

void RFIDHandler::Load()
{
	HisDevValueBase* value = dev->FindValue(RFIDDATATAG,LoadType);
	if (value!=NULL)
	{
		rfiddataflag = dynamic_cast<HisDevValue<string>*>(value);
	}
	value = dev->FindValue(NEWDATAFLAG,LoadType);
	if (value!=NULL)
	{
		newDataFlag = dynamic_cast<HisDevValue<bool>*>(value);
	}
}

RFIDHandler::~RFIDHandler() {
}

