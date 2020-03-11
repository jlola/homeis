/*
 * LogInService.cpp
 *
 *  Created on: Jan 7, 2013
 *      Author: root
 */
#include "document.h"		// rapidjson's DOM-style API
#include "prettywriter.h"
#include "stringbuffer.h"
#include "filestream.h"	// wrapper of C stream for prettywriter as output

#include "LogInService.h"

using namespace rapidjson;

LoginService::LoginService(void)
{
}


LoginService::~LoginService(void)
{
}

const std::shared_ptr<http_response> LoginService::render_GET(const http_request&)
{
	return std::shared_ptr<http_response>(new http_response());
}

const std::shared_ptr<http_response> LoginService::render_POST(const http_request& r)
{
	return std::shared_ptr<http_response>(new http_response());
}

const std::shared_ptr<http_response> LoginService::render_PUT(const http_request& r)
{
	return std::shared_ptr<http_response>(new http_response());
}

//void LoginService::handle( LogicalConnection* pClient, IncomingPacket* pRequest )
//{
//	WebsocketDataMessage& request = (WebsocketDataMessage&)(*pRequest);
//	WebsocketClient& client = (WebsocketClient&) (*pClient);
//
//	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
//	Document responsedoc;
//
//	responsedoc.SetObject();
//
//	if (document.Parse<0>((char*)request.GetData().c_str()).HasParseError())//ParseInsitu<0>((char*)raw.c_str()).HasParseError())
//			return;
//
//	WebsocketDataMessage response(eLoginService);
//
//	if (document.HasMember("Command"))
//	{
//		std::string command = document["Command"].GetString();
//		if (command == "Login")
//		{
//			std::string login = document["UserName"].GetString();
//			std::string password = document["Password"].GetString();
//			if (client.Login( login, password))
//			{
//				responsedoc.AddMember("LogResult",true,responsedoc.GetAllocator());
//			}
//			else
//			{
//				responsedoc.AddMember("LogResult",false,responsedoc.GetAllocator());
//			}
//			responsedoc.AddMember("Command","Login",responsedoc.GetAllocator());
//
//			StringBuffer buffer;
//			PrettyWriter<StringBuffer> wr(buffer);
//			responsedoc.Accept(wr);
//			std::string json = buffer.GetString();
//
//			response.SetData(json);
//		}
//		if (command == "Logout")
//		{
//			client.Logout();
//
//			responsedoc.AddMember("LogResult",true,responsedoc.GetAllocator());
//			responsedoc.AddMember("Command","Logout",responsedoc.GetAllocator());
//
//			StringBuffer buffer;
//			PrettyWriter<StringBuffer> wr(buffer);
//			responsedoc.Accept(wr);
//			std::string json = buffer.GetString();
//
//			response.SetData(json);
//		}
//
//		if (command=="IsLogged")
//		{
//			responsedoc.AddMember("LogResult",true,responsedoc.GetAllocator());
//			responsedoc.AddMember("Command","IsLogged",responsedoc.GetAllocator());
//
//			StringBuffer buffer;
//			PrettyWriter<StringBuffer> wr(buffer);
//			responsedoc.Accept(wr);
//			std::string json = buffer.GetString();
//
//			response.SetData(json);
//		}
//	}
//
//
//	client.PushPacket(&response);
//}
