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
#include "PoppyDebugTools.h"
#include "HisDevLCD.h"

using namespace std;

HisDevLCD::HisDevLCD(xmlNodePtr node, LOW_devLCD* pdev) :
	HisDevDallas::HisDevDallas(node, pdev),
	dev(NULL),lightEnabled(NULL),firstrefresh(false),refreshRow1(false),refreshRow2(false),refreshRow3(false),refreshRow4(false),refreshLihtEnabled(false),error(false)
	,rowaddr1(0x00),rowaddr2(0x00),rowaddr3(0x00),rowaddr4(0x00),
	row1(0),row2(0),row3(0),row4(0),lcdOn(false)
{
	STACK
	dev = pdev;
	//set zero period becaouse LCD refresh only if needs
	SetScanPeriod(0);
}

HisDevLCD::HisDevLCD(LOW_devLCD* pdev) :
	HisDevDallas::HisDevDallas(pdev),dev(NULL),lightEnabled(NULL),
	firstrefresh(false),refreshRow1(false),refreshRow2(false),refreshRow3(false),refreshRow4(false),refreshLihtEnabled(false),lcdOn(false)

{
	STACK
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
	STACK
	int* pint = static_cast<int*>(args);
	HisDevValueBase* tag = dynamic_cast<HisDevValueBase*>(hisbase);
	if (tag->GetPinNumber()==*pint)
		return true;
	return false;
}

HisDevValue<string>* HisDevLCD::CreateRow(string name,WriteToDeviceRequestDelegate & delegate,int pinno)
{
	STACK
	std::string strid = dev->getID().getRomIDString();
	HisDevValue<string>* row = new HisDevValue<string>(strid,EHisDevDirection::Write, EDataType::String,pinno,string(""));
	row->delegateWrite = delegate;
	row->SetName(name);
	return row;
}

HisDevValue<int>* HisDevLCD::CreateRowAddr(string name,WriteToDeviceRequestDelegate & delegate,int pinno,int adr)
{
	STACK
	std::string strid = dev->getID().getRomIDString();
	HisDevValue<int>* rowaddr = new HisDevValue<int>(strid,EHisDevDirection::ReadWrite, EDataType::Int,pinno,adr);
	rowaddr->delegateWrite = delegate;
	rowaddr->SetName(name);
	return rowaddr;
}

void HisDevLCD::CreateDataPoints()
{
	STACK
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
	STACK
	switch(args.GetValue()->GetPinNumber())
	{
	case LIGHTENABLED:
		refreshLihtEnabled = true;
		break;
	case ID_ROW1:
		refreshRow1 = true;
		break;
	case ID_ROW2:
		refreshRow2 = true;
		break;
	case ID_ROW3:
		refreshRow3 = true;
		break;
	case ID_ROW4:
		refreshRow4 = true;
		break;
		DevError = false;
	break;
	}
	NeedRefresh();
}

void HisDevLCD::DoInternalLoad(xmlNodePtr & node)
{
	STACK
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
	STACK
	HisDevDallas::DoInternalSave(node);
}

void HisDevLCD::DoInternalRefresh()
{
	STACK
	if (!lcdOn)
	{
		dev->LCDOn();
	}

	error = !dev->IsConnected();

	if (refreshRow1)
	{
		STACK_SECTION("refreshRow1")
		refreshRow1 = false;
		if (!error) dev->WriteToLCD(row1->GetValue().c_str(),rowaddr1->GetValue());
		row1->ReadedValueFromDevice(row1->GetValue(),error);
		rowaddr1->ReadedValueFromDevice(rowaddr1->GetValue(),error);
	}
	if (refreshRow2)
	{
		STACK_SECTION("refreshRow2")
		refreshRow2 = false;
		if (!error) dev->WriteToLCD(row2->GetValue().c_str(),rowaddr2->GetValue());
		row2->ReadedValueFromDevice(row2->GetValue(),error);
		rowaddr2->ReadedValueFromDevice(rowaddr2->GetValue(),error);
	}
	if (refreshRow3)
	{
		STACK_SECTION("refreshRow3")
		refreshRow3 = false;
		if (!error) dev->WriteToLCD(row3->GetValue().c_str(),rowaddr3->GetValue());
		row3->ReadedValueFromDevice(row3->GetValue(),error);
		rowaddr3->ReadedValueFromDevice(rowaddr3->GetValue(),error);
	}
	if (refreshRow4)
	{
		STACK_SECTION("refreshRow4")
		refreshRow4 = false;
		if (!error) dev->WriteToLCD(row4->GetValue().c_str(),rowaddr4->GetValue());
		row4->ReadedValueFromDevice(row4->GetValue(),error);
		rowaddr4->ReadedValueFromDevice(rowaddr4->GetValue(),error);
	}
	if (!error && (refreshLihtEnabled || !lcdOn))
	{
		STACK_SECTION("refreshLight")
		refreshLihtEnabled = false;
		dev->LightOn(lightEnabled->GetValue());
		lightEnabled->ReadedValueFromDevice(lightEnabled->GetValue(),error);
	}

	lcdOn = true;
}

HisDevLCD::~HisDevLCD()
{
}


