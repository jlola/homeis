/*
 * HisExpressionId.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: Josef Lola
 */

#include "HisExpressionIdHolder.h"

const xmlChar* HisExpressionIdHolder::GetNodeNameInternal()
{
	return NODE_EXPRESSIONID_HOLDER;
}
void HisExpressionIdHolder::DoInternalSave(xmlNodePtr & node)
{
	HisBase::DoInternalSave(node);
	xmlSetProp(node, PROP_EXPRESSIONID,BAD_CAST expressionId.ToString().c_str());
}

void HisExpressionIdHolder::DoInternalLoad(xmlNodePtr & node)
{
	HisBase::DoInternalLoad(node);

	xmlChar* prop;
	prop = xmlGetProp(node, PROP_EXPRESSIONID);
	string strid = (const char*)prop;
	xmlFree(prop);
	expressionId = CUUID::Parse(strid);
}

HisExpressionIdHolder::HisExpressionIdHolder(CUUID pExpressionId)
{
	expressionId = pExpressionId;
}

HisExpressionIdHolder::HisExpressionIdHolder(xmlNodePtr pNode) :
		HisBase::HisBase(pNode)
{

}

CUUID HisExpressionIdHolder::GetExpressionId()
{
	return expressionId;
}

void HisExpressionIdHolder::SetExpressionId(CUUID pExpressionId)
{
	expressionId = pExpressionId;
}
