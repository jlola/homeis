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
	uint16_t length;
	SSerPortConfig config;
public:

	static string DriverName;

	ModbusSimulator(SSerPortConfig config,const uint16_t registers[], uint16_t length);
	ModbusSimulator(SSerPortConfig config);

	void SetRegisters(uint16_t registers[],uint16_t length);
	bool Init();
	string GetName();
	string GetPort();
	string GetDriverName();
	bool setBitInput(uint16_t address,uint16_t index, uint16_t state);
	bool getBitInput(uint16_t address,uint16_t index,bool & bit);

	bool getShortInput(uint16_t address,uint16_t index,uint16_t & input);

	bool setCoil(uint16_t address,uint16_t index, uint16_t state);
	bool getCoil(uint16_t address,uint16_t index,bool & coil);

	bool setHolding(uint16_t address,uint16_t index, uint16_t val);
	bool getHolding(uint16_t address,uint16_t index,uint16_t* holding);
	bool getHolding(uint16_t address,uint16_t index,uint16_t* holding,uint32_t timeOutMs);
	bool getHoldings(uint16_t address,uint16_t offset,uint16_t count,uint16_t* target);
	bool getHoldings(uint16_t address,uint16_t offset,uint16_t count,uint16_t* target,uint32_t timeOutMs);

	bool WaitForAlarm();
	int GetAlarmAddress();

	void AddConsumer(IDeviceEventConsumer* consumer);

	~ModbusSimulator();
};

#endif /* SRC_MODBUS_MODBUSSIMULATOR_H_ */
