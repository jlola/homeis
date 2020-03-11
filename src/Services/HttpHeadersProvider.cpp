/*
 * HttpHeadersProvider.cpp
 *
 *  Created on: 20. 12. 2017
 *      Author: pc
 */
#include "document.h"		// rapidjson's DOM-style API
#include "prettywriter.h"
#include "stringbuffer.h"
#include "filestream.h"	// wrapper of C stream for prettywriter as output

#include "HttpHeadersProvider.h"
#include "ParamsNames.h"

using namespace rapidjson;

HttpHeadersProvider::HttpHeadersProvider(string allowOrigin) {
	this->allowOrigin = allowOrigin;
}

string HttpHeadersProvider::GetContentTypeAppJson()
{
	return "application/json";
}

std::shared_ptr<http_response> & HttpHeadersProvider::AddHeaders(std::shared_ptr<http_response> & response)
{
	if (allowOrigin.length()>0)
		response->with_header("Access-Control-Allow-Origin",allowOrigin);
	return response;
}

string HttpHeadersProvider::GetMessageJson(string msg)
{
	Document respjsondoc;
	respjsondoc.SetObject();
	StringBuffer buffer;

	Value jsonvalue;
	jsonvalue.SetString(msg.c_str(),msg.length(),respjsondoc.GetAllocator());
	respjsondoc.AddMember(JSON_MESSAGE,jsonvalue, respjsondoc.GetAllocator());

	PrettyWriter<StringBuffer> wr(buffer);
	respjsondoc.Accept(wr);
	std::string json = buffer.GetString();

	return json;
}

string HttpHeadersProvider::GetErrorMessageJson(string message)
{
	Document respjsondoc;
	respjsondoc.SetObject();
	Value jsonvalue;
	jsonvalue.SetString(message.c_str(),message.length(),respjsondoc.GetAllocator());
	respjsondoc.AddMember(JSON_ERROR,jsonvalue, respjsondoc.GetAllocator());

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	respjsondoc.Accept(wr);
	std::string json = buffer.GetString();

	return json;
}

HttpHeadersProvider::~HttpHeadersProvider() {
}

