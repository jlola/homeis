/*
 * ModbusProcessor.h
 *
 *  Created on: 11. 12. 2019
 *      Author: pc
 */

#ifndef SRC_MODBUS_MODBUSPROCESSOR_H_
#define SRC_MODBUS_MODBUSPROCESSOR_H_

#include "IBlockingQueue.h"
#include "HisDevBase.h"

/*
 * consumer of blocking queue
 */
class ModbusProcessor {
	IBlockingQueue<HisDevBase*>* deviceQueue;
	pthread_t processThread;
	bool running;
	static void* ProcessThreadFunction(void* args);
	void Process();
public:
	ModbusProcessor(IBlockingQueue<HisDevBase*>* deviceQueue);
	void Start();
	void Stop();
	virtual ~ModbusProcessor();
};

#endif /* SRC_MODBUS_MODBUSPROCESSOR_H_ */
