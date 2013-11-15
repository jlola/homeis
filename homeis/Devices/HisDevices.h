/*
 * HisDevices.h
 *
 *  Created on: Feb 16, 2013
 *      Author: Josef Lola
 */

#ifndef HISDEVICES_H_
#define HISDEVICES_H_

#include "HisDevBase.h"
#include "homeis/Common/CUUID.h"

using namespace std;

class HisDevices {
	xmlDocPtr doc;       /* document pointer */
	string devicesFileName;
	LOW_network *network;
	std::vector<HisDevBase*> devices;
public:
	HisDevices(string fileName,LOW_network *network);
	int Find(CUUID RecordId);
	size_t Size();
	void AddScanned();
	void Add(HisDevBase *hisdev);
	void Scan();
	void Refresh();
	int Find(LOW_deviceID id);
	HisDevValueBase* FindValue(string address);
	HisDevValueBase* FindValue(CUUID valueId);
	~HisDevices();
	//void SortByRecordId();
	void SortByNextScanTime();
	long GetNextDelay();
	long GetNextDelayTimeMS();
	void Save();
	void Load();
	void Delete(uint16_t index);
	HisDevBase *operator[](unsigned int i);
};

#endif /* HISDEVICES_H_ */
