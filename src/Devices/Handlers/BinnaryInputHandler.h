/*
 * BinnaryInputHandler.h
 *
 *  Created on: 16. 12. 2017
 *      Author: pc
 */

#ifndef SRC_DEVICES_HANDLERS_BINNARYINPUTHANDLER_H_
#define SRC_DEVICES_HANDLERS_BINNARYINPUTHANDLER_H_

#include "IHisDevModbus.h"
#include "IModbusHandler.h"

class BinnaryInputHandler : public IModbusHandler
{
	typedef struct
	{
		uint16_t PinNumber:8;
		uint16_t Value:1;
		uint16_t Quality:1;
		uint16_t Latch:1;
		uint16_t LatchDirection:1;
	} SBinInput;


	static string LoadType;
	STypedef stypedef;
	SBinInput* sbininputs;
	vector<HisDevValue<bool>*> valuesInput;
	IHisDevModbus* dev;
	IHisDevFactory* factory;
public:
	BinnaryInputHandler(IHisDevModbus* devModbus,IHisDevFactory* factory);
	bool Remove(CUUID id);
	void CreateOrValidInputs(bool addnew);
	bool Scan(bool addnew);
	void RefreshOutputs(bool force);
	void Refresh(bool modbusSuccess);
	void Load();
	virtual ~BinnaryInputHandler();
};

#endif /* SRC_DEVICES_HANDLERS_BINNARYINPUTHANDLER_H_ */
