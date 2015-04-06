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

//const xmlChar *HisDevBase::GetNodeNameInternal()
//{
//	return KEY_DEVICENODE;
//}

HisDevBase::~HisDevBase()
{

}

HisDevBase::HisDevBase()
{
	scanPeriodMs = 10000;
	dataSource = EDataSource::Const;
	enabled = false;
}

HisDevBase::HisDevBase(xmlNodePtr node)
	: HisBase::HisBase(node)
{
	scanPeriodMs = 10000;
	dataSource = EDataSource::Const;
	enabled = false;
}

void HisDevBase::DoInternalSave(xmlNodePtr & node)
{
	HisBase::DoInternalSave(node);

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
	return enabled;
}

void HisDevBase::Enable(bool penabled)
{
	enabled = penabled;
}
