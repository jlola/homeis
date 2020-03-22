/*
 * BinnaryOutputHandler.h
 *
 *  Created on: 16. 12. 2017
 *      Author: pc
 */

#ifndef SRC_DEVICES_HANDLERS_BINNARYOUTPUTHANDLER_H_
#define SRC_DEVICES_HANDLERS_BINNARYOUTPUTHANDLER_H_

#include <vector>
#include "IHisDevModbus.h"
#include "IModbusHandler.h"
#include "IHisBase.h"

using namespace std;

#define OW_SCAN_PINNUMBER	100

class BinnaryOutputHandler : public IModbusHandler
{
	typedef struct
	{
		uint16_t PinNumber:8;
		uint16_t Value:1;
		uint16_t Quality:1;
	} SBinOutput;

	vector<HisDevValue<bool>*> valuesOutput;
	int count;
	STypedef stypedef;
	IHisDevModbus* devModbus;
	SBinOutput* sbinoutputs;
	IHisDevFactory* factory;
public:
	static string LoadType;
	BinnaryOutputHandler(IHisDevModbus* devModbus,IHisDevFactory* factory);
	bool Scan(bool addnew);
	bool Remove(CUUID id);
	void RefreshOutputs(bool force);
	void Refresh(bool modbusSuccess);
	void Load();
	void CreateOrValidOutputs(bool addnew);
	~BinnaryOutputHandler();
};

#endif /* SRC_DEVICES_HANDLERS_BINNARYOUTPUTHANDLER_H_ */
