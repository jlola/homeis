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
#include <queue>          // std::queue

class HisDevRuntime
{
	std::queue<HisDevBase*> queue;
	HisDevices & devices;
	pthread_t thread;
	bool running;
	static void* ThreadFunction(void* obj);
public:
	HisDevRuntime(HisDevices & pdevices);
	void Start();
	void Stop();
};

#endif /* HISDEVRUNTIME_H_ */
