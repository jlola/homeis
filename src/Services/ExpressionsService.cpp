/*
 * ExpressionsService.cpp
 *
 *  Created on: Sep 17, 2013
 *      Author: root
 */

#include "ExpressionsService.h"
#include "PoppyDebugTools.h"
#include "StringBuilder.h"
#include "HisException.h"
#include "DevicesService.h"
#include "microhttpd.h"

ExpressionService::ExpressionService(HisDevFolderRoot* folder,
		IExpressionRuntime *pexpressionRuntime,
		HisDevices* pdevices,
		IUserManager* userManager,
		IHisDevFactory* factory,
		webserver* ws_i)
	: ServiceBase::ServiceBase(factory,userManager)
{
	STACK
	devices = pdevices;
	expressionRuntime = pexpressionRuntime;
	root = folder;

	//run all expressions in folder
	ws_i->register_resource("api/expression/{expressionId}/removetag/{tagId}", this, true);
	ws_i->register_resource("api/expression/{expressionId}/addtag/{tagId}", this, true);
	ws_i->register_resource("api/expression/run/{id}", this, true);
	ws_i->register_resource("api/expression/folder/{id}", this, true);
	ws_i->register_resource("api/expression/debuglog/{id}", this, true);
	ws_i->register_resource("api/expression/{id}", this, true);
	ws_i->register_resource("api/expression", this, true);
}

ExpressionService::~ExpressionService(void)
{

}

const http_response ExpressionService::GET(const http_request& req)
{
	STACK
	Document respjsondoc;
	respjsondoc.SetArray();

	string message;
	int response_code = MHD_HTTP_FORBIDDEN;
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
					throw HisException(expression->GetLastEvaluateError(),__FILE__, __LINE__);
				}
			}
			response_code = MHD_HTTP_OK;

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
					throw HisException("Not found",__FILE__, __LINE__);
			}
			response_code = MHD_HTTP_OK;
		}
		else if (path.find("/api/expression/folder")!=string::npos)
		{
			ExpressionsToJson(devices, strid , root, respjsondoc);
			response_code = MHD_HTTP_OK;
		}
		else if (path.find("/api/expression")!=string::npos)
		{
			if(!strid.empty())
			{
				LuaExpression* expression = NULL;
				CUUID id = CUUID::Parse(strid);
				expression = dynamic_cast<LuaExpression*>(root->FindExpression(id));
				if (expression!=NULL)
				{
					respjsondoc.SetObject();
					ExpressionToJson(devices, respjsondoc,expression,respjsondoc);
					response_code = MHD_HTTP_OK;
				}
				else
					response_code = MHD_HTTP_NOT_FOUND;
			}
			else
			{
				response_code = MHD_HTTP_NOT_FOUND;
			}
		}
		else
		{
			response_code = MHD_HTTP_NOT_FOUND;
		}
	}
	catch(HisException & ex)
	{
		respjsondoc.SetObject();
		string msg = ex.what();
		Value jsonvalue;
		jsonvalue.SetString(msg.c_str(),msg.length(),respjsondoc.GetAllocator());
		respjsondoc.AddMember("message",jsonvalue, respjsondoc.GetAllocator());
		response_code = MHD_HTTP_FORBIDDEN;
	}

	std::string json = DocumentToString(respjsondoc);
	return CreateResponseString(json,response_code);
}

