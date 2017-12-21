/*
 * ModbusManager.cpp
 *
 *  Created on: 21. 10. 2016
 *      Author: pc
 */
#include <stddef.h>
#include "converter.h"
#include "Common/HisException.h"
#include "Modbus/ModbusWrapper.h"
#include "Modbus/ModbusSimulator.h"
#include <Modbus/ModbusProvider.h>
#include "StringBuilder.h"

ModbusProvider::ModbusProvider(vector<SSerPortConfig> & serports)
{
	for (size_t i=0;i<serports.size();i++)
	{
		IModbus* m = CreateDriver(serports[i]);
		Add(m);
	}
}

IModbus* ModbusProvider::CreateDriver(SSerPortConfig serport)
{
	std::string driverType = Converter::strtolower((serport.Driver));
	IModbus* m = NULL;

	if (driverType=="modbus")
	{
		m = new Modbus(serport);
	}
	else if (driverType=="modbussimulator")
	{
		m = new ModbusSimulator(serport);
	}
	else
	{
		string message = StringBuilder::Format("Unsupported driver type %s",driverType.c_str());
		throw HisException(message);
	}

	if (!m->Init())
		throw HisException(StringBuilder::Format("Init of driver %s failed.",serport.Name.c_str()));

	return m;
}

void ModbusProvider::Add(IModbus* connector)
{
	connectors.push_back(connector);
}

IModbus* ModbusProvider::Find(string name)
{
	for(size_t i=0;i<connectors.size();i++)
	{
		if (connectors[i]->GetName()==name)
			return connectors[i];
	}
	return NULL;
}

ModbusProvider::~ModbusProvider() {
	// TODO Auto-generated destructor stub
}

