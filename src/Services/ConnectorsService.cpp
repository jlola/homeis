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

ConnectorsService::ConnectorsService(vector<SSerPortConfig> & pserports, IHttpHeadersProvider & headersProvider)
 : headersProvider(headersProvider), serports(pserports)
{
}

const http_response ConnectorsService::render_GET(const http_request& req)
{
	Document respjsondoc;
	respjsondoc.SetArray();

	for(size_t i=0;i<serports.size();i++)
	{
		Value valport;
		valport.SetObject();
		Value strval;
		strval.SetString(serports[i].Name.c_str(),respjsondoc.GetAllocator());
		valport.AddMember("Name",strval,respjsondoc.GetAllocator());
		strval.SetString(serports[i].Driver.c_str(),respjsondoc.GetAllocator());
		valport.AddMember("Driver",strval,respjsondoc.GetAllocator());
		strval.SetString(serports[i].Port.c_str(),respjsondoc.GetAllocator());
		valport.AddMember("Port",strval,respjsondoc.GetAllocator());
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

