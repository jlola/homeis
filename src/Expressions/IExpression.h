/*
 * Expression.h
 *
 *  Created on: Jul 28, 2013
 *      Author: root
 */

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include "srutil/event/event.hpp"
#include "srutil/delegate/delegate.hpp"
#include "Common/CUUID.h"

class IExpression
{
public:
	virtual CUUID GetRecordId()=0;
	virtual bool Evaluate()=0;
	virtual void StartListening()=0;
	virtual void StopListening()=0;
	virtual ~IExpression() {}
	virtual std::string GetName()=0;
	virtual void Start()=0;
};

#endif /* EXPRESSION_H_ */
