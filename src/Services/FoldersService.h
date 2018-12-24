/*
 * FoldersService.h
 *
 *  Created on: May 29, 2013
 *      Author: root
 */

#ifndef FOLDERSSERVICE_H_
#define FOLDERSSERVICE_H_

#include "DevicesService.h"
#include "document.h"		// rapidjson's DOM-style API
#include "filestream.h"	// wrapper of C stream for prettywriter as output
#include "Folder/HisDevFolder.h"
#include "Folder/HisDevFolderRoot.h"
#include "IHttpHeadersProvider.h"
#include "ServiceBase.h"
#include "ParamsNames.h"

using namespace std;
using namespace httpserver;
using namespace rapidjson;

class FoldersService : public ServiceBase
{
	ILogger & logger;
	HisDevFolderRoot & root;
	HisDevices & devices;
public:
	FoldersService(HisDevices & dev,HisDevFolderRoot & proot,
			IUserManager* userManager,
			IHisDevFactory* factory,
			webserver* ws_i);
	~FoldersService(void);
	const http_response GET(const http_request& req);
	const http_response POST(const http_request& req);
	const http_response PUT(const http_request& req);
	const http_response DELETE(const http_request& req);
	void FoldersToJson(HisDevFolderRoot & root, HisDevFolder *pFolder, Document & respjsondoc, bool foldersOnly);
	bool AddValueIdToFolder(string strFolderId, string strJson,string & message);
	string DeleteDevValue(string strDevValueRecordId);
	static void FolderToJson(HisDevFolderRoot & root,IHisBase *pParentFolder, IHisBase *pFolder, Document & respjsondoc);
	bool CreateFolder(string strJson,string & message);
	bool UpdateFolder(string strid, string strJson,string & message);
};

#endif /* FOLDERSSERVICE_H_ */
