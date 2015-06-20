/*
 * ExpressionsService.cpp
 *
 *  Created on: Sep 17, 2013
 *      Author: root
 */

#include "ExpressionsService.h"
#include "homeis/Common/HisException.h"

ExpressionService::ExpressionService(HisDevFolderRoot* folder,ExpressionRuntime *pexpressionRuntime, HisDevices* pdevices)
{
	devices = pdevices;
	expressionRuntime = pexpressionRuntime;
	root = folder;
}

ExpressionService::~ExpressionService(void)
{

}

void ExpressionService::render_GET(const http_request& req, http_response** res)
{
	Document respjsondoc;
	respjsondoc.SetArray();

	string strid = req.get_arg("id");
	string path = req.get_path();

	try
	{
		//run all expressions in folder
		if (path.find("/api/expression/run")!=string::npos)
		{
			LuaExpression* expression = NULL;

			if(!strid.empty())
			{
				CUUID id = CUUID::Parse(strid);
				expression = dynamic_cast<LuaExpression*>(root->FindExpression(id));
			}

			if (expression!=NULL)
			{
				if (!expression->ForceEvaluate())
				{
					throw HisException(expression->GetLastEvaluateError());
				}
			}

		}//load data for all expression in folder
		else if (path.find("/api/expression/folder")!=string::npos)
		{
//			HisDevFolder* folder = NULL;
//			vector<LuaExpression*> expressions;
//			HisDevFolder* rootFolder = root->GetFolder();
//
//			if(!strid.empty())
//			{
//				CUUID id = CUUID::Parse(strid);
//				folder = dynamic_cast<HisDevFolder*>(rootFolder->Find(id));
//			}
//
//			if (folder!=NULL)
//			{
//				expressions = folder->GetItems<LuaExpression>();
//				for (size_t i=0;i<expressions.size();i++)
//				{
//					ExpressionToJson(expressions[i],respjsondoc);
//				}
//			}
			ExpressionsToJson(strid , root, respjsondoc);
		}
	}
	catch(HisException & ex)
	{

			//respjsondoc.SetArray();
		respjsondoc.SetObject();
		StringBuffer buffer;
		string msg = ex.what();
		Value jsonvalue;
		jsonvalue.SetString(msg.c_str(),msg.length(),respjsondoc.GetAllocator());
		respjsondoc.AddMember("message",jsonvalue, respjsondoc.GetAllocator());

		PrettyWriter<StringBuffer> wr(buffer);
		respjsondoc.Accept(wr);
		std::string json = buffer.GetString();
		*res = new http_string_response(json, 403, "application/json");
		return;
	}

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	respjsondoc.Accept(wr);
	std::string json = buffer.GetString();
	*res = new http_string_response(json, 200, "application/json");
}

void ExpressionService::ExpressionsToJson(string strid, HisDevFolderRoot* root, Document & respjsondoc)
{
	HisDevFolder* folder = NULL;
	vector<LuaExpression*> expressions;
	HisDevFolder* rootFolder = root->GetFolder();

	if(!strid.empty())
	{
		CUUID id = CUUID::Parse(strid);
		folder = dynamic_cast<HisDevFolder*>(rootFolder->Find(id));
	}

	if (folder!=NULL)
	{
		expressions = folder->GetItems<LuaExpression>();
		for (size_t i=0;i<expressions.size();i++)
		{
			ExpressionToJson(expressions[i],respjsondoc);
		}
	}
}

void ExpressionService::ExpressionToJson(LuaExpression *pExpression, Document & respjsondoc)
{
	Value d(kObjectType);
	string strvalue = pExpression->GetName();
	Value jsonvalue;
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember("name",jsonvalue, respjsondoc.GetAllocator());

	strvalue = (const char*)pExpression->GetNodeName();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember("NodeName",jsonvalue, respjsondoc.GetAllocator());

	strvalue = pExpression->GetExpression();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember("expression",jsonvalue, respjsondoc.GetAllocator());

	strvalue = pExpression->GetLastEvaluateError();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember("errormessage",jsonvalue, respjsondoc.GetAllocator());

	strvalue = pExpression->GetDescription();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember("description",jsonvalue, respjsondoc.GetAllocator());

	strvalue = pExpression->GetRecordId().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember("id",jsonvalue, respjsondoc.GetAllocator());

	jsonvalue.SetBool(pExpression->GetRunning());//SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember("running",jsonvalue, respjsondoc.GetAllocator());

	//if (pExpression->GetParent()->GetRecordId()!=root->GetFolder()->GetRecordId())
	{
		strvalue = pExpression->GetParent()->GetRecordId().ToString();
		jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
		d.AddMember("parentId",jsonvalue, respjsondoc.GetAllocator());
	}
	respjsondoc.PushBack(d, respjsondoc.GetAllocator());
}

