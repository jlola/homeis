/*
 * HisDevIO2408.h
 *
 *  Created on: 22. 11. 2015
 *      Author: Josef LOla
 */

#ifndef SRC_DEVICES_HISDEVIO2408_H_
#define SRC_DEVICES_HISDEVIO2408_H_

#include "LOW_devDS2408.h"
#include "HisDallas.h"
#include "logger.h"
#include "Common/CUUID.h"

class HisDevIO2408: public HisDevDallas {
	HisDevValue<bool>* valueOutput[8];
	LOW_devDS2408 *dev;
	WriteToDeviceRequestDelegate delegate;
protected:
	virtual void DoInternalRefresh(bool alarm);
	virtual void DoInternalSave(xmlNodePtr & node);
	virtual void DoInternalLoad(xmlNodePtr & node);
	const xmlChar* GetNodeNameInternal();
	void CreateDataPoints(bool error);
	void WriteToDevice(ValueChangedEventArgs args);
public:
	HisDevIO2408(LOW_devDS2408* pdev);
	HisDevIO2408(xmlNodePtr node,LOW_devDS2408* pdev);
	virtual ~HisDevIO2408();
};

#endif /* SRC_DEVICES_HISDEVIO2408_H_ */
