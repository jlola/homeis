/*
 * Modbus.cpp
 *
 *  Created on: 18. 10. 2016
 *      Author: pc
 */

#include <stdio.h>
#include <cerrno>
#include "Logger.h"
#include <Modbus/ModbusWrapper.h>

Modbus::Modbus(SSerPortConfig config)
: modbusmutex(NULL),ctx(NULL)
{
	modbusmutex = HisLock::CreateMutex();
	this->config = config;
}

string Modbus::GetPort()
{
	return config.Port;
}

string Modbus::GetName()
{
	return config.Name;
}

bool Modbus::Init()
{
	ctx = modbus_new_rtu(config.Port.c_str(),115200,'N',8,1);
	if (ctx == NULL) {
		fprintf(stderr, "Unable to create the libmodbus context\n");
		CLogger::Error("Unable to create the libmodbus context %s\n",config.Port.c_str());
		return false;
	}
	if (modbus_connect(ctx) == -1) {
	    fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
	    modbus_free(ctx);
	    return false;
	}

	struct timeval timeout;

	modbus_get_byte_timeout(ctx, &timeout);

	timeout.tv_sec = 0;
	timeout.tv_usec = 10000;
	modbus_set_byte_timeout(ctx, &timeout);

	timeout.tv_sec = 0;
	timeout.tv_usec = 20000;
	modbus_set_response_timeout(ctx, &timeout);


	return true;
}

int Modbus::SetSlave(uint16_t address)
{
	int status = modbus_set_slave(ctx,address);
	if (status == -1) {
		fprintf(stderr, "Set slave: %s\n", modbus_strerror(errno));
		CLogger::Error("Error set slave: %d, error: %s",address,modbus_strerror(errno));
	}
	return status;
}

bool Modbus::setBitInput(uint16_t address,uint16_t index, uint16_t state) {
	modbusmutex->lock();
	int status = SetSlave(address);
	if (status == 0)
	{
		status = modbus_write_bit(ctx,index,state?TRUE:FALSE);
		if (status < 0) {
			fprintf(stderr, "Error setBitInput: %s\n", modbus_strerror(errno));
			CLogger::Error("Error setBitInput: %d, error: %s",address,modbus_strerror(errno));
		}
	}
	modbusmutex->unlock();
	return status < 0 ? false : true;
}
bool Modbus::getBitInput(uint16_t address,uint16_t index,bool & bit){
	modbusmutex->lock();
	int status = SetSlave(address);
	if (status==0)
	{
		uint8_t* dest = new uint8_t[index+1];
		status = modbus_read_input_bits(ctx,index,1,dest);
		delete[] dest;
		if (status == -1) {
			fprintf(stderr, "Error getBitInput: %s\n", modbus_strerror(errno));
			CLogger::Error("Error getBitInput: %d, error: %s",address,modbus_strerror(errno));
		}
	}
	modbusmutex->unlock();
	return status < 0 ? false : true;
}

bool Modbus::getShortInput(uint16_t address,uint16_t index,uint16_t & input){
	int status = SetSlave(address);
	if (status == 0)
	{
		uint16_t* dest = new uint16_t[index+1];
		status = modbus_read_input_registers(ctx,index,1,dest);
		delete[] dest;
		if (status == -1) {
			fprintf(stderr, " Error getShortInput: %s\n", modbus_strerror(errno));
			CLogger::Error("Error getShortInput: %d, error: %s",address,modbus_strerror(errno));
		}
	}
	modbusmutex->unlock();
	return status < 0 ? false : true;
}

bool Modbus::setCoil(uint16_t address,uint16_t index, uint16_t state){
	modbusmutex->lock();
	int status = SetSlave(address);
	if (status==0)
	{
		status = modbus_write_bit(ctx,index,state?TRUE:FALSE);
		if (status == -1) {
			fprintf(stderr, "Error setCoil: %s\n", modbus_strerror(errno));
			CLogger::Error("Error setCoil: %d, error: %s",address,modbus_strerror(errno));
		}
	}

	modbusmutex->unlock();
	return status < 0 ? false : true;
}
bool Modbus::getCoil(uint16_t address,uint16_t index,bool & coil){
	modbusmutex->lock();
	int status = SetSlave(address);
	if (status==0)
	{
		uint8_t* dest = new uint8_t[index+1];
		status = modbus_read_input_bits(ctx,index,1,dest);
		delete[] dest;
		if (status == -1) {
			fprintf(stderr, "Error getCoil: %s\n", modbus_strerror(errno));
			CLogger::Error("Error getCoil: %d, error: %s",address,modbus_strerror(errno));
		}
	}
	modbusmutex->unlock();
	return status < 0 ? false : true;
}

bool Modbus::setHolding(uint16_t address,uint16_t index, uint16_t val){
	modbusmutex->lock();
	int status = SetSlave(address);
	if (status==0)
	{
		status = modbus_write_register(ctx,index,val);
		if (status == -1) {
			fprintf(stderr, "Error setHolding: %s\n", modbus_strerror(errno));
			CLogger::Error("Error setHolding: %d, error: %s",address,modbus_strerror(errno));
		}
	}
	modbusmutex->unlock();
	return status < 0 ? false : true;
}
bool Modbus::getHolding(uint16_t address,uint16_t index,uint16_t* holding){
	modbusmutex->lock();
	int status = SetSlave(address);
	if (status==0)
	{
		uint16_t dest[2];
		status = modbus_read_registers(ctx,index,sizeof(dest)/2,dest);
		if (status == -1) {
			fprintf(stderr, "Error getHolding: %s\n", modbus_strerror(errno));
			CLogger::Error("Error getHolding: %d, error: %s",address,modbus_strerror(errno));
		}
		*holding = dest[0];
	}
	modbusmutex->unlock();
	return status < 0 ? false : true;
}

bool Modbus::getHoldings(uint16_t address,uint16_t offset,uint16_t count,uint16_t* target)
{
	modbusmutex->lock();
	int status = SetSlave(address);
	if (status==0)
	{
		status = modbus_read_registers(ctx,offset,count,target);
		if (status == -1) {
			fprintf(stderr, "Error getHoldings: %s\n", modbus_strerror(errno));
			CLogger::Error("Error getHoldings: %d, error: %s",address,modbus_strerror(errno));
		}
	}
	modbusmutex->unlock();
	return status < 0 ? false : true;
}
