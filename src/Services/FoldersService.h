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
#include "prettywriter.h"
#include "stringbuffer.h"
#include "filestream.h"	// wrapper of C stream for prettywriter as output
#include "Folder/HisDevFolder.h"
#include "Folder/HisDevFolderRoot.h"
#include "IHttpHeadersProvider.h"

using namespace std;
using namespace httpserver;
using namespace rapidjson;

class FoldersService : public http_resource
{
	HisDevFolderRoot & root;
	HisDevices & devices;
	IHttpHeadersProvider & headersProvider;
	IHisDevFactory* factory;
public:
	FoldersService(HisDevices & dev,HisDevFolderRoot & proot, IHttpHeadersProvider & headersProvider,IHisDevFactory* factory);
	~FoldersService(void);
	const http_response render_GET(const http_request& req);
	const http_response render_POST(const http_request& req);
	const http_response render_PUT(const http_request& req);
	const http_response render_DELETE(const http_request& req);
	void FoldersToJson(HisDevFolderRoot & root, HisDevFolder *pFolder, Document & respjsondoc, bool foldersOnly);
	bool AddValueIdToFolder(string strFolderId, string strJson,string & message);
	string DeleteDevValue(string strDevValueRecordId);
	static void FolderToJson(HisDevFolderRoot & root,IHisBase *pParentFolder, IHisBase *pFolder, Document & respjsondoc);
	bool CreateFolder(string strJson,string & message);
	bool UpdateFolder(string strid, string strJson,string & message);
};

#endif /* FOLDERSSERVICE_H_ */
