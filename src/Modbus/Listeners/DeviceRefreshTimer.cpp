/*
 * DeviceRefreshTimer.cpp
 *
 *  Created on: 11. 12. 2019
 *      Author: pc
 */

#include "DeviceRefreshTimer.h"

DeviceRefreshTimer::DeviceRefreshTimer(HisDevices & devices, IBlockingQueue<HisDevBase*>* mqueue)
: running(false), processThread(0),devices(devices)
{

}

void* DeviceRefreshTimer::ProcessThreadFunction(void* obj)
{
	//devices.Refresh();

	return NULL;
}

void DeviceRefreshTimer::Start()
{
	running = true;
	pthread_create( &processThread, NULL, DeviceRefreshTimer::ProcessThreadFunction, (void*)this );
}

void DeviceRefreshTimer::Stop()
{
	running = false;
	pthread_join( processThread, NULL);
	processThread = 0;
}

DeviceRefreshTimer::~DeviceRefreshTimer() {

}

