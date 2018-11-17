/*
 * DeviceAtoms.cpp
 *
 *  Created on: 1. 6. 2018
 *      Author: pc
 */

#ifdef HOMEISTEST

#include "DevicesAtom.h"
#include "HisException.h"
#include "StringBuilder.h"
#include "OneWireHandler.h"
#include "UsersAtom.h"

DevicesAtom::DevicesAtom(HomeIsStarter & homeisStarter)
	: homeisStarter(homeisStarter)
{
}

string DevicesAtom::CreateDevice(string name)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	document.SetObject();

	document.AddMember(JSON_NAME,name.c_str(),document.GetAllocator());

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	document.Accept(wr);
	const std::string json(buffer.GetString());
	string response;

	string hashPassword;
	UsersAtom usersAtom(&homeisStarter);
	usersAtom.LoginUser("admin","admin","sessionId",hashPassword);

	urlCode = homeisStarter.GetClient().Post("api/devices",json,hashPassword,response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);
	return response;
}

string DevicesAtom::GetDeviceId()
{
	Document devicesdoc = GetDevices();
	const Value& devices = devicesdoc;
	string tagid = "";

	SizeType ob = 0;
	return devices[ob][JSON_ID].GetString();
}

string DevicesAtom::FindTagId(string deviceId, string tagName)
{
	Document devicedoc = GetDevice(deviceId);
	string tagid = "";

	deviceId = devicedoc[JSON_ID].GetString();
	const Value& tags = devicedoc[JSON_TAGS];
	for(SizeType t = 0 ;t < tags.Size();t++)
	{
		string acttagName = tags[t][JSON_NAME].GetString();
		if (acttagName==tagName)
		{
			tagid = tags[t][JSON_ID].GetString();
			break;
		}
	}
	return tagid;
}

Document DevicesAtom::GetTag(string tagId)
{
	string response;
	Document document;
	string url = StringBuilder::Format("/api/devices/devvalue/%s",tagId.c_str());
	urlCode = homeisStarter.GetClient().Get(url.c_str(),response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);
	if (document.Parse<0>((char*)response.c_str()).HasParseError() == true)
		throw ArgumentNullException("response");
	return document;
}

void DevicesAtom::UpdateDevicePropertyUint(string devid,string name,uint32_t value)
{
	Value jsonvalue;
	jsonvalue.SetUint(value);
	UpdateDeviceProperty(devid,name,jsonvalue);
}

void DevicesAtom::UpdateDeviceProperty(string deviceId,string name, Value & value)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	document.SetObject();

	document.AddMember(name.c_str(),value,document.GetAllocator());

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	document.Accept(wr);
	const std::string json(buffer.GetString());

	string response;
	string url = StringBuilder::Format("/api/devices/%s",deviceId.c_str());

	string hashPassword;
	UsersAtom usersAtom(homeisStarter.GetClient());
	usersAtom.LoginUser("admin","admin","sessionId",hashPassword);

	urlCode = homeisStarter.GetClient().Put(url,json,hashPassword,response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);
}

string DevicesAtom::GetDevicePropertyString(string deviceId,string name)
{
	string response;
	Document document = GetDevice(deviceId);
	return document[name.c_str()].GetString();
}

int DevicesAtom::GetDevicePropertyInt(string deviceId,string name)
{
	string response;
	Document document = GetDevice(deviceId);
	return document[name.c_str()].GetInt();
}

int DevicesAtom::GetDevicePropertyUInt(string deviceId,string name)
{
	string response;
	Document document = GetDevice(deviceId);
	return document[name.c_str()].GetUint();
}

Document DevicesAtom::GetDevice(string deviceId)
{
	Document document;
	string response;
	string url = StringBuilder::Format("/api/devices/%s",deviceId.c_str());
	urlCode = homeisStarter.GetClient().Get(url,response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);
	if (document.Parse<0>((char*)response.c_str()).HasParseError() == true)
		throw ArgumentNullException("response");
	return document;
}

Document DevicesAtom::GetDevices()
{
	string response;
	urlCode = homeisStarter.GetClient().Get("api/devices",response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	if (document.Parse<0>((char*)response.c_str()).HasParseError() == true)
		throw ArgumentNullException("response");
	return document;
}

string DevicesAtom::CreateEmailTag(string deviceId, string name,string sender, string receivers)
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

	string hashPassword;
	UsersAtom usersAtom(homeisStarter.GetClient());
	usersAtom.LoginUser("admin","admin","sessionId",hashPassword);

	urlCode = homeisStarter.GetClient().Post("api/devices/devvalue/",json,hashPassword,response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);
	return response;
}

void DevicesAtom::WriteToTag(string tagId,string value)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	document.SetObject();

	document.AddMember(JSON_ID,tagId.c_str(),document.GetAllocator());
	document.AddMember(JSON_VALUE,value.c_str(),document.GetAllocator());

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	document.Accept(wr);
	const std::string json(buffer.GetString());
	string response;
	string url = StringBuilder::Format("api/devices/devvalue/%s",tagId.c_str());

	string hashPassword;
	UsersAtom usersAtom(homeisStarter.GetClient());
	usersAtom.LoginUser("admin","admin","sessionId",hashPassword);

	urlCode = homeisStarter.GetClient().Put(url.c_str(),json,hashPassword,response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);
}

DevicesAtom::~DevicesAtom() {
}

#endif
