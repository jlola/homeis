/*
 * IHisDevFactory.h
 *
 *  Created on: 29. 12. 2017
 *      Author: pc
 */

#ifndef SRC_DEVICES_IHISDEVFACTORY_H_
#define SRC_DEVICES_IHISDEVFACTORY_H_

#include <libxml/parser.h>
#include <libxml/tree.h>
#include "IHisBase.h"
#include "IWriteToDevice.h"
#include "IEmailSender.h"
#include "IFile.h"
#include "IDirectory.h"
#include "IHttpHeadersProvider.h"
//#include "HisDevBase.h"
//#include "IBlockingQueue.h"

class IHisDevFactory
{
public:

	virtual IHttpHeadersProvider* GetHeadersProvider()=0;

	virtual IDirectory *GetDirectory()=0;

	virtual IFile *GetFile()=0;

	virtual IHisBase *Create(xmlNodePtr node)=0;

	virtual IEmailSender* GetEmailSender()=0;

	virtual ~IHisDevFactory(){}
};


#endif /* SRC_DEVICES_IHISDEVFACTORY_H_ */
