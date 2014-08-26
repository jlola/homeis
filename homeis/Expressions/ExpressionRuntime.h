/*
 * ExpressionRuntime.h
 *
 *  Created on: Nov 8, 2013
 *      Author: Josef Lola
 */

#ifndef EXPRESSIONRUNTIME_H_
#define EXPRESSIONRUNTIME_H_

#include <vector>
#include <pthread.h>

#include "LOW_thread_mutex.h"
#include "LOW_thread_Factory.h"
#include "IExpression.h"

using namespace std;

class ExpressionRuntime
{
	pthread_attr_t attr;
	pthread_t thread;
	bool running;
	vector<IExpression*> expressions;
	static void* ThreadFunction(void* obj);
public:
	int Find(IExpression* expression);
	ExpressionRuntime();
	LOW_thread_mutex  *__expressionMutex;  /**< Mutex for exclusive access. */
	void Add(IExpression* pExpression);
	void Remove(IExpression* pExpression);
	void Evaluate();
	void Start();
	void Stop();
};

#endif /* EXPRESSIONRUNTIME_H_ */
