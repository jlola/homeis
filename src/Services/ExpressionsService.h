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
#include "ParamsNames.h"
#include "ServiceBase.h"

using namespace std;
using namespace httpserver;
using namespace rapidjson;

class ExpressionService : public ServiceBase
{
private:
	HisDevFolderRoot* root;
	HisDevices* devices;
	IExpressionRuntime *expressionRuntime;
	LuaExpression* CreateOrUpdateExpression(string strJson,string & message);
public:
	ExpressionService(HisDevFolderRoot* folder,
			IExpressionRuntime *pexpressionRuntime,
			HisDevices* pdevices,
			IUserManager* userManager,
			IHisDevFactory* factory,
			webserver* ws_i);
	~ExpressionService(void);
	bool RemoveValueIdFromExpression(string strExpressionId, string strTagId, string & message);
	bool DeleteExpression(string strid,string & message);
	bool AddValueIdToExpression(string strExpressionId, string strTagId,string & message);
	static void ExpressionToJson(HisDevices* devices, Value & d, LuaExpression *pExpression, Document & respjsondoc);
	static void ExpressionsToJson(HisDevices* devices,string strFolderId, HisDevFolderRoot* root, Document & respjsondoc);
	static void ExpressionDebugLogToJson(LuaExpression *pExpression, Document & respjsondoc);
	const std::shared_ptr<http_response> GET(const http_request& r);
	const std::shared_ptr<http_response> POST(const http_request& r);
	const std::shared_ptr<http_response> PUT(const http_request& req);
	const std::shared_ptr<http_response> DELETE(const http_request& req);
};

#endif /* EXPRESSIONSSERVICE_H_ */
