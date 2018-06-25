/*
 * ModbusDeviceService.h
 *
 *  Created on: 21. 10. 2016
 *      Author: pc
 */

#ifndef SRC_SERVICES_MODBUSDEVICESERVICE_H_
#define SRC_SERVICES_MODBUSDEVICESERVICE_H_

#include <httpserver.hpp>
#include <Modbus/ModbusProvider.h>
#include "document.h"		// rapidjson's DOM-style API
#include "prettywriter.h"
#include "HisDevices.h"
#include "HisDevModbus.h"
#include "HttpHeadersProvider.h"
#include "IHisDevFactory.h"

using namespace std;
using namespace httpserver;
using namespace rapidjson;

class ModbusDeviceService : public http_resource {
	IHttpHeadersProvider & headersProvider;
	HisDevices *devices;
	IModbusProvider* mm;
	IHisDevFactory* factory;
public:
	ModbusDeviceService(HisDevices *devices
			,IModbusProvider* mm
			,IHttpHeadersProvider & headersProvider
			,IHisDevFactory* factory,
			webserver* ws_i);
	virtual ~ModbusDeviceService();
	const http_response render_GET(const http_request& req);
};

#endif /* SRC_SERVICES_MODBUSDEVICESERVICE_H_ */