void ExpressionService::ExpressionsToJson(HisDevices* devices,string strFolderId, HisDevFolderRoot* root, Document & respjsondoc)
{
	STACK
	HisDevFolder* folder = NULL;
	vector<LuaExpression*> expressions;
	HisDevFolder* rootFolder = root->GetFolder();

	if(!strFolderId.empty())
	{
		CUUID folderid = CUUID::Parse(strFolderId);
		folder = dynamic_cast<HisDevFolder*>(rootFolder->Find(folderid));
	}

	if (folder!=NULL)
	{
		expressions = folder->GetItems<LuaExpression>();
		for (size_t i=0;i<expressions.size();i++)
		{
			Value d(kObjectType);
			ExpressionToJson(devices, d,expressions[i],respjsondoc);
			respjsondoc.PushBack(d, respjsondoc.GetAllocator());
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
		respjsondoc.PushBack(jsonvalue, respjsondoc.GetAllocator());
	}
}

bool ExpressionService::RemoveValueIdFromExpression(string strExpressionId, string strTagId, string & message)
{
	CUUID devValueId = CUUID::Parse(strTagId);
	HisDevValueBase* devValue = devices->FindValue(devValueId);
	if (devValue!=NULL)
	{
		CUUID expressionId = CUUID::Parse(strExpressionId);
		LuaExpression* expression = dynamic_cast<LuaExpression*>(root->FindExpression(expressionId));
		if (expression!=NULL)
		{
			HisDevValueId* objValueId = expression->FindDevValueId(devValueId);
			if (objValueId!=NULL)
			{
				if (expression->Remove(objValueId->GetRecordId())!=NULL)
				{
					root->Save();
					return true;
				}
			}
			else
			{
				message = "Not found";
			}
		}
	}
	return false;
}

bool ExpressionService::AddValueIdToExpression(string strExpressionId, string strTagId,string & message)
{
	STACK

	CUUID devValueId = CUUID::Parse(strTagId);
	HisDevValueBase* devValue = devices->FindValue(devValueId);
	if (devValue!=NULL)
	{
		CUUID expressionId = CUUID::Parse(strExpressionId);
		LuaExpression* expression = dynamic_cast<LuaExpression*>(root->FindExpression(expressionId));
		if (expression!=NULL)
		{
			HisDevValueId* objValueId = expression->FindDevValueId(devValueId);
			if (objValueId==NULL)
			{
				objValueId = new HisDevValueId(devValue,GetFactory());
				expression->Add(objValueId);
				root->Save();
			}
			return true;
		}
	}

	return false;
}


void ExpressionService::ExpressionToJson(HisDevices* devices, Value & d, LuaExpression *pExpression, Document & respjsondoc)
{
	STACK

	string strvalue = pExpression->GetName();
	Value jsonvalue;

	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_NAME,jsonvalue, respjsondoc.GetAllocator());

	strvalue = pExpression->GetCreateDateTime().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_CREATETIME,jsonvalue, respjsondoc.GetAllocator());

	strvalue = pExpression->GetModifyDateTime().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_MODIDFYTIME,jsonvalue, respjsondoc.GetAllocator());

	strvalue = (const char*)pExpression->GetNodeName();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_NODENAME,jsonvalue, respjsondoc.GetAllocator());

	strvalue = pExpression->GetExpression();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_EXPRESSION,jsonvalue, respjsondoc.GetAllocator());

	strvalue = pExpression->GetLastEvaluateError();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_ERRORMESSAGE,jsonvalue, respjsondoc.GetAllocator());

	strvalue = pExpression->GetDescription();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_DESCRIPTION,jsonvalue, respjsondoc.GetAllocator());

	strvalue = pExpression->GetRecordId().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_ID,jsonvalue, respjsondoc.GetAllocator());

	jsonvalue.SetBool(pExpression->GetRunning());//SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_RUNNING,jsonvalue, respjsondoc.GetAllocator());

	//if (pExpression->GetParent()->GetRecordId()!=root->GetFolder()->GetRecordId())
	{
		strvalue = pExpression->GetParent()->GetRecordId().ToString();
		jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
		d.AddMember(JSON_PARENTID,jsonvalue, respjsondoc.GetAllocator());
	}

	Value tagsArray(kArrayType);
	auto items = pExpression->GetAllItems();
	for (size_t i=0;i<items.size();i++)
	{
		HisDevValueId* valueId = dynamic_cast<HisDevValueId*>(items[i]);
		Value d(kObjectType);
		HisDevValueBase* devValue = devices->FindValue(valueId->GetDeviceValueId());
		if (devValue!=NULL)
		{
			DevicesService::DevValueToJson(d,valueId,devValue,respjsondoc);
			tagsArray.PushBack(d, respjsondoc.GetAllocator());
		}
	}
	d.AddMember(JSON_TAGS,tagsArray,respjsondoc.GetAllocator());
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
		if (document.HasMember(JSON_PARENTID) && document[JSON_PARENTID].IsString())
		{
			parentid = CUUID::Parse(document[JSON_PARENTID].GetString());
		}

		CUUID expressionid;
		if (document.HasMember(JSON_ID) && document[JSON_ID].IsString())
		{
			expressionid = CUUID::Parse(document[JSON_ID].GetString());
		}

		LuaExpression* expressionObj = NULL;
		HisDevFolder* parent = dynamic_cast<HisDevFolder*>(root->GetFolder()->Find(parentid));
		if (parent==NULL) parent = root->GetFolder();
		bool saveReq = false;
		expressionObj = dynamic_cast<LuaExpression*>(parent->Find(expressionid));

		if (document.HasMember(JSON_NAME) && document[JSON_NAME].IsString())
		{
			string name = document[JSON_NAME].GetString();
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
				expressionObj = new LuaExpression(parent,devices,name,expressionRuntime,GetFactory() );
				parent->Add(expressionObj);
				saveReq = true;
			}

			if (document.HasMember(JSON_DESCRIPTION) && document[JSON_DESCRIPTION].IsString())
			{
				string description = document[JSON_DESCRIPTION].GetString();
				if (description!=expressionObj->GetDescription())
				{
					expressionObj->SetDescription(description);
					saveReq = true;
				}
			}

			if (document.HasMember(JSON_EXPRESSION) && document[JSON_EXPRESSION].IsString())
			{
				string strexpression = document[JSON_EXPRESSION].GetString();
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

			if (document.HasMember(JSON_RUNNING))
			{
				bool running = false;
				if (document[JSON_RUNNING].IsString())
				{
					string strrunning = document[JSON_RUNNING].GetString();
					running = strrunning=="1" ? true : false;
				}
				else if (document[JSON_RUNNING].IsBool())
					running = document[JSON_RUNNING].GetBool();
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

const http_response ExpressionService::POST(const http_request& req)
{
	STACK
	std::string content = req.get_content();
	int response_code = MHD_HTTP_FORBIDDEN;
	string message = "";


	LuaExpression* expression = CreateOrUpdateExpression(content,message);
	if (expression!=NULL)
	{
		Document respjsondoc;
		respjsondoc.SetObject();

		ExpressionToJson(devices, respjsondoc,expression,respjsondoc);

		message = DocumentToString(respjsondoc);
		response_code = MHD_HTTP_OK;
	}
	else
	{
		message = GetErrorMessageJson("Not found");
		response_code = MHD_HTTP_NOT_FOUND;
	}

	return CreateResponseString(message,response_code);
}

const http_response ExpressionService::PUT(const http_request& req)
{
	STACK
	std::string content = req.get_content();
	int response_code = MHD_HTTP_FORBIDDEN;
	string message = "";
	string path = req.get_path();

	//0  /1			/2				/3		/4
	//api/expression/{expressionId}/addtag
	if (req.get_path_piece(0)=="api" &&
		req.get_path_piece(1)=="expression" &&
		req.get_path_piece(3)=="addtag")
	{
		string strExpressionId = req.get_arg("expressionId");
		string strTagId = req.get_arg("tagId");
		if (AddValueIdToExpression(strExpressionId,strTagId,message))
		{
			response_code = MHD_HTTP_OK;
		}
	}
	else if (CreateOrUpdateExpression(content,message))
	{
		response_code = MHD_HTTP_OK;
	}

	if (response_code != MHD_HTTP_OK)
	{
		message = GetErrorMessageJson(message);
	}

	return CreateResponseString(message,response_code);
}

const http_response ExpressionService::DELETE(const http_request& req)
{
	STACK
	int response_code = MHD_HTTP_FORBIDDEN;
	string message = "";
	string strid = req.get_arg("id");


	if (req.get_path_piece(0)=="api" &&
			req.get_path_piece(1)=="expression" &&
			req.get_path_piece(3)=="removetag")
	{
		string tagid = req.get_arg("tagId");
		string strExpressionId = req.get_arg("expressionId");
		if (RemoveValueIdFromExpression(strExpressionId, tagid, message))
		{
			response_code = MHD_HTTP_OK;
		}
	}
	else if (DeleteExpression(strid,message))
	{
		response_code = MHD_HTTP_OK;
		message = "OK";
	}
	else
	{
		message = GetErrorMessageJson(message);
	}

	return CreateResponseString(message,response_code);
}

