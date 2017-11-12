/*
 * HisBase.cpp
 *
 *  Created on: May 19, 2013
 *      Author: Josef Lola
 */

#include "HisBase.h"
#include "HisDevFactory.h"

HisBase::HisBase()
{
	createDate = DateTime::Now();
	modifyDate = createDate;
	isnew = true;
	isloaded = false;
	node = NULL;
	parent = NULL;
	name = "-------";
}

const xmlChar* HisBase::GetNodeName()
{
	return GetNodeNameInternal();
}

void HisBase::SetModifyDateTime(DateTime modifyDate)
{
	this->modifyDate = modifyDate;
}

DateTime HisBase::GetModifyDateTime()
{
	return modifyDate;
}

DateTime HisBase::GetCreateDateTime()
{
	return createDate;
}

vector<HisBase*> HisBase::GetAllItems()
{
	return items;
}

void HisBase::ClearIsNew()
{
	isnew = false;
}

HisBase::HisBase(xmlNodePtr pnode)
{
	parent = NULL;
	isloaded = false;
	isnew = false;
	name = "-------";
	node = pnode;
}

void HisBase::Load()
{
	if (isnew && !isloaded)
	{
		isloaded = true;
		node = xmlNewNode(NULL,this->GetNodeName());
		return;
	}
	if (!isloaded)
	{
		isloaded = true;
		DoInternalLoad(node);
	}
}

//bool HisBase::Deserialize(string strjson)
//{
//	Document document;
//	if (document.Parse<0>((char*)strjson.c_str()).HasParseError())
//		return NULL;
//}

//void HisBase::Serialize(Document jsonDoc)
//{
//	Value d(kObjectType);
//	//FillDeviceToJson(d,tagvalues[v],respjsondoc);
//	jsonDoc.PushBack(d, jsonDoc.GetAllocator());
//}

void HisBase::Save()
{
	Load();
	isnew = false;
	DoInternalSave(node);
}

bool HisBase::FindProcByName(HisBase* hisbase,void* args)
{
	string* name = static_cast<string*>(args);
	if (hisbase->GetName()==*name)
		return true;
	return false;
}

HisBase* HisBase::FindByName(string name)
{
	return Find(FindProcByName,&name);
}

HisBase* HisBase::Find(CUUID id)
{
	Load();

	for(size_t i=0;i<items.size();i++)
	{
		if (id == items[i]->GetRecordId()) return items[i];
		HisBase* result = items[i]->Find(id);
		if (result) return result;
	}
	return NULL;
}


HisBase* HisBase::Find(FindProc proc,void* args)
{
	Load();

	for(size_t i=0;i<items.size();i++)
	{
		if (proc(items[i],args)) return items[i];
		HisBase* result = items[i]->Find(proc,args);
		if (result) return result;
	}
	return NULL;
}

void HisBase::Add(HisBase *pitem)
{
	Load();

	if (FindIndex(pitem->GetRecordId()) < 0)
	{
		items.push_back(pitem);
		xmlAddChild(GetNodePtr(),pitem->GetNodePtr());
		pitem->SetParent(this);
	}
}

int HisBase::FindIndex(CUUID puuid)
{
	for(size_t i=0;i<items.size();i++)
	{
		if (items[i]->GetRecordId()==puuid) return i;
	}
	return -1;
}

HisBase* HisBase::Remove(CUUID puuid)
{
	int index = FindIndex(puuid);
	if ( index >= 0 )
	{
		HisBase *item = items[index];
		xmlNodePtr child = item->GetNodePtr();
		items.erase(items.begin()+index);
		xmlUnlinkNode(child);
		return item;
	}
	return NULL;
}

void HisBase::DoInternalSave(xmlNodePtr & node)
{
	if (node==NULL)	node = xmlNewNode(NULL,this->GetNodeName());

	modifyDate = DateTime::Now();

	xmlSetProp(node, PROP_MODIFY_DATE,BAD_CAST modifyDate.ToString().c_str());
	xmlSetProp(node, PROP_CREATE_DATE,BAD_CAST createDate.ToString().c_str());
	xmlSetProp(node, PROP_NAME,BAD_CAST name.c_str());
	xmlSetProp(node, PROP_RECORDID, BAD_CAST recordId.ToString().c_str());

	for(size_t i=0;i<items.size();i++)
	{
		HisBase* f = items[i];
		f->Save();
	}
}


void HisBase::FreeItems()
{
	//HisDevFolder *pfolder = NULL;
	//pokud uz neco naloudovany, chci to uvolnit
	if ( items.size() > 0 )
	{
		for(int i=items.size()-1;i>=0;i--)
		{
			delete(items[i]);
			items[i] = NULL;
		}
		items.clear();
	}
}

void HisBase::DoInternalLoad(xmlNodePtr & node)
{
	if (node!=NULL)
	{
		if (!xmlStrcmp(node->name,GetNodeName()))
		{
			string strtemp;
			xmlChar* prop;

			if (xmlHasProp(node,BAD_CAST PROP_NAME))
			{
				prop = xmlGetProp(node,BAD_CAST PROP_NAME);
				name = (const char*)prop;
				xmlFree(prop);
			}

			if (xmlHasProp(node,BAD_CAST PROP_MODIFY_DATE))
			{
				prop = xmlGetProp(node,BAD_CAST PROP_MODIFY_DATE);
				strtemp = (const char*)prop;
				xmlFree(prop);
				DateTime::TryParse(strtemp,modifyDate);
			}

			if (xmlHasProp(node,BAD_CAST PROP_CREATE_DATE))
			{
				prop = xmlGetProp(node,BAD_CAST PROP_CREATE_DATE);
				strtemp = (const char*)prop;
				xmlFree(prop);
				DateTime::TryParse(strtemp,createDate);
			}

			if (xmlHasProp(node,BAD_CAST PROP_RECORDID))
			{
				prop = xmlGetProp(node,BAD_CAST PROP_RECORDID);
				recordId = CUUID::Parse((const char*)prop);//Converter::stoi((const char*)prop);
				xmlFree(prop);
			}

			FreeItems();
			xmlNodePtr cur = node->children;
			xmlNodePtr next;
			HisBase *pitem = NULL;
			while(cur)
			{
				next = cur->next;
				pitem = HisDevFactory::Instance().Create(cur);
				if (pitem!=NULL)
				{
					pitem->SetParent(this);
					pitem->Load();
					items.push_back(pitem);
				}
				cur = next;
			}
		}
	}
}

xmlNodePtr HisBase::GetNodePtr()
{
	Load();
	return node;
}

//void HisBase::SetNodePtr(xmlNodePtr pnode)
//{
//	if (pnode!=node)
//	{
//		//FreeValueNodes();
//		xmlNodePtr cur = node->children;
//		if (cur)
//		{
//			xmlNodePtr next;
//			while(cur)
//			{
//				next = cur->next;
//				xmlUnlinkNode(cur);
//				xmlFreeNode(cur);
//				cur = next;
//			}
//		}
//		xmlFreeNode(node);
//
//		node = pnode;
//	}
//}

string HisBase::GetName()
{
	return name;
}

void HisBase::SetName(string pname)
{
	name = pname;
}

CUUID HisBase::GetRecordId()
{
	return recordId;
}

HisBase* HisBase::GetParent()
{
	return parent;
}

void HisBase::SetParent(HisBase* pParent)
{
	parent = pParent;
}

HisBase::~HisBase()
{
	FreeItems();
	xmlUnlinkNode(node);
	xmlFreeNode(node);
}

