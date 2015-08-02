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
#include "LOWdevLCD.h"


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


bool LOW_devLCD::WriteToLCD(const char* text, uint8_t rowAddress)
{
	linkLock  lock( *this);

	try
	{
		cmd_MatchROM();

		int length = strlen(text);

		byteVec_t outData;

		outData.push_back(static_cast<uint8_t>( WriteScratchpad_COMMAND));

		outData.push_back(rowAddress);

		for(int i=0;i<length;i++)
		{
			outData.push_back(text[i]);
		}

		getLink().writeData(outData);

		if ( getLink().resetBus() == false )
			throw noDevice_error( "Reset indicated no devices", __FILE__, __LINE__);

		cmd_MatchROM();

		getLink().writeData((uint8_t)CopyScratchpadToLCD_COMMAND);


		if ( getLink().resetBus() == false )
				throw noDevice_error( "Reset indicated no devices", __FILE__, __LINE__);

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
