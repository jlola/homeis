/*
 * ModbusService.h
 *
 *  Created on: 8. 11. 2016
 *      Author: pc
 */

#ifndef SRC_SERVICES_MODBUSSERVICE_H_
#define SRC_SERVICES_MODBUSSERVICE_H_

#include <httpserver.hpp>
#include <Modbus/IModbusProvider.h>
#include "document.h"		// rapidjson's DOM-style API
#include "prettywriter.h"
#include "HisDevices.h"
#include "HttpHeadersProvider.h"

using namespace std;
using namespace httpserver;
using namespace rapidjson;

class ModbusService : public http_resource
{
	IHttpHeadersProvider & headersProvider;
	IModbusProvider* mm;
public:
	ModbusService(IModbusProvider* mm, IHttpHeadersProvider & headersProvider);
	void render_GET(const http_request& req, http_response** res);
	const http_response render_GET(const http_request& req);
	const http_response render_PUT(const http_request& req);
	virtual ~ModbusService();
};

#endif /* SRC_SERVICES_MODBUSSERVICE_H_ */
