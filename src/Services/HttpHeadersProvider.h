/*
 * HttpHeadersProvider.h
 *
 *  Created on: 20. 12. 2017
 *      Author: pc
 */

#ifndef SRC_SERVICES_HTTPHEADERSPROVIDER_H_
#define SRC_SERVICES_HTTPHEADERSPROVIDER_H_

#include <string>

using namespace std;

#include "IHttpHeadersProvider.h"

class HttpHeadersProvider : public IHttpHeadersProvider
{
	string allowOrigin;
public:
	HttpHeadersProvider(string allowOrigin);
	http_response_builder& AddHeaders(http_response_builder & responseBuilder);
	string GetContentTypeAppJson();

	string GetErrorMessageJson(string message);

	virtual ~HttpHeadersProvider();
};

#endif /* SRC_SERVICES_HTTPHEADERSPROVIDER_H_ */
