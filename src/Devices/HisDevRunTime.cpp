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
#include "EHisDevDirection.h"
#include "HisDevBase.h"
#include "ValueEventArgs.h"
#include "PoppyDebugTools.h"
#include "HisDevRunTime.h"

HisDevRuntime::HisDevRuntime(HisDevices & pdevices) :
	logger(CLogger::GetLogger()),
	devices(pdevices),
	thread(0),
	running(false)
{
}

void* HisDevRuntime::ThreadFunction(void* obj)
{
	STACK
	HisDevRuntime* runtime = (HisDevRuntime*)obj;

	runtime->logger.Info("Strat thread HisDevRuntime::ThreadFunction with threadid: %ul",pthread_self());

	while(runtime->running)
	{
		runtime->devices.Refresh();
	}

	runtime->logger.Info("End thread HisDevRuntime::ThreadFunction with threadid: %ul",pthread_self());
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
