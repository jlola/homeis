/*
 * ModifiedMdbus.cpp
 *
 *  Created on: 10. 10. 2019
 *      Author: pc
 */

#include <ModifiedMdbus.h>
#include <stdint.h>
#include "ModbusRequest.h"
#include "logger.h"
#include <byteswap.h>
using namespace std;
#include "crc.h"

string ModifiedMdbus::DriverName = "modbus";

ModifiedMdbus::ModifiedMdbus(SSerPortConfig config) :
		logger(CLogger::GetLogger()),
		config(config),
		serialPort(config.Port,BaudRate::B_115200)
{
	modbusmutex = HisLock::CreateMutex();
}

bool ModifiedMdbus::Init()
{
	serialPort.SetTimeout1(-1);
	this->serialPort.SetReceiver(this);
	serialPort.Open();
	return true;
}

string ModifiedMdbus::GetName()
{
	return config.Name;
}
string ModifiedMdbus::GetPort()
{
	return config.Port;
}

string ModifiedMdbus::GetDriverName()
{
	return DriverName;
}

bool ModifiedMdbus::Send(uint8_t* data,uint16_t len,const int timeoutMs)
{
	buffer.clear();
	serialPort.Write(data,len);
	event.Reset();
	auto signaled = event.Wait(timeoutMs);
	return signaled;
}

void ModifiedMdbus::OnData(std::vector<uint8_t> data)
{
	for (size_t i=0;i<data.size();i++)
		buffer.push_back(data[i]);

	if (DataCompleted())
	{
		event.Signal();
	}
}

bool ModifiedMdbus::DataCompleted()
{
	if (buffer[OFFSET_FUNC]==FUNC_GETHOLDINGS)
	{
								/*addr + func + countValue+count+2crc*/
		const size_t totalCount = 3 + buffer[OFFSET_COUNT] + 2;
		return buffer.size() == totalCount;
	}
	if (buffer[OFFSET_FUNC]==FUNC_SETHOLDING)
	{
		const size_t totalCount = 8;/*addr + func + firsthi + firstlo+valueHI+valueLow+2crc*/
		return buffer.size() == totalCount;
	}
	return false;
}

bool  ModifiedMdbus::setBitInput(uint16_t address,uint16_t index, uint16_t state)
{
	throw "Not implemented exception";
}

bool  ModifiedMdbus::getBitInput(uint16_t address,uint16_t index,bool & bit)
{
	throw "Not implemented exception";
}

bool ModifiedMdbus::getShortInput(uint16_t address,uint16_t index,uint16_t & input)
{
	throw "Not implemented exception";
}


bool ModifiedMdbus::setCoil(uint16_t address,uint16_t index, uint16_t state)
{
	throw "Not implemented exception";
}

bool ModifiedMdbus::getCoil(uint16_t address,uint16_t index,bool & coil)
{
	throw "Not implemented exception";
}

bool ModifiedMdbus::setHolding(uint16_t address,uint16_t offset, uint16_t val)
{
	HisLock lock(modbusmutex);

	const int func = 0x06;
	int timeoutMs = 50;

	ModbusRequest request;
	request.Address = address;
	request.Function = func;
	request.StartingAddress = bswap_16(offset);
	request.Count = bswap_16(val);
	request.CRC = crc.calc((uint8_t*)&request, sizeof(request)-2);


	if (Send((uint8_t*)&request, sizeof(request),timeoutMs))
	{
		ModbusRequest response = *(ModbusRequest*)&buffer[0];
		if (response.Address == request.Address &&
			response.CRC == request.CRC)
			return true;
	}
	else
	{
		logger.Error("Error getHolding: %d, error: %s",address,"timeout");
	}
	return false;
}

bool ModifiedMdbus::getHolding(uint16_t address,uint16_t index,uint16_t* holding)
{
	return getHoldings(address,index,1,holding,50);
}
bool ModifiedMdbus::getHolding(uint16_t address,uint16_t index,uint16_t* holding, uint32_t timeOut)
{
	return getHoldings(address,index,1,holding,timeOut);
}
bool ModifiedMdbus::getHoldings(uint16_t address,uint16_t offset,uint16_t count,uint16_t* target)
{
	return getHoldings(address,offset,count,target,50);
}

bool ModifiedMdbus::checkFrameCrc(const uint8_t *p, uint8_t length) {
	// Enable CRC clock
	uint32_t crc0 = crc.calc(p, length - 2);
	uint32_t crc1 = p[length - 2] | (p[length - 1] << 8);
	return crc0 == crc1;
	return true;
}

bool ModifiedMdbus::getHoldings(uint16_t address,uint16_t offset,uint16_t count,uint16_t* target, uint32_t timeoutMs)
{
	HisLock lock(modbusmutex);
	ModbusRequest request;
	request.Address = address;
	request.Function = FUNC_GETHOLDINGS;
	request.StartingAddress = bswap_16(offset);
	request.Count = bswap_16(count);
	request.CRC = crc.calc((uint8_t*)&request, sizeof(request)-2);


	if (Send((uint8_t*)&request, sizeof(request),timeoutMs))
	{
		//check buffer
		if (buffer[0]!=address)
			logger.Error("Error getHolding: %d, error: %s",address,"wrong address");
		else if (buffer[1]!=FUNC_GETHOLDINGS)
			logger.Error("Error getHolding: %d, error: %s",address,"incorrect function");
		else if (buffer[2]!=count*2)
			logger.Error("Error getHolding: %d, error: incorrect count of bytes request:%d, response:%d",address,count*2,buffer[2]);
		else if (!checkFrameCrc(&buffer[0],buffer.size()))
			logger.Error("Error getHolding: %d, error: %s",address,"incorrect crc");
		else
		{
			memcpy(target,&buffer[3],count*2);
			for (int i=0;i<count;i++)
				target[i] = bswap_16(target[i]);
			return true;
		}
	}
	else
	{
		if (address!=1 ) {
			logger.Error("Error getHolding: %d, error: %s",address,"timeout");
		}
	}
	return false;
}




ModifiedMdbus::~ModifiedMdbus() {

}

