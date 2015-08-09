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
	dev(NULL),lightEnabled(NULL),firstrefresh(false),needRefresh(false)
	,rowaddr1(0x00),rowaddr2(0x00),rowaddr3(0x00),rowaddr4(0x00),
	row1(0),row2(0),row3(0),row4(0)
{
	dev = pdev;
	//set zero period becaouse LCD refresh only if needs
	SetScanPeriod(0);
}

HisDevLCD::HisDevLCD(LOW_devLCD* pdev) :
	HisDevDallas::HisDevDallas(pdev),dev(NULL),lightEnabled(NULL),firstrefresh(false),needRefresh(false)
//,row1Addr(0x00),row2Addr(0x40),row3Addr(0x20),row4Addr(0x50)
{
	dev = pdev;
	CreateDataPoints();
	//set zero period becaouse LCD refresh only if needs
	SetScanPeriod(0);
}

const xmlChar* HisDevLCD::GetNodeNameInternal()
{
	return (const xmlChar *)NODE_NAME_LCD;
}

bool FindFunction(HisBase* hisbase,void* args)
{
	int* pint = static_cast<int*>(args);
	HisDevValueBase* tag = dynamic_cast<HisDevValueBase*>(hisbase);
	if (tag->GetPinNumber()==*pint)
		return true;
	return false;
}

HisDevValue<string>* HisDevLCD::CreateRow(string name,WriteToDeviceRequestDelegate & delegate,int pinno)
{
	std::string strid = dev->getID().getRomIDString();
	HisDevValue<string>* row = new HisDevValue<string>(strid,EHisDevDirection::Write, EDataType::String,pinno,string(""));
	row->delegateWrite = delegate;
	row->SetName(name);
	return row;
}

HisDevValue<int>* HisDevLCD::CreateRowAddr(string name,WriteToDeviceRequestDelegate & delegate,int pinno,int adr)
{
	std::string strid = dev->getID().getRomIDString();
	HisDevValue<int>* rowaddr = new HisDevValue<int>(strid,EHisDevDirection::ReadWrite, EDataType::Int,pinno,adr);
	rowaddr->delegateWrite = delegate;
	rowaddr->SetName(name);
	return rowaddr;
}

void HisDevLCD::CreateDataPoints()
{
	std::string strid = dev->getID().getRomIDString();

	WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevLCD, &HisDevLCD::WriteToDevice>(this);
	//create data points
	lightEnabled = new HisDevValue<bool>(strid, EHisDevDirection::ReadWrite, EDataType::Bool, LIGHTENABLED,false);
	lightEnabled->SetName("LightEnabled");
	lightEnabled->delegateWrite = delegate;
	lightEnabled->Load();
	Add(lightEnabled);

	row1 = CreateRow("Row1",delegate,ID_ROW1);
	row2 = CreateRow("Row2",delegate,ID_ROW2);
	row3 = CreateRow("Row3",delegate,ID_ROW3);
	row4 = CreateRow("Row4",delegate,ID_ROW4);
	Add(row1);
	Add(row2);
	Add(row3);
	Add(row4);

	rowaddr1 = CreateRowAddr("Row1address",delegate,ID_ROW1ADDR,0x00);
	rowaddr2 = CreateRowAddr("Row2address",delegate,ID_ROW2ADDR,0x40);
	rowaddr3 = CreateRowAddr("Row3address",delegate,ID_ROW3ADDR,0x50);
	rowaddr4 = CreateRowAddr("Row4address",delegate,ID_ROW4ADDR,0x60);
	Add(rowaddr1);
	Add(rowaddr2);
	Add(rowaddr3);
	Add(rowaddr4);
}

void HisDevLCD::WriteToDevice(ValueChangedEventArgs args)
{
	needRefresh = true;
//	string text;
//	switch(args.GetValue()->GetPinNumber())
//	{
//	case LIGHTENABLED:
//		break;
//	case ID_ROW1:
//		//dev->WriteToLCD(row1->GetValue().c_str(),rowaddr1->GetValue());
//		break;
//	case ID_ROW2:
//		//dev->WriteToLCD(row2->GetValue().c_str(),rowaddr2->GetValue());
//		break;
//	case ID_ROW3:
//		//dev->WriteToLCD(row3->GetValue().c_str(),rowaddr3->GetValue());
//		break;
//	case ID_ROW4:
//		//dev->WriteToLCD(row4->GetValue().c_str(),rowaddr4->GetValue());
//		break;
//		DevError = false;
//	break;
//	}
}

void HisDevLCD::DoInternalLoad(xmlNodePtr & node)
{
	HisDevDallas::DoInternalLoad(node);

	WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevLCD, &HisDevLCD::WriteToDevice>(this);

	vector<HisDevValueBase*> values = GetItems<HisDevValueBase>();
	for(size_t i=0;i<values.size();i++)
	{
		values[i]->delegateWrite = delegate;
		switch(values[i]->GetPinNumber())
		{
		case LIGHTENABLED:
			lightEnabled = dynamic_cast<HisDevValue<bool>*>(values[i]);
			break;
		case ID_ROW1:
			row1 = dynamic_cast<HisDevValue<string>*>(values[i]);
			break;
		case ID_ROW2:
			row2 = dynamic_cast<HisDevValue<string>*>(values[i]);
			break;
		case ID_ROW3:
			row3 = dynamic_cast<HisDevValue<string>*>(values[i]);
			break;
		case ID_ROW4:
			row4 = dynamic_cast<HisDevValue<string>*>(values[i]);
			break;
		case ID_ROW1ADDR:
			rowaddr1 = dynamic_cast<HisDevValue<int>*>(values[i]);
			break;
		case ID_ROW2ADDR:
			rowaddr2 = dynamic_cast<HisDevValue<int>*>(values[i]);
			break;
		case ID_ROW3ADDR:
			rowaddr3 = dynamic_cast<HisDevValue<int>*>(values[i]);
			break;
		case ID_ROW4ADDR:
			rowaddr4 = dynamic_cast<HisDevValue<int>*>(values[i]);
			break;
		}
	}
}

void HisDevLCD::DoInternalSave(xmlNodePtr & node)
{
	HisDevDallas::DoInternalSave(node);
}

void HisDevLCD::DoInternalRefresh()
{
	if (!firstrefresh)
	{
		firstrefresh = true;

		lightEnabled->SetValueFromDevice(lightEnabled->GetValue(),false);

		row1->SetValueFromDevice(row1->GetValue(),false);
		row2->SetValueFromDevice(row2->GetValue(),false);
		row3->SetValueFromDevice(row3->GetValue(),false);
		row4->SetValueFromDevice(row4->GetValue(),false);

		rowaddr1->SetValueFromDevice(rowaddr1->GetValue(),false);
		rowaddr2->SetValueFromDevice(rowaddr2->GetValue(),false);
		rowaddr3->SetValueFromDevice(rowaddr3->GetValue(),false);
		rowaddr4->SetValueFromDevice(rowaddr4->GetValue(),false);

		DevError = false;
	}
	else if (needRefresh)
	{
		needRefresh = false;
		dev->WriteToLCD(row1->GetValue().c_str(),rowaddr1->GetValue());
		dev->WriteToLCD(row2->GetValue().c_str(),rowaddr2->GetValue());
		dev->WriteToLCD(row3->GetValue().c_str(),rowaddr3->GetValue());
		dev->WriteToLCD(row4->GetValue().c_str(),rowaddr4->GetValue());
	}
}

HisDevLCD::~HisDevLCD()
{
}


