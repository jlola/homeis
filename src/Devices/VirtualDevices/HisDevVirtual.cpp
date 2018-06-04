/*
 * HisDevVirtual.cpp
 *
 *  Created on: May 9, 2013
 *      Author: root
 */

#include "HisDevBase.h"
#include "PoppyDebugTools.h"
#include "HisException.h"
#include "HisDevValueEmail.h"
#include "HisDevVirtual.h"

string HisDevVirtual::LoadType = "HisDevVirtual";

HisDevVirtual::HisDevVirtual(IHisDevFactory* factory) :
	HisDevBase::HisDevBase(factory)
{
	SetError(false);
}

HisDevVirtual::HisDevVirtual(xmlNodePtr node,IHisDevFactory* factory)
	: HisDevBase::HisDevBase(node,factory)
{
	SetError(false);
}

const xmlChar* HisDevVirtual::GetNodeNameInternal()
{
	return (const xmlChar*)"HisDevVirtual";
}

void HisDevVirtual::WriteToDevice(ValueChangedEventArgs & args)
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
	STACK
	HisDevValueBase* result = NULL;
	IHisDevFactory* factory = GetFactory();
	switch(ptype)
	{
		case EDataType::Bool:
		{
			HisDevValue<bool>* bltag = new HisDevValue<bool>(address, EHisDevDirection::ReadWrite, ptype, pinNo,false,LoadType,factory,this);
			bltag->ReadedValueFromDevice(false,false);
			result = bltag;
			break;
		}
		case EDataType::Double:
		{
			HisDevValue<double>* doubleTag = new HisDevValue<double>(address, EHisDevDirection::ReadWrite, ptype, pinNo,0,LoadType,factory,this);
			result = doubleTag;
			doubleTag->ReadedValueFromDevice(0,false);
			break;
		}
		case EDataType::Int:
		{
			HisDevValue<int>* intTag = new HisDevValue<int>(address, EHisDevDirection::ReadWrite, ptype, pinNo,0,LoadType,factory,this);
			intTag->ReadedValueFromDevice(0,false);
			result= intTag;
			break;
		}
		case EDataType::String:
		{
			HisDevValue<string>* stringTag = new HisDevValue<string>(address, EHisDevDirection::ReadWrite, ptype, pinNo,string(""),LoadType,factory,this);
			stringTag->ReadedValueFromDevice("",false);
			result = stringTag;
			break;
		}
		case EDataType::Uint:
		{
			HisDevValue<unsigned int>* uintTag = new HisDevValue<unsigned int>(address, EHisDevDirection::ReadWrite, ptype, pinNo,0,LoadType,factory,this);
			uintTag->ReadedValueFromDevice(0,false);
			result = uintTag;
			break;
		}
		case EDataType::Email:
		{
			HisDevValueEmail* emailTag = new HisDevValueEmail(this,factory);
			emailTag->ReadedValueFromDevice("",false);
			result = emailTag;
			break;
		}
		case EDataType::Enum:
		case EDataType::Unknown:
			return result;
	}
	return result;
}

HisDevValueBase* HisDevVirtual::AddDevValue(EDataType type)
{
	STACK
//	if (type==EDataType::Email)
//		throw Exception("Tag dataType Email should not be created by AddDevValue");
	std::string strid = this->GetRecordId().ToString();
	vector<HisDevValueBase*> values = GetItems<HisDevValueBase>();
	//WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevVirtual, &HisDevVirtual::WriteToDevice>(this);
	HisDevValueBase* value = CreateHisDevValue(strid, EHisDevDirection::ReadWrite, type, values.size());
	value->SetDevice(this);
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
		values[i]->SetDevice(this);
		SetErrorFalse(values[i]);
	}
}

void HisDevVirtual::SetErrorFalse(HisDevValueBase* tag)
{
	switch(tag->GetDataType())
	{
		case EDataType::Bool:
		{
			HisDevValue<bool>* blvalue = dynamic_cast<HisDevValue<bool>*>(tag);
			blvalue->ReadedValueFromDevice(blvalue->GetValue(),false);
		}
		break;
		case EDataType::Double:
		{
			HisDevValue<double>* doubleValue = dynamic_cast<HisDevValue<double>*>(tag);
			doubleValue->ReadedValueFromDevice(doubleValue->GetValue(),false);
		}
		break;
		case EDataType::Email:
		{
			HisDevValue<string>* emailValue = dynamic_cast<HisDevValue<string>*>(tag);
			emailValue->ReadedValueFromDevice(emailValue->GetValue(),false);
		}
		break;
		case EDataType::Int:
		{
			HisDevValue<int>* intValue = dynamic_cast<HisDevValue<int>*>(tag);
			intValue->ReadedValueFromDevice(intValue->GetValue(),false);
		}
		break;
		case EDataType::String:
		{
			HisDevValue<string>* stringValue = dynamic_cast<HisDevValue<string>*>(tag);
			stringValue->ReadedValueFromDevice(stringValue->GetValue(),false);
		}
		break;
		case EDataType::Uint:
		{
			HisDevValue<unsigned int>* uintValue = dynamic_cast<HisDevValue<unsigned int>*>(tag);
			uintValue->ReadedValueFromDevice(uintValue->GetValue(),false);
		}
		break;
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
