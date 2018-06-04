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
#include "Common/CUUID.h"
#include "IWriteToDevice.h"

#define INTERNAL (xmlChar*)"internal"
#include "Devices/HisDevBase.h"

class HisDevVirtual : public HisDevBase, IWriteToDevice
{
protected:
	virtual void DoInternalLoad(xmlNodePtr & node);
	virtual void DoInternalSave(xmlNodePtr & node);

public:
	static string LoadType;

	HisDevVirtual(IHisDevFactory* factory);

	HisDevVirtual(xmlNodePtr node,IHisDevFactory* factory);

	const xmlChar* GetNodeNameInternal();

	void DeleteDevValue(CUUID id);

	HisDevValueBase* CreateHisDevValue(string addr,EHisDevDirection direction,EDataType ptype,int pinNo);

	HisDevValueBase* AddDevValue(EDataType ptype);

	virtual void DoInternalRefresh(bool alarm);

	void SetErrorFalse(HisDevValueBase* value);

	void OnError();

	void WriteToDevice(ValueChangedEventArgs & args);

	static bool IsInternal(xmlNodePtr pnode);
};

#endif /* HISDEVVIRTUAL_H_ */
