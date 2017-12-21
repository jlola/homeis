/*
 * HisDevFactory.cpp
 *
 *  Created on: Mar 10, 2013
 *      Author: Josef Lola
 */

#include "converter.h"
#include <sstream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "srutil/event/event.hpp"
#include "srutil/delegate/delegate.hpp"

#include "logger.h"
#include "converter.h"

#include "EDataType.h"
#include "EHisDevDirection.h"

#include "HisDevBase.h"
#include "Common/HisBase.h"
#include "HisDevValue.h"
#include "Expressions/LuaExpression.h"
#include "HisDevFactory.h"



HisDevFactory::HisDevFactory()
{
	devices = NULL;
	rootFolder = NULL;
	expressionRuntime = NULL;
}

HisDevFactory::~HisDevFactory()
{

}

HisDevFactory HisDevFactory::instance;

HisDevFactory & HisDevFactory::Instance()
{
	return HisDevFactory::instance;
}

void HisDevFactory::SetExpressionRuntime(ExpressionRuntime *pExpressionRuntime)
{
	expressionRuntime = pExpressionRuntime;
}

void HisDevFactory::SetDevices(HisDevices* pdevices)
{
	devices = pdevices;
}

void HisDevFactory::SetRooFolder(HisDevFolderRoot* proot)
{
	rootFolder = proot;
}

HisBase *HisDevFactory::Create(xmlNodePtr node)
{
	if (node->type==XML_ELEMENT_NODE)
	{
		if (!xmlStrcmp(node->name,NODE_VALUEID))
			return new HisDevValueId(node);
		if (!xmlStrcmp(node->name,NODE_FODLER))
			return new HisDevFolder(node);
		if (!xmlStrcmp(node->name,NODE_VALUE))
			return HisDevValueBase::Create(node);
		if (!xmlStrcmp(node->name,NODE_EXPRESSION))
			return new LuaExpression(node,devices,expressionRuntime);
		CLogger::Error("HisDevFactory::Create | Not implemented %s constructor.",node->name);
	}
	return NULL;
}

