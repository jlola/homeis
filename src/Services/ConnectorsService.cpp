/*
 * ConnectorsService.cpp
 *
 *  Created on: 15. 11. 2016
 *      Author: pc
 */
#include "document.h"		// rapidjson's DOM-style API
#include "prettywriter.h"
#include "stringbuffer.h"
#include "filestream.h"	// wrapper of C stream for prettywriter as output
#include <Services/ConnectorsService.h>
#include "microhttpd.h"

ConnectorsService::ConnectorsService(IModbusProvider & provider,
		IUserManager* userManager,
		IHisDevFactory* factory,
		webserver* ws_i)
 : ServiceBase::ServiceBase(factory,userManager),
   provider(provider)
{
	ws_i->register_resource("api/connectors", this, true);
}

const http_response ConnectorsService::GET(const http_request& req)
{
	Document respjsondoc;
	respjsondoc.SetArray();

	vector<IModbus*> connectors = provider.GetConnectors();

	for(size_t i=0;i<connectors.size();i++)
	{
		Value valport;
		valport.SetObject();
		Value strval;
		strval.SetString(connectors[i]->GetName().c_str(),respjsondoc.GetAllocator());
		valport.AddMember("name",strval,respjsondoc.GetAllocator());
		strval.SetString(connectors[i]->GetDriverName().c_str(),respjsondoc.GetAllocator());
		valport.AddMember("driver",strval,respjsondoc.GetAllocator());
		strval.SetString(connectors[i]->GetPort().c_str(),respjsondoc.GetAllocator());
		valport.AddMember("port",strval,respjsondoc.GetAllocator());
		respjsondoc.PushBack(valport,respjsondoc.GetAllocator());
	}

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	respjsondoc.Accept(wr);
	std::string json = buffer.GetString();
	int response_code = MHD_HTTP_OK;
	return CreateResponseString(json,response_code);
}

ConnectorsService::~ConnectorsService() {
}

