/*
 * HisDevFactory.h
 *
 *  Created on: Mar 10, 2013
 *      Author: Josef Lola
 */

#ifndef HISDEVFACTORY_H_
#define HISDEVFACTORY_H_

#include "LOW_deviceID.h"
#include "Devices/HisDevices.h"
#include "Expressions/ExpressionRuntime.h"
#include "Folder/HisDevFolderRoot.h"

class HisDevFactory
{
private:
	static HisDevFactory instance;
	HisDevices* devices;
	HisDevFolderRoot *rootFolder;
	ExpressionRuntime* expressionRuntime;

	HisDevFactory();
	virtual ~HisDevFactory();

public:

	static HisDevFactory & Instance();

	void SetDevices(HisDevices* pdevices);

	void SetRooFolder(HisDevFolderRoot* proot);

	void SetExpressionRuntime(ExpressionRuntime *pExpressionRuntime);

	HisBase *Create(xmlNodePtr node);
};

#endif /* HISDEVFACTORY_H_ */
