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
	refreshThread(0),
	running(false)
{
	queue = new BlockingQueue<int>();
}

void* HisDevRuntime::RefreshThreadFunction(void* obj)
{
	HisDevRuntime* runtime = (HisDevRuntime*)obj;

	runtime->Refresh();

	return NULL;
}

void HisDevRuntime::Refresh()
{
	STACK

	logger.Info("Strat thread HisDevRuntime::Refresh with threadid: %ul",pthread_self());

	while(running)
	{
		devices.Refresh();
		sleep(1);
	}

	logger.Info("End thread HisDevRuntime::Refresh with threadid: %ul",pthread_self());
}

void HisDevRuntime::Start()
{
	running = true;
	pthread_create( &refreshThread, NULL, HisDevRuntime::RefreshThreadFunction, (void*)this );
}

void HisDevRuntime::Stop()
{
	running = false;
	pthread_join( refreshThread, NULL);
}
