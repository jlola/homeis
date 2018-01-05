/*
 * HisDevices.h
 *
 *  Created on: Feb 16, 2013
 *      Author: Josef Lola
 */

#ifndef HISDEVICES_H_
#define HISDEVICES_H_

#include <Modbus/IModbusProvider.h>
#include "HisDevBase.h"
#include "Common/CUUID.h"
#include "Common/HisLock.h"
#include <queue>          // std::queue
#include "Devices/HisDevBase.h"
#include "Expressions/IExpressionRuntime.h"
#include "IHisDevFactory.h"

using namespace std;

class HisDevices {
	xmlDocPtr doc;       /* document pointer */
	string devicesFileName;
	std::vector<HisDevBase*> devices;
	HisLock refreshMutex;
	queue<HisDevBase*> devqueue;
	OnRefreshDelegate onRefreshdelegate;
	IModbusProvider* modbusProvider;
public:
	HisDevices(string fileName,IModbusProvider* modbusProvider);
	int Find(CUUID RecordId);
	size_t Size();
	void AddScanned();
	void Add(HisDevBase *hisdev);
	void Refresh();
	HisDevValueBase* FindValue(string address);
	HisDevValueBase* FindValue(CUUID valueId);
	~HisDevices();
	void AddToRefreshQueue(HisDevBase* hisDevBase);
	int FindModbusDev(int addressId);
	void Save();
	void Load(IHisDevFactory* factory);
	void Delete(uint16_t index);
	HisDevBase *operator[](unsigned int i);
};

#endif /* HISDEVICES_H_ */
