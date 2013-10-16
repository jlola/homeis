/*
 * HisDevVirtual.cpp
 *
 *  Created on: May 9, 2013
 *      Author: root
 */

#include "HisDevBase.h"
#include "HisDevVirtual.h"


HisDevVirtual::HisDevVirtual(EDataType dataType)
{
	type = dataType;

	CreateDataPoints();
}

HisDevVirtual::HisDevVirtual(xmlNodePtr node)
	: HisDevBase::HisDevBase(node)
{
}

void HisDevVirtual::WriteToDevice(ValueChangedEventArgs args)
{
	vector<HisDevValueBase*> values = GetItems<HisDevValueBase>();
	for(size_t i=0;i<values.size();i++)
	{
		values[i]->FireOnValueChanged(args);
	}
}

void HisDevVirtual::CreateDataPoints()
{
	std::string strid = this->GetRecordId().ToString();
	WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevVirtual, &HisDevVirtual::WriteToDevice>(this);
	//create data points
	//xmlNodePtr valuenode = GetOrCreateNode(0,GetNodePtr());
	HisDevValueBase* value;
	value = CreateHisDevValue(strid, EHisDevDirection::ReadWrite, type, 0);
	value->delegateWrite = delegate;
	value->Load();
	//values.push_back(value);
	Add(value);
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
	}
	return result;
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

	//value->delegateWrite = delegate;
	//value->Load();

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
