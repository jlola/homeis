/*
 * ExpressionAtom.cpp
 *
 *  Created on: 11. 12. 2018
 *      Author: pc
 */
#include <string>
#include "StringBuilder.h"
#include "HisException.h"
#include "ExpressionAtom.h"

ExpressionAtom::ExpressionAtom(HomeIsStarter * homeisStarter)
	: homeisStarter(homeisStarter),
	  client(&homeisStarter->GetClient())
{
}

void ExpressionAtom::AddTagToExpression(string expressionId,string tagId,string hashPassword)
{
	std::string reqest = StringBuilder::Format("api/expression/%s/addtag/%s",expressionId.c_str(),tagId.c_str());

	string response;
	long http_code = 0;
	CURLcode urlCode = client->Put(reqest,"",hashPassword,response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);
}

void ExpressionAtom::RemoveTagFromExpression(string expressionId,string tagId,string user, string hashPassword)
{
	std::string reqest = StringBuilder::Format("api/expression/%s/removetag/%s",expressionId.c_str(),tagId.c_str());

	string response;
	long http_code = 0;
	CURLcode urlCode = client->Delete(reqest,"",user,hashPassword,response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);
}

Document ExpressionAtom::GetExpression(string strExprId)
{
	std::string reqest = StringBuilder::Format("api/expression/%s",strExprId.c_str());
	string response;
	long http_code = 0;
	CURLcode urlCode = client->Get(reqest,response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);
	Document respdoc;
	if (respdoc.Parse<0>((char*)response.c_str()).HasParseError() == true)
		throw ArgumentNullException("response");
	return respdoc;
}

Document ExpressionAtom::CreateExpression(string parentId,bool running, string name,string description, string expression,string nodeName, string hashPassword)
{
	std::string reqest = StringBuilder::Format("api/expression");

	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	document.SetObject();
	Value val;
	document.AddMember(JSON_PARENTID,parentId.c_str(),document.GetAllocator());
	val.SetBool(running);
	document.AddMember(JSON_RUNNING,val,document.GetAllocator());
	document.AddMember(JSON_DESCRIPTION,description.c_str(),document.GetAllocator());
	document.AddMember(JSON_EXPRESSION,expression.c_str(),document.GetAllocator());
	document.AddMember(JSON_NAME,name.c_str(),document.GetAllocator());
	document.AddMember(JSON_NODENAME,nodeName.c_str(),document.GetAllocator());

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	document.Accept(wr);
	const std::string json(buffer.GetString());
	string response;
	long http_code = 0;
	CURLcode urlCode = client->Post(reqest,json,hashPassword,response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);
	Document respdoc;
	if (respdoc.Parse<0>((char*)response.c_str()).HasParseError() == true)
		throw ArgumentNullException("response");
	return respdoc;
}

ExpressionAtom::~ExpressionAtom() {
}

