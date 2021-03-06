/*
 * HisDevValueBase.cpp
 *
 *  Created on: 21. 4. 2018
 *      Author: pc
 */


#include <string>
#include <sstream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "EDataType.h"
#include "EHisDevDirection.h"
#include "srutil/event/event.hpp"
#include "srutil/delegate/delegate.hpp"
#include "ValueEventArgs.h"
#include "Helpers/StringBuilder.h"
#include "HisDevValue.h"
#include "Base64.h"
#include "HisDevValueEmail.h"
#include "HisDevValueBase.h"

using namespace std;

HisDevValueBase::HisDevValueBase(std::string pdevaddr,
	EHisDevDirection direct,
	EDataType pdatatype,
	string ppinNumber,
	string loadType,
	IHisDevFactory* factory,
	IWriteToDevice* deviceWriter) :
	HisBase::HisBase(factory),
	logger(CLogger::GetLogger()),
	deviceWriter(deviceWriter)
{
	loadtype = loadType;
	pinNumber = ppinNumber;
	direction = direct;
	datatype = pdatatype;
	devaddr = pdevaddr;
	deviceError = true;
	allowForceOutput = false;
}

HisDevValueBase::HisDevValueBase(xmlNodePtr pnode,
		IHisDevFactory* factory,
		IWriteToDevice* deviceWriter) :
	HisBase::HisBase(pnode,factory),
	logger(CLogger::GetLogger()),
	deviceWriter(deviceWriter)
{
	pinNumber = "";
	direction = EHisDevDirection::Read;
	datatype = EDataType::Int;
	deviceError = true;
	allowForceOutput = false;
}

bool HisDevValueBase::GetForceOutput()
{
	return allowForceOutput;
}

void HisDevValueBase::SetForceOutput(bool force)
{
	allowForceOutput = force;
}

HisDevValueBase::HisDevValueBase(HisDevValueBase & src,
		IHisDevFactory* factory,
		IWriteToDevice* deviceWriter) :
	HisBase::HisBase(factory),
	logger(CLogger::GetLogger()),
	deviceWriter(deviceWriter)
{
	//pinname = src.pinname;
	devaddr = src.devaddr;
	pinNumber = src.pinNumber;
	direction = src.direction;
	datatype = src.datatype;
	deviceError = src.deviceError;
	loadtype = src.loadtype;
	allowForceOutput = false;
}

void HisDevValueBase::SetDevice(IWriteToDevice* deviceWriter)
{
	this->deviceWriter = deviceWriter;
}

void HisDevValueBase::SetError()
{
	this->deviceError = true;
}

HisDevValueBase* HisDevValueBase::Create(xmlNodePtr node,
		IHisDevFactory* factory,
		IWriteToDevice* deviceWriter)
{
	STACK
	EDataType datatype;
	xmlChar* prop = NULL;
	if (xmlHasProp(node,PAR_DATATYPE))
	{
		prop = xmlGetProp(node,PAR_DATATYPE);
		datatype = (EDataType)Converter::stoi((char*)prop);
		xmlFree(prop);

		switch(datatype)
		{
			case EDataType::Bool:
				return new HisDevValue<bool>(node,false,factory,deviceWriter);
			case EDataType::Double:
				return new HisDevValue<double>(node,0,factory,deviceWriter);
			case EDataType::Int:
				return new HisDevValue<int>(node,0,factory,deviceWriter);
			case EDataType::String:
				return new HisDevValue<string>(node,string(""),factory,deviceWriter);
			case EDataType::Uint:
				return new HisDevValue<uint32_t>(node,0,factory,deviceWriter);
			case EDataType::Email:
				return new HisDevValueEmail(node,deviceWriter,factory);
			default:
				return NULL;
		}
	}
	return NULL;
}

void HisDevValueBase::DoInternalSave(xmlNodePtr & node)
{
	HisBase::DoInternalSave(node);
	Base64 b64;

	if (node != NULL)
	{
		//xmlSetProp(node,PAR_PINNAME,(xmlChar*)pinname.c_str());
		xmlSetProp(node,PAR_LOADTYPE,(xmlChar*)loadtype.c_str());
		xmlSetProp(node,PAR_PINNO,(xmlChar*)pinNumber.c_str());
		xmlSetProp(node,PAR_VALUE,(xmlChar*) b64.b64encode(GetStringValue()).c_str());
		xmlSetProp(node,PAR_DATATYPE,(xmlChar*)Converter::itos(datatype).c_str());
		xmlSetProp(node,PAR_DEV_ADDR,(xmlChar*)devaddr.c_str());
		xmlSetProp(node,PAR_UNIT,(xmlChar*)unit.c_str());
		xmlSetProp(node,PAR_ADDRESSNAME,(xmlChar*)addressName.c_str());
		xmlSetProp(node,PAR_DIRECTION,(xmlChar*)Converter::itos(direction).c_str());
	}
}

