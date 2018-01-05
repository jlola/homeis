/*
 * HisDevFolder.cpp
 *
 *  Created on: May 12, 2013
 *      Author: root
 */

#include "HisDevFolder.h"

HisDevFolder::HisDevFolder(string pname,IHisDevFactory* factory)
	: HisBase::HisBase(factory)
{
	SetName(pname);
}

HisDevFolder::HisDevFolder(xmlNodePtr pNode,IHisDevFactory* factory) :
		HisBase::HisBase(pNode,factory)
{
	Load();
}

HisDevValueId *HisDevFolder::FindValueId(CUUID valueId)
{
	Load();

	vector<IHisBase*> items = GetAllItems();
	for(size_t i=0;i<items.size();i++)
	{
		HisDevValueId* value = dynamic_cast<HisDevValueId*>(items[i]);
		if (value!=NULL)
		{
			if (valueId == value->GetRecordId()) return value;
		}
		HisDevFolder* folder = dynamic_cast<HisDevFolder*>(items[i]);
		if (folder!=NULL)
		{
			HisDevValueId* result = folder->FindValueId(valueId);
			if (result) return result;
		}
	}

	return NULL;
}

void HisDevFolder::DoInternalSave(xmlNodePtr & node)
{
	Load();

	HisBase::DoInternalSave(node);
}

const xmlChar* HisDevFolder::GetNodeNameInternal()
{
	return NODE_FODLER;
}

void HisDevFolder::DoInternalLoad(xmlNodePtr & node)
{
	HisBase::DoInternalLoad(node);
}

HisDevFolder::~HisDevFolder()
{

}




