/*
 * HisDevIO2406.cpp
 *
 *  Created on: Nov 30, 2013
 *      Author: root
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

#include "HisDevIO2406.h"

HisDevIO2406::HisDevIO2406(xmlNodePtr node, LOW_devDS2406* pdev) :
	HisDevDallas::HisDevDallas(node, pdev),outputchanged(false)
{
	valueALatch = NULL;
	valueBLatch = NULL;

	valueAInput = NULL;
	valueAOutput = NULL;

	valueBInput = NULL;
	valueBOutput = NULL;
	dev = pdev;
}

HisDevIO2406::HisDevIO2406(LOW_devDS2406* pdev) :
	HisDevDallas::HisDevDallas(pdev), outputchanged(false)
{
	dev = pdev;

	valueALatch = NULL;
	valueBLatch = NULL;

	valueAInput = NULL;
	valueAOutput = NULL;

	valueBInput = NULL;
	valueBOutput = NULL;

	CreateDataPoints();
	Init();
}

const xmlChar* HisDevIO2406::GetNodeNameInternal()
{
	return (const xmlChar *)"DS2406";
}

void HisDevIO2406::CreateDataPoints()
{
	std::string strid = dev->getID().getRomIDString();
	WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevIO2406, &HisDevIO2406::WriteToDevice>(this);
	//create data points
	//xmlNodePtr valuenode = NULL;//GetOrCreateNode(PINNO_INPUTA,GetNodePtr());
	valueAInput = new HisDevValue<bool>(strid, EHisDevDirection::Read, EDataType::Bool, PINNO_INPUTA,false);
	valueAInput->SetName("PIOAInput");
	valueAInput->delegateWrite = delegate;
	valueAInput->Load();
	Add(valueAInput);

	//valuenode = GetOrCreateNode(PINNO_INPUTB,GetNodePtr());
	valueBInput = new HisDevValue<bool>(strid, EHisDevDirection::Read, EDataType::Bool, PINNO_INPUTB,false);
	valueBInput->delegateWrite = delegate;
	valueBInput->SetName("PIOBInput");
	valueBInput->Load();
	Add(valueBInput);

	//valuenode = GetOrCreateNode(PINNO_OUTPUTA,GetNodePtr());
	valueAOutput = new HisDevValue<bool>(strid,EHisDevDirection::ReadWrite,EDataType::Bool,PINNO_OUTPUTA,false);
	valueAOutput->delegateWrite = delegate;
	valueAOutput->SetName("PIOAOutput");
	valueAOutput->Load();
	Add(valueAOutput);

	//valuenode = GetOrCreateNode(PINNO_OUTPUTB,GetNodePtr());
	valueBOutput = new HisDevValue<bool>(strid,EHisDevDirection::ReadWrite,EDataType::Bool,PINNO_OUTPUTB,false);
	valueBOutput->delegateWrite = delegate;
	valueBOutput->SetName("PIOBOutput");
	valueBOutput->Load();
	Add(valueBOutput);

	valueALatch = new HisDevValue<bool>(strid,EHisDevDirection::ReadWrite,EDataType::Bool,PINNO_LATCHA,false);
	valueALatch->delegateWrite = delegate;
	valueALatch->SetName("PIOALatch");
	valueALatch->Load();
	Add(valueALatch);

	valueBLatch = new HisDevValue<bool>(strid,EHisDevDirection::ReadWrite,EDataType::Bool,PINNO_LATCHB,false);
	valueBLatch->delegateWrite = delegate;
	valueBLatch->SetName("PIOBLatch");
	valueBLatch->Load();
	Add(valueBLatch);
}

void HisDevIO2406::DoInternalLoad(xmlNodePtr & node)
{
	HisDevDallas::DoInternalLoad(node);
	WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevIO2406, &HisDevIO2406::WriteToDevice>(this);

	vector<HisDevValue<bool>*> values = GetItems<HisDevValue<bool>>();

	for(size_t i=0;i<values.size();i++)
	{
		HisDevValue<bool> *value = values[i];
		value->delegateWrite = delegate;
		switch(value->GetPinNumber())
		{
			case PINNO_INPUTA:
				valueAInput = value;
				break;
			case PINNO_INPUTB:
				valueBInput = value;
				break;
			case PINNO_OUTPUTA:
				valueAOutput = value;
				break;
			case PINNO_OUTPUTB:
				valueBOutput = value;
				break;
			case PINNO_LATCHA:
				valueALatch = value;
				break;
			case PINNO_LATCHB:
				valueBLatch = value;
				break;
		}
	}
	SetScanPeriod(30000);
	Init();
}

void HisDevIO2406::WriteToDevice(ValueChangedEventArgs args)
{
	switch(args.GetValue()->GetPinNumber())
	{
		case PINNO_OUTPUTA:
		case PINNO_OUTPUTB:
			//dev->cmd_WritePIO(	valueAOutput->GetValue() ? LOW_devDS2413::pioTransistorOn : LOW_devDS2413::pioTransistorOff,
			//					valueBOutput->GetValue() ? LOW_devDS2413::pioTransistorOn : LOW_devDS2413::pioTransistorOff);
			outputchanged = true;
			SetChanged();
			NeedRefresh();
			break;
	}
}

void HisDevIO2406::Init()
{

	LOW_devDS2406::pioTransistor_t boolnewAValue = valueAOutput->GetValue() ? LOW_devDS2406::pioTransistor_t::pioTransistorOff :
						LOW_devDS2406::pioTransistor_t::pioTransistorOn;
	LOW_devDS2406::pioTransistor_t boolnewBValue = valueBOutput->GetValue() ? LOW_devDS2406::pioTransistor_t::pioTransistorOff :
						LOW_devDS2406::pioTransistor_t::pioTransistorOn;

	try
	{
		LOW_devDS2406::channelInfo_t chInfo = dev->getChannel(LOW_devDS2406::CRC_disable,
				LOW_devDS2406::chanSelect_t::chanASelect,
				LOW_devDS2406::asyncInterleaveMode,
				LOW_devDS2406::noToggleMode,
				LOW_devDS2406::readMode,
				LOW_devDS2406::resetLatches);

		dev->cmd_WriteStatus(LOW_devDS2406::chanSelect_t::chanASelect,
				LOW_devDS2406::sourceSelect_t::curStatusSelect,
				chInfo.sensedLevel_pioA ? LOW_devDS2406::activePolarity_t::activeLow : LOW_devDS2406::activePolarity_t::activeHigh,
				boolnewAValue,
				boolnewBValue);

		valueALatch->ReadedValueFromDevice(chInfo.activityLatch_pioA,false);
		valueBLatch->ReadedValueFromDevice(chInfo.activityLatch_pioB,false);
	}
	catch(LOW_exception & ex)
	{

	}
}

void HisDevIO2406::DoInternalRefresh(bool alarm)
{
	LOW_devDS2406::channelInfo_t chInfo;

	LOW_devDS2406::statusRegister_t status;
	bool newError = GetError();

	LOW_devDS2406::pioTransistor_t boolnewAValue = valueAOutput->GetValue() ? LOW_devDS2406::pioTransistor_t::pioTransistorOff :
					LOW_devDS2406::pioTransistor_t::pioTransistorOn;
	LOW_devDS2406::pioTransistor_t boolnewBValue = valueBOutput->GetValue() ? LOW_devDS2406::pioTransistor_t::pioTransistorOff :
					LOW_devDS2406::pioTransistor_t::pioTransistorOn;

	try
	{
		//nactu channel
		//CLogger::Info("DS2406 - Before getChannel");
		chInfo = dev->getChannel(LOW_devDS2406::CRCtype_t::CRC_disable,
				LOW_devDS2406::chanSelect_t::chanASelect,
				LOW_devDS2406::asyncInterleaveMode,
				LOW_devDS2406::noToggleMode,
				LOW_devDS2406::readMode,
				LOW_devDS2406::noResetLatches);
		//CLogger::Info("DS2406 - After getChannel");
		//nactu status
		//CLogger::Info("DS2406 - Before read status");
		dev->cmd_ReadStatus(&status);
		//CLogger::Info("DS2406 - After read status");
		//aktualizuju latch
		if (chInfo.activityLatch_pioA || newError != GetError())
				valueALatch->ReadedValueFromDevice(chInfo.activityLatch_pioA,newError);
		if (chInfo.activityLatch_pioB || newError != GetError())
			valueBLatch->ReadedValueFromDevice(chInfo.activityLatch_pioB,newError);

		newError = false;

		//change polarity
		LOW_devDS2406::activePolarity_t newPolarity = chInfo.sensedLevel_pioA ? LOW_devDS2406::activePolarity_t::activeLow : LOW_devDS2406::activePolarity_t::activeHigh;
		if (newPolarity != status.activePolarity)
		{
			CLogger::Info("DS2406 - cmd_WriteStatus change polarity");
			dev->cmd_WriteStatus(LOW_devDS2406::chanSelect_t::chanASelect,
				LOW_devDS2406::sourceSelect_t::curStatusSelect,
				chInfo.sensedLevel_pioA ? LOW_devDS2406::activePolarity_t::activeLow : LOW_devDS2406::activePolarity_t::activeHigh,
				boolnewAValue,
				boolnewBValue);
			//CLogger::Info("DS2406 - After cmd_WriteStatus change polarity");
		}

		//musim resetovat latch pokud byl nastaveny
		if (chInfo.activityLatch_pioA || chInfo.activityLatch_pioB)
		{
			CLogger::Info("DS2406 - getChannel Reset latch");
			chInfo = dev->getChannel(LOW_devDS2406::CRC_disable,
				LOW_devDS2406::chanSelect_t::chanASelect,
				LOW_devDS2406::interleaveMode_t::asyncInterleaveMode,
				LOW_devDS2406::noToggleMode,
				LOW_devDS2406::readMode,
				LOW_devDS2406::resetLatches);
			//CLogger::Info("DS2406 - after getChannel Reset latch");
		}
	}
	catch(LOW_exception & ex)
	{
		const char* msg = ex.message.c_str();
		CLogger::Info(msg);
		newError = true;
	}

	valueAInput->ReadedValueFromDevice(chInfo.sensedLevel_pioA,newError);
	valueBInput->ReadedValueFromDevice(chInfo.sensedLevel_pioB,newError);

	valueAOutput->ReadedValueFromDevice(chInfo.channelFFQ_pioA,newError);
	valueBOutput->ReadedValueFromDevice(chInfo.channelFFQ_pioB,newError);

	if (newError != GetError())
		valueALatch->ReadedValueFromDevice(chInfo.activityLatch_pioA,newError);
	if (newError != GetError())
		valueBLatch->ReadedValueFromDevice(chInfo.activityLatch_pioB,newError);

	SetError(newError);
}

HisDevIO2406::~HisDevIO2406()
{
}
