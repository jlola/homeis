/*
 * UsersService.cpp
 *
 *  Created on: 29. 10. 2018
 *      Author: pc
 */
#include <string.h>
#include <httpserver.hpp>
#include "PoppyDebugTools.h"
#include "microhttpd.h"
#include "HisException.h"
#include "IUser.h"
#include "UsersService.h"
#include "User.h"
#include "StringBuilder.h"



UsersService::UsersService(webserver* ws_i, IUserManager* userManager, IHisDevFactory* factory)
	: ServiceBase::ServiceBase(factory, userManager)
{
	this->userManager = userManager;
	ws_i->register_resource(string(URL_LOGIN), this, true);
	ws_i->register_resource(string("api/users/logout"), this, true);
	ws_i->register_resource(string("api/users"), this, true);
	ws_i->register_resource(string("api/users/{userId}"), this, true);
}

const http_response UsersService::GET(const http_request& req)
{
	STACK
	Document respjsondoc;
	respjsondoc.SetObject();
	int response_code = MHD_HTTP_FORBIDDEN;

	string strusreid = req.get_arg(JSON_USERID);

	string path = req.get_path();
	IUser* user = NULL;

	//vrati vsechny polozky ve slozce
	if (path.find("/api/users")!=string::npos)
	{
		if (!strusreid.empty())
		{
			try
			{
				CUUID id = CUUID::Parse(strusreid);
				int index = userManager->FindUser(id);
				if (index>=0)
					user = userManager->GetUser(index);
			}
			catch(...)
			{
				user = NULL;
			}
			if (user!=NULL)
			{
				respjsondoc.SetObject();
				UserToJson(user,respjsondoc,respjsondoc);
				response_code = MHD_HTTP_OK;
			}
		}
		else
		{
			respjsondoc.SetArray();
			for(size_t i=0;i<userManager->Size();i++)
			{
				Value devjson(kObjectType);
				UserToJson(userManager->GetUser(i),devjson,respjsondoc);
				respjsondoc.PushBack(devjson,respjsondoc.GetAllocator());
			}
			response_code = MHD_HTTP_OK;
		}
	}
	else
	{
		response_code = MHD_HTTP_NOT_FOUND;
	}

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	respjsondoc.Accept(wr);
	const std::string json(buffer.GetString());

	return CreateResponseString(json,response_code);
}

void UsersService::UserToJson(IUser* user, Value & userJson, Document & respjsondoc)
{
	STACK
	IHisBase* userBase = dynamic_cast<IHisBase*>(user);
	string strvalue;
	Value jsonvalue;

	strvalue = userBase->GetRecordId().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	userJson.AddMember(JSON_ID,jsonvalue, respjsondoc.GetAllocator());

	strvalue = user->GetUserName();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	userJson.AddMember(JSON_USERNAME,jsonvalue, respjsondoc.GetAllocator());

	strvalue = user->GetFirstName();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	userJson.AddMember(JSON_FIRSTNAME,jsonvalue, respjsondoc.GetAllocator());

	strvalue = user->GetLastName();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	userJson.AddMember(JSON_LASTNAME,jsonvalue, respjsondoc.GetAllocator());
}


const http_response UsersService::POST(const http_request& req)
{
	STACK
	std::string content = req.get_content();
	string path = req.get_path();
	string message;
	int response_code = MHD_HTTP_FORBIDDEN;

	if (path.find(URL_LOGIN)!=string::npos)
	{
		string sessionHash;
		auto result = Authentize(req.get_content(),req.get_requestor(), sessionHash);
		if (result)
		{
			message = CreateResponse(sessionHash);
			response_code = MHD_HTTP_OK;
		}
		else
		{
			response_code = MHD_HTTP_UNAUTHORIZED;
		}
	}
	else if (path.find("/api/users")!=string::npos)
	{
		auto userName = GetUserNameFromJson(content);
		auto user = userManager->FindByUserName(userName);
		if (user==NULL)
		{
			user = new User(GetFactory(),userName);
			UpdateUser(user,content);
			userManager->Add(user);
			response_code = MHD_HTTP_OK;
		}
		else
		{
			message = GetErrorMessageJson(StringBuilder::Format("User %s already exists",user->GetUserName().c_str()));
			response_code = MHD_HTTP_NOT_FOUND;
		}
	}

	return CreateResponseString(message,response_code);
}

string UsersService::GetUserNameFromJson(string content)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	if (document.Parse<0>((char*)content.c_str()).HasParseError())
		throw "Wrong data";

	string userName;
	if (document.HasMember(JSON_USERNAME) && document[JSON_USERNAME].IsString())
	{
		userName = document[JSON_USERNAME].GetString();
	}
	return userName;
}

