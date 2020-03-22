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
#include <unistd.h>
using namespace std;
#include "crc.h"
#include "BlockingQueue.h"
#include "PoppyDebugTools.h"
#include "StringBuilder.h"

string ModifiedMdbus::DriverName = "modbus";

ModifiedMdbus::ModifiedMdbus(SSerPortConfig config) :
		logger(CLogger::GetLogger()),
		config(config),
		serialPort(config.Port,BaudRate::B_115200)
{
	modbusmutex = HisLock::CreateMutex();
	receiving = false;
	events = new BlockingQueue<int>();
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
	getholdingsBuffer.clear();
	vector<uint8_t> vdata;
	for(int i=0;i<len;i++)
		vdata.push_back(data[i]);

	string strdata = StringBuilder::join(vdata,",");
	logger.Trace("Sended: %s",strdata.c_str());
	serialPort.Write(data,len);
	event.Reset();
	auto signaled = event.Wait(timeoutMs);
	return signaled;
}

void ModifiedMdbus::AddConsumer(IDeviceEventConsumer* consumer)
{
	if (consumer!=NULL)
	{
		consumers.push_back(consumer);
	}
}

void ModifiedMdbus::Stop()
{
	events->RequestShutdown();
}

int ModifiedMdbus::WaitForAddress()
{
	int addr;

	if (events->Pop(addr))
	{
		return addr;
	}
	return -1;
}

void ModifiedMdbus::OnData(std::vector<uint8_t> data)
{
	STACK

	receiving = true;
	bool isvalid;

	string strdata = StringBuilder::join(data,",");
	logger.Trace("Received: %s",strdata.c_str());

	for (size_t i=0;i<data.size();i++)
	{
		buffer.push_back(data[i]);
		if (DataCompleted(isvalid))
		{
			if (event.IsWaiting())
			{
				for(size_t c=0;c<buffer.size();c++)
				{
					getholdingsBuffer.push_back(buffer[c]);
				}
				buffer.clear();
				event.Signal();
			}
			else
			{
				if (!checkFrameCrc(&buffer[0],buffer.size()))
				{
					logger.Error("Received wrong data first byte: %d",buffer[0]);
				}
				else
				{
					//fire event on consumers
					for(size_t i=0;i<consumers.size();i++)
						consumers[i]->FireEvent(buffer[0]);
				}
			}
		}
		else if (!isvalid)
		{
			buffer.clear();
		}
	}

	if (!event.IsWaiting())
		buffer.clear();


	receiving = false;
}

bool ModifiedMdbus::DataCompleted(bool & valid)
{
	valid = true;
	if (buffer.size() > OFFSET_FUNC)
	{
		if (buffer[OFFSET_FUNC]==FUNC_GETHOLDINGS)
		{
								/*addr + func + countValue+count+2crc*/
			const size_t totalCount = 3 + buffer[OFFSET_COUNT] + 2;
			if (buffer.size() > totalCount) valid = false;
			return buffer.size() == totalCount;
		}
		else if (buffer[OFFSET_FUNC]==FUNC_SETHOLDING)
		{
			const size_t totalCount = 8;/*addr + func + firsthi + firstlo+valueHI+valueLow+2crc*/
			if (buffer.size() > totalCount) valid = false;
			return buffer.size() == totalCount;
		}
		else
		{
			valid = false;
		}
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

	while(receiving)
		usleep(1000);

	const int func = 0x06;
	int timeoutMs = 50;

	ModbusRequest request;
	request.Address = address;
	request.Function = func;
	request.StartingAddress = bswap_16(offset);
	request.Count = bswap_16(val);
	request.CRC = crc.calc((uint8_t*)&request, sizeof(request)-2);
	string errormsg;

	logger.Trace("SetHolding: %d, offset: %d, value: %d, timeout: %d",address, offset, val, timeoutMs);

	if (Send((uint8_t*)&request, sizeof(request),timeoutMs))
	{
		ModbusRequest response = *(ModbusRequest*)&getholdingsBuffer[0];
		if (response.Address == request.Address)
		{
			if (response.CRC == request.CRC)
				return true;
			else
				errormsg = "wrong CRC";
		}
		else
			errormsg = "wrong response address";
	}
	else
		errormsg = "timeout";

	logger.Error("Error setHolding: %d, offset: %d, value: %d, timeoutMs: %d error: %s",
			address, offset, val, timeoutMs, errormsg.c_str());

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

	while(receiving)
		usleep(1000);

	ModbusRequest request;
	request.Address = address;
	request.Function = FUNC_GETHOLDINGS;
	request.StartingAddress = bswap_16(offset);
	request.Count = bswap_16(count);
	request.CRC = crc.calc((uint8_t*)&request, sizeof(request)-2);
	string errormsg;

	logger.Trace("getHoldings: %d, offest: %d, count: %d, timeoutMs: %d",
				address, offset, count, timeoutMs);

	if (Send((uint8_t*)&request, sizeof(request),timeoutMs))
	{
		//check buffer
		if (getholdingsBuffer[0]!=address)
			errormsg = StringBuilder::Format("wrong address Sended: %d, Received: %d",address,getholdingsBuffer[0]);
		else if (getholdingsBuffer[1]!=FUNC_GETHOLDINGS)
			errormsg = StringBuilder::Format("Incorrect function. Request:%d, Response: %d",FUNC_GETHOLDINGS, getholdingsBuffer[1]);
		else if (getholdingsBuffer[2]!=count*2)
			errormsg =StringBuilder::Format("Incorrect count of bytes request:%d, response:%d",count*2,getholdingsBuffer[2]);
		else if (!checkFrameCrc(&getholdingsBuffer[0],getholdingsBuffer.size()))
			errormsg = "incorrect crc";
		else
		{
			memcpy(target,&getholdingsBuffer[3],count*2);
			for (int i=0;i<count;i++)
				target[i] = bswap_16(target[i]);
			return true;
		}
	}
	else
	{
		if (address!=1 ) {
			errormsg = "timeout";
		}
	}

	logger.Error("Error getHoldings: %d, offest: %d, count: %d, timeoutMs: %d, error: %s",
			address, offset, count, timeoutMs, errormsg.c_str());

	return false;
}




ModifiedMdbus::~ModifiedMdbus() {

}

