/*
 * HisDevVirtual.cpp
 *
 *  Created on: May 9, 2013
 *      Author: root
 */

#include "HisDevBase.h"
#include "HisDevVirtual.h"


HisDevVirtual::HisDevVirtual()
{
	type = EDataType::Unknown;
}

HisDevVirtual::HisDevVirtual(xmlNodePtr node)
	: HisDevBase::HisDevBase(node)
{
	type = EDataType::Unknown;
}

void HisDevVirtual::WriteToDevice(ValueChangedEventArgs args)
{
	vector<HisDevValueBase*> values = GetItems<HisDevValueBase>();
	for(size_t i=0;i<values.size();i++)
	{
		values[i]->FireOnValueChanged(args);
	}
}

HisDevValueBase* HisDevVirtual::CreateHisDevValue(string strid,EHisDevDirection direction,EDataType ptype,int pinNo)
{
	HisDevValueBase* result = NULL;
	switch(ptype)
	{
		case EDataType::Bool:
			result = new HisDevValue<bool>(strid, EHisDevDirection::ReadWrite, ptype, pinNo);
			break;
		case EDataType::Double:
			result = new HisDevValue<double>(strid, EHisDevDirection::ReadWrite, ptype, pinNo);
			break;
		case EDataType::Int:
			result = new HisDevValue<int>(strid, EHisDevDirection::ReadWrite, ptype, pinNo);
			break;
		case EDataType::String:
			result = new HisDevValue<string>(strid, EHisDevDirection::ReadWrite, ptype, pinNo);
			break;
		case EDataType::Uint:
			result = new HisDevValue<unsigned int>(strid, EHisDevDirection::ReadWrite, ptype, pinNo);
			break;
		case EDataType::Unknown:
			return result;
	}
	return result;
}

HisDevValueBase* HisDevVirtual::AddDevValue(EDataType ptype)
{
	std::string strid = this->GetRecordId().ToString();
	vector<HisDevValueBase*> values = GetItems<HisDevValueBase>();
	WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevVirtual, &HisDevVirtual::WriteToDevice>(this);
	HisDevValueBase* value = CreateHisDevValue(strid, EHisDevDirection::ReadWrite, type, values.size());
	value->delegateWrite = delegate;
	value->Load();
	Add(value);
	return value;
}

void HisDevVirtual::DoInternalSave(xmlNodePtr & node)
{
	HisDevBase::DoInternalSave(node);

	xmlSetProp(node, INTERNAL, BAD_CAST "1");
}

void HisDevVirtual::DoInternalLoad(xmlNodePtr & node)
{
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
	if (xmlHasProp(pnode,INTERNAL))
	{
		return true;
	}
	return false;
}

void HisDevVirtual::DoInternalRefresh()
{

}
