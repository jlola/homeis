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
#include "HisDevBase.h"
#include "Modbus/ModbusManager.h"
#include "Modbus/IModbus.h"
#include "HisDevValue.h"
#include "Common/HisLock.h"
#include "LOW_deviceID.h"

#define DEVMODBUS BAD_CAST	"DevModbus"
#define PROP_ADDRESS BAD_CAST "Address"
#define PROP_CONNECTION_NAME BAD_CAST "ConnectionName"
#define LOADTYPE_ONEWIRE "ONEWIRE"

#define OW_DEVICES_OFFSET	5
#define OW_SCAN_OFFSET 		0
#define OW_SCAN_PINNUMBER	100

class HisDevModbus : public HisDevBase
{
	typedef enum : uint16_t
	{
		None,
		BinInputs,
		BinOutputs,
		AInputs,
		AOutputs,
		DS18B20
	} ETypes;

	typedef enum : uint16_t
	{
		ReadCoil=1,
		ReadDiscreteInput=2,
		ReadHoldingRegisters=3,
		ReadInputRegisters=4,
		WriteCoil=5
	} EModbusFunc;

	typedef struct {
		uint16_t ModbusAddress;
		uint16_t CountOfTypes;
		uint16_t TypeDefsOffset;
		uint16_t ResetReg;
		uint16_t lastIndex;
	} SHeader;

	typedef struct
	{
		ETypes Type;
		uint16_t Count;
		uint16_t OffsetOfType;
		EModbusFunc Func;
	} STypedef;

	typedef struct
	{
		uint16_t PinNumber:8;
		uint16_t Value:1;
		uint16_t Quality:1;
		uint16_t Latch:1;
		uint16_t LatchDirection:1;
	} SBinInput;

	typedef struct
	{
		uint16_t PinNumber:8;
		uint16_t Value:1;
		uint16_t Quality:1;
	} SBinOutput;

	typedef struct
	{
		uint16_t scan;
		uint16_t count;
	} SOWHeader;

	typedef struct
	{
		uint32_t id1234;
		uint32_t id5678;
		uint16_t temperature;
		uint16_t error;
	} SDS18B20;

	LOW_thread_mutex  *refreshscanmutex;  /**< Mutex for exclusive access. */

	SHeader header;
	uint16_t* 	data;
	size_t 		size;

	STypedef* typesdefs;

	uint16_t sbinInputstypesdefsIndex;
	SBinInput* sbininputs;
	ModbusManager* modbusManager;
	SBinOutput* sbinoutputs;
	uint16_t sbinOutputstypesdefsIndex;

	bool refreshOutputs;
	int scanRequest;

	SOWHeader* owheader;
	uint16_t sowiretypesdefsIndex;
	SDS18B20* sds18b20s;


	IModbus* connection;
	int address;
	vector<HisDevValue<bool>*> valuesOutput;
	vector<HisDevValue<bool>*> valuesInput;
	//vector<HisDevValue<double>*> ds18b20s;

	HisDevValue<bool>* scantag;




	void CreateOrValidOneWireHeader(bool addnew);
	void CreateOrValidOneWire(bool addnew);
	void CreateOrValidInputs(bool addnew);
	void CreateOrValidOutputs(bool addnew);
	void ReleaseResources();
	void RefreshOutputs();
public:

	void WriteToDevice(ValueChangedEventArgs args);

	static bool Resolve(xmlNodePtr pnode);

	HisDevModbus(IModbus* pdev,int address);

	HisDevModbus(xmlNodePtr node,ModbusManager* modbusManager);

	bool Scan(bool addnew);

	HisDevValueBase* FindValue(string pinNumber);

	int GetAddress();

	std::string GetConnectionName();

	virtual const xmlChar* GetNodeNameInternal();

	virtual void DoInternalRefresh(bool alarm);

	virtual void DoInternalSave(xmlNodePtr & node);

	virtual void DoInternalLoad(xmlNodePtr & node);

	virtual void OnError();

	virtual ~HisDevModbus();
};

#endif /* SRC_DEVICES_HISDEVMODBUS_H_ */
