/*
 * HisDallas.h
 *
 *  Created on: Feb 22, 2013
 *      Author: root
 */

#ifndef HISDALLAS_H_
#define HISDALLAS_H_

#define PROP_FAMILY (const xmlChar *)"familyname"
#define PROP_ID   	(const xmlChar *)"id"

#include "HisDevBase.h"
#include "LOW_device.h"
#include "LOW_netSegment.h"
#include "LOW_network.h"

class HisDevDallas : public HisDevBase
{
private: LOW_device* device;

protected: virtual ~HisDevDallas();

/**/

public:	HisDevDallas(LOW_device* pdev);

public: HisDevDallas(xmlNodePtr node,LOW_device* pdev);

protected: virtual void DoInternalSave(xmlNodePtr & node);

protected: virtual void DoInternalLoad(xmlNodePtr & node);

protected: virtual void OnError();

public: static LOW_deviceID GetId(xmlNodePtr node);

public:	LOW_deviceID GetId();


};

#endif /* HISDALLAS_H_ */
