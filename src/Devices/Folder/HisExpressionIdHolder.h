/*
 * ExpressionId.h
 *
 *  Created on: Nov 7, 2013
 *      Author: Josef LOla
 */

#ifndef HISEXPRESSIONID_H_
#define HISEXPRESSIONID_H_

#include "Common/HisBase.h"

#define NODE_EXPRESSIONID_HOLDER BAD_CAST "ExpressionIdHolder"
#define PROP_EXPRESSIONID BAD_CAST "ExpressionId"

class HisExpressionIdHolder : public HisBase
{
	CUUID expressionId;
protected:
	const xmlChar* GetNodeNameInternal();
	virtual void DoInternalSave(xmlNodePtr & node);
	virtual void DoInternalLoad(xmlNodePtr & node);
public:
	HisExpressionIdHolder(CUUID pExpressionId);
	HisExpressionIdHolder(xmlNodePtr pNode);
	CUUID GetExpressionId();
	void SetExpressionId(CUUID expressionId);
};

#endif /* EXPRESSIONID_H_ */
