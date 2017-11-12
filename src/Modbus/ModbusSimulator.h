/*
 * ModbusSimulator.h
 *
 *  Created on: 16. 11. 2016
 *      Author: pc
 */

#ifndef SRC_MODBUS_MODBUSSIMULATOR_H_
#define SRC_MODBUS_MODBUSSIMULATOR_H_

#include "HomeIsConfig.h"
#include "IModbus.h"

class ModbusSimulator : public IModbus
{
	uint16_t* registers;
	SSerPortConfig config;
public:
	ModbusSimulator(SSerPortConfig config, uint16_t* & registers);
	ModbusSimulator(SSerPortConfig config);

	bool Init();
	string GetName();
	string GetPort();
	bool setBitInput(uint16_t address,uint16_t index, uint16_t state);
	bool getBitInput(uint16_t address,uint16_t index,bool & bit);

	bool getShortInput(uint16_t address,uint16_t index,uint16_t & input);

	bool setCoil(uint16_t address,uint16_t index, uint16_t state);
	bool getCoil(uint16_t address,uint16_t index,bool & coil);

	bool setHolding(uint16_t address,uint16_t index, uint16_t val);
	bool getHolding(uint16_t address,uint16_t index,uint16_t* holding);
	bool getHoldings(uint16_t address,uint16_t offset,uint16_t count,uint16_t* target);

	~ModbusSimulator();
};

#endif /* SRC_MODBUS_MODBUSSIMULATOR_H_ */
