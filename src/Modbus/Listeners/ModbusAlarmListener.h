/*
 * ModbusAlarmListener.h
 *
 *  Created on: 11. 12. 2019
 *      Author: pc
 */

#ifndef SRC_MODBUS_LISTENERS_MODBUSALARMLISTENER_H_
#define SRC_MODBUS_LISTENERS_MODBUSALARMLISTENER_H_

#include "BlockingQueue.h"
#include "HisDevModbus.h"

class ModbusAlarmListener {
public:
	ModbusAlarmListener(BlockingQueue<HisDevBase*>* queue);

	void Start();
	void Stop();

	virtual ~ModbusAlarmListener();
};

#endif /* SRC_MODBUS_LISTENERS_MODBUSALARMLISTENER_H_ */