const http_response UsersService::PUT(const http_request& req)
{
	STACK
	std::string content = req.get_content();
	string path = req.get_path();
	string message;
	int response_code = MHD_HTTP_FORBIDDEN;


	if (path.find("/api/users")!=string::npos)
	{
		string strUserId = req.get_arg("userid");
		CUUID userId = CUUID::Parse(strUserId);
		auto index = userManager->FindUser(userId);
		auto user = userManager->GetUser(index);
		if (UpdateUser(user,content))
		{
			response_code = MHD_HTTP_OK;
			message = "OK";
		}
	}

	return CreateResponseString(message,response_code);
}

string UsersService::CreateResponse(string sessionHash)
{
	STACK
	Document respjsondoc;
	respjsondoc.SetObject();
	Value jsonvalue;

	jsonvalue.SetString(sessionHash.c_str(),sessionHash.length(),respjsondoc.GetAllocator());
	respjsondoc.AddMember(JSON_SESSIONHASH,jsonvalue, respjsondoc.GetAllocator());

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	respjsondoc.Accept(wr);
	const std::string json(buffer.GetString());

	return json;
}

bool UsersService::Authentize(string strjson, string ip, string & sessionHash)
{
	STACK
	Document document;
	if (document.Parse<0>((char*)strjson.c_str()).HasParseError())
		return false;

	string userName;
	string password;
	string sessionId;

	if (document.HasMember(JSON_USERNAME) && document[JSON_USERNAME].IsString())
	{
		userName = document[JSON_USERNAME].GetString();
	}
	if (document.HasMember(JSON_PASSWORD) && document[JSON_PASSWORD].IsString())
	{
		password = document[JSON_PASSWORD].GetString();
	}
	if (document.HasMember(JSON_SESSIONID) && document[JSON_SESSIONID].IsString())
	{
		sessionId = document[JSON_SESSIONID].GetString();
	}

	auto result = userManager->Authentize(userName,password,sessionId,ip,sessionHash,DateTime::Now());

	return result;
}

const http_response UsersService::DELETE(const http_request& req)
{
	STACK
	std::string content = req.get_content();
	string path = req.get_path();
	int response_code = MHD_HTTP_FORBIDDEN;
	string msg;


	if (path.find("/api/users")!=string::npos)
	{
		string strUserId = req.get_arg("id");
		CUUID userId = CUUID::Parse(strUserId);
		if (DeleteUser(userId,msg))
		{
			msg = "OK";
			response_code = MHD_HTTP_OK;
		}
	}

	string json = GetFactory()->GetHeadersProvider()->GetMessageJson(msg);
	return CreateResponseString(json,response_code);
}

bool UsersService::DeleteUser(CUUID userId, string & msg)
{
	STACK
	auto index = userManager->FindUser(userId);
	if ( index >= 0 )
	{
		auto user = userManager->Delete(index);
		delete user;
		user = NULL;
		return true;
	}

	return false;
}

bool UsersService::UpdateUser(IUser* user, string strjson)
{
	STACK
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.

	if (document.Parse<0>((char*)strjson.c_str()).HasParseError())
		return false;
	bool saveReq = false;

		if (document.HasMember(JSON_USERNAME) && document[JSON_USERNAME].IsString())
		{
			string name = document[JSON_USERNAME].GetString();
			if (name!=user->GetUserName())
			{
				user->SetUserName(name);
				saveReq = true;
			}
		}
		if (document.HasMember(JSON_FIRSTNAME) && document[JSON_FIRSTNAME].IsString())
		{
			string firstname = document[JSON_FIRSTNAME].GetString();
			if (firstname!=user->GetFirstName())
			{
				user->SetFirstName(firstname);
				saveReq = true;
			}
		}
		if (document.HasMember(JSON_LASTNAME) && document[JSON_LASTNAME].IsString())
		{
			string lastname = document[JSON_LASTNAME].GetString();
			if (lastname!=user->GetLastName())
			{
				user->SetLastName(lastname);
				saveReq = true;
			}
		}
		if (document.HasMember(JSON_PASSWORD) && document[JSON_PASSWORD].IsString())
		{
			string password = document[JSON_PASSWORD].GetString();
			if (!user->IsPasswordValid(user->sha256(password)))
			{
				user->SetPassword(password);
				saveReq = true;
			}
		}

		if (saveReq) userManager->Save();

		return true;
}


UsersService::~UsersService() {
}

