/*
 * HisDevRunTime.h
 *
 *  Created on: Mar 16, 2013
 *      Author: Josef Lola
 */

#ifndef HISDEVRUNTIME_H_
#define HISDEVRUNTIME_H_

#include <pthread.h>
#include "HisDevices.h"
#include "logger.h"
#include "BlockingQueue.h"


class HisDevRuntime
{
	BlockingQueue<int>* queue;
	ILogger & logger;
	HisDevices & devices;
	pthread_t refreshThread;
	bool running;
	static void* RefreshThreadFunction(void* obj);
public:
	HisDevRuntime(HisDevices & pdevices);
	void Refresh();
	void Start();
	void Stop();
};

#endif /* HISDEVRUNTIME_H_ */
