/*
 * Modbus.h
 *
 *  Created on: 18. 10. 2016
 *      Author: pc
 */

#ifndef SRC_MODBUS_MODBUSWRAPPER_H_
#define SRC_MODBUS_MODBUSWRAPPER_H_

#include "Modbus/IModbus.h"
#include "modbus.h"
#include <string.h>
#include <vector>
#include "Common/HisLock.h"
#include "HomeIsConfig.h"

using namespace std;

class Modbus : public IModbus
{
	LOW_thread_mutex  *modbusmutex;  /**< Mutex for exclusive access. */
	SSerPortConfig config;
	modbus_t *ctx;
	int SetSlave(uint16_t address);
public:

	Modbus(SSerPortConfig config);
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

	virtual ~Modbus(){}
};

#endif /* SRC_MODBUS_MODBUSWRAPPER_H_ */
