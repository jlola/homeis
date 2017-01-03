/*
 * ModbusManager.h
 *
 *  Created on: 21. 10. 2016
 *      Author: pc
 */

#ifndef SRC_MODBUS_MODBUSMANAGER_H_
#define SRC_MODBUS_MODBUSMANAGER_H_

#include <vector>
#include <string.h>
#include "IModbus.h"

using namespace std;

class ModbusManager {
	vector<IModbus*> connectors;
public:
	IModbus* Find(string name);
	ModbusManager();
	virtual ~ModbusManager();
	void Add(IModbus* connector);
};

#endif /* SRC_MODBUS_MODBUSMANAGER_H_ */
