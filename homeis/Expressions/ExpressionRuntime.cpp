/*
 * ExpressionRuntime.cpp
 *
 *  Created on: Nov 8, 2013
 *      Author: Josef Lola
 */

#include <unistd.h>

#include "homeis/Common/HisLock.h"
#include "ExpressionRuntime.h"
#include "homeis/Helpers/logger.h"

extern "C" {

#include "stack_trace.h"

}

ExpressionRuntime::ExpressionRuntime()
{
	this->__expressionMutex = HisLock::CreateMutex();
	running = false;
}



void ExpressionRuntime::Evaluate()
{
	HisLock lock(this->__expressionMutex);
	for(size_t i=0;i<expressions.size();i++)
	{
		expressions[i]->Evaluate();
	}
}

void* ExpressionRuntime::ThreadFunction(void* obj)
{
	//set_signal_handler("/home/linaro/homeis/dis");
	ExpressionRuntime* runtime = (ExpressionRuntime*)obj;

	while(runtime->running)
	{
		usleep(100);
		runtime->Evaluate();
	}

	return NULL;
}

void ExpressionRuntime::Add(IExpression* pExpression)
{
	HisLock lock(this->__expressionMutex);
	int index = Find(pExpression);
	if (index<0)
	{
		expressions.push_back(pExpression);
	}
}

void ExpressionRuntime::Remove(IExpression* pExpression)
{
	HisLock lock(this->__expressionMutex);

	int index = Find(pExpression);
	if (index>=0)
	{
		expressions.erase(expressions.begin() + index);
	}
}

int ExpressionRuntime::Find(IExpression* expression)
{
	for(size_t i=0;i<expressions.size();i++)
	{
		if (expressions[i]->GetRecordId()==expression->GetRecordId())
			return i;
	}
	return -1;
}

void ExpressionRuntime::Start()
{
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