void HisDevValueBase::DoInternalLoad(xmlNodePtr & node)
{
	STACK
	HisBase::DoInternalLoad(node);
	xmlChar* prop;

	if (node!=NULL)
	{
		if (xmlHasProp(node,PAR_ADDRESSNAME))
		{
			prop = xmlGetProp(node,PAR_ADDRESSNAME);
			addressName = (char*)prop;
			if (prop==NULL || addressName=="")
				addressName = GetName();
			xmlFree(prop);
		}
		if (xmlHasProp(node,PAR_LOADTYPE))
			{
				prop = xmlGetProp(node,PAR_LOADTYPE);
				loadtype = (char*)prop;
				xmlFree(prop);
			}
		if (xmlHasProp(node,PAR_UNIT))
		{
			prop = xmlGetProp(node,PAR_UNIT);
			unit = (char*)prop;
			xmlFree(prop);
		}
		if (xmlHasProp(node,PAR_PINNO))
		{
			prop = xmlGetProp(node,PAR_PINNO);
			pinNumber = (char*)prop;
			xmlFree(prop);
		}
		if (xmlHasProp(node,PAR_DATATYPE))
		{
			prop = xmlGetProp(node,PAR_DATATYPE);
			datatype = (EDataType)Converter::stoi((char*)prop);
			xmlFree(prop);
		}
		if (xmlHasProp(node,PAR_DIRECTION))
		{
			prop = xmlGetProp(node,PAR_DIRECTION);
			direction = (EHisDevDirection)Converter::stoi((char*)prop);
			xmlFree(prop);
		}
		if (xmlHasProp(node,PAR_DEV_ADDR))
		{
			prop = xmlGetProp(node,PAR_DEV_ADDR);
			devaddr = (char*)prop;
			xmlFree(prop);
		}
		if (xmlHasProp(node,PAR_VALUE))
		{
			prop = xmlGetProp(node,PAR_VALUE);
			string strval = (char*)prop;
			xmlFree(prop);

			Base64 b64;
			string strvaldecoded = b64.b64decode(strval);

			bool oldallowforceOutput = allowForceOutput;
			this->allowForceOutput = true;
			this->ForceStringValue(strvaldecoded,false);
			allowForceOutput = oldallowforceOutput;
		}
	}
}

const xmlChar* HisDevValueBase::GetNodeNameInternal()
{
	return NODE_VALUE;
}

void HisDevValueBase::FireOnValueChanged(ValueChangedEventArgs args)
{
	STACK
	size_t size = delegatesMap.size();
	//CLogger::Info(StringBuilder::Format("HisDevValueBase::FireOnValueChanged | delegatesMap.size=%d",size).c_str());
	if (size>0)
	{
		for (std::map<void*,OnValueChangedDelegate>::iterator it=delegatesMap.begin(); it!=delegatesMap.end(); it++)
			it->second(args);
	}
}

/*
 *  registers listeners for tag change
 */
void HisDevValueBase::Register(OnValueChangedDelegate delegate, void* owner)
{
	STACK
	std::pair<std::map<void*,OnValueChangedDelegate>::iterator,bool> ret;
	ret = delegatesMap.insert(std::pair<void*,OnValueChangedDelegate>(owner,delegate));
	if (ret.second==false)
	{
		logger.Error("Delegate already added HisDevBase");
	}
	//size_t size = delegatesMap.size();
}

/*
 * unregister tag change handler
 */
void HisDevValueBase::UnRegister(void* owner)
{
	STACK
	delegatesMap.erase(owner);
}

std::string HisDevValueBase::GetLoadType()
{
	return this->loadtype;
}

void HisDevValueBase::SetLoadType(std::string loadtype)
{
	this->loadtype = loadtype;
}


void HisDevValueBase::SetAddressName(string pAddressName)
{
	addressName = pAddressName;
}

std::string HisDevValueBase::GetAddressName()
{
	return addressName;
}

std::string HisDevValueBase::GetAddress()
{
	STACK
	std::ostringstream address;
	address << devaddr << "." << pinNumber;
	return address.str();
}

EHisDevDirection HisDevValueBase::GetDirection()
{
	return direction;
}

