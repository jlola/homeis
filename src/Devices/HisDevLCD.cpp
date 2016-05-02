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
	dev(NULL),lightEnabled(NULL),firstrefresh(false),refreshRow1(false),refreshRow2(false),refreshRow3(false),refreshRow4(false),
	refreshbuttoncnt1(false),refreshbuttoncnt2(false),refreshbuttoncnt3(false),refreshbuttoncnt4(false)
	,refreshLihtEnabled(false)
	,rowaddr1(0x00),rowaddr2(0x00),rowaddr3(0x00),rowaddr4(0x00),
	row1(0),row2(0),row3(0),row4(0),lcdOn(false),input1(NULL),input2(NULL),input3(NULL),input4(NULL),
	inputcnt1(NULL),inputcnt2(NULL),inputcnt3(NULL),inputcnt4(NULL)
{
	STACK
	dev = pdev;
	//set zero period becaouse LCD refresh only if needs
	SetScanPeriod(10000);
}

HisDevLCD::HisDevLCD(LOW_devLCD* pdev) :
	HisDevDallas::HisDevDallas(pdev),dev(NULL),lightEnabled(NULL),
	firstrefresh(false),refreshRow1(false),refreshRow2(false),refreshRow3(false),refreshRow4(false),
	refreshbuttoncnt1(false),refreshbuttoncnt2(false),refreshbuttoncnt3(false),refreshbuttoncnt4(false),
	refreshLihtEnabled(false),lcdOn(false)
	,input1(NULL),input2(NULL),input3(NULL),input4(NULL),
	inputcnt1(NULL),inputcnt2(NULL),inputcnt3(NULL),inputcnt4(NULL)
{
	STACK
	dev = pdev;
	CreateDataPoints();
	//set zero period becaouse LCD refresh only if needs
	SetScanPeriod(10000);
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

void HisDevLCD::CreateRow(HisDevValue<string>* & row, string name,WriteToDeviceRequestDelegate & delegate,int pinno)
{
	STACK
	if (row==NULL)
	{
		std::string strid = dev->getID().getRomIDString();
		row = new HisDevValue<string>(strid,EHisDevDirection::Write, EDataType::String,pinno,string(""));
		row->delegateWrite = delegate;
		row->SetName(name);
		Add(row);
	}
}

void HisDevLCD::CreateInput(HisDevValue<bool>* & input,string name,int pinno)
{
	STACK
	if (input==NULL)
	{
		std::string strid = dev->getID().getRomIDString();
		input = new HisDevValue<bool>(strid,EHisDevDirection::Read, EDataType::Bool,pinno,false);
		input->SetName(name);
		Add(input);
	}
}

void HisDevLCD::CreateInputCounter(HisDevValue<uint32_t>* & inputcnt, string name,int pinno)
{
	STACK
	if (inputcnt==NULL)
	{
		std::string strid = dev->getID().getRomIDString();
		inputcnt = new HisDevValue<uint32_t>(strid,EHisDevDirection::ReadWrite, EDataType::Uint,pinno,0);
		inputcnt->SetName(name);
		Add(inputcnt);
	}
}

void HisDevLCD::CreateRowAddr(HisDevValue<int>* rowaddr,string name,WriteToDeviceRequestDelegate & delegate,int pinno,int adr)
{
	STACK
	if (rowaddr==NULL)
	{
		std::string strid = dev->getID().getRomIDString();
		rowaddr = new HisDevValue<int>(strid,EHisDevDirection::ReadWrite, EDataType::Int,pinno,adr);
		rowaddr->delegateWrite = delegate;
		rowaddr->SetName(name);
	}
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

	lightEnabled = new HisDevValue<bool>(strid, EHisDevDirection::ReadWrite, EDataType::Bool, LIGHTENABLED,false);
	lightEnabled->SetName("LightEnabled");
	lightEnabled->delegateWrite = delegate;
	lightEnabled->Load();
	Add(lightEnabled);

	CreateRow(row1,"Row1",delegate,ID_ROW1);
	CreateRow(row2,"Row2",delegate,ID_ROW2);
	CreateRow(row3,"Row3",delegate,ID_ROW3);
	CreateRow(row4,"Row4",delegate,ID_ROW4);

	CreateRowAddr(rowaddr1,"Row1address",delegate,ID_ROW1ADDR,0x00);
	CreateRowAddr(rowaddr2,"Row2address",delegate,ID_ROW2ADDR,0x40);
	CreateRowAddr(rowaddr3,"Row3address",delegate,ID_ROW3ADDR,0x50);
	CreateRowAddr(rowaddr4,"Row4address",delegate,ID_ROW4ADDR,0x60);

	CreateInputs();
}

void HisDevLCD::CreateInputs()
{
	CreateInput(input1,"Tlacitko1",ID_INPUT1);
	CreateInput(input2,"Tlacitko2",ID_INPUT2);
	CreateInput(input3,"Tlacitko3",ID_INPUT3);
	CreateInput(input4,"Tlacitko4",ID_INPUT4);

	CreateInputCounter(inputcnt1,"TlacitkoCitac1",ID_INPUTCNT1);
	CreateInputCounter(inputcnt2,"TlacitkoCitac2",ID_INPUTCNT2);
	CreateInputCounter(inputcnt3,"TlacitkoCitac3",ID_INPUTCNT3);
	CreateInputCounter(inputcnt4,"TlacitkoCitac4",ID_INPUTCNT4);
}

void HisDevLCD::WriteToDevice(ValueChangedEventArgs args)
{
	SetChanged();
//	STACK
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
	case ID_INPUTCNT1:
		refreshbuttoncnt1 = true;
		break;
	case ID_INPUTCNT2:
		refreshbuttoncnt2 = true;
		break;
	case ID_INPUTCNT3:
		refreshbuttoncnt3 = true;
		break;
	case ID_INPUTCNT4:
		refreshbuttoncnt4 = true;
		break;
	break;
	}
	//NeedRefresh();
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
		case ID_INPUT1:
			input1 = dynamic_cast<HisDevValue<bool>*>(values[i]);
			break;
		case ID_INPUT2:
			input2 = dynamic_cast<HisDevValue<bool>*>(values[i]);
			break;
		case ID_INPUT3:
			input3 = dynamic_cast<HisDevValue<bool>*>(values[i]);
			break;
		case ID_INPUT4:
			input4 = dynamic_cast<HisDevValue<bool>*>(values[i]);
			break;
		case ID_INPUTCNT1:
			inputcnt1 = dynamic_cast<HisDevValue<uint32_t>*>(values[i]);
			break;
		case ID_INPUTCNT2:
			inputcnt2 = dynamic_cast<HisDevValue<uint32_t>*>(values[i]);
			break;
		case ID_INPUTCNT3:
			inputcnt3 = dynamic_cast<HisDevValue<uint32_t>*>(values[i]);
			break;
		case ID_INPUTCNT4:
			inputcnt4 = dynamic_cast<HisDevValue<uint32_t>*>(values[i]);
			break;
		}
	}
	CreateInputs();
}

