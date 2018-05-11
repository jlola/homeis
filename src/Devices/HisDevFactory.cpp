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



HisDevFactory::HisDevFactory(IExpressionRuntime *expressionRuntime,
		HisDevices* devices,
		IEmailSender* emailSender,
		IFile* file,
		IDirectory* directory
		)
{
	this->directory = directory;
	this->file = file;
	this->devices = devices;
	this->expressionRuntime = expressionRuntime;
	this->emailSender = emailSender;
}

HisDevFactory::~HisDevFactory()
{

}

IEmailSender* HisDevFactory::GetEmailSender()
{
	return emailSender;
}

IDirectory* HisDevFactory::GetDirectory()
{
	return directory;
}

IFile *HisDevFactory::GetFile()
{
	return file;
}

IHisBase *HisDevFactory::Create(xmlNodePtr node)
{
	ILogger & logger = CLogger::GetLogger();
	if (node->type==XML_ELEMENT_NODE)
	{
		if (!xmlStrcmp(node->name,NODE_VALUEID))
			return new HisDevValueId(node,this);
		if (!xmlStrcmp(node->name,NODE_FODLER))
			return new HisDevFolder(node,this);
		if (!xmlStrcmp(node->name,NODE_VALUE))
			return HisDevValueBase::Create(node,this,NULL);
		if (!xmlStrcmp(node->name,NODE_EXPRESSION))
			return new LuaExpression(node,devices,expressionRuntime,this);
		logger.Error("HisDevFactory::Create | Not implemented %s constructor.",node->name);
	}
	return NULL;
}

