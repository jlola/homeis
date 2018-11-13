/*
 * ServiceBase.h
 *
 *  Created on: 15. 10. 2018
 *      Author: pc
 */

#ifndef SRC_SERVICES_SERVICEBASE_H_
#define SRC_SERVICES_SERVICEBASE_H_

#include <httpserver.hpp>
#include "IHisDevFactory.h"
#include "IUserManager.h"

#define URL_LOGIN "api/users/login"

using namespace std;
using namespace httpserver;

class ServiceBase  : public http_resource
{
private:
	IHisDevFactory* factory;
	IUserManager* userManager;

	const http_response render_GET(const http_request& req);
	const http_response render_POST(const http_request& req);
	const http_response render_PUT(const http_request& req);
	const http_response render_DELETE(const http_request& req);
	const http_response render_OPTIONS(const http_request& req);
protected:
	bool Authorize(const http_request& req);
	http_response CreateResponseString(string json,int response_code);
	string GetErrorMessageJson(string message);
	IHisDevFactory* GetFactory();
public:
	http_response UnauthorizedResponse();
	ServiceBase(IHisDevFactory* factory,IUserManager* userManager);
	virtual const http_response GET(const http_request& req);
	virtual const http_response POST(const http_request& req);
	virtual const http_response PUT(const http_request& req);
	virtual const http_response DELETE(const http_request& req);
	virtual ~ServiceBase();
};

#endif /* SRC_SERVICES_SERVICEBASE_H_ */
