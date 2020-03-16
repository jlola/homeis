/*
 * DS18B20Tests.cpp
 *
 *  Created on: 4. 12. 2017
 *      Author: pc
 */
#ifdef HOMEISTEST

#include "DS18B20Tests.h"
#include "fakeit.hpp"

namespace AF {

DS18B20Tests::DS18B20Tests() {

}

TEST_F(DS18B20Tests,ConversionTest)
{
//	ASSERT_EQ(25.0625,HisDevTemp18B20::GetDouble(0x0191));
//	ASSERT_EQ(0.0,HisDevTemp18B20::GetDouble(0x0000));
//	ASSERT_EQ(-0.5,HisDevTemp18B20::GetDouble(0xFFF8));
//	ASSERT_EQ(-10.125,HisDevTemp18B20::GetDouble(0xFF5E));
//	ASSERT_EQ(-25.0625,HisDevTemp18B20::GetDouble(0xFE6F));
//	ASSERT_EQ(-55,HisDevTemp18B20::GetDouble(0xFC90));
}

//TEST_F(DS18B20Tests,ConvertToString)
//{
//	char buffer[100];
//	double value = -1.2;
//	sprintf(buffer,"%1.2f",value);
//	string stringdouble = buffer;
//	ASSERT_EQ(stringdouble,"-1.20");
//}

TEST_F(DS18B20Tests,ScratchPadTest)
{
//	LOW_devDS1820::scratchpadDS1820_t  scratchpad;
//	scratchpad.tempMSB = 0b11111111;
//	scratchpad.tempLSB = 0b11111000;
//	int16_t intPart    = (scratchpad.tempMSB<<8) | scratchpad.tempLSB;
//	ASSERT_EQ(-0.5,HisDevTemp18B20::GetDouble(intPart));
}

DS18B20Tests::~DS18B20Tests() {

}

}

#endif
