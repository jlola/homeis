/*
 * LogService.h
 *
 *  Created on: 23. 5. 2017
 *      Author: pc
 */

#ifndef SRC_SERVICES_LOGSERVICE_H_
#define SRC_SERVICES_LOGSERVICE_H_

#include <httpserver.hpp>
#include "HttpHeadersProvider.h"
#include "logger.h"
#include "File.h"
#include "Directory.h"

using namespace std;
using namespace httpserver;

class LogService : public http_resource {
	ILogger & logger;
	IHttpHeadersProvider & httpHeadersProvider;
	File file;
	Directory directory;
public:
	LogService(IHttpHeadersProvider & httpHeadersProvider);
	//void render_GET(const http_request&, http_response**);
	const http_response render_GET(const http_request& req);
	virtual ~LogService();
};

#endif /* SRC_SERVICES_LOGSERVICE_H_ */