EDataType HisDevValueBase::GetDataType()
{
	return datatype;
}

string HisDevValueBase::GetPinNumber()
{
	return pinNumber;
}

HisDevValueBase::~HisDevValueBase()
{

}

DateTime HisDevValueBase::GetValueChangeTime()
{
	return valueChangeTime;
}

bool HisDevValueBase::ForceStringValue(string strvalue, bool checkChange)
{
	STACK
	STACK_VAL("strvalue",strvalue)
	try
	{
		switch(this->datatype)
		{
			case EDataType::Double:
			{
				STACK_SECTION("Double")
				HisDevValue<double>* value = dynamic_cast<HisDevValue<double>*>(this);
				double val = atof(strvalue.c_str());
				if (val!=value->GetValue() || !checkChange)
					value->ForceValue(val);
				return true;
			}
			case EDataType::Bool:
			{
				STACK_SECTION("Bool")
				if (strvalue[0]=='1' || strvalue[0]=='0' || strvalue==string("true") || strvalue==string("false"))
				{
					bool newvalue = strvalue[0]=='1' || strvalue == "true" ? true : false;
					HisDevValue<bool>* value = dynamic_cast<HisDevValue<bool>*>(this);
					if (value->GetValue()!=newvalue || !checkChange)
						value->ForceValue(newvalue);
					return true;
				}
				else
				{
					logger.Error("Error write bool to value: Incorrect format");
				}
				return false;
			}
			case EDataType::Int:
			{
				STACK_SECTION("Int")
				HisDevValue<int>* value = dynamic_cast<HisDevValue<int>*>(this);
				int val = Converter::stoi(strvalue);
				if (val!=value->GetValue() || !checkChange)
					value->ForceValue(val);
				return true;
			}
			case EDataType::Email:
			case EDataType::String:
			{
				STACK_SECTION("String")
				HisDevValue<string>* value = dynamic_cast<HisDevValue<string>*>(this);
				if (strvalue!=value->GetValue() || !checkChange)
					value->ForceValue(strvalue);
				return true;
			}
			case EDataType::Uint:
			{
				STACK_SECTION("Uint")
				HisDevValue<uint32_t>* value = dynamic_cast<HisDevValue<uint32_t>*>(this);
				uint32_t val = Converter::stoui(strvalue,10);
				if (val!=value->GetValue() || !checkChange)
					value->ForceValue(val);
				return true;
			}
			default:
				return false;
		}
		return false;
	}
	catch(...)
	{
		logger.Error(string("HisDevValue::ForceStringValue | Error set value: " + strvalue).c_str());
	}
	return false;
}

std::string HisDevValueBase::GetStringValue()
{
	STACK
	switch(datatype)
	{
		case EDataType::Double:
		{
			STACK_SECTION("Double")
			HisDevValue<double>* value = dynamic_cast<HisDevValue<double>*>(this);
			//std::ostringstream s;
			char strdouble[100];
			sprintf(strdouble,"%1.2f",value->GetValue());//s << value->GetValue();
			return strdouble;//s.str();
		}
		case EDataType::Bool:
		{
			STACK_SECTION("Bool")
			HisDevValue<bool>* value = dynamic_cast<HisDevValue<bool>*>(this);
			std::ostringstream s;
			s << value->GetValue();
			return s.str();
		}
		case EDataType::Int:
		{
			STACK_SECTION("Int")
			HisDevValue<int>* value = dynamic_cast<HisDevValue<int>*>(this);
			std::ostringstream s;
			s << value->GetValue();
			return s.str();
		}
		case EDataType::Email:
		case EDataType::String:
		{
			STACK_SECTION("String")
			HisDevValue<string>* value = dynamic_cast<HisDevValue<string>*>(this);
			return value->GetValue();
		}
		case EDataType::Uint:
		{
			STACK_SECTION("Uint")
			HisDevValue<uint32_t>* value = dynamic_cast<HisDevValue<uint32_t>*>(this);
			std::ostringstream s;
			s << value->GetValue();
			return s.str();
		}
		default:
			return "";
	}
	return "";
}

std::string HisDevValueBase::GetPinName()
{
	return GetName();
}

void HisDevValueBase::SetUnit(string pUnit)
{
	unit = pUnit;
}

string HisDevValueBase::GetUnit()
{
	return unit;
}

void HisDevValueBase::SetPinName(std::string pname)
{
	//pinname = pname;
	SetName(pname);
}

bool HisDevValueBase::GetDeviceError()
{
	return deviceError;
}
