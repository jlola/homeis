/*
 * UsersService.h
 *
 *  Created on: 29. 10. 2018
 *      Author: pc
 */

#ifndef SRC_SERVICES_USERSSERVICE_H_
#define SRC_SERVICES_USERSSERVICE_H_

#include "document.h"		// rapidjson's DOM-style API
#include "prettywriter.h"
#include "stringbuffer.h"
#include "ServiceBase.h"
#include "ParamsNames.h"
#include "IUserManager.h"

using namespace std;
using namespace httpserver;
using namespace rapidjson;

class UsersService : public ServiceBase
{
	IUserManager* userManager;
	string GetUserNameFromJson(string content);
public:
	UsersService(webserver* ws_i, IUserManager* userManager, IHisDevFactory* factory);
	void UserToJson(IUser* user, Value & userJson, Document & respjsondoc);
	bool Authentize(string strjson, string ip, string & sessionHash);
	string CreateResponse(string sessionHash);
	const std::shared_ptr<http_response> GET(const http_request& req);
	const std::shared_ptr<http_response> PUT(const http_request& req);
	const std::shared_ptr<http_response> POST(const http_request& req);
	const std::shared_ptr<http_response> DELETE(const http_request& req);
	bool Logout(string strjson, string ip);
	bool UpdateUser(IUser* user, string strjson);
	void UserToStringJson(IUser* user,string & json);
	bool SetPassword(string strjson);
	bool DeleteUser(CUUID userId, string & msg);
	virtual ~UsersService();
};

#endif /* SRC_SERVICES_USERSSERVICE_H_ */
