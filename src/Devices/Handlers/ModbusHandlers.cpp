/*
 * ModbusHandlers.cpp
 *
 *  Created on: 28. 12. 2017
 *      Author: pc
 */

#include <stddef.h>
#include "BinnaryInputHandler.h"
#include "BinnaryOutputHandler.h"
#include "OneWireHandler.h"
#include "ModbusHandlers.h"

using namespace std;

ModbusHandlers::ModbusHandlers(HisDevModbus* dev,IHisDevFactory* factory) :
	dev(dev)
{
	handlers.push_back(new BinnaryInputHandler(dev,factory));
	handlers.push_back(new BinnaryOutputHandler(dev,factory));
	handlers.push_back(new OneWireHandler(dev,factory));
}

void ModbusHandlers::Load()
{
	for(size_t i=0;i<handlers.size();i++)
	{
		handlers[i]->Load();
	}
//	vector<HisDevValue<bool>*> values = dev->GetItems<HisDevValue<bool>>();
//
//	for(size_t i=0;i<values.size();i++)
//	{
//		HisDevValue<bool> *value = values[i];
//		switch(value->GetDirection())
//		{
//			case EHisDevDirection::Read:
//				valuesInput.push_back(value);
//				break;
//			case EHisDevDirection::ReadWrite:
//			case EHisDevDirection::Write:
//				int pinno = Converter::stoi(value->GetPinNumber());
//				if (pinno==OW_SCAN_PINNUMBER)
//					scantag = value;
//				else
//					valuesOutput.push_back(value);
//				break;
//		}
//	}

}

bool ModbusHandlers::Scan(bool addnew)
{
	bool result = true;
	for(size_t i=0;i<handlers.size();i++)
	{
		result &= handlers[i]->Scan(addnew);
	}
	return result;
}

void ModbusHandlers::RefreshOutputs()
{
	for(size_t i=0;i<handlers.size();i++)
	{
		handlers[i]->RefreshOutputs();
	}
}
void ModbusHandlers::Refresh(bool modbusSuccess)
{
	for(size_t i=0;i<handlers.size();i++)
	{
		handlers[i]->Refresh(modbusSuccess);
	}
}

ModbusHandlers::~ModbusHandlers() {
	for(size_t i=0;i<handlers.size();i++)
	{
		delete handlers[i];
		handlers[i] = NULL;
	}
}

