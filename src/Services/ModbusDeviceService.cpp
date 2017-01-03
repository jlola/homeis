/*
 * ModbusDeviceService.cpp
 *
 *  Created on: 21. 10. 2016
 *      Author: pc
 */

#include "document.h"		// rapidjson's DOM-style API
#include "prettywriter.h"
#include "stringbuffer.h"

#include "PoppyDebugTools.h"
#include "Services/ModbusDeviceService.h"
#include "StringBuilder.h"
#include "converter.h"


ModbusDeviceService::ModbusDeviceService(HisDevices* devices,ModbusManager* mm)
{
	this->mm = mm;
	this->devices = devices;
}

ModbusDeviceService::~ModbusDeviceService()
{

}

void ModbusDeviceService::render_GET(const http_request& req, http_response** res)
{
	STACK
	Document document;
	document.SetObject();

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	Value jsonvalue;

	string connector = req.get_arg("connectorname");
	string strid = req.get_arg("address");
	int address = Converter::stoui(strid,10);

	int index = devices->FindModbusDev(address);
	if (index>0)
	{
		HisDevModbus* dev = reinterpret_cast<HisDevModbus*>(devices->operator [](index));
		if (dev!=NULL)
		{
			dev->Scan(true);
			devices->Save();
		}

//		document.SetObject();
//		jsonvalue.SetObject();
//		jsonvalue.SetString("Error",document.GetAllocator());
//		document.AddMember("Result",jsonvalue,document.GetAllocator());
//		string message = StringBuilder::Format("Device address %d already exists",address);
//		jsonvalue.SetString(message.c_str(),document.GetAllocator());
//		document.AddMember("Message",jsonvalue,document.GetAllocator());

		document.SetObject();
		jsonvalue.SetString("Success",document.GetAllocator());
		document.AddMember("Result",jsonvalue,document.GetAllocator());
	}
	else
	{
		IModbus* m = mm->Find(connector);
		if (m!=NULL)
		{
			HisDevModbus* dev = new HisDevModbus(m,address);
			dev->Scan(true);
			devices->Add(dev);
			devices->Save();

			document.SetObject();
			jsonvalue.SetString("Success",document.GetAllocator());
			document.AddMember("Result",jsonvalue,document.GetAllocator());
		}
		else
		{
			document.SetObject();
			jsonvalue.SetString("Error",document.GetAllocator());
			document.AddMember("Result",jsonvalue,document.GetAllocator());
			jsonvalue.SetString("Connector not found",document.GetAllocator());
			document.AddMember("Message",jsonvalue,document.GetAllocator());
		}
	}
	document.Accept(wr);
	std::string json = buffer.GetString();
	*res = new http_response(http_response_builder(json, 200,"application/json").string_response());
}
