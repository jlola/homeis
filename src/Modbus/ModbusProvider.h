/*
 * ModbusManager.h
 *
 *  Created on: 21. 10. 2016
 *      Author: pc
 */

#ifndef SRC_MODBUS_MODBUSPROVIDER_H_
#define SRC_MODBUS_MODBUSPROVIDER_H_


#include <vector>
#include <string.h>
#include "HomeIsConfig.h"
#include "IModbusProvider.h"

using namespace std;

class ModbusProvider : public IModbusProvider
{
private:
	vector<IModbus*> connectors;
	IModbus* CreateDriver(SSerPortConfig serport);
public:
	ModbusProvider(vector<SSerPortConfig> & serports);
	IModbus* Find(string name);
	vector<IModbus*> & GetConnectors();
	virtual ~ModbusProvider();
	void Add(IModbus* connector);
};

#endif /* SRC_MODBUS_MODBUSPROVIDER_H_ */
