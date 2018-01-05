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

BinnaryOutputHandler::BinnaryOutputHandler(HisDevModbus* devModbus, IHisDevFactory* factory)
	: count(0),devModbus(devModbus),sbinoutputs(NULL),factory(factory) {
	if (devModbus==NULL)
	{
		string message = StringBuilder::Format("NULL value of  devModbus Line: %s, Value: %d.", __FILE__, __LINE__);
		CLogger::Error(message.c_str());
		throw HisException(message,__FILE__,__LINE__);
	}
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
	vector<HisDevValue<bool>*> values = devModbus->GetItems<HisDevValue<bool>>();

	for(size_t i=0;i<values.size();i++)
	{
		HisDevValue<bool> *value = values[i];

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
	if (devModbus->GetTypeDef(ETypes::BinOutputs,stypedef))
	{
		uint16_t* data;
		uint8_t size;
		devModbus->GetData(data,size);
		sbinoutputs = reinterpret_cast<SBinOutput*>(&data[stypedef.OffsetOfType]);
		CreateOrValidOutputs(addnew);
	}
	return true;
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
				output = new HisDevValue<bool>(Converter::itos(devModbus->GetAddress(),10),EHisDevDirection::Write,EDataType::Bool,soutput.PinNumber,false,LoadType,factory);
				devModbus->Add(output);
				valuesOutput.push_back(output);
			}
			WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevModbus, &HisDevModbus::WriteToDevice>(devModbus);
			output->delegateWrite = delegate;
			output->ReadedValueFromDevice(soutput.Value,false);
		}
	}
}

BinnaryOutputHandler::~BinnaryOutputHandler() {

}

