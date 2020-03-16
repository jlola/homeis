/*
 * ModbusAtom.cpp
 *
 *  Created on: 17. 6. 2018
 *      Author: pc
 */


#ifdef HOMEISTEST

#include <string>
#include "StringBuilder.h"
#include "ModbusAtom.h"


ModbusAtom::ModbusAtom(HomeIsStarter & homeisStarter)
: homeisStarter(homeisStarter) {
}

string ModbusAtom::Scan(string driverName, int address)
{
	//api/modbus/scan/{connectorname}/{address}
	string response;
	std::string scandevice = StringBuilder::Format("api/modbus/scan/%s/%d",driverName.c_str(),address);
	urlCode = homeisStarter.GetClient().Get(scandevice,response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);
	return response;
}


ModbusAtom::~ModbusAtom() {
}

#endif
