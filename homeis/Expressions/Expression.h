/*
 * Expression.h
 *
 *  Created on: Jul 28, 2013
 *      Author: root
 */

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

class IExpression
{
public:
	virtual bool Evaluate()=0;
	virtual void StartListening()=0;
	virtual void StopListening()=0;
};

#endif /* EXPRESSION_H_ */
