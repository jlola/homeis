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
	ws_i->register_resource(string("api/users/findbyname/{userName}"), this, true);
	ws_i->register_resource(string("api/users/changepassword"), this, true);
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

	if (path.find("/api/users/findbyname")!=string::npos)
	{
		string userName = req.get_arg("userName");
		auto user = userManager->FindByUserName(userName);
		if (user!=NULL)
		{
			UserToJson(user,respjsondoc,respjsondoc);
			response_code = MHD_HTTP_OK;
		}
		else
		{
			response_code = MHD_HTTP_NOT_FOUND;
		}
	}
	//vrati vsechny polozky ve slozce
	else if (path.find("/api/users")!=string::npos)
	{
		if (!strusreid.empty())
		{
			try
			{
				CUUID id = CUUID::Parse(strusreid);
				user = userManager->FindUser(id);
				if (user!=NULL)
				{
					respjsondoc.SetObject();
					UserToJson(user,respjsondoc,respjsondoc);
					response_code = MHD_HTTP_OK;
				}
			}
			catch(...)
			{
				response_code = MHD_HTTP_BAD_REQUEST;
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
		response_code = MHD_HTTP_BAD_REQUEST;
	}

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	respjsondoc.Accept(wr);
	const std::string json(buffer.GetString());

	return CreateResponseString(json,response_code);
}

void UsersService::UserToStringJson(IUser* user, string & json)
{
	Document respjsondoc;
	respjsondoc.SetObject();
	UserToJson(user,respjsondoc,respjsondoc);
	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	respjsondoc.Accept(wr);
	json = buffer.GetString();
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
	else if (path.find("/api/users/logout")!=string::npos)
	{
		if (Logout(req.get_content(),req.get_requestor()))
		{
			response_code = MHD_HTTP_OK;
		}
		else
		{
			response_code = MHD_HTTP_NOT_FOUND;
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
			UserToStringJson(user,message);
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

	if (path.find("api/users/changepassword")!=string::npos)
	{
		try
		{
			if (SetPassword(content))
			{
				response_code = MHD_HTTP_OK;
			}
		}
		catch(...)
		{
			response_code = MHD_HTTP_BAD_REQUEST;
		}
	}
	else if (path.find("/api/users")!=string::npos)
	{
		string strUserId = req.get_arg("userId");
		try
		{
			CUUID userId = CUUID::Parse(strUserId);
			auto user = userManager->FindUser(userId);
			if (UpdateUser(user,content))
			{
				response_code = MHD_HTTP_OK;
				UserToStringJson(user,message);
				response_code = MHD_HTTP_OK;
			}
		}
		catch(...)
		{
			response_code = MHD_HTTP_BAD_REQUEST;
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

bool UsersService::Logout(string strjson, string ip)
{
	STACK
	Document document;
	if (!document.Parse<0>((char*)strjson.c_str()).HasParseError())
	{
		if (document.HasMember(JSON_SESSIONHASH) && document[JSON_SESSIONHASH].IsString())
		{
			auto sessionHash = document[JSON_SESSIONHASH].GetString();
			auto result = userManager->LogSessionOut(sessionHash,ip);
			return result;
		}
	}

	return false;
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
		string strUserId = req.get_arg("userId");
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
	auto user = userManager->FindUser(userId);
	if ( user!= NULL )
	{
		CUUID adminid = userManager->GetAdmin()->GetRecordId();
		CUUID loggeduserId = GetSession()->GetUser()->GetRecordId();
		if (!userManager->IsUserLogged(user) &&	adminid==loggeduserId)
		{
			userManager->Delete(user);
			delete user;
			user = NULL;
			return true;
		}
		else
		{
			msg = "User is logged in and can not be deleted just now";
			return false;
		}
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

		if (saveReq) userManager->Save();

		return true;
}

bool UsersService::SetPassword(string strjson)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.

	if (document.Parse<0>((char*)strjson.c_str()).HasParseError())
		return false;
	IUser* user = NULL;
	string newPassword;
	string oldPassword;

	if (document.HasMember(JSON_ID) && document[JSON_ID].IsString())
	{
		string strid = document[JSON_ID].GetString();
		CUUID userid = CUUID::Parse(strid);
		user = userManager->FindUser(userid);

		if (user!=NULL)
		{
			if (document.HasMember(JSON_NEWPASSWORD) && document[JSON_NEWPASSWORD].IsString())
			{
				newPassword = document[JSON_NEWPASSWORD].GetString();
			}
			if (document.HasMember(JSON_OLDPASSWORD) && document[JSON_OLDPASSWORD].IsString())
			{
				oldPassword = document[JSON_OLDPASSWORD].GetString();
			}

			bool loggedAdmin = GetSession()->GetUser()->IsAdmin();
			if (user->SetPassword(oldPassword,newPassword,loggedAdmin))
			{
				userManager->Save();
				return true;
			}
		}
	}

	return false;
}


UsersService::~UsersService() {
}

