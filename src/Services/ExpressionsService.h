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
#include "HttpHeadersProvider.h"

using namespace std;
using namespace httpserver;
using namespace rapidjson;

class ExpressionService : public http_resource
{
private:
	IHttpHeadersProvider & headersProvider;
	HisDevFolderRoot* root;
	HisDevices* devices;
	IExpressionRuntime *expressionRuntime;
	LuaExpression* CreateOrUpdateExpression(string strJson,string & message);
public:
	ExpressionService(HisDevFolderRoot* folder,IExpressionRuntime *pexpressionRuntime, HisDevices* pdevices, IHttpHeadersProvider & headersProvider);
	~ExpressionService(void);
	bool DeleteExpression(string strid,string & message);
	static void ExpressionToJson(IHisBase* pParent,LuaExpression *pExpression, Document & respjsondoc);
	static void ExpressionsToJson(string strid, HisDevFolderRoot* root, Document & respjsondoc);
	static void ExpressionDebugLogToJson(LuaExpression *pExpression, Document & respjsondoc);
	const http_response render_GET(const http_request& r);
	const http_response render_POST(const http_request& r);
	const http_response render_PUT(const http_request& req);
	const http_response render_DELETE(const http_request& req);
};

#endif /* EXPRESSIONSSERVICE_H_ */
