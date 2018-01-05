/*
 * HisDevFolderRoot.h
 *
 *  Created on: May 24, 2013
 *      Author: root
 */

#ifndef HISDEVFOLDERROOT_H_
#define HISDEVFOLDERROOT_H_

#include <vector>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "Common/CUUID.h"
#include "Common/HisBase.h"
#include "Devices/Folder/HisDevFolder.h"
#include "Expressions/IExpression.h"

using namespace std;

class HisDevFolderRoot
{
	IHisDevFactory* factory;
	HisDevFolder* folder;
	string fileName;
	xmlDocPtr doc;
public:
	void Load();
	void Save();
	HisDevValueId* FindValueId(CUUID devValueId);
	IExpression* FindExpression(CUUID expressionId);
	HisDevFolder *GetFolder();
	HisDevFolderRoot(string ppath,IHisDevFactory* factory);
	~HisDevFolderRoot();
};

#endif /* HISDEVFOLDERROOT_H_ */
