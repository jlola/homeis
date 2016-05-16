/*
 * LOWdevDS2408.cpp
 *
 *  Created on: 14. 10. 2015
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
#include "logger.h"
#include <LOW_devDS2408.h>

const std::string LOW_devDS2408::familyName = "DS2408";

LOW_devDS2408::LOW_devDS2408( LOW_netSegment &inSegment, const LOW_deviceID &inDevID) :
  LOW_device( inSegment, inDevID, familyCode)
{

}

int LOW_devDS2408::initHelper = initialize();
int LOW_devDS2408::initialize()
{
  LOW_deviceFactory::registerSpecificCtor( familyCode, &LOW_devDS2408::new_Instance);
  return 0;
}

bool LOW_devDS2408::getIsExternalPowered() const
{
  return true;
}

LOW_devDS2408::~LOW_devDS2408()
{

}

//=====================================================================================
//
// static methods
//

LOW_device* LOW_devDS2408::new_Instance( LOW_netSegment &inNetSegment, const LOW_deviceID &inDevID)
{
  return new LOW_devDS2408( inNetSegment, inDevID);
}

//=====================================================================================
//
// public methods
//
bool LOW_devDS2408::WritePIO(uint8_t piobyte)
{
	linkLock  lock( *this);

	cmd_MatchROM();
	vector<uint8_t> writedata;
	writedata.push_back(static_cast<uint8_t>(W1_F29_FUNC_CHANN_ACCESS_WRITE));
	uint8_t invertedpiobyte = ~piobyte;
	writedata.push_back(invertedpiobyte);
	writedata.push_back(piobyte);
	getLink().writeData(writedata);

	if (getLink().readDataByte() != 0xAA)
		return false;
	uint8_t newstate = getLink().readDataByte();

	if (newstate!=invertedpiobyte)
		return false;

	return true;
}

bool LOW_devDS2408::WriteControlStatus(uint8_t status)
{
	linkLock  lock( *this);
	vector<uint8_t> writedata;
	writedata.push_back(static_cast<uint8_t>(W1_F29_FUNC_WRITE_COND_SEARCH_REG));
	writedata.push_back(static_cast<uint8_t>(W1_F29_REG_CONTROL_AND_STATUS));
	writedata.push_back(static_cast<uint8_t>(0x00));
	writedata.push_back(static_cast<uint8_t>(status));
	cmd_MatchROM();
	getLink().writeData(writedata);

	return true;
}

bool LOW_devDS2408::ReadPIO(uint8_t & value)
{
	linkLock  lock( *this);

	cmd_MatchROM();
	getLink().writeData(static_cast<uint8_t>(W1_F29_FUNC_CHANN_ACCESS_READ));

	value = getLink().readDataByte();

	value =~value;

	return true;
}

bool LOW_devDS2408::WritePIO(uint8_t pin, bool value)
{
	linkLock  lock( *this);

	cmd_MatchROM();

	uint8_t oldvalue = 0x00;
	if (!ReadPIO(oldvalue))
		return false;

	uint8_t newvalue = value ? oldvalue | (0x01 << pin) : oldvalue & ~(0x01 << pin);

	if (!WritePIO(newvalue))
		return false;

	return true;
}

//int _read_reg(owCommand_t address, unsigned char* buf)
//
//{
//
//	owCommand_t wrbuf[3];
//
//
//	if (!buf)
//
//		return -1;
//
//
//
//	wrbuf[0] = W1_F29_FUNC_READ_PIO_REGS;
//
//	wrbuf[1] = address;
//
//	wrbuf[2] = 0;
//
//	w1_write_block(sl->master, wrbuf, 3);
//
//	*buf = w1_read_8(sl->master);
//
//	mutex_unlock(&sl->master->bus_mutex);
//
//	dev_dbg(&sl->dev, "mutex unlocked");
//
//	return 1;
//
//}
