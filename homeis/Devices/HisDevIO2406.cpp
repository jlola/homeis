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
	HisDevDallas::HisDevDallas(node, pdev)
{
	valueAInput = NULL;
	valueAOutput = NULL;

	valueBInput = NULL;
	valueBOutput = NULL;
	dev = pdev;
}

HisDevIO2406::HisDevIO2406(LOW_devDS2406* pdev) :
	HisDevDallas::HisDevDallas(pdev)
{
	dev = pdev;
	CreateDataPoints();
}

void HisDevIO2406::CreateDataPoints()
{
	std::string strid = dev->getID().getRomIDString();
	WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevIO2406, &HisDevIO2406::WriteToDevice>(this);
	//create data points
	//xmlNodePtr valuenode = NULL;//GetOrCreateNode(PINNO_INPUTA,GetNodePtr());
	valueAInput = new HisDevValue<bool>(strid, EHisDevDirection::Read, EDataType::Bool, PINNO_INPUTA);
	valueAInput->delegateWrite = delegate;
	valueAInput->Load();
	Add(valueAInput);

	//valuenode = GetOrCreateNode(PINNO_INPUTB,GetNodePtr());
	valueBInput = new HisDevValue<bool>(strid, EHisDevDirection::Read, EDataType::Bool, PINNO_INPUTB);
	valueBInput->delegateWrite = delegate;
	valueBInput->Load();
	Add(valueBInput);

	//valuenode = GetOrCreateNode(PINNO_OUTPUTA,GetNodePtr());
	valueAOutput = new HisDevValue<bool>(strid,EHisDevDirection::ReadWrite,EDataType::Bool,PINNO_OUTPUTA);
	valueAOutput->delegateWrite = delegate;
	valueAOutput->Load();
	Add(valueAOutput);

	//valuenode = GetOrCreateNode(PINNO_OUTPUTB,GetNodePtr());
	valueBOutput = new HisDevValue<bool>(strid,EHisDevDirection::ReadWrite,EDataType::Bool,PINNO_OUTPUTB);
	valueBOutput->delegateWrite = delegate;
	valueBOutput->Load();
	Add(valueBOutput);
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
		}
	}
}

void HisDevIO2406::WriteToDevice(ValueChangedEventArgs args)
{
	switch(args.GetValue()->GetPinNumber())
	{
		case PINNO_INPUTA:
		case PINNO_INPUTB:
			break;
		case PINNO_OUTPUTA:
		case PINNO_OUTPUTB:
			//dev->cmd_WritePIO(	valueAOutput->GetValue() ? LOW_devDS2413::pioTransistorOn : LOW_devDS2413::pioTransistorOff,
			//					valueBOutput->GetValue() ? LOW_devDS2413::pioTransistorOn : LOW_devDS2413::pioTransistorOff);
			break;
	}
}

void HisDevIO2406::DoInternalRefresh()
{
	LOW_devDS2406::cmd_ChannelAccess *chAccess = NULL;


	bool devError = false;
	try
	{
		chAccess = new LOW_devDS2406::cmd_ChannelAccess(
				*dev,
				LOW_devDS2406::cmd_ChannelAccess::CRC_disable,
				LOW_devDS2406::chanBothSelect,
				LOW_devDS2406::cmd_ChannelAccess::asyncInterleaveMode,
				LOW_devDS2406::cmd_ChannelAccess::noToggleMode,
				LOW_devDS2406::cmd_ChannelAccess::readMode,
				LOW_devDS2406::cmd_ChannelAccess::noResetLatches);
		//dev->cmd_ReadStatus(status);
	}
	catch(...)
	{
		devError = true;
	}

	LOW_devDS2406::cmd_ChannelAccess::channelInfo_t chanInfo = chAccess->getChannelInfo();
	delete chAccess;

	bool ioatemp = chanInfo.sensedLevel_pioA;
	bool iobtemp = chanInfo.sensedLevel_pioB;


	valueAInput->SetValueFromDevice(ioatemp,devError);
	valueBInput->SetValueFromDevice(iobtemp,devError);

	//valueAOutput->SetValueFromDevice(latchA,devError);
	//valueBOutput->SetValueFromDevice(latchB,devError);

	DevError = devError;
}

HisDevIO2406::~HisDevIO2406()
{
}
