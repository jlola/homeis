/*
 * ExpressionsService.cpp
 *
 *  Created on: Sep 17, 2013
 *      Author: root
 */

#include "ExpressionsService.h"
#include "PoppyDebugTools.h"
#include "StringBuilder.h"
#include "Common/HisException.h"

ExpressionService::ExpressionService(HisDevFolderRoot* folder, IExpressionRuntime *pexpressionRuntime, HisDevices* pdevices)
{
	STACK
	devices = pdevices;
	expressionRuntime = pexpressionRuntime;
	root = folder;
}

ExpressionService::~ExpressionService(void)
{

}

const http_response ExpressionService::render_GET(const http_request& req)
{
	STACK
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
		else if (path.find("/api/expression/debuglog")!=string::npos)
		{
			LuaExpression* expression = NULL;

			if(!strid.empty())
			{
				CUUID id = CUUID::Parse(strid);
				expression = dynamic_cast<LuaExpression*>(root->FindExpression(id));

				if (expression!=NULL)
				{
					ExpressionDebugLogToJson(expression,respjsondoc);
				}
				else
					throw HisException("Not found");
			}
		}
		else if (path.find("/api/expression/folder")!=string::npos)
		{
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
		//*res = new http_string_response(json, 403, "application/json");
		http_response resp(http_response_builder(json, 403,"application/json").string_response());
		return resp;
	}

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	respjsondoc.Accept(wr);
	std::string json = buffer.GetString();
	//*res = new http_string_response(json, 200, "application/json");
	http_response resp(http_response_builder(json, 200,"application/json").string_response());
	return resp;
}

void ExpressionService::ExpressionsToJson(string strid, HisDevFolderRoot* root, Document & respjsondoc)
{
	STACK
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
			ExpressionToJson(folder, expressions[i],respjsondoc);
		}
	}
}

void ExpressionService::ExpressionDebugLogToJson(LuaExpression *pExpression, Document & respjsondoc)
{
	STACK
	Value d(kObjectType);
	Value jsonvalue;
	vector<string> logs = pExpression->GetLogs();
	for(size_t i=0;i<logs.size();i++)
	{
		string strvalue = logs[i];
		jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
		//d.AddMember("log",jsonvalue, respjsondoc.GetAllocator());
		respjsondoc.PushBack(jsonvalue, respjsondoc.GetAllocator());
	}
}

void ExpressionService::ExpressionToJson(HisBase* pParent, LuaExpression *pExpression, Document & respjsondoc)
{
	STACK
	Value d(kObjectType);
	string strvalue = pExpression->GetName();
	Value jsonvalue;

	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember("name",jsonvalue, respjsondoc.GetAllocator());

	strvalue = pExpression->GetCreateDateTime().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember("createtime",jsonvalue, respjsondoc.GetAllocator());

	strvalue = pExpression->GetModifyDateTime().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember("modifytime",jsonvalue, respjsondoc.GetAllocator());

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
	STACK
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
	STACK
	try
	{
		Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.

		if (document.Parse<0>((char*)strJson.c_str()).HasParseError())
			return NULL;

		CUUID parentid;
		if (document.HasMember("parentId") && document["parentId"].IsString())
		{
			parentid = CUUID::Parse(document["parentId"].GetString());
		}

		CUUID expressionid;
		if (document.HasMember("id") && document["id"].IsString())
		{
			expressionid = CUUID::Parse(document["id"].GetString());
		}

		LuaExpression* expressionObj = NULL;
		HisDevFolder* parent = dynamic_cast<HisDevFolder*>(root->GetFolder()->Find(parentid));
		if (parent==NULL) parent = root->GetFolder();
		bool saveReq = false;
		expressionObj = dynamic_cast<LuaExpression*>(parent->Find(expressionid));

		if (document.HasMember("name") && document["name"].IsString())
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

			if (document.HasMember("description") && document["description"].IsString())
			{
				string description = document["description"].GetString();
				if (description!=expressionObj->GetDescription())
				{
					expressionObj->SetDescription(description);
					saveReq = true;
				}
			}

			if (document.HasMember("expression") && document["expression"].IsString())
			{
				string strexpression = document["expression"].GetString();
				if (strexpression!=expressionObj->GetExpression())
				{
					if (expressionObj->GetRunning())
					{
						expressionObj->SetRunning(false);
						expressionObj->SetExpression(strexpression);
						expressionObj->SetRunning(true);
					}
					else
					{
						expressionObj->SetExpression(strexpression);
					}
					saveReq = true;
				}
			}

			if (document.HasMember("running"))
			{
				bool running = false;
				if (document["running"].IsString())
				{
					string strrunning = document["running"].GetString();
					running = strrunning=="1" ? true : false;
				}
				else if (document["running"].IsBool())
					running = document["running"].GetBool();
				else
				{
					message = StringBuilder::Format("Property 'running': can not convert received value to bool");
				}

				if (running!=expressionObj->GetRunning())
				{
					expressionObj->SetRunning(running);
					saveReq = true;
				}
			}

			if (saveReq) root->Save();
		}
		else
		{
			message = "Json does not contain valid name field";
		}
		return expressionObj;
	}
	catch(std::exception & ex)
	{
		message = ex.what();
		return NULL;
	}
}

