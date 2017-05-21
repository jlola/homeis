/*
 * IExpressionRuntime.h
 *
 *  Created on: 17. 3. 2017
 *      Author: pc
 */

#ifndef SRC_EXPRESSIONS_IEXPRESSIONRUNTIME_H_
#define SRC_EXPRESSIONS_IEXPRESSIONRUNTIME_H_

#include "IExpression.h"

class IExpressionRuntime {
protected:
	IExpressionRuntime() {}
public:
	virtual void Add(IExpression* pExpression) = 0;
	virtual void Remove(IExpression* pExpression) = 0;
	virtual ~IExpressionRuntime() {}
};

#endif /* SRC_EXPRESSIONS_IEXPRESSIONRUNTIME_H_ */
