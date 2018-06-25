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
#include "PoppyDebugTools.h"
#include "HisException.h"
#include "RFIDHandler.h"

using namespace std;

ModbusHandlers::ModbusHandlers(HisDevModbus* dev,IHisDevFactory* factory) :
	dev(dev)
{
	STACK

	if (dev==NULL)
		throw ArgumentNullException("dev");
	if (factory==NULL)
		throw ArgumentNullException("factory");

	handlers.push_back(new BinnaryInputHandler(this->dev,factory));
	handlers.push_back(new BinnaryOutputHandler(this->dev,factory));
	handlers.push_back(new OneWireHandler(this->dev,factory));
	handlers.push_back(new RFIDHandler(this->dev,factory));
}

void ModbusHandlers::Load()
{
	STACK
	for(size_t i=0;i<handlers.size();i++)
	{
		handlers[i]->Load();
	}
}

bool ModbusHandlers::Remove(CUUID id)
{
	for(size_t i=0;i<handlers.size();i++)
	{
		if (handlers[i]->Remove(id))
			return true;
	}
	return false;
}

bool ModbusHandlers::Scan(bool addnew)
{
	STACK
	bool result = true;
	for(size_t i=0;i<handlers.size();i++)
	{
		result &= handlers[i]->Scan(addnew);
	}
	return result;
}

void ModbusHandlers::RefreshOutputs()
{
	STACK
	for(size_t i=0;i<handlers.size();i++)
	{
		handlers[i]->RefreshOutputs();
	}
}
void ModbusHandlers::Refresh(bool modbusSuccess)
{
	STACK
	for(size_t i=0;i<handlers.size();i++)
	{
		handlers[i]->Refresh(modbusSuccess);
	}
}

ModbusHandlers::~ModbusHandlers()
{
	STACK
	for(size_t i=0;i<handlers.size();i++)
	{
		delete handlers[i];
		handlers[i] = NULL;
	}
}

