/*
 * ModbusSimulator.cpp
 *
 *  Created on: 16. 11. 2016
 *      Author: pc
 */

#include <string.h>
#include <Modbus/ModbusSimulator.h>


uint16_t registers_default[] = {
		6,		//0
	    3,
	    10,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    1,		//10
	    2,
	    25,
	    3,
	    2,
	    2,
	    30,
	    3,
	    5,
	    1,
	    35,		//20
	    3,
	    0,
	    0,
	    0,
	    0x0209,		//25
	    0x0207,
	    0,
	    0,
	    0,
	    0x0003,		//30
	    0x0002,
	    0,
	    0,
	    0,
	    0,		//35
	    0,
	    0,
	    0,
	    0,
	    0,		//40
	    0,
	    0,
	    0,
	    0,
	    0,		//45
	    0,
	    0,
	    0,
	    0,
	    0,		//50
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0,
	    0};

uint16_t* registers_defaulti = registers_default;

string ModbusSimulator::DriverName = "modbussimulator";

string ModbusSimulator::GetDriverName()
{
	return ModbusSimulator::DriverName;
}

ModbusSimulator::ModbusSimulator(SSerPortConfig config, uint16_t* & registers)
{
	this->registers = registers;
	this->config = config;
}

ModbusSimulator::ModbusSimulator(SSerPortConfig config)
	: ModbusSimulator(config,registers_defaulti)
{

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
bool ModbusSimulator::getHolding(uint16_t address,uint16_t index,uint16_t* holding)
{
	if (address==1 || address==registers[0])
	{
		*holding = registers[index];
		return true;
	}
	return false;
}
bool ModbusSimulator::getHoldings(uint16_t address,uint16_t offset,uint16_t count,uint16_t* target)
{
	if (address==1 || address==registers[0])
	{
		memcpy(target,&registers[offset],sizeof(uint16_t)*count);
		return true;
	}
	return false;
}


ModbusSimulator::~ModbusSimulator() {

}

