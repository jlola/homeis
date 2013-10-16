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

using namespace std;
using namespace httpserver;
using namespace rapidjson;

class FoldersService : public http_resource<FoldersService>
{
	HisDevFolderRoot* root;
public:
	FoldersService(HisDevFolderRoot* proot);
	~FoldersService(void);
	void render_GET(const http_request&, http_response**);
	void render_POST(const http_request& r, http_response** res);
	void render_PUT(const http_request& req, http_response** res);
	void render_DELETE(const http_request& req, http_response** res);
	void FoldersToJson(HisDevFolder *pFolder, Document & respjsondoc);
	void FolderToJson(HisBase *pFolder, Document & respjsondoc);
	bool CreateFolder(string strJson);
	bool UpdateFolder(string strid, string strJson);
};

#endif /* FOLDERSSERVICE_H_ */
