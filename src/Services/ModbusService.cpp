/*
 * ModbusService.cpp
 *
 *  Created on: 8. 11. 2016
 *      Author: pc
 */

#include "document.h"		// rapidjson's DOM-style API
#include "prettywriter.h"
#include "stringbuffer.h"
#include "microhttpd.h"
#include "ModbusService.h"
#include "ParamsNames.h"

using namespace rapidjson;

ModbusService::ModbusService(IModbusProvider* mm,
		IUserManager* userManager,
		IHisDevFactory* factory,
		webserver* ws_i)
	: ServiceBase::ServiceBase(factory,userManager)
{
	this->mm = mm;
	ws_i->register_resource("api/modbus/registers/{connectorname}/{devaddress}/{baseaddress}/{value}", this, true);
}

const http_response ModbusService::GET(const http_request& req)
{
	//api/modbus/holdings/{connectorname}/{devaddress}/{baseaddress}/{count}
	string connectorName = req.get_arg("connectorname");
	string strDevAddress = req.get_arg("devaddress");
	string strBaseAddress = req.get_arg("baseaddress");
	string strCount = req.get_arg("value");

	int devAddress = Converter::stoi(strDevAddress);
	int baseAddress = Converter::stoi(strBaseAddress);
	int count = Converter::stoi(strCount);

	int response_code = MHD_HTTP_FORBIDDEN;
	Document document;
	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	Value jsonvalue;

	IModbus* modbus = mm->Find(connectorName);
	if (modbus!=NULL)
	{
		uint16_t* target = new uint16_t[count];
		if (modbus->getHoldings(devAddress,baseAddress,count,target))
		{
			document.SetArray();
			for(int i=0;i<count;i++)
			{
				Value jsonvalue;
				jsonvalue.SetUint(target[i]);
				document.PushBack(jsonvalue, document.GetAllocator());
			}
			response_code = MHD_HTTP_OK;
		}
		else
		{
			response_code = MHD_HTTP_FORBIDDEN;
			document.SetObject();
			jsonvalue.SetObject();
			jsonvalue.SetString("Error",document.GetAllocator());
			document.AddMember(JSON_RESULT,jsonvalue,document.GetAllocator());
			jsonvalue.SetString("Modbus getHoldings failed",document.GetAllocator());
			document.AddMember(JSON_MESSAGE,jsonvalue,document.GetAllocator());
		}

		delete[] target;
		target = NULL;
	}
	else
	{
		document.SetObject();
		jsonvalue.SetString("Error",document.GetAllocator());
		document.AddMember(JSON_RESULT,jsonvalue,document.GetAllocator());
		jsonvalue.SetString("Connector not found",document.GetAllocator());
		document.AddMember(JSON_MESSAGE,jsonvalue,document.GetAllocator());
	}

	document.Accept(wr);
	std::string json = buffer.GetString();

	return CreateResponseString(json,response_code);
}

const http_response ModbusService::PUT(const http_request& req)
{
	string connectorName = req.get_arg("connectorname");
	string strDevAddress = req.get_arg("devaddress");
	string strBaseAddress = req.get_arg("baseaddress");
	string strValue = req.get_arg("value");

	int devAddress = Converter::stoi(strDevAddress);
	int baseAddress = Converter::stoi(strBaseAddress);
	int value = Converter::stoi(strValue);

	Document document;
	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	document.SetObject();
	Value jsonvalue;

	IModbus* modbus = mm->Find(connectorName);
	if (modbus!=NULL)
	{

		if (modbus->setHolding(devAddress,baseAddress,value))
		{
			jsonvalue.SetString("OK",document.GetAllocator());
			document.AddMember("Result",jsonvalue,document.GetAllocator());
		}
		else
		{
			jsonvalue.SetString("Error",document.GetAllocator());
			document.AddMember(JSON_RESULT,jsonvalue,document.GetAllocator());
			jsonvalue.SetString("Modbus setHolding failed",document.GetAllocator());
			document.AddMember(JSON_MESSAGE,jsonvalue,document.GetAllocator());
		}
	}
	else
	{
		jsonvalue.SetString("Error",document.GetAllocator());
		document.AddMember(JSON_RESULT,jsonvalue,document.GetAllocator());
		jsonvalue.SetString("Connector not found",document.GetAllocator());
		document.AddMember(JSON_MESSAGE,jsonvalue,document.GetAllocator());
	}
	document.Accept(wr);
	std::string json = buffer.GetString();

	return CreateResponseString(json, MHD_HTTP_OK);
}

ModbusService::~ModbusService() {
}