const http_response ExpressionService::render_POST(const http_request& req)
{
	STACK
	std::string content = req.get_content();
	string message = "";

	if (req.get_user()=="a" && req.get_pass()=="a")
	{
		LuaExpression* expression = CreateOrUpdateExpression(content,message);
		if (expression!=NULL)
		{
			Document respjsondoc;
			respjsondoc.SetArray();

			ExpressionToJson(expression->GetParent(), expression,respjsondoc);

			StringBuffer buffer;
			PrettyWriter<StringBuffer> wr(buffer);
			respjsondoc.Accept(wr);
			std::string json = buffer.GetString();

			//*res = new http_string_response(json, 200, "application/json");
			http_response resp(http_response_builder(json, 200,"application/json").string_response());
			return resp;
		}
	}
	else
	{
		string message = "Autentication error";
		//*res = new http_string_response(message.c_str(), 401, "application/json");
		http_response resp(http_response_builder(message, 401,"application/json").string_response());
		return resp;
	}
	//*res = new http_string_response(message.c_str(), 403, "application/json");
	http_response resp(http_response_builder(message, 403,"application/json").string_response());
	return resp;
}

const http_response ExpressionService::render_PUT(const http_request& req)
{
	STACK
	std::string content = req.get_content();
	string message = "";
	if (req.get_user()=="a" && req.get_pass()=="a")
	{

		if (CreateOrUpdateExpression(content,message))
		{
			//*res = new http_string_response("", 200, "application/json");
			http_response resp(http_response_builder("", 200,"application/json").string_response());
			return resp;
		}
	}
	else
	{
		message = "Autentication error";
		//*res = new http_string_response(message.c_str(), 401, "application/json");
		http_response resp(http_response_builder(message, 401,"application/json").string_response());
		return resp;
	}
	//*res = new http_string_response(message, 403, "application/json");
	http_response resp(http_response_builder(message, 403,"application/json").string_response());
	return resp;
}

const http_response ExpressionService::render_DELETE(const http_request& req)
{
	STACK
	string message = "";
	if (req.get_user()=="a" && req.get_pass()=="a")
	{
		string strid = req.get_arg("id");
		if (DeleteExpression(strid,message))
		{
			//*res = new http_string_response("OK", 200, "application/json");
			http_response resp(http_response_builder("OK", 200,"application/json").string_response());
			return resp;
		}
	}
	else
	{
		message = "Autentication error";
		//*res = new http_string_response(message.c_str(), 401, "application/json");
		http_response resp(http_response_builder(message, 401,"application/json").string_response());
		return resp;
	}
	//*res = new http_string_response(message.c_str(), 403, "application/json");
	http_response resp(http_response_builder(message, 403,"application/json").string_response());
	return resp;
}

