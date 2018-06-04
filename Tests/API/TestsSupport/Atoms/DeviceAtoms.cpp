/*
 * DeviceAtoms.cpp
 *
 *  Created on: 1. 6. 2018
 *      Author: pc
 */

#ifdef HOMEISTEST

#include "DeviceAtoms.h"
#include "StringBuilder.h"

DeviceAtoms::DeviceAtoms(HomeIsStarter & homeisStarter)
	: homeisStarter(homeisStarter)
{
}

string DeviceAtoms::CreateDevice(string name)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	document.SetObject();

	document.AddMember(JSON_NAME,name.c_str(),document.GetAllocator());

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	document.Accept(wr);
	const std::string json(buffer.GetString());
	string response;
	urlCode = homeisStarter.GetClient().Post("api/devices",json,response,http_code);
	AssertCurlResponse();
	return response;
}

string DeviceAtoms::GetDevices()
{
	string response;
	urlCode = homeisStarter.GetClient().Get("api/devices",response,http_code);
	AssertCurlResponse();
	return response;
}

void DeviceAtoms::AssertCurlResponse()
{
	if (CURLcode::CURLE_OK != urlCode)
		throw StringBuilder::Format("Assert urlCode: Expected: %d, Actual: %d",CURLcode::CURLE_OK,urlCode).c_str();
	if (MHD_HTTP_OK != http_code)
		throw StringBuilder::Format("Assert http_code: Expected: %d, Actual: %d",MHD_HTTP_OK,http_code).c_str();
}

string DeviceAtoms::CreateEmailTag(string deviceId, string name)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	document.SetObject();

//	{
//	  "parentid":"b974e7b0-9ede-4f36-8b1a-ef335057fb50",
//	  "name":"email tag",
//	  "type":5,
//	  "logged":true,
//	  "internal":true,
//	  "force":false,
//	  "direction":0,
//	  "enabled":true,
//	  "sender": "name@email.cz",
//	  "receivers":"name1@email.cz;name2@email.cz",
//	  "setOutputState":0,
//	  "error":false
//	}

	string sender = "sender";
	string receivers = "receiver1;receiver2";

	document.AddMember(JSON_PARENTID,deviceId.c_str(),document.GetAllocator());
	document.AddMember(JSON_NAME,name.c_str(),document.GetAllocator());
	document.AddMember(JSON_TYPE,EDataType::Email,document.GetAllocator());
	document.AddMember(JSON_INTERNAL,true,document.GetAllocator());
	document.AddMember(JSON_ENABLED,true,document.GetAllocator());
	document.AddMember(JSON_SENDER,sender.c_str(),document.GetAllocator());
	document.AddMember(JSON_RECEIVERS,receivers.c_str(),document.GetAllocator());

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	document.Accept(wr);
	const std::string json(buffer.GetString());
	string response;
	urlCode = homeisStarter.GetClient().Post("api/devices/devvalue/",json,response,http_code);
	AssertCurlResponse();
	return response;
}

DeviceAtoms::~DeviceAtoms() {
}

#endif
