/*
 * Modbus.cpp
 *
 *  Created on: 18. 10. 2016
 *      Author: pc
 */

#include <stdio.h>
#include <cerrno>
#include "linuxcs.h"
#include "StringBuilder.h"
#include <Modbus/ModbusWrapper.h>

using namespace std;
string Modbus::DriverName = "modbus";

Modbus::Modbus(SSerPortConfig config)
: modbusmutex(NULL),ctx(NULL),logger(CLogger::GetLogger())
{
	modbusmutex = HisLock::CreateMutex();
	this->config = config;
}

string Modbus::GetPort()
{
	return config.Port;
}

string Modbus::GetDriverName()
{
	return Modbus::DriverName;
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
		logger.Error("Unable to create the libmodbus context %s\n",config.Port.c_str());
		return false;
	}

	//modbus_set_debug(ctx,1);

	if (modbus_connect(ctx) == -1) {
		string message = StringBuilder::Format("Connection failed: %s\n", modbus_strerror(errno));
	    printf(message.c_str());
	    logger.Error(message.c_str());
	    modbus_free(ctx);
	    return false;
	}

	uint32_t sec;
	uint32_t usec;


	modbus_get_byte_timeout(ctx, &sec,&usec);

	sec = 0;
	usec = 100000;
	modbus_set_byte_timeout(ctx, sec, usec);

	sec = 0;
	usec = 100000;
	modbus_set_response_timeout(ctx, sec,usec);
	return true;
}

int Modbus::SetSlave(uint16_t address)
{
	int status = modbus_set_slave(ctx,address);
	if (status == -1) {
		fprintf(stderr, "Set slave: %s\n", modbus_strerror(errno));
		logger.Error("Error set slave: %d, error: %s",address,modbus_strerror(errno));
	}
	return status;
}

bool Modbus::setBitInput(uint16_t address,uint16_t index, uint16_t state) {
	HisLock lock(modbusmutex);
	int status = SetSlave(address);
	if (status == 0)
	{
		status = modbus_write_bit(ctx,index,state?TRUE:FALSE);
		if (status < 0) {
			fprintf(stderr, "Error setBitInput: %s\n", modbus_strerror(errno));
			logger.Error("Error setBitInput: %d, error: %s",address,modbus_strerror(errno));
		}
	}
	return status < 0 ? false : true;
}

bool Modbus::getBitInput(uint16_t address,uint16_t index,bool & bit){
	HisLock lock(modbusmutex);
	int status = SetSlave(address);
	if (status==0)
	{
		uint8_t* dest = new uint8_t[index+1];
		status = modbus_read_input_bits(ctx,index,1,dest);
		delete[] dest;
		if (status == -1) {
			fprintf(stderr, "Error getBitInput: %s\n", modbus_strerror(errno));
			logger.Error("Error getBitInput: %d, error: %s",address,modbus_strerror(errno));
		}
	}
	return status < 0 ? false : true;
}

bool Modbus::getShortInput(uint16_t address,uint16_t index,uint16_t & input){
	HisLock lock(modbusmutex);
	int status = SetSlave(address);
	if (status == 0)
	{
		uint16_t* dest = new uint16_t[index+1];
		status = modbus_read_input_registers(ctx,index,1,dest);
		delete[] dest;
		if (status == -1) {
			fprintf(stderr, " Error getShortInput: %s\n", modbus_strerror(errno));
			logger.Error("Error getShortInput: %d, error: %s",address,modbus_strerror(errno));
		}
	}
	return status < 0 ? false : true;
}

bool Modbus::setCoil(uint16_t address,uint16_t index, uint16_t state){
	HisLock lock(modbusmutex);
	int status = SetSlave(address);
	if (status==0)
	{
		status = modbus_write_bit(ctx,index,state?TRUE:FALSE);
		if (status == -1) {
			fprintf(stderr, "Error setCoil: %s\n", modbus_strerror(errno));
			logger.Error("Error setCoil: %d, error: %s",address,modbus_strerror(errno));
		}
	}
	return status < 0 ? false : true;
}

bool Modbus::getCoil(uint16_t address,uint16_t index,bool & coil){
	HisLock lock(modbusmutex);
	int status = SetSlave(address);
	if (status==0)
	{
		uint8_t* dest = new uint8_t[index+1];
		status = modbus_read_input_bits(ctx,index,1,dest);
		delete[] dest;
		if (status == -1) {
			fprintf(stderr, "Error getCoil: %s\n", modbus_strerror(errno));
			logger.Error("Error getCoil: %d, error: %s",address,modbus_strerror(errno));
		}
	}
	return status < 0 ? false : true;
}

bool Modbus::setHolding(uint16_t address,uint16_t index, uint16_t val){
	HisLock lock(modbusmutex);
	int status = SetSlave(address);
	if (status==0)
	{
		status = modbus_write_register(ctx,index,val);
		if (status == -1) {
			fprintf(stderr, "Error setHolding: %s\n", modbus_strerror(errno));
			logger.Error("Error setHolding: %d, error: %s",address,modbus_strerror(errno));
		}
	}
	return status < 0 ? false : true;
}

bool Modbus::getHolding(uint16_t address,uint16_t index,uint16_t* holding){
	HisLock lock(modbusmutex);
	int status = SetSlave(address);
	if (status==0)
	{
		uint16_t dest[2];
		status = modbus_read_registers(ctx,index,sizeof(dest)/2,dest);
		if (status == -1) {
			modbus_set_error_recovery(ctx, MODBUS_ERROR_RECOVERY_PROTOCOL);
			modbus_set_error_recovery(ctx, MODBUS_ERROR_RECOVERY_LINK );
			fprintf(stderr, "Error getHolding: %s\n", modbus_strerror(errno));
			logger.Error("Error getHolding: %d, error: %s",address,modbus_strerror(errno));
		}
		*holding = dest[0];
	}
	return status < 0 ? false : true;
}

bool Modbus::getHoldings(uint16_t address,uint16_t offset,uint16_t count,uint16_t* target)
{
	HisLock lock(modbusmutex);
	int status = SetSlave(address);
	if (status==0)
	{
		status = modbus_read_registers(ctx,offset,count,target);
		if (status == -1) {
			modbus_set_error_recovery(ctx,(modbus_error_recovery_mode)( MODBUS_ERROR_RECOVERY_PROTOCOL | MODBUS_ERROR_RECOVERY_LINK));
			//modbus_set_error_recovery(ctx, MODBUS_ERROR_RECOVERY_LINK );
			fprintf(stderr, "Error getHoldings: %s\n", modbus_strerror(errno));
			logger.Error("Error getHoldings: %d, error: %s",address,modbus_strerror(errno));
		}
	}
	return status < 0 ? false : true;
}