bool ExpressionService::DeleteExpression(string strid,string & message)
{
	if (strid.empty()) {
		message = "DeleteExpression | Id is empty. Can not be deleted";
		return false;
	}

	HisDevFolder* rootFolder = root->GetFolder();
	if(!strid.empty())
	{
		CUUID id = CUUID::Parse(strid);
		LuaExpression* expression = dynamic_cast<LuaExpression*>(rootFolder->Find(id));
		if (expression!=NULL)
		{
			if (!expression->GetRunning())
			{
				HisDevFolder* parentFolder = dynamic_cast<HisDevFolder*>(expression->GetParent());
				if (parentFolder!=NULL)
				{
					parentFolder->Remove(expression->GetRecordId());
					delete(expression);
					root->Save();
					return true;
				}
				else
				{
					message = "DeleteExpression | Parent folder is empty.";
				}
			}
			else
			{
				message = "DeleteExpression | Expression is running. Can not be deleted.";
			}
		}
		else
		{
			message = "DeleteExpression | Expression not found";
		}
	}

	return false;
}

LuaExpression* ExpressionService::CreateOrUpdateExpression(string strJson,string & message)
{
	try
	{
		Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.

		if (document.Parse<0>((char*)strJson.c_str()).HasParseError())
			return NULL;

		CUUID parentid;
		if (document.HasMember("parentId"))
		{
			parentid = CUUID::Parse(document["parentId"].GetString());
		}

		CUUID expressionid;
		if (document.HasMember("id"))
		{
			expressionid = CUUID::Parse(document["id"].GetString());
		}

		LuaExpression* expressionObj = NULL;
		HisDevFolder* parent = dynamic_cast<HisDevFolder*>(root->GetFolder()->Find(parentid));
		if (parent==NULL) parent = root->GetFolder();
		bool saveReq = false;
		expressionObj = dynamic_cast<LuaExpression*>(parent->Find(expressionid));

		if (document.HasMember("name"))
		{
			string name = document["name"].GetString();
			if (expressionObj!=NULL)
			{
				if (expressionObj->GetName()!=name)
				{
					expressionObj->SetName(name);
					saveReq = true;
				}
			}
			else
			{
				expressionObj = new LuaExpression(parent,devices,name,expressionRuntime );
				parent->Add(expressionObj);
				saveReq = true;
			}

			if (document.HasMember("description"))
			{
				string description = document["description"].GetString();
				if (description!=expressionObj->GetDescription())
				{
					expressionObj->SetDescription(description);
					saveReq = true;
				}
			}

			if (document.HasMember("expression"))
			{
				string strexpression = document["expression"].GetString();
				if (strexpression!=expressionObj->GetExpression())
				{
					expressionObj->SetExpression(strexpression);
					saveReq = true;
				}
			}

			if (document.HasMember("running"))
			{
				string strrunning = document["running"].GetString();
				bool running = strrunning=="1" ? true : false;
				if (running!=expressionObj->GetRunning())
				{
					expressionObj->SetRunning(running);
					saveReq = true;
				}
			}

			if (saveReq) root->Save();
		}
		return expressionObj;
	}
	catch(HisException & ex)
	{
		message = ex.what();
		return NULL;
	}
}

void ExpressionService::render_POST(const http_request& req, http_response** res)
{
	std::string content = req.get_content();
	string message = "";

	if (req.get_user()=="a" && req.get_pass()=="a")
	{
		LuaExpression* expression = CreateOrUpdateExpression(content,message);
		if (expression!=NULL)
		{
			Document respjsondoc;
			respjsondoc.SetArray();

			ExpressionToJson(expression,respjsondoc);

			StringBuffer buffer;
			PrettyWriter<StringBuffer> wr(buffer);
			respjsondoc.Accept(wr);
			std::string json = buffer.GetString();

			*res = new http_string_response(json, 200, "application/json");
			return;
		}
	}
	else
	{
		string message = "Autentication error";
		*res = new http_string_response(message.c_str(), 401, "application/json");
		return;
	}
	*res = new http_string_response(message.c_str(), 403, "application/json");
}

void ExpressionService::render_PUT(const http_request& req, http_response** res)
{
	std::string content = req.get_content();
	string message = "";
	if (req.get_user()=="a" && req.get_pass()=="a")
	{

		if (CreateOrUpdateExpression(content,message))
		{
			*res = new http_string_response("", 200, "application/json");
			return;
		}
	}
	else
	{
		message = "Autentication error";
		*res = new http_string_response(message.c_str(), 401, "application/json");
		return;
	}
	*res = new http_string_response(message, 403, "application/json");
}

void ExpressionService::render_DELETE(const http_request& req, http_response** res)
{
	string message = "";
	if (req.get_user()=="a" && req.get_pass()=="a")
	{
		string strid = req.get_arg("id");
		if (DeleteExpression(strid,message))
		{
			*res = new http_string_response("OK", 200, "application/json");
			return;
		}
	}
	else
	{
		message = "Autentication error";
		*res = new http_string_response(message.c_str(), 401, "application/json");
		return;
	}
	*res = new http_string_response(message.c_str(), 403, "application/json");
}

