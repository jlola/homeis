/*
 * HisDevices.h
 *
 *  Created on: Feb 16, 2013
 *      Author: Josef Lola
 */

#ifndef HISDEVICES_H_
#define HISDEVICES_H_

#include <queue>          // std::queue
#include "IModbusProvider.h"
#include "HisDevBase.h"
#include "CUUID.h"
#include "HisLock.h"
#include "HisDevBase.h"
#include "IHisDevFactory.h"
#include "logger.h"
#include "IBlockingQueue.h"

#define DEVICEBASE 						0
#define DEVADDR_OFFSET					DEVICEBASE+0
#define COUNT_OF_TYPES_OFFSET			DEVICEBASE+1
#define TYPE_DEFS_OFFSET				DEVICEBASE+2
#define RESET_REG_OFFSET				DEVICEBASE+3
#define LAST_INDEX						DEVICEBASE+4
#define CHANGE_FLAG						DEVICEBASE+5

using namespace std;

class HisDevices : public IDeviceEventConsumer
{
	ILogger & logger;
	xmlDocPtr doc;       /* document pointer */
	string devicesFileName;
	std::vector<HisDevBase*> devices;
	HisLock refreshMutex;
	queue<HisDevBase*> devqueue;
	OnRefreshDelegate onRefreshdelegate;
	IModbusProvider* modbusProvider;
	IBlockingQueue<HisDevBase*>* deviceQueue;

	void AddToRefreshQueue(HisDevBase* hisDevBase);
	void RegisterToModbus();
public:
	HisDevices(string fileName,IModbusProvider* modbusProvider,IBlockingQueue<HisDevBase*>* deviceQueue);
	int Find(CUUID RecordId);
	size_t Size();
	void AddScanned();
	/*
	 * reads address=1 and if got response clear alarm
	 */
	//HisDevBase* ReadAlarmDevice(IModbus* modbus);
	void Add(HisDevBase *hisdev);
	void Refresh();
	HisDevValueBase* FindValue(string address);
	HisDevValueBase* FindValue(CUUID valueId);
	~HisDevices();
	int FindModbusDev(int addressId);
	void Save();
	void Load(IHisDevFactory* factory);
	void Delete(uint16_t index);
	HisDevBase *operator[](unsigned int i);
	void FireEvent(int adr);
};

#endif /* HISDEVICES_H_ */
