/*
 * HisDevFactory.h
 *
 *  Created on: Mar 10, 2013
 *      Author: Josef Lola
 */

#ifndef HISDEVFACTORY_H_
#define HISDEVFACTORY_H_

#include "Devices/HisDevices.h"
#include "Expressions/ExpressionRuntime.h"
#include "Folder/HisDevFolderRoot.h"
#include "IHisDevFactory.h"

class HisDevFactory : public IHisDevFactory
{
private:
	//static HisDevFactory instance;
	HisDevices* devices;
	IExpressionRuntime* expressionRuntime;

	virtual ~HisDevFactory();

public:
	HisDevFactory(IExpressionRuntime *expressionRuntime,
				HisDevices* devices);

	IHisBase* Create(xmlNodePtr node);
};

#endif /* HISDEVFACTORY_H_ */
