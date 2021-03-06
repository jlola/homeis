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

#include "IExpression.h"
#include "HisDevBase.h"
#include "IExpressionRuntime.h"
#include <queue>          // std::queue
#include "logger.h"

using namespace std;

class ExpressionRuntime : public IExpressionRuntime
{
	ILogger & logger;
	pthread_attr_t attr;
	pthread_t thread;
	bool running;
	vector<IExpression*> expressions;
	queue<IExpression*> exqueue;
	LPCRITICAL_SECTION expressionsToEvalMutex;  /**< Mutex for exclusive access. */
	static void* ThreadFunction(void* obj);
public:
	int Find(IExpression* expression);
	ExpressionRuntime();
	LPCRITICAL_SECTION __addremoveMutex;  /**< Mutex for exclusive access. */
	void AddToEvaluateQueue(HisDevBase* hisDevBase);
	void Add(IExpression* pExpression);
	void Remove(IExpression* pExpression);
	void Evaluate();
	void Start();
	void Stop();
};

#endif /* EXPRESSIONRUNTIME_H_ */
