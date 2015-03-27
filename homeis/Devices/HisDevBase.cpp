/*
 * HisDevBase.cpp
 *
 *  Created on: Feb 17, 2013
 *      Author: root
 */


#include "converter.h"
#include <sstream>
#include <wchar.h>
#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "srutil/event/event.hpp"
#include "srutil/delegate/delegate.hpp"

#include "logger.h"
#include "converter.h"

#include "EDataType.h"
#include "EHisDevDirection.h"

#include "HisDevBase.h"

//xmlNodePtr HisDevBase::GetOrCreateNode(uint16_t pinNo,xmlNodePtr parentNode)
//{
//	xmlNodePtr node = NULL;
//	if (parentNode!=NULL)
//	{
//		xmlNodePtr cur = parentNode->children;
//		while (cur != NULL) {
//			std::string nodename = (char*)cur->name;
//			if (nodename==(char*)KEY_NODE_VALUE)
//			{
//
//				if (xmlHasProp(cur,KEY_PAR_PINNAME))
//				{
//					xmlChar* prop = xmlGetProp(cur,KEY_PAR_PINNO);
//					string strpinno = (char*)prop;
//					xmlFree(prop);
//					if (Converter::stoi(strpinno)==pinNo)
//						return cur;
//				}
//			}
//			cur = cur->next;
//		}
//		//node = xmlNewNode(NULL,KEY_NODE_VALUE);
//		//xmlAddChild(parentNode,node);
//	}
//	return node;
//}

//void HisDevBase::FreeValueNodes()
//{
//	xmlNodePtr cur = GetNodePtr()->children;
//	xmlNodePtr next;
//	while(cur)
//	{
//		next = cur->next;
//		xmlUnlinkNode(cur);
//		xmlFreeNode(cur);
//		cur = next;
//	}
//}

const xmlChar *HisDevBase::GetNodeNameInternal()
{
	return KEY_DEVICENODE;
}

HisDevBase::~HisDevBase()
{
//	FreeValueNodes();

//	while(!values.empty())
//	{
//		HisDevValueBase* base = values[values.size()-1];
//		//xmlNodePtr valuenode = GetOrCreateNode(base->GetPinNumber(),devnode);
//		values.pop_back();
//		delete(base);
//	}
}

HisDevBase::HisDevBase()
{
	scanPeriodMs = 10000;
	dataSource = EDataSource::Const;
	network = NULL;
	enabled = false;
}

HisDevBase::HisDevBase(xmlNodePtr node)
	: HisBase::HisBase(node)
{
	network = NULL;
	scanPeriodMs = 10000;
	dataSource = EDataSource::Const;
	enabled = false;
}

void HisDevBase::DoInternalSave(xmlNodePtr & node)
{
	HisBase::DoInternalSave(node);

//	for(uint16_t i=0;i<values.size();i++)
//	{
//		values[i]->Save();
//	}
}

timeval HisDevBase::ComputeNextScanTime()
{
	int64_t useconds = HisDateTime::timeval_to_usec(HisDateTime::Now());
	useconds += scanPeriodMs * 1000;
	return HisDateTime::usec_to_timeval(useconds);
}

timeval HisDevBase::GetNextScanTime()
{
	timeval val;
	val.tv_sec = 0;
	val.tv_usec = 0;
	return val;
}

void HisDevBase::Refresh()
{
	lastScanTime = HisDateTime::Now();

	DoInternalRefresh();
}

void HisDevBase::DoInternalLoad(xmlNodePtr & node)
{
	HisBase::DoInternalLoad(node);

//	if (node!=NULL)
//	{
//		for(uint16_t i=0;i<values.size();i++)
//		{
//			values[i]->Load();
//		}
//	}
}

//HisDevBase::HisDevBase()
//{
//  scanPeriodMs = 10000;
//}


uint32_t HisDevBase::GetScanPeriod()
{
	return scanPeriodMs;
}

void HisDevBase::SetScanPeriod(uint32_t period)
{
	this->scanPeriodMs = period;
}


vector<HisDevValueBase*> HisDevBase::GetValues()
{
	return GetItems<HisDevValueBase>();
}

bool HisDevBase::IsEnabled()
{
	return enabled;
}

void HisDevBase::Enable(bool penabled)
{
	enabled = penabled;
}
