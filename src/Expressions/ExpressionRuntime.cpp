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
	this->__addremoveMutex = HisLock::CreateMutex();
	expressionsToEvalMutex = HisLock::CreateMutex();
	running = false;
}

void ExpressionRuntime::AddToEvaluateQueue(HisDevBase* hisDevBase)
{
	HisLock lock(expressionsToEvalMutex);
	vector<IExpression*> expressionsToEval = hisDevBase->GetExpressions();
	for (size_t i=0;i<expressionsToEval.size();i++)
	{
		exqueue.push(expressionsToEval[i]);
		CLogger::Info("Added to evaluate %s to expresseionqueue",expressionsToEval[i]->GetName().c_str());
	}
}

void ExpressionRuntime::Evaluate()
{
	STACK
	IExpression* expr = NULL;

	for(size_t i=0;i<expressions.size();i++)
	{
		while(!exqueue.empty())
		{
			HisLock lock(expressionsToEvalMutex);
			expr = exqueue.front();
			exqueue.pop();
			lock.Unlock();

			CLogger::Info("Start queued expression evaluate %s",expr->GetName().c_str());
			expr->Evaluate();
			CLogger::Info("Stop queued expression evaluate %s",expr->GetName().c_str());
		}

		//int* test = 0x00;
		//*test = 10;
		expressions[i]->Evaluate();
		usleep(1000);
	}
}


void* ExpressionRuntime::ThreadFunction(void* obj)
{
	STACK
	//set_signal_handler("/home/linaro/homeis/dis");
	ExpressionRuntime* runtime = (ExpressionRuntime*)obj;
	CLogger::Info("Strat thread ExpressionRuntime::ThreadFunction with threadid: %ul",pthread_self());

	while(runtime->running)
	{
		try
		{
			runtime->Evaluate();
		}
		catch(std::exception & ex)
		{
			CLogger::Error("ExpressionRuntime::ThreadFunction exception. Error: %s",ex.what());
		}
		catch(...)
		{
			string msg = Stack::GetTraceString();
			CLogger::Error("ExpressionRuntime::ThreadFunction unexpected error. Stack: %s",msg.c_str());
		}
	}

	return NULL;
}

void ExpressionRuntime::Add(IExpression* pExpression)
{
	STACK
	HisLock lock(this->__addremoveMutex);
	int index = Find(pExpression);
	if (index<0)
	{
		expressions.push_back(pExpression);
	}
}

void ExpressionRuntime::Remove(IExpression* pExpression)
{
	STACK
	HisLock lock(this->__addremoveMutex);

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
	vector<IExpression*> expressionscopy(expressions);
	for(size_t i=0;i<expressionscopy.size();i++)
	{
		expressionscopy[i]->Start();
	}

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
