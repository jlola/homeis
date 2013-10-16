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
#include "homeis/Common/CUUID.h"
#include "homeis/Common/HisBase.h"
#include "homeis/Devices/Folder/HisDevFolder.h"

using namespace std;

class HisDevFolderRoot
{
	HisDevFolder* folder;
	string fileName;
	xmlDocPtr doc;
public:
	void Load();
	void Save();
	HisDevValueId* FindValueId(CUUID devValueId);
	HisDevFolder *GetFolder();
	HisDevFolderRoot(string ppath);
	~HisDevFolderRoot();
};

#endif /* HISDEVFOLDERROOT_H_ */
