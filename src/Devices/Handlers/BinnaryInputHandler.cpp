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
	if (dev->GetTypeDef(ETypes::BinInputs,stypedef))
	{
		uint16_t* data;
		uint8_t size;
		dev->GetData(data,size);
		sbininputs = reinterpret_cast<SBinInput*>(&data[stypedef.OffsetOfType]);
		CreateOrValidInputs(addnew);
	}
	return false;
}

void BinnaryInputHandler::Load()
{
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

void BinnaryInputHandler::RefreshOutputs()
{

}

void BinnaryInputHandler::Refresh(bool modbusSuccess)
{
	STACK
	for(size_t v=0;v<valuesInput.size();v++)
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
}

BinnaryInputHandler::~BinnaryInputHandler() {
	// TODO Auto-generated destructor stub
}

