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
#include "document.h"		// rapidjson's DOM-style API

using namespace rapidjson;

#define URL_LOGIN "api/users/login"

using namespace std;
using namespace httpserver;

class ServiceBase  : public http_resource
{
private:
	IHisDevFactory* factory;
	IUserManager* userManager;
	Session* session;

	const std::shared_ptr<http_response> render_GET(const http_request& req);
	const std::shared_ptr<http_response> render_POST(const http_request& req);
	const std::shared_ptr<http_response> render_PUT(const http_request& req);
	const std::shared_ptr<http_response> render_DELETE(const http_request& req);
	const std::shared_ptr<http_response> render_OPTIONS(const http_request& req);
protected:
	Session* GetSession();
	bool Authorize(const http_request& req);
	const std::shared_ptr<http_response> CreateResponseString(string json,int response_code);
	const std::shared_ptr<http_response> CreateResponseFile(string path,int response_code);
	string GetErrorMessageJson(string message);
	IHisDevFactory* GetFactory();
public:
	string DocumentToString(Document & doc);
	std::shared_ptr<http_response> UnauthorizedResponse();
	ServiceBase(IHisDevFactory* factory,IUserManager* userManager);
	virtual const std::shared_ptr<http_response> GET(const http_request& req);
	virtual const std::shared_ptr<http_response> POST(const http_request& req);
	virtual const std::shared_ptr<http_response> PUT(const http_request& req);
	virtual const std::shared_ptr<http_response> DELETE(const http_request& req);
	virtual ~ServiceBase();
};

#endif /* SRC_SERVICES_SERVICEBASE_H_ */
