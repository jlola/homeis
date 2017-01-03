/*
 * ModbusService.cpp
 *
 *  Created on: 8. 11. 2016
 *      Author: pc
 */

#include "document.h"		// rapidjson's DOM-style API
#include "prettywriter.h"
#include "stringbuffer.h"
#include <Services/ModbusService.h>

using namespace rapidjson;

ModbusService::ModbusService(ModbusManager* mm)
{
	this->mm = mm;
}

void ModbusService::render_GET(const http_request& req, http_response** res)
{
	//api/modbus/holdings/{connectorname}/{devaddress}/{baseaddress}/{count}
	string connectorName = req.get_arg("connectorname");
	string strDevAddress = req.get_arg("devaddress");
	string strBaseAddress = req.get_arg("baseaddress");
	string strCount = req.get_arg("value");

	int devAddress = Converter::stoi(strDevAddress);
	int baseAddress = Converter::stoi(strBaseAddress);
	int count = Converter::stoi(strCount);

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
		}
		else
		{
			document.SetObject();

			jsonvalue.SetObject();
			jsonvalue.SetString("Error",document.GetAllocator());
			document.AddMember("Result",jsonvalue,document.GetAllocator());
			jsonvalue.SetString("Modbus getHoldings failed",document.GetAllocator());
			document.AddMember("Message",jsonvalue,document.GetAllocator());
		}

		delete[] target;
		target = NULL;
	}
	else
	{
		document.SetObject();
		jsonvalue.SetString("Error",document.GetAllocator());
		document.AddMember("Result",jsonvalue,document.GetAllocator());
		jsonvalue.SetString("Connector not found",document.GetAllocator());
		document.AddMember("Message",jsonvalue,document.GetAllocator());
	}

	document.Accept(wr);
	std::string json = buffer.GetString();
	*res = new http_response(http_response_builder(json, 200,"application/json").string_response());
}

void ModbusService::render_PUT(const http_request& req, http_response** res)
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
			document.AddMember("Result",jsonvalue,document.GetAllocator());
			jsonvalue.SetString("Modbus setHolding failed",document.GetAllocator());
			document.AddMember("Message",jsonvalue,document.GetAllocator());
		}
	}
	else
	{
		jsonvalue.SetString("Error",document.GetAllocator());
		document.AddMember("Result",jsonvalue,document.GetAllocator());
		jsonvalue.SetString("Connector not found",document.GetAllocator());
		document.AddMember("Message",jsonvalue,document.GetAllocator());
	}
	document.Accept(wr);
	std::string json = buffer.GetString();
	*res = new http_response(http_response_builder(json, 200,"application/json").string_response());
}

ModbusService::~ModbusService() {
}

