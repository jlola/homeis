/*
 * FoldersService.h
 *
 *  Created on: May 29, 2013
 *      Author: root
 */

#ifndef FOLDERSSERVICE_H_
#define FOLDERSSERVICE_H_

#include "document.h"		// rapidjson's DOM-style API
#include "prettywriter.h"
#include "stringbuffer.h"
#include "filestream.h"	// wrapper of C stream for prettywriter as output
#include "Folder/HisDevFolder.h"
#include "Folder/HisDevFolderRoot.h"
#include "OneWireDevicesService.h"

using namespace std;
using namespace httpserver;
using namespace rapidjson;

class FoldersService : public http_resource<FoldersService>
{
	HisDevFolderRoot* root;
	HisDevices & devices;
public:
	FoldersService(HisDevices & dev,HisDevFolderRoot* proot);
	~FoldersService(void);
	void render_GET(const http_request&, http_response**);
	void render_POST(const http_request& r, http_response** res);
	void render_PUT(const http_request& req, http_response** res);
	void render_DELETE(const http_request& req, http_response** res);
	void FoldersToJson(HisDevFolderRoot* root, HisDevFolder *pFolder, Document & respjsondoc, bool foldersOnly);
	bool AddValueIdToFolder(string strFolderId, string strJson);
	string DeleteDevValue(string strDevValueRecordId);
	static void FolderToJson(HisDevFolderRoot* root,HisBase *pFolder, Document & respjsondoc);
	bool CreateFolder(string strJson);
	bool UpdateFolder(string strid, string strJson);
};

#endif /* FOLDERSSERVICE_H_ */
