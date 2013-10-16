/*
 * HisDevFactory.h
 *
 *  Created on: Mar 10, 2013
 *      Author: Josef Lola
 */

#ifndef HISDEVFACTORY_H_
#define HISDEVFACTORY_H_

#include "LOW_deviceID.h"
#include "HisDevTemp18B20.h"
#include "HisDevIO2413.h"
#include "homeis/Devices/HisDevices.h"
#include "Folder/HisDevFolderRoot.h"

class HisDevFactory
{
private:
	static HisDevFactory instance;
	HisDevices* devices;
	HisDevFolderRoot *rootFolder;

	HisDevFactory();
	virtual ~HisDevFactory();
public:
	static HisDevFactory & Instance();

	void SetDevices(HisDevices* pdevices);

	void SetRooFolder(HisDevFolderRoot* proot);

	HisBase *Create(xmlNodePtr node);

	HisDevBase * Create(xmlNodePtr notptr,LOW_device* dev);

	HisDevBase * Create(LOW_device* dev);
};

#endif /* HISDEVFACTORY_H_ */
