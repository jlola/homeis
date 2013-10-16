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
#include "homeis/Common/CUUID.h"
#include "homeis/Common/HisBase.h"

using namespace std;

#define KEY_DEVICENODE (const xmlChar *)"device"

class HisDevBase : public HisBase
{
	bool enabled;
	LOW_network *network;
	uint32_t scanPeriodMs;
	timeval lastScanTime;
	timeval nextScanTime;
	EDataSource dataSource;
	//void FreeValueNodes();
protected:
	//vector<HisDevValueBase*> values;
	HisDevBase(xmlNodePtr node,CUUID recordid);
	HisDevBase(xmlNodePtr node);
	virtual void DoInternalSave(xmlNodePtr & node);
	virtual void DoInternalLoad(xmlNodePtr & node);
	virtual void DoInternalRefresh()=0;
	//xmlNodePtr GetOrCreateNode(uint16_t pinNo, xmlNodePtr parentNode);
	const xmlChar* GetNodeNameInternal();
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

	timeval ComputeNextScanTime();
	timeval GetNextScanTime();
	void FireOnValueChanged(ValueChangedEventArgs args);
	void Register(OnValueChangedDelegate delegate,void* owner);
	void UnRegister(void* owner);
	bool IsEnabled();
	void Enable(bool enabled);

	uint32_t GetScanPeriod();
	void SetScanPeriod(uint32_t period);
};

#endif /* HISDEVBASE_H_ */
