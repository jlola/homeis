/*
 * HisDevIO2408.cpp
 *
 *  Created on: 22. 11. 2015
 *      Author: Josef Lola
 */

#include <HisDevIO2408.h>
#include "PoppyDebugTools.h"

HisDevIO2408::HisDevIO2408(LOW_devDS2408* pdev):
	HisDevDallas::HisDevDallas(pdev),dev(pdev)
{
	STACK
	delegate = WriteToDeviceRequestDelegate::from_method<HisDevIO2408, &HisDevIO2408::WriteToDevice>(this);
	SetName((char*)GetNodeName());
	dev->WriteControlStatus(0x00);
	SetError(!dev->WritePIO(0x00));
	memset(valueOutput,0x00,sizeof(HisDevValue<bool>*)*8);
	CreateDataPoints(GetError());
}

HisDevIO2408::HisDevIO2408(xmlNodePtr node,LOW_devDS2408* pdev):
	HisDevDallas::HisDevDallas(node, pdev),dev(pdev)
{
	STACK
	delegate = WriteToDeviceRequestDelegate::from_method<HisDevIO2408, &HisDevIO2408::WriteToDevice>(this);
	dev->WriteControlStatus(0x00);
	SetError(!dev->WritePIO(0x00));
	memset(valueOutput,0x00,sizeof(HisDevValue<bool>*)*8);
}

void HisDevIO2408::WriteToDevice(ValueChangedEventArgs args)
{
	STACK
	HisDevValue<bool>* dpoint = valueOutput[args.GetValue()->GetPinNumber()];
	SetError(!dev->WritePIO(dpoint->GetPinNumber(),dpoint->GetValue()));
	dpoint->ReadedValueFromDevice(dpoint->GetValue(),GetError());
	SetChanged();
	NeedRefresh();
}

void HisDevIO2408::CreateDataPoints(bool error)
{
	STACK
	std::string strid = dev->getID().getRomIDString();
	for(int i=0;i<8;i++)
	{
		HisDevValue<bool>* point = new HisDevValue<bool>(strid, EHisDevDirection::ReadWrite, EDataType::Bool, i,false);
		point->delegateWrite = delegate;
		point->Load();
		point->ReadedValueFromDevice(false,error);
		valueOutput[i] = point;
		Add(point);
	}
}

void HisDevIO2408::DoInternalRefresh(bool alarm)
{
	uint8_t pio = 0;
	for(int i=0;i<8;i++)
	{
		if (valueOutput[i]->GetValue()) pio |= (1 << i);
	}

	//CLogger::Info("DS2408 - before WritePIO");
	bool newerror = !dev->WritePIO(pio);
	//CLogger::Info("DS2408 - after WritePIO");

	if (newerror!=GetError())
	{
		if (!newerror)
		{
			dev->WriteControlStatus(0x00);
		}
		SetError(newerror);
		for(int i=0;i<8;i++)
			valueOutput[i]->ReadedValueFromDevice(valueOutput[i]->GetValue(),newerror);
	}
}

void HisDevIO2408::DoInternalSave(xmlNodePtr & node)
{
	HisDevDallas::DoInternalSave(node);
}

void HisDevIO2408::DoInternalLoad(xmlNodePtr & node)
{
	STACK
	HisDevDallas::DoInternalLoad(node);

	vector<HisDevValue<bool>*> values = GetItems<HisDevValue<bool>>();

	for(size_t i=0;i<values.size();i++)
	{
		HisDevValue<bool> *value = values[i];
		value->delegateWrite = delegate;
		valueOutput[value->GetPinNumber()] = value;
		value->SetValue(false);
		//error = !dev->WritePIO(value->GetPinNumber() ,value->GetValue());
		value->ReadedValueFromDevice(false,GetError());

	}
}
const xmlChar* HisDevIO2408::GetNodeNameInternal()
{
	return (const xmlChar *)"DS2408";
}

HisDevIO2408::~HisDevIO2408() {

}

