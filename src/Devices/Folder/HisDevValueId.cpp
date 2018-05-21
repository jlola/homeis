/*
 * HisDevValueId.cpp
 *
 *  Created on: May 27, 2013
 *      Author: root
 */
#include "HisException.h"
#include "HisDevValueId.h"

const xmlChar* HisDevValueId::GetNodeNameInternal()
{
	return NODE_VALUEID;
}
void HisDevValueId::DoInternalSave(xmlNodePtr & node)
{
	HisBase::DoInternalSave(node);
	xmlSetProp(node, PROP_DEVVALID,BAD_CAST deviceValueId.ToString().c_str());
}

void HisDevValueId::DoInternalLoad(xmlNodePtr & node)
{
	HisBase::DoInternalLoad(node);

	xmlChar* prop;
	prop = xmlGetProp(node, PROP_DEVVALID);
	string strid = (const char*)prop;
	xmlFree(prop);
	deviceValueId = CUUID::Parse(strid);
}

HisDevValueId::HisDevValueId(HisDevValueBase* deviceValue,IHisDevFactory* factory) :
	HisBase::HisBase(factory)
{
	if (deviceValue==NULL)
		throw new ArgumentNullException("deviceValue");
	deviceValueId = deviceValue->GetRecordId();
}

HisDevValueId::HisDevValueId(xmlNodePtr pNode,IHisDevFactory* factory) :
		HisBase::HisBase(pNode,factory)
{

}

CUUID HisDevValueId::GetDeviceValueId()
{
	return deviceValueId;
}

void HisDevValueId::SetDeviceValueId(CUUID pDeviceValueId)
{
	deviceValueId = pDeviceValueId;
}
