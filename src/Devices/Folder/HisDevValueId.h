/*
 * HisDevValueId.h
 *
 *  Created on: May 27, 2013
 *      Author: root
 */

#ifndef HISDEVVALUEID_H_
#define HISDEVVALUEID_H_

#include "Common/HisBase.h"
#include "HisDevValueBase.h"

#define NODE_VALUEID BAD_CAST "valueid"
#define PROP_DEVVALID BAD_CAST "DeviceValueId"

class HisDevValueId : public HisBase
{
	CUUID deviceValueId;
protected:
	const xmlChar* GetNodeNameInternal();
	virtual void DoInternalSave(xmlNodePtr & node);
	virtual void DoInternalLoad(xmlNodePtr & node);
public:
	HisDevValueId(HisDevValueBase* deviceValue,IHisDevFactory* factory);
	HisDevValueId(xmlNodePtr pNode,IHisDevFactory* factory);
	CUUID GetDeviceValueId();
	void SetDeviceValueId(CUUID deviceValueId);

};

#endif /* HISDEVVALUEID_H_ */
