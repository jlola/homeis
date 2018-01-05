/*
 * RFIDHandler.h
 *
 *  Created on: 4. 1. 2018
 *      Author: pc
 */

#ifndef SRC_DEVICES_HANDLERS_RFIDHANDLER_H_
#define SRC_DEVICES_HANDLERS_RFIDHANDLER_H_

#include "HisDevModbus.h"
#include "IModbusHandler.h"

class RFIDHandler : public IModbusHandler {
	vector<HisDevValue<uint>*> values;
	STypedef stypedef;
	HisDevModbus* dev;
	IHisDevFactory* factory;
	static string LoadType;
	void CreateOrValidTags(bool addnew);
public:
	RFIDHandler(HisDevModbus* devModbus,IHisDevFactory* factory);

	bool Scan(bool addnew);
	void RefreshOutputs();
	void Refresh(bool modbusSuccess);
	void Load();

	virtual ~RFIDHandler();
};

#endif /* SRC_DEVICES_HANDLERS_RFIDHANDLER_H_ */
