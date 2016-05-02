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
#include "LOW_portSerial.h"

//const xmlChar *HisDevBase::GetNodeNameInternal()
//{
//	return KEY_DEVICENODE;
//}

HisDevBase::~HisDevBase()
{

}

HisDevBase::HisDevBase() :
		enabled(true),scanPeriodMs(10000),error(true),changed(false),needRefresh(false)
{
	STACK
	refreshmutex = HisLock::CreateMutex();
	dataSource = EDataSource::Const;
	uint64_t curTimeUs = HisDateTime::timeval_to_usec(HisDateTime::Now());
	nextScanTime = curTimeUs + (rand() % scanPeriodMs)*1000;
}

HisDevBase::HisDevBase(xmlNodePtr node)
	: HisBase::HisBase(node),scanPeriodMs(10000),error(true),changed(false),needRefresh(false)
{
	STACK
	refreshmutex = HisLock::CreateMutex();
	dataSource = EDataSource::Const;
	enabled = false;
	uint64_t curTimeUs = HisDateTime::timeval_to_usec(HisDateTime::Now());
	nextScanTime = curTimeUs + (rand() % scanPeriodMs)*1000;
}

void HisDevBase::SetChanged()
{
	changed = true;
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

bool HisDevBase::GetError()
{
	return error;
}
void HisDevBase::SetError(bool perror)
{
	error = perror;
}

void HisDevBase::NeedRefresh()
{
	//STACK
	if (changed==true)
	{
		changed = false;
		if (!error)
		{
			needRefresh = true;
			if (OnRefresh!=0)
				OnRefresh(this);
		}
	}
}

vector<IExpression*> HisDevBase::GetExpressions()
{
	return expressions;
}

void HisDevBase::AddExpression(IExpression* pExpression)
{
	expressions.push_back(pExpression);
}

void HisDevBase::RemoveExpression(IExpression* pExpression)
{
	std::vector<IExpression*>::iterator position = std::find(expressions.begin(), expressions.end(),pExpression);
	if (position != expressions.end()) // == myVector.end() means the element was not found
		expressions.erase(position);
}

void HisDevBase::Refresh(bool alarm)
{
	STACK

	//HisLock lock(refreshmutex);

	uint64_t curTimeUs = HisDateTime::timeval_to_usec(HisDateTime::Now());

	if (curTimeUs >= nextScanTime || needRefresh || alarm)
	{
		needRefresh = false;
		nextScanTime =curTimeUs + scanPeriodMs*1000;
		try
		{
			DoInternalRefresh(alarm);
		}
		catch(LOW_portSerial::portSerial_error & ex)
		{
			OnError();
		}
		catch(LOW_exception & ex)
		{
			string msg = "Error in %s name: %s | error: %s\nStack trace: " + Stack::GetTraceString();
			CLogger::Error( msg.c_str(), GetNodeName(), GetName().c_str(), ex.message.c_str());
			OnError();
		}
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

//period ms
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
