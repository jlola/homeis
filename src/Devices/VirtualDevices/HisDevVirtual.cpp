/*
 * HisDevVirtual.cpp
 *
 *  Created on: May 9, 2013
 *      Author: root
 */

#include "HisDevBase.h"
#include "PoppyDebugTools.h"
#include "HisDevVirtual.h"


HisDevVirtual::HisDevVirtual()
{
}

HisDevVirtual::HisDevVirtual(xmlNodePtr node)
	: HisDevBase::HisDevBase(node)
{
}

const xmlChar* HisDevVirtual::GetNodeNameInternal()
{
	return (const xmlChar*)"HisDevVirtual";
}

void HisDevVirtual::WriteToDevice(ValueChangedEventArgs args)
{
	STACK
	vector<HisDevValueBase*> values = GetItems<HisDevValueBase>();
	for(size_t i=0;i<values.size();i++)
	{
		values[i]->FireOnValueChanged(args);
	}
}

HisDevValueBase* HisDevVirtual::CreateHisDevValue(string address,EHisDevDirection direction,EDataType ptype,int pinNo)
{
	STACK
	HisDevValueBase* result = NULL;
	switch(ptype)
	{
		case EDataType::Bool:
			result = new HisDevValue<bool>(address, EHisDevDirection::ReadWrite, ptype, pinNo,false);
			break;
		case EDataType::Double:
			result = new HisDevValue<double>(address, EHisDevDirection::ReadWrite, ptype, pinNo,0);
			break;
		case EDataType::Int:
			result = new HisDevValue<int>(address, EHisDevDirection::ReadWrite, ptype, pinNo,0);
			break;
		case EDataType::String:
			result = new HisDevValue<string>(address, EHisDevDirection::ReadWrite, ptype, pinNo,string(""));
			break;
		case EDataType::Uint:
			result = new HisDevValue<unsigned int>(address, EHisDevDirection::ReadWrite, ptype, pinNo,0);
			break;
		case EDataType::Unknown:
			return result;
	}
	return result;
}

HisDevValueBase* HisDevVirtual::AddDevValue(EDataType ptype)
{
	STACK
	std::string strid = this->GetRecordId().ToString();
	vector<HisDevValueBase*> values = GetItems<HisDevValueBase>();
	WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevVirtual, &HisDevVirtual::WriteToDevice>(this);
	HisDevValueBase* value = CreateHisDevValue(strid, EHisDevDirection::ReadWrite, ptype, values.size());
	value->delegateWrite = delegate;
	value->Load();
	Add(value);
	return value;
}

void HisDevVirtual::DeleteDevValue(CUUID devValueId)
{
	STACK
	HisBase* hisbase = Remove(devValueId);
	HisDevValueBase* devValue = dynamic_cast<HisDevValueBase*>(hisbase);
	if (devValue!=NULL)
	{
		delete(devValue);
	}
}

void HisDevVirtual::DoInternalSave(xmlNodePtr & node)
{
	STACK

	HisDevBase::DoInternalSave(node);

	xmlSetProp(node, INTERNAL, BAD_CAST "1");
}

void HisDevVirtual::DoInternalLoad(xmlNodePtr & node)
{
	STACK
	HisDevBase::DoInternalLoad(node);

	WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevVirtual, &HisDevVirtual::WriteToDevice>(this);

	vector<HisDevValueBase*> values = GetItems<HisDevValueBase>();

	for(size_t i=0;i<values.size();i++)
	{
		values[i]->delegateWrite = delegate;
	}
}

bool HisDevVirtual::IsInternal(xmlNodePtr pnode)
{
	STACK
	if (xmlHasProp(pnode,INTERNAL))
	{
		return true;
	}
	return false;
}

void HisDevVirtual::DoInternalRefresh()
{

}