void HisDevLCD::DoInternalSave(xmlNodePtr & node)
{
	STACK
	HisDevDallas::DoInternalSave(node);
}

bool HisDevLCD::GetValue(byteVec_t counters, size_t index)
{
	if (counters.size()<=index+1)
	{
		//uint16_t counterValue = (counters[index+1] << 4) | counters[index+1];

		return true;
	}
	return false;
}

void HisDevLCD::DoInternalRefresh(bool alarm)
{
	STACK

	bool error = GetError();
	//refreshRow1 = refreshRow2 = refreshRow3 = refreshRow4 = true;
	refreshLihtEnabled = true;

	bool newerror = true;//!dev->IsConnected();
	byteVec_t counters(16);
	newerror = !dev->ReadCounters(counters);
	uint8_t inputs;
	newerror = !dev->ReadInputs(inputs);

	input1->ReadedValueFromDevice((inputs & (1<<0))>0,newerror);
	input2->ReadedValueFromDevice((inputs & (1<<1))>0,newerror);
	input3->ReadedValueFromDevice((inputs & (1<<2))>0,newerror);
	input4->ReadedValueFromDevice((inputs & (1<<3))>0,newerror);

	if (!lcdOn || newerror!=error)
	{
		dev->LCDOn();
	}
	if (refreshRow1 || newerror!=error)
	{
		STACK_SECTION("refreshRow1")
		refreshRow1 = false;
		if (!error) dev->WriteToLCD(row1->GetValue().c_str(),rowaddr1->GetValue());
		row1->ReadedValueFromDevice(row1->GetValue(),error);
		rowaddr1->ReadedValueFromDevice(rowaddr1->GetValue(),error);
	}
	if (refreshRow2 || newerror!=error)
	{
		STACK_SECTION("refreshRow2")
		refreshRow2 = false;
		if (!error) dev->WriteToLCD(row2->GetValue().c_str(),rowaddr2->GetValue());
		row2->ReadedValueFromDevice(row2->GetValue(),error);
		rowaddr2->ReadedValueFromDevice(rowaddr2->GetValue(),error);
	}
	if (refreshRow3 || newerror!=error)
	{
		STACK_SECTION("refreshRow3")
		refreshRow3 = false;
		if (!error) dev->WriteToLCD(row3->GetValue().c_str(),rowaddr3->GetValue());
		row3->ReadedValueFromDevice(row3->GetValue(),error);
		rowaddr3->ReadedValueFromDevice(rowaddr3->GetValue(),error);
	}
	if (refreshRow4 || newerror!=error)
	{
		STACK_SECTION("refreshRow4")
		refreshRow4 = false;
		if (!error) dev->WriteToLCD(row4->GetValue().c_str(),rowaddr4->GetValue());
		row4->ReadedValueFromDevice(row4->GetValue(),error);
		rowaddr4->ReadedValueFromDevice(rowaddr4->GetValue(),error);
	}
	if (refreshLihtEnabled || !lcdOn || newerror!=error)
	{
		STACK_SECTION("refreshLight")
		refreshLihtEnabled = false;
		dev->LightOn(lightEnabled->GetValue());
		lightEnabled->ReadedValueFromDevice(lightEnabled->GetValue(),error);
	}

	SetError(newerror);
	lcdOn = true;
}

HisDevLCD::~HisDevLCD()
{
}


