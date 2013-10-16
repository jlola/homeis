/*
 * HisDevFolder.h
 *
 *  Created on: May 12, 2013
 *      Author: root
 */

#ifndef HISDEVFOLDER_H_
#define HISDEVFOLDER_H_

#include <vector>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "homeis/Common/CUUID.h"
#include "homeis/Common/HisBase.h"
#include "HisDevValueId.h"

using namespace std;

//#define PROP_NAME   BAD_CAST"name"
#define NODE_FODLER   BAD_CAST "folder"


class HisDevFolder : public HisBase
{
private:

protected:
	virtual void DoInternalSave(xmlNodePtr & node);
	virtual void DoInternalLoad(xmlNodePtr & node);

	const xmlChar* GetNodeNameInternal();
public:
	//int Find(CUUID id);
	//HisDevFolder* Find(CUUID id);
	HisDevFolder(string name);
	HisDevFolder(xmlNodePtr pNode);

	//najde vazebni objekt podle HisDevValue::RecordId
	HisDevValueId *FindValueId(CUUID valueId);
	virtual ~HisDevFolder();
};

#endif /* HISDEVFOLDER_H_ */
