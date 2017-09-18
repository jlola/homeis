/*
 * ModbusDeviceService.h
 *
 *  Created on: 21. 10. 2016
 *      Author: pc
 */

#ifndef SRC_SERVICES_MODBUSDEVICESERVICE_H_
#define SRC_SERVICES_MODBUSDEVICESERVICE_H_

#include <httpserver.hpp>
#include "document.h"		// rapidjson's DOM-style API
#include "prettywriter.h"
#include "HisDevices.h"
#include "Modbus/ModbusManager.h"
#include "HisDevModbus.h"

using namespace std;
using namespace httpserver;
using namespace rapidjson;

class ModbusDeviceService : public http_resource {
	HisDevices *devices;
	ModbusManager* mm;
public:
	ModbusDeviceService(HisDevices *devices,ModbusManager* mm);
	virtual ~ModbusDeviceService();
	const http_response render_GET(const http_request& req);
};

#endif /* SRC_SERVICES_MODBUSDEVICESERVICE_H_ */
