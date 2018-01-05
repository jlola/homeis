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
#include "microhttpd.h"


ModbusDeviceService::ModbusDeviceService(HisDevices* devices
		,IModbusProvider* mm
		,IHttpHeadersProvider & headersProvider,
		IHisDevFactory* factory)
	: headersProvider(headersProvider),factory(factory)
{
	this->mm = mm;
	this->devices = devices;
}

ModbusDeviceService::~ModbusDeviceService()
{

}

const http_response ModbusDeviceService::render_GET(const http_request& req)
{
	STACK
	int resonse_code = MHD_HTTP_FORBIDDEN;
	Document document;
	document.SetObject();

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
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
		document.AddMember("Result",jsonvalue,document.GetAllocator());
		resonse_code = MHD_HTTP_OK;
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
			document.AddMember("Result",jsonvalue,document.GetAllocator());
			resonse_code = MHD_HTTP_OK;
		}
		else
		{
			document.SetObject();
			jsonvalue.SetString("Error",document.GetAllocator());
			document.AddMember("Result",jsonvalue,document.GetAllocator());
			jsonvalue.SetString("Connector not found",document.GetAllocator());
			document.AddMember("Message",jsonvalue,document.GetAllocator());
			resonse_code = MHD_HTTP_NOT_FOUND;
		}

	}
	document.Accept(wr);
	std::string json = buffer.GetString();
	http_response_builder response_builder(json, resonse_code,headersProvider.GetContentTypeAppJson());
	headersProvider.AddHeaders(response_builder);
	http_response resp(response_builder.string_response());
	return resp;
}
