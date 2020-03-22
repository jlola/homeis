/*
 * BinnaryInputHandler.cpp
 *
 *  Created on: 16. 12. 2017
 *      Author: pc
 */

#include "BinnaryInputHandler.h"
#include "StringBuilder.h"

string BinnaryInputHandler::LoadType = "BinnaryInputHandler";

BinnaryInputHandler::BinnaryInputHandler(IHisDevModbus* dev,IHisDevFactory* factory)
	: factory(factory)
{
	sbininputs = NULL;
	this->dev = dev;
}

bool BinnaryInputHandler::Scan(bool addnew)
{
	STACK
	if (dev->GetTypeDef(ETypes::BinInputs,&stypedef))
	{
		uint16_t* data = NULL;
		uint8_t size;
		if (dev->GetData(data,size))
		{
			sbininputs = reinterpret_cast<SBinInput*>(&data[stypedef.OffsetOfType]);
			CreateOrValidInputs(addnew);
			return true;
		}
		else
		{
			sbininputs = NULL;
		}
	}
	else
	{
		sbininputs = NULL;
	}
	return false;
}

void BinnaryInputHandler::Load()
{
	STACK
	vector<HisDevValue<bool>*> values = dev->GetBoolItems();

	for(size_t i=0;i<values.size();i++)
	{
		HisDevValue<bool> *value = values[i];

		if (value->GetLoadType()==LoadType)
			valuesInput.push_back(value);
		else
		{
			switch(value->GetDirection())
			{
				case EHisDevDirection::Read:
					valuesInput.push_back(value);
					break;
				default:
					break;
			}
		}
	}
}

void BinnaryInputHandler::CreateOrValidInputs(bool addnew)
{
	STACK
	if (sbininputs!=NULL)
	{
		for(int i=0;i<stypedef.Count;i++)
		{

			SBinInput sinput = sbininputs[i];
			HisDevValueBase* valuebase = dev->FindValue(Converter::itos(sinput.PinNumber));
			HisDevValue<bool>* input = NULL;
			input = dynamic_cast<HisDevValue<bool>*>(valuebase);
			if ((input==NULL && addnew) || input!=NULL)
			{
				if (input==NULL)
				{
					input = new HisDevValue<bool>(Converter::itos(dev->GetAddress(),10),
							EHisDevDirection::Read,
							EDataType::Bool,
							Converter::itos(sinput.PinNumber),
							false,
							LoadType,
							factory,
							dev);
					input->SetName(StringBuilder::Format("BinaryInput%d",sinput.PinNumber));
					input->SetAddressName(StringBuilder::Format("BinaryInput%d",sinput.PinNumber));
					dev->Add(input);
				}
				input->SetValue(sinput.Value);
				valuesInput.push_back(input);
			}
		}
	}
}

bool BinnaryInputHandler::Remove(CUUID id)
{
	STACK
	if (valuesInput.size()>0)
	{
		size_t index=-1;
		for(size_t v=0;v<valuesInput.size();v++)
		{
			if (valuesInput[v]->GetRecordId()==id)
			{
				index = v;
				break;
			}
		}
		if (index>=0)
		{
			valuesInput.erase(valuesInput.begin() + index);
			return true;
		}
	}
	return false;
}

void BinnaryInputHandler::RefreshOutputs(bool force)
{

}

void BinnaryInputHandler::Refresh(bool modbusSuccess)
{
	STACK

	for(size_t v=0;v<valuesInput.size();v++)
	{
		if (sbininputs!=NULL)
		{
			for(int i=0;i<stypedef.Count;i++)
			{
				int strpinno = Converter::stoi(valuesInput[v]->GetPinNumber());
				if (strpinno==sbininputs[i].PinNumber)
				{
					bool readedrror = sbininputs[i].Quality == 1 ? false : true;
					bool value = sbininputs[i].Value == 1 ? true : false;
					valuesInput[v]->ReadedValueFromDevice(value,readedrror && !modbusSuccess);
					break;
				}
			}
		}
		else
		{
			valuesInput[v]->ReadedValueFromDevice(valuesInput[v]->GetValue(),true);
		}
	}
}

BinnaryInputHandler::~BinnaryInputHandler() {
}

