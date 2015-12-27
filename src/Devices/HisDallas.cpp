/*
 * HisDallas.cpp
 *
 *  Created on: Feb 22, 2013
 *      Author: Josef Lola
 */

#include <string>
#include <sstream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "srutil/event/event.hpp"
#include "srutil/delegate/delegate.hpp"

#include "logger.h"
#include "converter.h"
#include "EDataType.h"
#include "EHisDevDirection.h"
#include "ValueEventArgs.h"

#include "LOW_deviceID.h"
#include "HisDevBase.h"
#include "Common/HisException.h"
#include "Common/CUUID.h"
#include "PoppyDebugTools.h"
#include "HisDallas.h"

HisDevDallas::HisDevDallas(LOW_device* pdev)
{
	STACK
	device = pdev;
	DevError = false;
}

HisDevDallas::HisDevDallas(xmlNodePtr node,LOW_device* pdev) :
	HisDevBase::HisDevBase(node)
{
	STACK
	device = pdev;
	DevError = false;
}

LOW_deviceID HisDevDallas::GetId(xmlNodePtr node)
{
	STACK
	std::string strid;
	std::vector<uint8_t> idbytes;
	if (xmlHasProp(node,PROP_ID))
	{
		xmlChar* prop = xmlGetProp(node,PROP_ID);
		strid = (char*)prop;
		xmlFree(prop);
		idbytes = Converter::stobytes(strid);
	}

	LOW_deviceID id(idbytes);

	return id;
}



LOW_deviceID HisDevDallas::GetId()
{
	STACK
	if (device!=NULL) return device->getID();
	if (GetNodePtr()!=NULL) return HisDevDallas::GetId(GetNodePtr());
	throw "HisDevDallas::GetId empty device and devnode";
}

void HisDevDallas::DoInternalSave(xmlNodePtr & node)
{
	STACK
	HisDevBase::DoInternalSave(node);

	if (device==NULL) throw "HisDevDallas::DoInternalSave - device is NULL";
	xmlSetProp(node,PROP_ID,(const xmlChar*)device->getID().getRomIDString().c_str());
}

void HisDevDallas::DoInternalLoad(xmlNodePtr & node)
{
	STACK
	HisDevBase::DoInternalLoad(node);
	if (device==NULL) throw HisException("HisDevDallas::DoInternalLoad - device is NULL");

}

HisDevDallas::~HisDevDallas()
{
}

