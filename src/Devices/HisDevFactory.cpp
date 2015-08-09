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
#include "HisDevTemp18B20.h"
#include "HisDevIO2413.h"
#include "Common/HisBase.h"
#include "HisDevValue.h"
#include "Expressions/LuaExpression.h"
#include "LOWdevLCD.h"
#include "HisDevLCD.h"
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
	//if (HisDevFactory::factory==NULL) HisDevFactory::factory = new HisDevFactory();
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
	if (!xmlStrcmp(node->name,NODE_VALUEID))
		return new HisDevValueId(node);
	if (!xmlStrcmp(node->name,NODE_FODLER))
		return new HisDevFolder(node);
	if (!xmlStrcmp(node->name,NODE_VALUE))
		return HisDevValueBase::Create(node);
	if (!xmlStrcmp(node->name,NODE_EXPRESSION))
		return new LuaExpression(node,devices,expressionRuntime);
	CLogger::Error("HisDevFactory::Create | Not implemented %s constructor.",node->name);
	return NULL;
}

HisDevBase * HisDevFactory::Create(xmlNodePtr notptr,LOW_device* dev)
{
	LOW_deviceID id = HisDevDallas::GetId(notptr);
	if (dev!=NULL)
	{
		switch(dev->getFamilyCode())
		{
			case LOW_devDS1820::familyCode:
				return (HisDevBase*)new HisDevTemp18B20(notptr,(LOW_devDS1820*)dynamic_cast<LOW_devDS1820*>(dev));
			break;
			case LOW_devDS2413::familyCode:
				return (HisDevBase*)new HisDevIO2413(notptr,(LOW_devDS2413*)dynamic_cast<LOW_devDS2413*>(dev));
			break;
			case LOW_devLCD::familyCode:
				return (HisDevBase*)new HisDevLCD(notptr,(LOW_devLCD*)dynamic_cast<LOW_devLCD*>(dev));
		}
	}
	return NULL;
}

HisDevBase * HisDevFactory::Create(LOW_device* dev)
{
	switch(dev->getFamilyCode())
	{
		case LOW_devDS1820::familyCode:
			return (HisDevBase*)new HisDevTemp18B20((LOW_devDS1820*)dev);
		break;
		case LOW_devDS2413::familyCode:
			return (HisDevBase*)new HisDevIO2413((LOW_devDS2413*)dev);
		break;
		case LOW_devLCD::familyCode:
			return (HisDevBase*)new HisDevLCD((LOW_devLCD*)dev);
		break;
	}
	return NULL;
}

