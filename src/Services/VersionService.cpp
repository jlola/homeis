/*
 * VersionService.cpp
 *
 *  Created on: 6. 1. 2019
 *      Author: pc
 */

#include <VersionService.h>
#include "Version.h"
#include "StringBuilder.h"
#include "ParamsNames.h"

VersionService::VersionService(webserver* ws_i,
		IUserManager* userManager,
		IHisDevFactory* factory)
	: ServiceBase::ServiceBase(factory,userManager)
{
	ws_i->register_resource(string("api/version"), this, true);
}

const http_response VersionService::GET(const http_request& req)
{
	return CreateResponseString(VersionToJson(),MHD_HTTP_OK);
}

string VersionService::VersionToJson()
{
	Document document;
	document.SetObject();
	Value jsonvalue;
	string strversion = StringBuilder::Format("%d.%d.%8d",VERSION_MAIN,VERSION_SEC,VERSION_BUILD);
	//jsonvalue.SetString(strversion.c_str(),document.GetAllocator());
	document.AddMember(JSON_HOMEISVERSION,strversion.c_str(),document.GetAllocator());

	return DocumentToString(document);
}

VersionService::~VersionService()
{
}

