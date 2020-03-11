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
#include "ServiceBase.h"

using namespace std;
using namespace httpserver;
using namespace rapidjson;

class ModbusDeviceService : public ServiceBase {
	HisDevices *devices;
	IModbusProvider* mm;
	IHisDevFactory* factory;
public:
	ModbusDeviceService(HisDevices *devices,
			IModbusProvider* mm,
			IUserManager* userManager,
			IHisDevFactory* factory,
			webserver* ws_i);
	virtual ~ModbusDeviceService();
	const std::shared_ptr<http_response> GET(const http_request& req);
};

#endif /* SRC_SERVICES_MODBUSDEVICESERVICE_H_ */
