/*
 * IHttpHeadersProvider.h
 *
 *  Created on: 20. 12. 2017
 *      Author: pc
 */

#ifndef SRC_SERVICES_IHTTPHEADERSPROVIDER_H_
#define SRC_SERVICES_IHTTPHEADERSPROVIDER_H_

#include "httpserver.hpp"

using namespace std;
using namespace httpserver;

class IHttpHeadersProvider
{
public:
	virtual std::shared_ptr<http_response> & AddHeaders(std::shared_ptr<http_response> & responseBuilder)=0;
	virtual string GetContentTypeAppJson()=0;
	virtual string GetErrorMessageJson(string message)=0;
	virtual string GetMessageJson(string msg)=0;
	virtual ~IHttpHeadersProvider(){}
};




#endif /* SRC_SERVICES_IHTTPHEADERSPROVIDER_H_ */
