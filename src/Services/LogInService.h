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

class LoginService : public http_resource
{
public:
	LoginService(void);
	~LoginService(void);
	//void handle( LogicalConnection* pClient, IncomingPacket* pRequest );
	const std::shared_ptr<http_response> render_GET(const http_request&);
	const std::shared_ptr<http_response> render_POST(const http_request& r);
	const std::shared_ptr<http_response> render_PUT(const http_request& r);

};

#endif /* LOGINSERVICE_H_ */
