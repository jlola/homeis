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

class IHisDevFactory
{
public:
	virtual IHisBase *Create(xmlNodePtr node)=0;

	virtual ~IHisDevFactory(){}
};


#endif /* SRC_DEVICES_IHISDEVFACTORY_H_ */
