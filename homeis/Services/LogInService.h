/*
 * LogInService.h
 *
 *  Created on: Jan 7, 2013
 *      Author: root
 */

#ifndef LOGINSERVICE_H_
#define LOGINSERVICE_H_

#include <httpserver.hpp>

using namespace std;
using namespace httpserver;

class LoginService : public http_resource<LoginService>
{
public:
	LoginService(void);
	~LoginService(void);
	//void handle( LogicalConnection* pClient, IncomingPacket* pRequest );
	void render_GET(const http_request&, http_response**);
	void render_POST(const http_request& r, http_response** res);
	void render_PUT(const http_request& r, http_response** res);

};

#endif /* LOGINSERVICE_H_ */
