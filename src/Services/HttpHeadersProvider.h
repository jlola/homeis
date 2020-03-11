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
	std::shared_ptr<http_response> & AddHeaders(std::shared_ptr<http_response> & responseBuilder);
	string GetContentTypeAppJson();
	string GetMessageJson(string message);
	string GetErrorMessageJson(string message);

	virtual ~HttpHeadersProvider();
};

#endif /* SRC_SERVICES_HTTPHEADERSPROVIDER_H_ */
