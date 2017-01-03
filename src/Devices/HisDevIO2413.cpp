/*
 * HisDevIO2413.cpp
 *
 *  Created on: Mar 30, 2013
 *      Author: Josef Lola
 */
#include <string>
#include <sstream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "srutil/event/event.hpp"
#include "srutil/delegate/delegate.hpp"

#include "logger.h"
#include "converter.h"
#include "EDataType.h"
#include "EHisDevDirection.h"
#include "ValueEventArgs.h"

#include "HisDevValue.h"
#include "HisDevBase.h"
#include "HisDevIO2413.h"

HisDevIO2413::HisDevIO2413(LOW_devDS2413* pdev) :
	HisDevDallas::HisDevDallas(pdev)
{
	dev = pdev;
	CreateDataPoints();
}

HisDevIO2413::HisDevIO2413(xmlNodePtr node, LOW_devDS2413* pdev) :
	HisDevDallas::HisDevDallas(node, pdev)
{
	dev = pdev;
	valueAInput = NULL;
	valueBInput = NULL;
	valueAOutput = NULL;
	valueBOutput = NULL;
}

const xmlChar* HisDevIO2413::GetNodeNameInternal()
{
	return (const xmlChar*)"HisDevIO2413";
}

void HisDevIO2413::CreateDataPoints()
{
	std::string strid = dev->getID().getRomIDString();
	WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevIO2413, &HisDevIO2413::WriteToDevice>(this);
	//create data points
	//xmlNodePtr valuenode = NULL;//GetOrCreateNode(PINNO_INPUTA,GetNodePtr());
	valueAInput = new HisDevValue<bool>(strid, EHisDevDirection::Read, EDataType::Bool,PINNO_INPUTA,false);
	valueAInput->delegateWrite = delegate;
	valueAInput->Load();
	valueAInput->SetName("InputA");
	Add(valueAInput);

	//valuenode = GetOrCreateNode(PINNO_INPUTB,GetNodePtr());
	valueBInput = new HisDevValue<bool>(strid, EHisDevDirection::Read, EDataType::Bool,PINNO_INPUTB,false);
	valueBInput->delegateWrite = delegate;
	valueBInput->Load();
	valueBInput->SetName("InputB");
	Add(valueBInput);

	//valuenode = GetOrCreateNode(PINNO_OUTPUTA,GetNodePtr());
	valueAOutput = new HisDevValue<bool>(strid,EHisDevDirection::ReadWrite,EDataType::Bool,PINNO_OUTPUTA,false);
	valueAOutput->delegateWrite = delegate;
	valueAOutput->Load();
	valueAOutput->SetName("OutputA");
	Add(valueAOutput);

	//valuenode = GetOrCreateNode(PINNO_OUTPUTB,GetNodePtr());
	valueBOutput = new HisDevValue<bool>(strid,EHisDevDirection::ReadWrite,EDataType::Bool,PINNO_OUTPUTB,false);
	valueBOutput->delegateWrite = delegate;
	valueBOutput->Load();
	valueBOutput->SetName("OutputB");
	Add(valueBOutput);
}


void HisDevIO2413::WriteToDevice(ValueChangedEventArgs args)
{
	switch(Converter::stoi(args.GetValue()->GetPinNumber()))
	{
		case PINNO_INPUTA:
		case PINNO_INPUTB:
			break;
		case PINNO_OUTPUTA:
		case PINNO_OUTPUTB:
			dev->cmd_WritePIO(	valueAOutput->GetValue() ? LOW_devDS2413::pioTransistorOn : LOW_devDS2413::pioTransistorOff,
								valueBOutput->GetValue() ? LOW_devDS2413::pioTransistorOn : LOW_devDS2413::pioTransistorOff);
			break;
	}
}

void HisDevIO2413::DoInternalRefresh(bool alarm)
{
	uint8_t status;
	bool devError = false;
	try
	{
		dev->cmd_ReadStatus(status);
	}
	catch(...)
	{
		devError = true;
	}

	bool ioatemp = (status & LOW_devDS2413::PIOAPinStateBit) != 0;
	bool iobtemp = (status & LOW_devDS2413::PIOBPinStateBit) != 0;

	bool latchA = (status & LOW_devDS2413::PIOAOutLatchStateBit) != 0;
	bool latchB = (status & LOW_devDS2413::PIOBOutLatchStateBit) != 0;

	valueAInput->ReadedValueFromDevice(ioatemp,devError);
	valueBInput->ReadedValueFromDevice(iobtemp,devError);

	valueAOutput->ReadedValueFromDevice(latchA,devError);
	valueBOutput->ReadedValueFromDevice(latchB,devError);

	SetError(devError);
}

//void HisDevIO2413::DoInternalSave(xmlNodePtr & node)
//{
//	HisDevDallas::DoInternalSave(node);
//
//	for(uint16_t i=0;i<values.size();i++)
//	{
//		values[i]->Save();
//	}
//}
//
void HisDevIO2413::DoInternalLoad(xmlNodePtr & node)
{
	HisDevDallas::DoInternalLoad(node);
	WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevIO2413, &HisDevIO2413::WriteToDevice>(this);

	vector<HisDevValue<bool>*> values = GetItems<HisDevValue<bool>>();

	for(size_t i=0;i<values.size();i++)
	{
		HisDevValue<bool> *value = values[i];
		value->delegateWrite = delegate;
		switch(Converter::stoi(value->GetPinNumber()))
		{
			case PINNO_INPUTA:
				valueAInput = value;
				break;
			case PINNO_INPUTB:
				valueBInput = value;
				break;
			case PINNO_OUTPUTA:
				valueAOutput = value;
				break;
			case PINNO_OUTPUTB:
				valueBOutput = value;
				break;
		}
	}
}


HisDevIO2413::~HisDevIO2413()
{

}

