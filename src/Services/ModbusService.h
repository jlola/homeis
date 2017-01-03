/*
 * ModbusService.h
 *
 *  Created on: 8. 11. 2016
 *      Author: pc
 */

#ifndef SRC_SERVICES_MODBUSSERVICE_H_
#define SRC_SERVICES_MODBUSSERVICE_H_

#include <httpserver.hpp>
#include "document.h"		// rapidjson's DOM-style API
#include "prettywriter.h"
#include "HisDevices.h"
#include "Modbus/ModbusManager.h"

using namespace std;
using namespace httpserver;
using namespace rapidjson;

class ModbusService : public http_resource
{
	ModbusManager* mm;
public:
	ModbusService(ModbusManager* mm);
	void render_GET(const http_request& req, http_response** res);
	void render_PUT(const http_request& req, http_response** res);
	virtual ~ModbusService();
};

#endif /* SRC_SERVICES_MODBUSSERVICE_H_ */
