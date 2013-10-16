/*
 *  HisDevValue.cpp
 *
 *  Created on: Mar 24, 2013
 *      Author: Josef Lola
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
#include "logger.h"
#include "homeis/Helpers/StringBuilder.h"
#include "HisDevValue.h"

HisDevValueBase::HisDevValueBase(std::string pdevaddr, EHisDevDirection direct, EDataType pdatatype,int ppinNumber)
{
	pinNumber = ppinNumber;
	direction = direct;
	datatype = pdatatype;
	devaddr = pdevaddr;
	deviceError = true;
}

HisDevValueBase::HisDevValueBase(xmlNodePtr pnode) :
	HisBase::HisBase(pnode)
{

}

bool HisDevValueBase::GetForceOutput()
{
	return allowForceOutput;
}

void HisDevValueBase::SetForceOutput(bool force)
{
	allowForceOutput = force;
}

HisDevValueBase::HisDevValueBase(HisDevValueBase & src)
{
	pinname = src.pinname;
	devaddr = src.devaddr;
	pinNumber = src.pinNumber;
	direction = src.direction;
	datatype = src.datatype;
	deviceError = src.deviceError;
}

HisDevValueBase* HisDevValueBase::Create(xmlNodePtr pNode)
{
	EDataType datatype;
	xmlChar* prop = NULL;
	if (xmlHasProp(pNode,PAR_DATATYPE))
	{
		prop = xmlGetProp(pNode,PAR_DATATYPE);
		datatype = (EDataType)Converter::stoi((char*)prop);
		xmlFree(prop);

		switch(datatype)
		{
			case EDataType::Bool:
				return new HisDevValue<bool>(pNode);
			case EDataType::Double:
				return new HisDevValue<double>(pNode);
			case EDataType::Int:
				return new HisDevValue<int>(pNode);
			case EDataType::String:
				return new HisDevValue<string>(pNode);
			case EDataType::Uint:
				return new HisDevValue<uint32_t>(pNode);
			case EDataType::Unknown:
				return NULL;
		}
	}
	return NULL;
}

void HisDevValueBase::DoInternalSave(xmlNodePtr & node)
{
	HisBase::DoInternalSave(node);

	if (node != NULL)
	{
		xmlSetProp(node,PAR_PINNAME,(xmlChar*)pinname.c_str());
		xmlSetProp(node,PAR_PINNO,(xmlChar*)Converter::itos(pinNumber).c_str());
		xmlSetProp(node,PAR_VALUE,(xmlChar*)GetStringValue().c_str());
		xmlSetProp(node,PAR_DATATYPE,(xmlChar*)Converter::itos(datatype).c_str());
		xmlSetProp(node,PAR_DEV_ADDR,(xmlChar*)devaddr.c_str());
		xmlSetProp(node,PAR_UNIT,(xmlChar*)unit.c_str());
		xmlSetProp(node,PAR_DIRECTION,(xmlChar*)Converter::itos(direction).c_str());
	}
}

void HisDevValueBase::DoInternalLoad(xmlNodePtr & node)
{
	HisBase::DoInternalLoad(node);

	xmlChar* prop;

	if (xmlHasProp(node,PAR_PINNAME))
	{
		prop = xmlGetProp(node,PAR_PINNAME);
		pinname = (char*)prop;
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
		pinNumber = Converter::stoi((char*)prop);
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
		this->ForceStringValue(strval);
	}
}

const xmlChar* HisDevValueBase::GetNodeNameInternal()
{
	return NODE_VALUE;
}

void HisDevValueBase::FireOnValueChanged(ValueChangedEventArgs args)
{
	size_t size = delegatesMap.size();
	CLogger::Info(StringBuilder::Format("HisDevValueBase::FireOnValueChanged | delegatesMap.size=%d",size).c_str());
	for (std::map<void*,OnValueChangedDelegate>::iterator it=delegatesMap.begin(); it!=delegatesMap.end(); it++)
		it->second(args);
}

/*
 *  registers listeners for tag change
 */
