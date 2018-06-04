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

ConnectorsService::ConnectorsService(IModbusProvider & provider, IHttpHeadersProvider & headersProvider)
 : headersProvider(headersProvider), provider(provider)
{
}

const http_response ConnectorsService::render_GET(const http_request& req)
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
	http_response_builder response_builder(json, MHD_HTTP_OK,headersProvider.GetContentTypeAppJson());
	headersProvider.AddHeaders(response_builder);
	http_response resp(response_builder.string_response());
	return resp;
}

ConnectorsService::~ConnectorsService() {
}

