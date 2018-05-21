/*
 * BinnaryOutputHandler.cpp
 *
 *  Created on: 16. 12. 2017
 *      Author: pc
 */

#include "BinnaryOutputHandler.h"
#include "PoppyDebugTools.h"
#include "converter.h"
#include "StringBuilder.h"
#include "HisException.h"

string BinnaryOutputHandler::LoadType = "BinnaryOutputHandler";

BinnaryOutputHandler::BinnaryOutputHandler(IHisDevModbus* devModbus, IHisDevFactory* factory)
	: count(0),devModbus(devModbus),sbinoutputs(NULL),factory(factory)
{
	if (devModbus==NULL)
		throw ArgumentNullException(string("devModbus"));
	if (factory==NULL)
		throw ArgumentNullException(string("factory"));
}

void BinnaryOutputHandler::RefreshOutputs()
{
	STACK
	for(size_t v=0;v<valuesOutput.size();v++)
	{
		int pinno = Converter::stoi(valuesOutput[v]->GetPinNumber());

		for(int i=0;i<stypedef.Count;i++)
		{
			if (pinno==sbinoutputs[i].PinNumber)
			{
				//bool readedrror = sbinoutputs[i].Quality == 1 ? false : true;
				sbinoutputs[i].Value = valuesOutput[v]->GetValue();
				uint16_t writevalue = *((uint16_t*)&sbinoutputs[i]);
				bool success = devModbus->setHolding(stypedef.OffsetOfType+i,writevalue);
				valuesOutput[v]->ReadedValueFromDevice(valuesOutput[v]->GetValue(),!success);
				break;
			}
		}
	}
}

void BinnaryOutputHandler::Load()
{
	STACK
	vector<HisDevValue<bool>*> values = devModbus->GetBoolItems();

	for(size_t i=0;i<values.size();i++)
	{
		HisDevValue<bool> *value = values[i];
		value->SetDevice(devModbus);
		if (value->GetLoadType()==LoadType)
			valuesOutput.push_back(value);
		else
		{
			switch(value->GetDirection())
			{
				case EHisDevDirection::ReadWrite:
				case EHisDevDirection::Write:
				{
					int pinno = Converter::stoi(value->GetPinNumber());
					if (pinno!=OW_SCAN_PINNUMBER)
						valuesOutput.push_back(value);
					break;
				}
				default:
					break;
			}
		}
	}
}

void BinnaryOutputHandler::Refresh(bool modbusSuccess)
{

}

bool BinnaryOutputHandler::Scan(bool addnew)
{
	STACK
	if (devModbus->GetTypeDef(ETypes::BinOutputs,&stypedef))
	{
		uint16_t* data;
		uint8_t size;
		if (devModbus->GetData(data,size))
		{
			sbinoutputs = reinterpret_cast<SBinOutput*>(&data[stypedef.OffsetOfType]);
			CreateOrValidOutputs(addnew);
			return true;
		}
	}
	return false;
}

bool BinnaryOutputHandler::Remove(CUUID id)
{
	if (valuesOutput.size()>0)
	{
		size_t index=-1;
		for(size_t v=0;v<valuesOutput.size();v++)
		{
			if (valuesOutput[v]->GetRecordId()==id)
			{
				index = v;
				break;
			}
		}
		if (index>=0)
		{
			valuesOutput.erase(valuesOutput.begin() + index);
			return true;
		}
	}
	return false;
}

void BinnaryOutputHandler::CreateOrValidOutputs(bool addnew)
{
	STACK
	for(int i=0;i<stypedef.Count;i++)
	{
		SBinOutput soutput = sbinoutputs[i];
		HisDevValueBase* valuebase = devModbus->FindValue(Converter::itos(soutput.PinNumber));

		HisDevValue<bool>* output = NULL;
		output = dynamic_cast<HisDevValue<bool>*>(valuebase);

		if (( output==NULL && addnew )|| output != NULL)
		{
			if (output == NULL)
			{
				output = new HisDevValue<bool>(
						Converter::itos(devModbus->GetAddress(),10),
						EHisDevDirection::Write,
						EDataType::Bool,
						soutput.PinNumber,
						false,
						LoadType,
						factory,
						devModbus);
				output->SetName(StringBuilder::Format("BinaryOutput%d",soutput.PinNumber));
				devModbus->Add(output);
			}
			valuesOutput.push_back(output);
			output->SetDevice(devModbus);
			//WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevModbus, &HisDevModbus::WriteToDevice>(devModbus);
			//output->delegateWrite = delegate;
			output->ReadedValueFromDevice(soutput.Value,false);
		}
	}
}

BinnaryOutputHandler::~BinnaryOutputHandler() {

}

