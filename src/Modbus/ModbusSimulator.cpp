/*
 * ModbusSimulator.cpp
 *
 *  Created on: 16. 11. 2016
 *      Author: pc
 */

#include <string.h>
#include "ModbusSimulator.h"


const uint16_t registers_default[] =
//						10					  20							 30					 35
//{ 6,3,10,0,40,0,0,0,0,0,1,2,25,3,2,2,30,3,5,1,35,3,0,0,0,0x0209,0x0207,0,0,0,0x0003,0x0002,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
{8,4,10,3200,120,0,0,0,0,0,2,1,27,3,1,1,28,3,5,1,29,3,6,1,94,3,0,3,770,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,12290,13872,14389,12867,3395,778,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

string ModbusSimulator::DriverName = "modbussimulator";

string ModbusSimulator::GetDriverName()
{
	return ModbusSimulator::DriverName;
}

ModbusSimulator::ModbusSimulator(SSerPortConfig config,const uint16_t registers[], uint16_t length)
{
	this->registers = new uint16_t[length];
	this->length = length;
	memcpy(this->registers,registers,length*sizeof(uint16_t));
	this->config = config;
}

ModbusSimulator::ModbusSimulator(SSerPortConfig config)
	: ModbusSimulator(config,registers_default,sizeof(registers_default)/sizeof(uint16_t))
{

}

void ModbusSimulator::SetRegisters(uint16_t registers[], uint16_t length)
{
	if (this->registers != NULL && length > this->length)
	{
		delete[] this->registers;
		this->registers = new uint16_t[length];
		this->length = length;
	}

	memcpy(this->registers,registers,length*sizeof(uint16_t));
}

bool ModbusSimulator::Init()
{
	return true;
}
string ModbusSimulator::GetName()
{
	return config.Name;
}
string ModbusSimulator::GetPort()
{
	return config.Port;
}

bool ModbusSimulator::setBitInput(uint16_t address,uint16_t index, uint16_t state)
{
	return true;
}
bool ModbusSimulator::getBitInput(uint16_t address,uint16_t index,bool & bit)
{
	return true;
}

bool ModbusSimulator::getShortInput(uint16_t address,uint16_t index,uint16_t & input)
{
	return true;
}
bool ModbusSimulator::setCoil(uint16_t address,uint16_t index, uint16_t state)
{
	return true;
}
bool ModbusSimulator::getCoil(uint16_t address,uint16_t index,bool & coil)
{
	return true;
}

bool ModbusSimulator::setHolding(uint16_t address,uint16_t index, uint16_t val)
{
	if (address==1 || address==registers[0])
	{
		registers[index] = val;
		return true;
	}
	return false;
}

bool ModbusSimulator::getHolding(uint16_t address,uint16_t index,uint16_t* holding,uint32_t timeOutMs)
{
	if (index >= length)
	{
		throw "Out of range exception";
	}

	if (address==1 || address==registers[0])
	{
		*holding = registers[index];
		return true;
	}
	return false;
}

bool ModbusSimulator::getHolding(uint16_t address,uint16_t index,uint16_t* holding)
{
	return getHolding(address,index,holding, 0);
}

bool ModbusSimulator::getHoldings(uint16_t address,uint16_t offset,uint16_t count,uint16_t* target,uint32_t timeOutMs)
{
	if (address==1 || address==registers[0])
		{
			if (offset + count > length)
			{
				throw "Out of range exception";
			}

			memcpy(target,&registers[offset],sizeof(uint16_t)*count);
			return true;
		}
		return false;
}

bool ModbusSimulator::getHoldings(uint16_t address,uint16_t offset,uint16_t count,uint16_t* target)
{
	return getHoldings(address,offset,count,target,1000);
}


ModbusSimulator::~ModbusSimulator() {
	if (this->registers!=NULL)
		delete[] registers;
	this->registers = NULL;
}

