/*
 * HisDevVirtual.h
 *
 *  Created on: May 9, 2013
 *      Author: root
 */

#ifndef HISDEVVIRTUAL_H_
#define HISDEVVIRTUAL_H_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "homeis/Common/CUUID.h"

#define INTERNAL (xmlChar*)"internal"
#include "homeis/Devices/HisDevBase.h"

class HisDevVirtual : public HisDevBase
{
protected:
	virtual void DoInternalLoad(xmlNodePtr & node);
	virtual void DoInternalSave(xmlNodePtr & node);

public:
	HisDevVirtual();

	HisDevVirtual(xmlNodePtr node);

	HisDevValueBase* CreateHisDevValue(string strid,EHisDevDirection direction,EDataType ptype,int pinNo);

	HisDevValueBase* AddDevValue(EDataType ptype);

	virtual void DoInternalRefresh();

	void WriteToDevice(ValueChangedEventArgs args);

	static bool IsInternal(xmlNodePtr pnode);
};

#endif /* HISDEVVIRTUAL_H_ */
