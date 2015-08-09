/*
 * ExpressionsService.h
 *
 *  Created on: Sep 17, 2013
 *      Author: root
 */

#ifndef EXPRESSIONSSERVICE_H_
#define EXPRESSIONSSERVICE_H_

#include <httpserver.hpp>
#include "document.h"		// rapidjson's DOM-style API
#include "prettywriter.h"
#include "stringbuffer.h"
#include "filestream.h"	// wrapper of C stream for prettywriter as output
#include "Devices/Folder/HisDevFolder.h"
#include "Devices/Folder/HisDevFolderRoot.h"
#include "Devices/Folder/HisDevValueId.h"
#include "VirtualDevices/HisDevVirtual.h"
#include "Expressions/IExpression.h"
#include "Expressions/LuaExpression.h"

using namespace std;
using namespace httpserver;
using namespace rapidjson;

class ExpressionService : public http_resource<ExpressionService>
{
private:
	HisDevFolderRoot* root;
	HisDevices* devices;
	ExpressionRuntime *expressionRuntime;
	LuaExpression* CreateOrUpdateExpression(string strJson,string & message);
public:
	ExpressionService(HisDevFolderRoot* folder,ExpressionRuntime *pexpressionRuntime, HisDevices* pdevices);
	~ExpressionService(void);
	bool DeleteExpression(string strid,string & message);
	static void ExpressionToJson(LuaExpression *pExpression, Document & respjsondoc);
	static void ExpressionsToJson(string strid, HisDevFolderRoot* root, Document & respjsondoc);
	void render_GET(const http_request&, http_response**);
	void render_POST(const http_request& r, http_response** res);
	void render_PUT(const http_request& req, http_response** res);
	void render_DELETE(const http_request& req, http_response** res);
};

#endif /* EXPRESSIONSSERVICE_H_ */