void HisDevValueBase::Register(OnValueChangedDelegate delegate, void* owner)
{
	std::pair<std::map<void*,OnValueChangedDelegate>::iterator,bool> ret;
	ret = delegatesMap.insert(std::pair<void*,OnValueChangedDelegate>(owner,delegate));
	if (ret.second==false)
	{
		CLogger::Error("Delegate already added HisDevBase");
	}
	size_t size = delegatesMap.size();
}

/*
 * unregister tag change handler
 */
void HisDevValueBase::UnRegister(void* owner)
{
	delegatesMap.erase(owner);
}

std::string HisDevValueBase::GetAddress()
{
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

uint16_t HisDevValueBase::GetPinNumber()
{
	return pinNumber;
}

HisDevValueBase::~HisDevValueBase()
{

}

bool HisDevValueBase::ForceStringValue(string strvalue)
{
	try
	{
		switch(this->datatype)
		{
			case EDataType::Double:
			{
				HisDevValue<double>* value = dynamic_cast<HisDevValue<double>*>(this);
				double val = atof(strvalue.c_str());
				if (val!=value->GetValue())
					value->ForceValue(val);
				return true;
			}
			case EDataType::Bool:
			{
				if (strvalue[0]=='1' || strvalue[0]=='0' || strvalue==string("true") || strvalue==string("false"))
				{
					bool newvalue = strvalue[0]=='1' || strvalue == "true" ? true : false;
					HisDevValue<bool>* value = dynamic_cast<HisDevValue<bool>*>(this);
					if (value->GetValue()!=newvalue)
						value->ForceValue(newvalue);
					return true;
				}
				else
				{
					CLogger::Error("Error write bool to value: Incorrect format");
				}
				return false;
			}
			case EDataType::Int:
			{
				HisDevValue<int>* value = dynamic_cast<HisDevValue<int>*>(this);
				int val = Converter::stoi(strvalue,10);
				if (val!=value->GetValue())
					value->ForceValue(val);
				return false;
			}
			case EDataType::String:
			{
				HisDevValue<string>* value = dynamic_cast<HisDevValue<string>*>(this);
				if (strvalue!=value->GetValue())
					value->ForceValue(strvalue);
				return true;
			}
			case EDataType::Uint:
			{
				//HisDevValue<uint16_t>* value = dynamic_cast<HisDevValue<uint16_t>*>(this);
//				HisDevValue<int>* value = dynamic_cast<HisDevValue<int>*>(this);
//				int val = Converter::stoi(strvalue,10);
//				value->ForceValue(val);
				return false;
			}
			case EDataType::Unknown:
				return false;
		}
		return false;
	}
	catch(...)
	{
		CLogger::Error("Error set value");
	}
	return false;
}

std::string HisDevValueBase::GetStringValue()
{
	switch(datatype)
	{
		case EDataType::Double:
		{
			HisDevValue<double>* value = dynamic_cast<HisDevValue<double>*>(this);
			//std::ostringstream s;
			char strdouble[100];
			sprintf(strdouble,"%1.2f",value->GetValue());//s << value->GetValue();
			return strdouble;//s.str();
		}
		case EDataType::Bool:
		{
			HisDevValue<bool>* value = dynamic_cast<HisDevValue<bool>*>(this);
			std::ostringstream s;
			s << value->GetValue();
			return s.str();
		}
		case EDataType::Int:
		{
			HisDevValue<int>* value = dynamic_cast<HisDevValue<int>*>(this);
			std::ostringstream s;
			s << value->GetValue();
			return s.str();
		}
		case EDataType::String:
		{
			HisDevValue<string>* value = dynamic_cast<HisDevValue<string>*>(this);
			return value->GetValue();
		}
		case EDataType::Uint:
		{
			HisDevValue<uint16_t>* value = dynamic_cast<HisDevValue<uint16_t>*>(this);
			std::ostringstream s;
			s << value->GetValue();
			return s.str();
		}
		case EDataType::Unknown:
			return "";
	}
	return "";
}

std::string HisDevValueBase::GetPinName()
{
	return pinname;
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
	pinname = pname;
	SetName(pname);
}

bool HisDevValueBase::GetDeviceError()
{
	return deviceError;
}
