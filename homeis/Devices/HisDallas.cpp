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
#include "HisDallas.h"
#include "homeis/Common/CUUID.h"

HisDevDallas::HisDevDallas(LOW_device* pdev)
{
	device = pdev;
	DevError = false;
}

HisDevDallas::HisDevDallas(xmlNodePtr node,LOW_device* pdev) :
	HisDevBase::HisDevBase(node)
{
	device = pdev;
	DevError = false;
}

LOW_deviceID HisDevDallas::GetId(xmlNodePtr node)
{
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
	if (device!=NULL) return device->getID();
	if (GetNodePtr()!=NULL) return HisDevDallas::GetId(GetNodePtr());
	throw "HisDevDallas::GetId empty device and devnode";
}

void HisDevDallas::DoInternalSave(xmlNodePtr & node)
{
	HisDevBase::DoInternalSave(node);

	if (device==NULL) throw "HisDevDallas::DoInternalSave - device is NULL";
	xmlSetProp(node,PROP_ID,(const xmlChar*)device->getID().getRomIDString().c_str());
}

void HisDevDallas::DoInternalLoad(xmlNodePtr & node)
{
	HisDevBase::DoInternalLoad(node);
	if (device==NULL) throw "HisDevDallas::DoInternalLoad - device is NULL";
}

HisDevDallas::~HisDevDallas()
{
}

