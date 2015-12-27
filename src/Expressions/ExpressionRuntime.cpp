/*
 * ExpressionRuntime.cpp
 *
 *  Created on: Nov 8, 2013
 *      Author: Josef Lola
 */

#include <unistd.h>
#include <iostream>

#include "Common/HisLock.h"
#include "ExpressionRuntime.h"
#include "Helpers/logger.h"
#include "PoppyDebugTools.h"

extern "C" {

#include "stack_trace.h"

}

ExpressionRuntime::ExpressionRuntime() : thread(0)
{
	STACK
	this->__expressionMutex = HisLock::CreateMutex();
	running = false;
}



void ExpressionRuntime::Evaluate()
{
	STACK

	HisLock lock(this->__expressionMutex);
	for(size_t i=0;i<expressions.size();i++)
	{

		//int* test = 0x00;
		//*test = 10;
		expressions[i]->Evaluate();
	}
}

void* ExpressionRuntime::ThreadFunction(void* obj)
{
	STACK
	//set_signal_handler("/home/linaro/homeis/dis");
	ExpressionRuntime* runtime = (ExpressionRuntime*)obj;

	while(runtime->running)
	{
		usleep(10000);
		runtime->Evaluate();
	}

	return NULL;
}

void ExpressionRuntime::Add(IExpression* pExpression)
{
	STACK
	HisLock lock(this->__expressionMutex);
	int index = Find(pExpression);
	if (index<0)
	{
		expressions.push_back(pExpression);
	}
}

void ExpressionRuntime::Remove(IExpression* pExpression)
{
	STACK
	HisLock lock(this->__expressionMutex);

	int index = Find(pExpression);
	if (index>=0)
	{
		expressions.erase(expressions.begin() + index);
	}
}

int ExpressionRuntime::Find(IExpression* expression)
{
	STACK

	for(size_t i=0;i<expressions.size();i++)
	{
		if (expressions[i]->GetRecordId()==expression->GetRecordId())
			return i;
	}
	return -1;
}

void ExpressionRuntime::Start()
{
	STACK
	/* Initialize thread creation attributes */

	int s = pthread_attr_init(&attr);
   if (s != 0)
	   //handle_error_en(s, "pthread_attr_init");
	   CLogger::Error("pthread_attr_init");


	running = true;
	pthread_create( &thread, &attr, ExpressionRuntime::ThreadFunction, (void*)this );
}
void ExpressionRuntime::Stop()
{
	running = false;
	pthread_join( thread, NULL);
}
