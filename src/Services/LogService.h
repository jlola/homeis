/*
 * LogService.h
 *
 *  Created on: 23. 5. 2017
 *      Author: pc
 */

#ifndef SRC_SERVICES_LOGSERVICE_H_
#define SRC_SERVICES_LOGSERVICE_H_

#include "httpserver.hpp"
#include "HttpHeadersProvider.h"
#include "logger.h"
#include "File.h"
#include "Directory.h"
#include "ServiceBase.h"
#include "IHisDevFactory.h"

using namespace std;
using namespace httpserver;

class LogService : public ServiceBase {
	ILogger & logger;
	File file;
	Directory directory;
public:
	LogService(webserver* ws_i,
			IUserManager* userManager,
			IHisDevFactory* factory);
	//void render_GET(const http_request&, http_response**);
	const std::shared_ptr<http_response> GET(const http_request& req);
	virtual ~LogService();
};

#endif /* SRC_SERVICES_LOGSERVICE_H_ */
