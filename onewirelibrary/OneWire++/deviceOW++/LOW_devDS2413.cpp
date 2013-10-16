/*
 * LOW_devDS2413.cpp
 *
 *  Created on: Feb 4, 2013
 *      Author: root
 */

#include "LOW_devDS2413.h"

#include "LOW_platformMisc.h"
#include "LOW_helper_msglog.h"
#include "LOW_deviceFactory.h"
#include "LOW_netSegment.h"
#include "LOW_helper_crc.h"



//=====================================================================================
//
// static initializer
//

const std::string LOW_devDS2413::familyName = "DS2413";

int LOW_devDS2413::initHelper = initialize();
int LOW_devDS2413::initialize()
{
  LOW_deviceFactory::registerSpecificCtor( familyCode, &LOW_devDS2413::new_Instance);
  return 0;
}



//=====================================================================================
//
// static methods
//

LOW_device* LOW_devDS2413::new_Instance( LOW_netSegment &inNetSegment, const LOW_deviceID &inDevID)
{
  return new LOW_devDS2413( inNetSegment, inDevID);
}



//=====================================================================================
//
// constructors
//

LOW_devDS2413::LOW_devDS2413( LOW_netSegment &inSegment, const LOW_deviceID &inDevID) :
  LOW_device( inSegment, inDevID, familyCode)
{
  // reset latches and get basic information

}


LOW_devDS2413::~LOW_devDS2413()
{
}


//=====================================================================================
//
// methods
//

bool LOW_devDS2413::getIsExternalPowered() const
{
  return isExternalPowered;
}


bool LOW_devDS2413::IsValid(uint8_t val) const
{
	uint8_t high = (~val & 0xF0) >> 4;
	uint8_t low = val & 0x0F;
	if (low==high) return true;
	else return false;
}

void LOW_devDS2413::cmd_ReadStatus( uint8_t &outByte) const
{
	//this method is thread safe

	linkLock  lock( *this);

	// select device

	cmd_MatchROM();

	//write Channel read COMMAND

	getLink().writeData(static_cast<uint8_t>(ChannelRead_COMMAND));

	//read sample

	outByte = getLink().readDataByte();

	if (!IsValid(outByte))
		throw LOW_devDS2413::devDS2413_error("Read bad byte from DS2413", __FILE__, __LINE__);
	//reset bus

	getLink().resetBus();

}


void LOW_devDS2413::cmd_WritePIO(pioTransistor_t PIOA, pioTransistor_t PIOB) const
{

  //this method is thread safe

	linkLock  lock( *this);

	uint8_t inByte = 0xFF;

	//set bits in write register
	if (PIOA==pioTransistorOff) inByte &= ~PIOAOutLatchWriteBit;
	if (PIOB==pioTransistorOff) inByte &= ~PIOBOutLatchWriteBit;


  // select device

  cmd_MatchROM();

  //write Channel read COMMAND

  getLink().writeData(static_cast<uint8_t>(ChannelWrite_COMMAND));

  // write new pio state

  getLink().writeData(static_cast<uint8_t>(inByte));

  //invert reg

  inByte = ~inByte;

  //write inverted
  getLink().writeData(static_cast<uint8_t>(inByte));

  //read acnowlege byte
  if (getLink().readDataByte()!=Configmartion_COMMAND)
  	  throw devDS2413_error( "Error read confirmation command", __FILE__, __LINE__);

  //reset bus
  getLink().resetBus();
}



//=====================================================================================
//
// protected methods
//



