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
#include "ModifiedMdbus.h"

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

	if (driverType==Modbus::DriverName)
	{
		m = new ModifiedMdbus(serport);
	}
	else if (driverType==ModbusSimulator::DriverName)
	{
		m = new ModbusSimulator(serport);
	}
	else
	{
		string message = StringBuilder::Format("Unsupported driver type %s",driverType.c_str());
		throw MException(message);
	}

	if (!m->Init())
		throw MException(StringBuilder::Format("Init of driver %s failed.",serport.Name.c_str()));

	return m;
}

vector<IModbus*> & ModbusProvider::GetConnectors()
{
	return connectors;
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

