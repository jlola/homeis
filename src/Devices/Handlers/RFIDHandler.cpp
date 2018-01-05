/*
 * RFIDHandler.cpp
 *
 *  Created on: 4. 1. 2018
 *      Author: pc
 */

#include <RFIDHandler.h>

string RFIDHandler::LoadType = "RFIDHandler";

RFIDHandler::RFIDHandler(HisDevModbus* devModbus,IHisDevFactory* factory) {
	if (devModbus==NULL)
	{
		string message = StringBuilder::Format("NULL value of  devModbus Line: %s, Value: %d.", __FILE__, __LINE__);
		CLogger::Error(message.c_str());
		throw HisException(message,__FILE__,__LINE__);
	}
	this->dev = devModbus;
	this->factory = factory;
}

bool RFIDHandler::Scan(bool addnew)
{
	STACK

	if (dev->GetTypeDef(ETypes::BinInputs,stypedef))
	{
		uint16_t* data;
		uint8_t size;
		dev->GetData(data,size);
		//sbininputs = reinterpret_cast<SBinInput*>(&data[stypedef.OffsetOfType]);
		CreateOrValidTags(addnew);
	}

	return true;
}

void RFIDHandler::CreateOrValidTags(bool addnew)
{
	for(int i=0;i<stypedef.Count;i++)
	{
//		SBinInput sinput = sbininputs[i];
//		HisDevValueBase* valuebase = dev->FindValue(Converter::itos(sinput.PinNumber));
//		HisDevValue<bool>* input = NULL;
//		input = dynamic_cast<HisDevValue<bool>*>(valuebase);
//		if ((input==NULL && addnew) || input!=NULL)
//		{
//			if (input==NULL)
//			{
//				input = new HisDevValue<bool>(Converter::itos(dev->GetAddress(),10),
//						EHisDevDirection::Read,
//						EDataType::Bool,
//						Converter::itos(sinput.PinNumber),
//						false,
//						LoadType,
//						factory);
//				dev->Add(input);
//			}
//			input->SetValue(sinput.Value);
//			valuesInput.push_back(input);
//		}
	}
}

void RFIDHandler::RefreshOutputs()
{

}

void RFIDHandler::Refresh(bool modbusSuccess)
{

}

void RFIDHandler::Load()
{
	vector<HisDevValue<uint>*> values = dev->GetItems<HisDevValue<uint>>();

	for(size_t i=0;i<values.size();i++)
	{
		HisDevValue<uint> *value = values[i];

		if (value->GetLoadType()==LoadType)
			values.push_back(value);
	}
}

RFIDHandler::~RFIDHandler() {
}

