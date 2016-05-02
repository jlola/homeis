/*
 * LOWdevLCD.cpp
 *
 *  Created on: 24. 7. 2015
 *      Author: pc
 */

#include <string.h>
#include "LOW_platformMisc.h"
#include "LOW_helper_msglog.h"
#include "LOW_deviceFactory.h"
#include "LOW_netSegment.h"
#include "LOW_helper_crc.h"
#include "LOW_link.h"
#include "LOWdevLCD.h"
#include <unistd.h>
#include "PoppyDebugTools.h"
#include "logger.h"

const std::string LOW_devLCD::familyName = "LCD";


//=====================================================================================
//
// static initializer
//

int LOW_devLCD::initHelper = initialize();
int LOW_devLCD::initialize()
{
  LOW_deviceFactory::registerSpecificCtor( familyCode, &LOW_devLCD::new_Instance);
  return 0;
}

//=====================================================================================
//
// static methods
//

LOW_device* LOW_devLCD::new_Instance( LOW_netSegment &inNetSegment, const LOW_deviceID &inDevID)
{
  return new LOW_devLCD( inNetSegment, inDevID);
}



//=====================================================================================
//
// public constructors
//

LOW_devLCD::LOW_devLCD( LOW_netSegment &inSegment, const LOW_deviceID &inDevID) :
  LOW_device( inSegment, inDevID, familyCode)
{
	try
	{
	  if ( inSegment.getHasExternalPower() )
		isExternalPowered = true;
	  else
		isExternalPowered = false;

	  //cmd_RecallE2();
	}
	catch(...)
	{

	}
}

void LOW_devLCD::LCDOn()
{
	STACK
	cmd_MatchROM();
	getLink().writeData((uint8_t)LcdOff_COMMAND);
	sleep(1);
	cmd_MatchROM();
	getLink().writeData((uint8_t)LcdOn_COMMAND);
	sleep(1);
}

void LOW_devLCD::LightOn(bool enable)
{
	STACK
	cmd_MatchROM();
	if (enable)
		getLink().writeData((uint8_t)LightOn_COMMAND);
	else
		getLink().writeData((uint8_t)LightOff_COMMAND);
}

bool LOW_devLCD::IsConnected()
{
	STACK
	try
	{
		byteVec_t readData1(3);
		cmd_MatchROM();
		STACK_SECTION("getLink().writeData(static_cast<uint8_t>(ReadScratchpad_COMMAND));")
		getLink().writeData(static_cast<uint8_t>(ReadScratchpad_COMMAND));
		STACK_SECTION("getLink().readData(readData1, LOW_link::pullUp_NONE);")
		getLink().readData(readData1, LOW_link::pullUp_NONE);
		STACK_SECTION("for(size_t i=0;i<readData1.size()-1;i++)")
		for(size_t i=0;i<readData1.size()-1;i++)
		{
			if (readData1[i]==0xFF)
			{
				return false;
			}
		}
		return true;
	}
	catch(LOW_exception & ex)
	{
		string msg = "LOW_devLCD::IsConnected()" + ex.message;
		CLogger::Error(msg.c_str());
		getLink().resetLinkAdapter();
		return false;
	}

}

bool LOW_devLCD::ReadCounters(byteVec_t & counters)
{
	try
	{
		cmd_MatchROM();
		getLink().writeData(static_cast<uint8_t>(CopyGPIOcountersToScratchpad),LOW_link::pullUp_NONE);
		cmd_MatchROM();
		getLink().writeData(static_cast<uint8_t>(ReadScratchpad_COMMAND),LOW_link::pullUp_NONE);
		getLink().readData(counters);
		return true;
	}
	catch(LOW_exception & ex)
	{
		CLogger::Error(ex.message.c_str());
		return false;
	}
}

bool LOW_devLCD::ReadInputs(uint8_t & inputs)
{
	try
	{
		cmd_MatchROM();
		getLink().writeData(static_cast<uint8_t>(CopyGPIOInputStatesToScratchpad),LOW_link::pullUp_NONE);
		cmd_MatchROM();
		getLink().writeData(static_cast<uint8_t>(ReadScratchpad_COMMAND),LOW_link::pullUp_NONE);
		inputs = getLink().readDataByte();
		return true;
	}
	catch(LOW_exception & ex)
	{
		CLogger::Error(ex.message.c_str());
		return false;
	}
}

bool LOW_devLCD::WriteToLCD(const char* text, uint8_t rowAddress)
{
	STACK
	try
	{
		//CLogger::Info("Start write to LCD");
		int length = strlen(text);
		if (length>16) length = 16;
		byteVec_t outData;

		outData.push_back(static_cast<uint8_t>(WriteScratchpad_COMMAND));
		outData.push_back(rowAddress);
		for(int i=0;i<length;i++)
		{
			outData.push_back(text[i]);
		}
		cmd_MatchROM();
		getLink().writeData(outData, LOW_link::pullUp_NONE);

		cmd_MatchROM();
		getLink().writeData(static_cast<uint8_t>(CopyScratchpadToLCD_COMMAND),LOW_link::pullUp_NONE);

		//CLogger::Info("Stop write to LCD");
		return true;
	}
	catch(LOW_exception & ex)
	{
		return false;
	}
}


LOW_devLCD::~LOW_devLCD()
{
}
