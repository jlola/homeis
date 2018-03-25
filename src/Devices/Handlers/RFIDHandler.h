/*
 * RFIDHandler.h
 *
 *  Created on: 4. 1. 2018
 *      Author: pc
 */

#ifndef SRC_DEVICES_HANDLERS_RFIDHANDLER_H_
#define SRC_DEVICES_HANDLERS_RFIDHANDLER_H_

#include "IHisDevModbus.h"
#include "IModbusHandler.h"

#define MODBUS_DATA_SIZE 50
#define NEWDATAFLAG "NEWDATAFLAG"
#define RFIDDATATAG "RFIDDATATAG"

#define NEWDATAFLAG_OFFSET 0

typedef struct
{
	uint16_t NewDataFlag;
	uint8_t buffer[MODBUS_DATA_SIZE];
} SRFIDRegs;

class RFIDHandler : public IModbusHandler {
	HisDevValue<bool>* newDataFlag;
	HisDevValue<string>* rfiddataflag;
	STypedef stypedef;
	SRFIDRegs* srfidregs;
	IHisDevModbus* dev;
	IHisDevFactory* factory;
	static string LoadType;
	void CreateOrValidTags(bool addnew);
public:
	RFIDHandler(IHisDevModbus* devModbus,IHisDevFactory* factory);
	bool Remove(CUUID id);
	bool Scan(bool addnew);
	void RefreshOutputs();
	void Refresh(bool modbusSuccess);
	void Load();

	virtual ~RFIDHandler();
};

#endif /* SRC_DEVICES_HANDLERS_RFIDHANDLER_H_ */
