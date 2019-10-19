/*
 * IModbus.h
 *
 *  Created on: 18. 10. 2016
 *      Author: Josef Lola
 */

#ifndef SRC_MODBUS_IMODBUS_H_
#define SRC_MODBUS_IMODBUS_H_

#include <stdint.h>
#include <string>

using namespace std;

class IModbus
{

public:
	virtual bool Init()=0;
	virtual string GetDriverName()=0;
	virtual string GetName()=0;
	virtual string GetPort()=0;
	virtual bool setHolding(uint16_t address,uint16_t index, uint16_t val)=0;
	virtual bool getHolding(uint16_t address,uint16_t index,uint16_t* holding)=0;
	virtual bool getHolding(uint16_t address,uint16_t index,uint16_t* holding, uint32_t timeOutMs)=0;
	virtual bool getHoldings(uint16_t address,uint16_t offset,uint16_t count,uint16_t* target)=0;
	virtual bool getHoldings(uint16_t address,uint16_t offset,uint16_t count,uint16_t* target, uint32_t timeOutMs)=0;


	virtual ~IModbus() {}
};



#endif /* SRC_MODBUS_IMODBUS_H_ */
