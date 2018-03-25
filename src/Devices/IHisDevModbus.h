/*
 * IHisDevModbus.h
 *
 *  Created on: 21. 1. 2018
 *      Author: pc
 */

#ifndef SRC_DEVICES_IHISDEVMODBUS_H_
#define SRC_DEVICES_IHISDEVMODBUS_H_

#include "IHisBase.h"
#include "HisDevValue.h"
#include "IWriteToDevice.h"

typedef enum : uint16_t
{
	None,
	BinInputs,
	BinOutputs,
	AInputs,
	AOutputs,
	DS18B20Temp,
	RFID
} ETypes;

typedef enum : uint16_t
{
	ReadCoil=1,
	ReadDiscreteInput=2,
	ReadHoldingRegisters=3,
	ReadInputRegisters=4,
	WriteCoil=5
} EModbusFunc;

typedef struct
{
	ETypes Type;
	uint16_t Count;
	uint16_t OffsetOfType;
	EModbusFunc Func;
} STypedef;

class IHisDevModbus : public IWriteToDevice
{
public:

	virtual vector<HisDevValue<double>*> GetDoubleItems()=0;

	virtual vector<HisDevValue<bool>*> GetBoolItems()=0;

	virtual void Add(IHisBase *pitem)=0;

	virtual bool GetTypeDef(ETypes type,STypedef * stypedef)=0;

	virtual bool GetData(uint16_t** data, uint8_t & length)=0;

	virtual HisDevValueBase* FindValue(string pinNumber)=0;

	virtual int GetAddress()=0;

	virtual bool setHolding(uint16_t index, uint16_t val)=0;

	virtual HisDevValueBase* FindValue(string pinNumber,string loadType)=0;
};



#endif /* SRC_DEVICES_IHISDEVMODBUS_H_ */
