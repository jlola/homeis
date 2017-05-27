/*
 * LogService.h
 *
 *  Created on: 23. 5. 2017
 *      Author: pc
 */

#ifndef SRC_SERVICES_LOGSERVICE_H_
#define SRC_SERVICES_LOGSERVICE_H_

#include <httpserver.hpp>

using namespace std;
using namespace httpserver;

class LogService : public http_resource {
public:
	LogService();
	void render_GET(const http_request&, http_response**);
	virtual ~LogService();
};

#endif /* SRC_SERVICES_LOGSERVICE_H_ */
