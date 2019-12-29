/*
 * ModbusProcessor.cpp
 *
 *  Created on: 11. 12. 2019
 *      Author: pc
 */

#include <ModbusProcessor.h>

ModbusProcessor::ModbusProcessor(IBlockingQueue<HisDevBase*>* deviceQueue)
: deviceQueue(deviceQueue),
  processThread(0),
  running(false)
{

}

void* ModbusProcessor::ProcessThreadFunction(void* args)
{
	ModbusProcessor* modbusProcessor = (ModbusProcessor*)args;
	modbusProcessor->Process();
	return NULL;
}

void ModbusProcessor::Process()
{
	HisDevBase* dev;
	while(running)
	{
		if (deviceQueue->Pop(dev))
		{
			dev->Refresh();
		}
		else
		{
			break;
		}

	}
}

void ModbusProcessor::Start()
{
	running = true;
	if (processThread==0)
		pthread_create( &processThread, NULL, ModbusProcessor::ProcessThreadFunction, (void*)this );
}

void ModbusProcessor::Stop()
{
	running = false;
	deviceQueue->RequestShutdown();
	pthread_join( processThread, NULL);
	processThread = 0;
}

ModbusProcessor::~ModbusProcessor() {

}

