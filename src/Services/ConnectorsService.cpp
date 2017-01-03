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

ConnectorsService::ConnectorsService(vector<SSerPortConfig> & pserports)
 : serports(pserports)
{
}

void ConnectorsService::render_GET(const http_request& req, http_response** resp)
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
	//*res = new http_string_response(json, 200, "application/json");
	*resp = new http_response(http_response_builder(json, 200,"application/json").string_response());

}

ConnectorsService::~ConnectorsService() {
}

