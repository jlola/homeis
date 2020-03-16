/*
 * UsersAtom.cpp
 *
 *  Created on: 9. 11. 2018
 *      Author: pc
 */
#ifdef HOMEISTEST

#include <string>
#include "StringBuilder.h"
#include "HisException.h"

#include "UsersAtom.h"

UsersAtom::UsersAtom(HomeIsStarter* homeisStarter) :
	client(homeisStarter->GetClient())
{
}

UsersAtom::UsersAtom(Client & client) :
	client(client)
{
}

//returns hash
int UsersAtom::LoginUser(string userName, string password, string sessionId, string & hashPassword)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	document.SetObject();

	document.AddMember(JSON_USERNAME,userName.c_str(),document.GetAllocator());
	document.AddMember(JSON_PASSWORD,password.c_str(),document.GetAllocator());
	document.AddMember(JSON_SESSIONID,sessionId.c_str(),document.GetAllocator());

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	document.Accept(wr);
	const std::string json(buffer.GetString());
	string response;
	long http_code;

	CURLcode urlCode = client.Post("api/users/login",json,"","",response,http_code);
	if (CURLcode::CURLE_OK != urlCode)
		throw StringBuilder::Format("Assert urlCode: Expected: %d, Actual: %d",CURLcode::CURLE_OK,urlCode).c_str();

	if (http_code == MHD_HTTP_OK)
	{
		Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.

		if (document.Parse<0>((char*)response.c_str()).HasParseError() == true)
			throw ArgumentNullException("response");

		hashPassword = document[JSON_SESSIONHASH].GetString();
	}

	return http_code;
}

Document UsersAtom::CreateUser(string userName,string firstName,string lastName,string sessionHash)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	document.SetObject();

	document.AddMember(JSON_USERNAME,userName.c_str(),document.GetAllocator());
	document.AddMember(JSON_FIRSTNAME,firstName.c_str(),document.GetAllocator());
	document.AddMember(JSON_LASTNAME,lastName.c_str(),document.GetAllocator());

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	document.Accept(wr);
	const std::string json(buffer.GetString());
	string response;


	long http_code;
	CURLcode urlCode = client.Post("api/users",json,sessionHash,response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);
	Document documentResp;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	if (documentResp.Parse<0>((char*)response.c_str()).HasParseError() == true)
		throw ArgumentNullException("response");

	return documentResp;
}

string UsersAtom::UpdateUser(string userId, string userName,string firstName,string lastName,string sessionHash)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	document.SetObject();

	document.AddMember(JSON_ID,userId.c_str(),document.GetAllocator());
	document.AddMember(JSON_USERNAME,userName.c_str(),document.GetAllocator());
	document.AddMember(JSON_FIRSTNAME,firstName.c_str(),document.GetAllocator());
	document.AddMember(JSON_LASTNAME,lastName.c_str(),document.GetAllocator());

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	document.Accept(wr);
	const std::string json(buffer.GetString());
	string response;

	string url = StringBuilder::Format("api/users/%s",userId.c_str());

	long http_code;
	CURLcode urlCode = client.Put(url,json,sessionHash,response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);
	return response;
}

bool UsersAtom::DeleteUser(string userId,string loggeduser,string sessionHash)
{
	string url = StringBuilder::Format("api/users/%s",userId.c_str());

	long http_code;
	string response;
	string msg;
	CURLcode urlCode = client.Delete(url,msg,loggeduser,sessionHash,response,http_code);
	if (CURLcode::CURLE_OK != urlCode)
		throw StringBuilder::Format("Assert urlCode: Expected: %d, Actual: %d",CURLcode::CURLE_OK,urlCode).c_str();
	if (http_code==MHD_HTTP_OK)
		return true;
	return false;
}

void UsersAtom::UpdatePassword(string userId, string oldPassword,string newPassword,string sessionHash)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	document.SetObject();

	document.AddMember(JSON_ID,userId.c_str(),document.GetAllocator());
	document.AddMember(JSON_OLDPASSWORD,oldPassword.c_str(),document.GetAllocator());
	document.AddMember(JSON_NEWPASSWORD,newPassword.c_str(),document.GetAllocator());

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	document.Accept(wr);
	const std::string json(buffer.GetString());
	string response;

	string url = "api/users/changepassword";

	long http_code;
	CURLcode urlCode = client.Put(url,json,sessionHash,response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);
}

void UsersAtom::LogOutUser(string sessionHash)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	document.SetObject();

	document.AddMember(JSON_SESSIONHASH,sessionHash.c_str(),document.GetAllocator());

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	document.Accept(wr);
	const std::string json(buffer.GetString());
	string url = "api/users/logout";
	string response;
	long http_code;
	CURLcode urlCode = client.Post(url,json,sessionHash,response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);
}

Document UsersAtom::GetUser(CUUID userId)
{
	long http_code;
	CURLcode urlCode;
	string response;
	urlCode = client.Get(StringBuilder::Format("api/users/%s",userId.ToString().c_str()),response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);

	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	if (document.Parse<0>((char*)response.c_str()).HasParseError() == true)
		throw ArgumentNullException("response");

	return document;
}

Document UsersAtom::FindUser(string userName)
{
	long http_code;
	CURLcode urlCode;
	string response;
	urlCode = client.Get(StringBuilder::Format("api/users/findbyname/%s",userName.c_str()),response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);

	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	if (document.Parse<0>((char*)response.c_str()).HasParseError() == true)
		throw ArgumentNullException("response");

	return document;
}

Document UsersAtom::GetUsers()
{
	long http_code;
	CURLcode urlCode;
	string response;
	urlCode = client.Get("api/users",response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);

	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	printf(response.c_str());
	if (document.Parse<0>((char*)response.c_str()).HasParseError() == true)
		throw ArgumentNullException("response");

	return document;
}

UsersAtom::~UsersAtom() {
}

#endif
