/*
 * FileController.h
 *
 *  Created on: Apr 16, 2013
 *      Author: root
 */

#ifndef FILECONTROLLER_H_
#define FILECONTROLLER_H_

#include <httpserver.hpp>
#include "HisDevFactory.h"
#include "ServiceBase.h"

using namespace std;
using namespace httpserver;

class FileController : public ServiceBase {
	public:
		FileController(webserver* server,
				IUserManager* userManager,
				IHisDevFactory* factory
				);
		const http_response GET(const http_request&);
};


#endif /* FILECONTROLLER_H_ */
