/*
 * ModbusManager.cpp
 *
 *  Created on: 21. 10. 2016
 *      Author: pc
 */
#include <stddef.h>
#include <Modbus/ModbusManager.h>

ModbusManager::ModbusManager() {
	// TODO Auto-generated constructor stub

}

void ModbusManager::Add(IModbus* connector)
{
	connectors.push_back(connector);
}

IModbus* ModbusManager::Find(string name)
{
	for(size_t i=0;i<connectors.size();i++)
	{
		if (connectors[i]->GetName()==name)
			return connectors[i];
	}
	return NULL;
}

ModbusManager::~ModbusManager() {
	// TODO Auto-generated destructor stub
}

