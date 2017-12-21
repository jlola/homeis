/*
 * HttpHeadersProvider.cpp
 *
 *  Created on: 20. 12. 2017
 *      Author: pc
 */

#include "HttpHeadersProvider.h"

HttpHeadersProvider::HttpHeadersProvider(string allowOrigin) {
	this->allowOrigin = allowOrigin;
}

string HttpHeadersProvider::GetContentTypeAppJson()
{
	return "application/json";
}

http_response_builder& HttpHeadersProvider::AddHeaders(http_response_builder & responseBuilder)
{
	if (allowOrigin.length()>0)
		responseBuilder.with_header("Access-Control-Allow-Origin",allowOrigin);
	return responseBuilder;
}

HttpHeadersProvider::~HttpHeadersProvider() {
}

