/*
 * DeviceRefreshTimer.h
 *
 *  Created on: 11. 12. 2019
 *      Author: pc
 */

#ifndef SRC_MODBUS_LISTENERS_DEVICEREFRESHTIMER_H_
#define SRC_MODBUS_LISTENERS_DEVICEREFRESHTIMER_H_

#include <thread>
#include "HisDevBase.h"
#include "HisDevices.h"
#include "IBlockingQueue.h"

class DeviceRefreshTimer
{
	bool running;
	pthread_t processThread;
	HisDevices & devices;
public:
	DeviceRefreshTimer(HisDevices & devices, IBlockingQueue<HisDevBase*>* queue);
	static void* ProcessThreadFunction(void* obj);
	void Start();
	void Stop();
	virtual ~DeviceRefreshTimer();
};

#endif /* SRC_MODBUS_LISTENERS_DEVICEREFRESHTIMER_H_ */
