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
#include "IDirectory.h"

class HisDevFactory : public IHisDevFactory
{
private:
	IDirectory* directory;
	IFile* file;
	HisDevices* devices;
	IExpressionRuntime* expressionRuntime;
	IEmailSender* emailSender;

	virtual ~HisDevFactory();

public:
	HisDevFactory(IExpressionRuntime *expressionRuntime,
				HisDevices* devices,
				IEmailSender* emailSender,
				IFile* file,
				IDirectory* directory);

	IHisBase* Create(xmlNodePtr node);

	IEmailSender* GetEmailSender();

	IFile *GetFile();

	IDirectory* GetDirectory();
};

#endif /* HISDEVFACTORY_H_ */
