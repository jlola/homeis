/*
 * VersionService.h
 *
 *  Created on: 6. 1. 2019
 *      Author: pc
 */

#ifndef SRC_SERVICES_VERSIONSERVICE_H_
#define SRC_SERVICES_VERSIONSERVICE_H_

#include <httpserver.hpp>
#include "HttpHeadersProvider.h"
#include "logger.h"
#include "File.h"
#include "Directory.h"
#include "ServiceBase.h"
#include "IHisDevFactory.h"

class VersionService : ServiceBase
{
public:
	VersionService(webserver* ws_i,
			IUserManager* userManager,
			IHisDevFactory* factory);

	const http_response GET(const http_request& req);
	string VersionToJson();

	virtual ~VersionService();
};

#endif /* SRC_SERVICES_VERSIONSERVICE_H_ */
