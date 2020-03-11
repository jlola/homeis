/*
 * ModbusDeviceService.cpp
 *
 *  Created on: 21. 10. 2016
 *      Author: pc
 */

#include "document.h"		// rapidjson's DOM-style API
#include "PoppyDebugTools.h"
#include "Services/ModbusDeviceService.h"
#include "StringBuilder.h"
#include "converter.h"
#include "microhttpd.h"
#include "ParamsNames.h"


ModbusDeviceService::ModbusDeviceService(HisDevices* devices,
		IModbusProvider* mm,
		IUserManager* userManager,
		IHisDevFactory* factory,
		webserver* ws_i)
	: ServiceBase::ServiceBase(factory,userManager),
	  factory(factory)
{
	this->mm = mm;
	this->devices = devices;

	ws_i->register_resource("api/modbus/scan/{connectorname}/{address}", this, true);
}

ModbusDeviceService::~ModbusDeviceService()
{

}

const std::shared_ptr<http_response> ModbusDeviceService::GET(const http_request& req)
{
	STACK
	int response_code = MHD_HTTP_FORBIDDEN;
	Document document;
	document.SetObject();
	Value jsonvalue;

	string connector = req.get_arg("connectorname");
	string strid = req.get_arg("address");
	int address = Converter::stoui(strid,10);

	int index = devices->FindModbusDev(address);
	if (index>=0)
	{
		HisDevModbus* dev = reinterpret_cast<HisDevModbus*>(devices->operator [](index));
		if (dev!=NULL)
		{
			dev->Scan(true);
			devices->Save();
		}

		document.SetObject();
		jsonvalue.SetString("Success",document.GetAllocator());
		document.AddMember(JSON_RESULT,jsonvalue,document.GetAllocator());
		response_code = MHD_HTTP_OK;
	}
	else
	{
		IModbus* m = mm->Find(connector);
		if (m!=NULL)
		{
			HisDevModbus* dev = new HisDevModbus(m,address,factory);
			dev->Scan(true);
			devices->Add(dev);
			devices->Save();

			document.SetObject();
			jsonvalue.SetString("Success",document.GetAllocator());
			document.AddMember(JSON_RESULT,jsonvalue,document.GetAllocator());
			response_code = MHD_HTTP_OK;
		}
		else
		{
			document.SetObject();
			jsonvalue.SetString("Error",document.GetAllocator());
			document.AddMember(JSON_RESULT,jsonvalue,document.GetAllocator());
			jsonvalue.SetString("Connector not found",document.GetAllocator());
			document.AddMember(JSON_MESSAGE,jsonvalue,document.GetAllocator());
			response_code = MHD_HTTP_NOT_FOUND;
		}

	}

	std::string json = DocumentToString(document);

	return CreateResponseString(json,response_code);
}
