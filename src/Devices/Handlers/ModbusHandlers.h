/*
 * ModbusHandlers.h
 *
 *  Created on: 28. 12. 2017
 *      Author: pc
 */

#ifndef SRC_DEVICES_HANDLERS_MODBUSHANDLERS_H_
#define SRC_DEVICES_HANDLERS_MODBUSHANDLERS_H_

#include <vector>
#include "IModbusHandler.h"
#include "HisDevModbus.h"

class HisDevModbus;

class ModbusHandlers : public IModbusHandler
{
	HisDevModbus* dev;
	vector<IModbusHandler*> handlers;
public:
	bool Remove(CUUID id);
	ModbusHandlers(HisDevModbus* dev,IHisDevFactory* factory);
	bool Scan(bool addnew);
	void RefreshOutputs();
	void Refresh(bool modbusSuccess);
	void Load();
	virtual ~ModbusHandlers();
};

#endif /* SRC_DEVICES_HANDLERS_MODBUSHANDLERS_H_ */
