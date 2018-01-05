/*
 * HisModbusDevice.h
 *
 *  Created on: 21. 10. 2016
 *      Author: pc
 */

#ifndef SRC_DEVICES_HISDEVMODBUS_H_
#define SRC_DEVICES_HISDEVMODBUS_H_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <Modbus/IModbusProvider.h>
#include "HisDevBase.h"
#include "Modbus/IModbus.h"
#include "HisDevValue.h"
#include "Common/HisLock.h"
#include "LOW_deviceID.h"
#include "ModbusHandlers.h"
#include "IHisDevFactory.h"


#define DEVMODBUS BAD_CAST	"DevModbus"
#define PROP_ADDRESS BAD_CAST "Address"
#define PROP_CONNECTION_NAME BAD_CAST "ConnectionName"

typedef enum : uint16_t
{
	None,
	BinInputs,
	BinOutputs,
	AInputs,
	AOutputs,
	DS18B20Temp
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

class HisDevModbus : public HisDevBase
{
	typedef struct {
		uint16_t ModbusAddress;
		uint16_t CountOfTypes;
		uint16_t TypeDefsOffset;
		uint16_t ResetReg;
		uint16_t lastIndex;
	} SHeader;

	LPCRITICAL_SECTION refreshscanmutex;  /**< Mutex for exclusive access. */

	SHeader header;
	uint16_t* 	data;
	size_t 		size;

	STypedef* typesdefs;

	IModbusProvider* modbusProvider;

	bool refreshOutputs;
	int scanRequest;

	IModbus* connection;
	int address;
	ModbusHandlers handlers;

	void ReleaseResources();

public:
	bool GetData(uint16_t* & data, uint8_t & length);

	bool GetTypeDef(ETypes type,STypedef & stypedef);

	void WriteToDevice(ValueChangedEventArgs args);

	static bool Resolve(xmlNodePtr pnode);

	HisDevModbus(IModbus* pdev,int address,IHisDevFactory* factory);

	HisDevModbus(xmlNodePtr node,IModbusProvider* modbusManager,IHisDevFactory* factory);

	bool Scan(bool addnew);

	HisDevValueBase* FindValue(string pinNumber);

	int GetAddress();

	bool setHolding(uint16_t index, uint16_t val);

	IModbus* GetModbus();

	virtual const xmlChar* GetNodeNameInternal();

	virtual void DoInternalRefresh(bool alarm);

	virtual void DoInternalSave(xmlNodePtr & node);

	virtual void DoInternalLoad(xmlNodePtr & node);

	virtual void OnError();

	virtual ~HisDevModbus();
};

#endif /* SRC_DEVICES_HISDEVMODBUS_H_ */
