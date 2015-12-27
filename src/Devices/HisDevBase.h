/*
 * HisDevBase.h
 *
 *  Created on: Feb 16, 2013
 *      Author: root
 */

#ifndef HISDEVBASE_H_
#define HISDEVBASE_H_

#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <map>
#include "LOW_netSegment.h"
#include "LOW_network.h"
#include "LOW_device.h"
#include "logger.h"
#include "EDataType.h"
#include "ValueEventArgs.h"
#include "EDataSource.h"
#include "HisDevValue.h"
#include "Common/CUUID.h"
#include "srutil/event/event.hpp"
#include "srutil/delegate/delegate.hpp"

#include "Common/HisBase.h"

using namespace std;

class HisDevBase;

#ifdef SRUTIL_DELEGATE_PREFERRED_SYNTAX
typedef srutil::delegate<void ()> OnRefreshDelegate;
#else
 typedef srutil::delegate1<void,HisDevBase*> OnRefreshDelegate;
#endif


#define PROP_SCANPERIOD (const xmlChar *)"ScanPeriod"
//#define KEY_DEVICENODE (const xmlChar *)"device"

class HisDevBase : public HisBase
{
	bool needRefresh;
	bool enabled;
	uint64_t scanPeriodMs;
	uint64_t nextScanTime;
	EDataSource dataSource;
protected:
	HisDevBase(xmlNodePtr node,CUUID recordid);
	HisDevBase(xmlNodePtr node);
	virtual void DoInternalSave(xmlNodePtr & node);
	virtual void DoInternalLoad(xmlNodePtr & node);
	virtual void DoInternalRefresh()=0;
	//const xmlChar* GetNodeNameInternal();
public:
	HisDevBase();
	virtual ~HisDevBase();
	/*
	 * read device and if change any value fire event ValueChanged
	 */
	void Refresh();
	/*
	 *
	 */
	vector<HisDevValueBase*> GetValues();
	OnRefreshDelegate OnRefresh;
	timeval ComputeNextScanTime(timeval pLastScanTime);
	void FireOnValueChanged(ValueChangedEventArgs args);
	void Register(OnValueChangedDelegate delegate,void* owner);
	void UnRegister(void* owner);
	bool IsEnabled();
	void Enable(bool enabled);
	uint32_t GetScanPeriod();
	void SetScanPeriod(uint32_t period);
	void NeedRefresh();
};

#endif /* HISDEVBASE_H_ */
