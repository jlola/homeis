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
#include "PoppyDebugTools.h"
#include "HisDevBase.h"

//const xmlChar *HisDevBase::GetNodeNameInternal()
//{
//	return KEY_DEVICENODE;
//}

HisDevBase::~HisDevBase()
{

}

HisDevBase::HisDevBase() :
		needRefresh(false),enabled(true),scanPeriodMs(10000)
{
	STACK
	dataSource = EDataSource::Const;
	uint64_t curTimeUs = HisDateTime::timeval_to_usec(HisDateTime::Now());
	nextScanTime = curTimeUs + (rand() % scanPeriodMs)*1000;
}

HisDevBase::HisDevBase(xmlNodePtr node)
	: HisBase::HisBase(node),needRefresh(false),scanPeriodMs(10000)
{
	STACK
	dataSource = EDataSource::Const;
	enabled = false;
	uint64_t curTimeUs = HisDateTime::timeval_to_usec(HisDateTime::Now());
	nextScanTime = curTimeUs + (rand() % scanPeriodMs)*1000;
}

void HisDevBase::DoInternalSave(xmlNodePtr & node)
{
	STACK
	HisBase::DoInternalSave(node);

	xmlSetProp(node,PROP_SCANPERIOD,(const xmlChar*)Converter::itos(scanPeriodMs,10).c_str());
}

timeval HisDevBase::ComputeNextScanTime(timeval pLastScanTime)
{
	int64_t useconds = HisDateTime::timeval_to_usec(pLastScanTime);
	useconds += scanPeriodMs * 1000;
	return HisDateTime::usec_to_timeval(useconds);
}

void HisDevBase::NeedRefresh()
{
	STACK
	if (IsEnabled())
	{
		needRefresh = true;
		if (OnRefresh!=0)
			OnRefresh(this);
	}
}

void HisDevBase::Refresh()
{
	STACK
	uint64_t curTimeUs = HisDateTime::timeval_to_usec(HisDateTime::Now());

	if (curTimeUs >= nextScanTime || needRefresh)
	{
		needRefresh = false;
		nextScanTime =curTimeUs + scanPeriodMs*1000;
		DoInternalRefresh();
	}
}

void HisDevBase::DoInternalLoad(xmlNodePtr & node)
{
	STACK
	HisBase::DoInternalLoad(node);

	xmlChar* prop = NULL;
	if (xmlHasProp(node,PROP_SCANPERIOD))
	{
		prop = xmlGetProp(node,PROP_SCANPERIOD);
		string strScanPeriod = (const char*)prop;
		scanPeriodMs = Converter::stoui(strScanPeriod,10);
		xmlFree(prop);
	}
}

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
	return true;
}

void HisDevBase::Enable(bool penabled)
{
	enabled = penabled;
}
