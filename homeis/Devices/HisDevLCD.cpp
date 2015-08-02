/*
 * HisDevLCD.cpp
 *
 *  Created on: 27. 7. 2015
 *      Author: pc
 */

#include <string>
#include <sstream>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "srutil/event/event.hpp"
#include "srutil/delegate/delegate.hpp"

#include "logger.h"
#include "converter.h"
#include "EDataType.h"
#include "EHisDevDirection.h"
#include "ValueEventArgs.h"

#include "HisDevValue.h"
#include "HisDevBase.h"
#include "HisDallas.h"
#include "LOWdevLCD.h"
#include "HisDevLCD.h"

using namespace std;

HisDevLCD::HisDevLCD(xmlNodePtr node, LOW_devLCD* pdev) :
	HisDevDallas::HisDevDallas(node, pdev),
	dev(NULL),lightEnabled(NULL),firstrefresh(false)
//,row1Addr(0x00),row2Addr(0x40),row3Addr(0x20),row4Addr(0x50)
{
	dev = pdev;
}

HisDevLCD::HisDevLCD(LOW_devLCD* pdev) :
	HisDevDallas::HisDevDallas(pdev),dev(NULL),lightEnabled(NULL),firstrefresh(false)
//,row1Addr(0x00),row2Addr(0x40),row3Addr(0x20),row4Addr(0x50)
{
	dev = pdev;
	CreateDataPoints();
}

const xmlChar* HisDevLCD::GetNodeNameInternal()
{
	return (const xmlChar *)NODE_NAME_LCD;
}

void HisDevLCD::CreateDataPoints()
{
	std::string strid = dev->getID().getRomIDString();

	WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevLCD, &HisDevLCD::WriteToDevice>(this);
	//create data points
	lightEnabled = new HisDevValue<bool>(strid, EHisDevDirection::ReadWrite, EDataType::Bool, LIGHT_ENABLE,false);
	lightEnabled->SetName("LightEnabled");
	lightEnabled->delegateWrite = delegate;
	lightEnabled->Load();
	Add(lightEnabled);

	size_t rowscount = 4;
	for(size_t i=0;i<rowscount;i++)
	{
		string rowid = strid + Converter::itos(i+1);
		HisDevValue<string>* newrow = new HisDevValue<string>(rowid,EHisDevDirection::Write, EDataType::String,i+1,string(""));
		HisDevValue<int>* rowaddr = new HisDevValue<int>(rowid,EHisDevDirection::ReadWrite, EDataType::Int,i+1,(int)0x00);
		rowaddr->SetName("Adresa radku " + Converter::itos(i+1));
		newrow->delegateWrite = delegate;
		newrow->SetName("row" + Converter::itos(i+1));
		rows.push_back(newrow);
		rowaddrs.push_back(rowaddr);
		Add(newrow);
		Add(rowaddr);
	}
}

void HisDevLCD::WriteToDevice(ValueChangedEventArgs args)
{
	string text;
	switch(args.GetValue()->GetPinNumber())
	{
	case LIGHT_ENABLE:
		break;
	case ROW1:
		text = rows[args.GetValue()->GetPinNumber()-1]->GetValue();
		dev->WriteToLCD(text.c_str(),0x00);
		break;
	case ROW2:
		text = rows[args.GetValue()->GetPinNumber()-1]->GetValue();
		dev->WriteToLCD(text.c_str(),0x40);
		break;
	case ROW3:
	case ROW4:
		DevError = false;
	break;
	}
}

void HisDevLCD::DoInternalLoad(xmlNodePtr & node)
{
	HisDevDallas::DoInternalLoad(node);

//	if (xmlHasProp(node,NODE_PROP_ROW1ADDR))
//	{
//		xmlChar* prop = xmlGetProp(node,NODE_PROP_ROW1ADDR);
//		row1Addr = Converter::stoi((char*)prop,10);
//	}
//	if (xmlHasProp(node,NODE_PROP_ROW2ADDR))
//	{
//		xmlChar* prop = xmlGetProp(node,NODE_PROP_ROW2ADDR);
//		row2Addr = Converter::stoi((char*)prop,10);
//	}
//	if (xmlHasProp(node,NODE_PROP_ROW3ADDR))
//	{
//		xmlChar* prop = xmlGetProp(node,NODE_PROP_ROW3ADDR);
//		row3Addr = Converter::stoi((char*)prop,10);
//	}
//	if (xmlHasProp(node,NODE_PROP_ROW4ADDR))
//	{
//		xmlChar* prop = xmlGetProp(node,NODE_PROP_ROW4ADDR);
//		row4Addr = Converter::stoi((char*)prop,10);
//	}


	WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevLCD, &HisDevLCD::WriteToDevice>(this);

	vector<HisDevValue<std::string>*> values = GetItems<HisDevValue<std::string>>();
	for(size_t i=0;i<values.size();i++)
	{
		HisDevValue<string> *value = values[i];
		value->delegateWrite = delegate;
		rows.push_back(value);
	}

	vector<HisDevValue<bool>*> values2 = GetItems<HisDevValue<bool>>();
	for(size_t i=0;i<values2.size();i++)
	{
		if (values2[i]->GetPinNumber()==LIGHT_ENABLE)
		{
			lightEnabled = values2[i];
			lightEnabled->delegateWrite = delegate;
		}
	}
}

void HisDevLCD::DoInternalSave(xmlNodePtr & node)
{
	HisDevDallas::DoInternalSave(node);

//	std::string strAddr = Converter::itos(row1Addr,10);
//	xmlSetProp(node,NODE_PROP_ROW1ADDR,(const xmlChar*)strAddr.c_str());
//
//	strAddr = Converter::itos(row2Addr,10);
//	xmlSetProp(node,NODE_PROP_ROW2ADDR,(const xmlChar*)strAddr.c_str());
//
//	strAddr = Converter::itos(row3Addr,10);
//	xmlSetProp(node,NODE_PROP_ROW3ADDR,(const xmlChar*)strAddr.c_str());
//
//	strAddr = Converter::itos(row4Addr,10);
//	xmlSetProp(node,NODE_PROP_ROW4ADDR,(const xmlChar*)strAddr.c_str());
}

void HisDevLCD::DoInternalRefresh()
{
	if (!firstrefresh)
	{
		firstrefresh = true;

		lightEnabled->SetValueFromDevice(false,false);

		for(size_t i=0;i<rows.size();i++)
		{
			rows[i]->SetValueFromDevice("",false);
		}
		for(size_t i=0;i<rowaddrs.size();i++)
		{
			rowaddrs[i]->SetValueFromDevice(rowaddrs[i]->GetValue(),false);
		}
		DevError = false;
	}
}

HisDevLCD::~HisDevLCD()
{
}


