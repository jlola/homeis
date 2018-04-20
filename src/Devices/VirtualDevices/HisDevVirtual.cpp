/*
 * HisDevVirtual.cpp
 *
 *  Created on: May 9, 2013
 *      Author: root
 */

#include "HisDevBase.h"
#include "PoppyDebugTools.h"
#include "HisDevVirtual.h"

string HisDevVirtual::LoadType = "HisDevVirtual";

HisDevVirtual::HisDevVirtual(IHisDevFactory* factory) :
	HisDevBase::HisDevBase(factory)
{
}

HisDevVirtual::HisDevVirtual(xmlNodePtr node,IHisDevFactory* factory)
	: HisDevBase::HisDevBase(node,factory)
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

void HisDevVirtual::OnError()
{

}

HisDevValueBase* HisDevVirtual::CreateHisDevValue(string address,EHisDevDirection direction,EDataType ptype,int pinNo)
{
	//string pinNo = Converter::itos(ppinNo);
	STACK
	HisDevValueBase* result = NULL;
	IHisDevFactory* factory = GetFactory();
	switch(ptype)
	{
		case EDataType::Bool:
			result = new HisDevValue<bool>(address, EHisDevDirection::ReadWrite, ptype, pinNo,false,LoadType,factory,this);
			break;
		case EDataType::Double:
			result = new HisDevValue<double>(address, EHisDevDirection::ReadWrite, ptype, pinNo,0,LoadType,factory,this);
			break;
		case EDataType::Int:
			result = new HisDevValue<int>(address, EHisDevDirection::ReadWrite, ptype, pinNo,0,LoadType,factory,this);
			break;
		case EDataType::String:
			result = new HisDevValue<string>(address, EHisDevDirection::ReadWrite, ptype, pinNo,string(""),LoadType,factory,this);
			break;
		case EDataType::Uint:
			result = new HisDevValue<unsigned int>(address, EHisDevDirection::ReadWrite, ptype, pinNo,0,LoadType,factory,this);
			break;
		case EDataType::Email:
			break;
		case EDataType::Enum:
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
	//WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevVirtual, &HisDevVirtual::WriteToDevice>(this);
	HisDevValueBase* value = CreateHisDevValue(strid, EHisDevDirection::ReadWrite, ptype, values.size());
	value->SetWriteHandler(this);
	value->Load();
	Add(value);
	return value;
}

void HisDevVirtual::DeleteDevValue(CUUID devValueId)
{
	STACK
	IHisBase* hisbase = Remove(devValueId);
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

	//WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevVirtual, &HisDevVirtual::WriteToDevice>(this);

	vector<HisDevValueBase*> values = GetItems<HisDevValueBase>();

	for(size_t i=0;i<values.size();i++)
	{
		values[i]->SetWriteHandler(this);
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

void HisDevVirtual::DoInternalRefresh(bool alarm)
{

}
