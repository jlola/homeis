/*
 * HisDevRunTime.cpp
 *
 *  Created on: Mar 16, 2013
 *      Author: Josef Lola
 */
#include <unistd.h>
#include <vector>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "srutil/event/event.hpp"
#include "srutil/delegate/delegate.hpp"
#include "LOW_network.h"
#include "EHisDevDirection.h"
#include "HisDevBase.h"
#include "ValueEventArgs.h"
#include "HisDevRunTime.h"

HisDevRuntime::HisDevRuntime(HisDevices & pdevices) :
	devices(pdevices),thread(0),running(false)
{
}

void* HisDevRuntime::ThreadFunction(void* obj)
{
	HisDevRuntime* runtime = (HisDevRuntime*)obj;

	while(runtime->running)
	{
		usleep(10);
		runtime->devices.Refresh();
	}

	return NULL;
}

void HisDevRuntime::Start()
{
	running = true;
	pthread_create( &thread, NULL, HisDevRuntime::ThreadFunction, (void*)this );
}

void HisDevRuntime::Stop()
{
	running = false;
	pthread_join( thread, NULL);
}
