/*
 * OneWireHandler.h
 *
 *  Created on: 16. 12. 2017
 *      Author: pc
 */

#ifndef SRC_DEVICES_HANDLERS_ONEWIREHANDLER_H_
#define SRC_DEVICES_HANDLERS_ONEWIREHANDLER_H_

#include "IModbusHandler.h"
#include "HisDevValue.h"
#include "HisDevModbus.h"

#define OW_DEVICES_OFFSET	5
#define OW_SCAN_OFFSET 		0
#define OW_SCAN_PINNUMBER	100
#define SCAN_ONEWIRE_NAME "ScanOneWire"
#define LOADTYPE_ONEWIRE "ONEWIRE"

class OneWireHandler : public IModbusHandler
{
	typedef struct
	{
		uint16_t scan;
		uint16_t count;
	} SOWHeader;

	typedef struct
	{
		uint32_t id1234;
		uint32_t id5678;
		int16_t temperature;
		uint16_t error;
	} SDS18B20;

	HisDevValue<bool>* scantag;
	STypedef stypedef;
	SOWHeader* owheader;
	uint16_t sowiretypesdefsIndex;
	SDS18B20* sds18b20s;

	HisDevModbus* devModbus;
	bool scanRequest;
	IHisDevFactory* factory;

	void CreateOrValidOneWire(bool addnew);
	void CreateOrValidOneWireHeader(bool addnew);

public:

	static string LoadType;

	OneWireHandler(HisDevModbus* devModbus,IHisDevFactory* factory);

	void RefreshOutputs();

	void Refresh(bool modbusSuccess);

	bool Scan(bool addnew);

	void Load();

	virtual ~OneWireHandler();
};

#endif /* SRC_DEVICES_HANDLERS_ONEWIREHANDLER_H_ */
