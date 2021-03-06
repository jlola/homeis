/*
 * FoldersService.cpp
 *
 *  Created on: May 29, 2013
 *      Author: root
 */
#include <httpserver.hpp>
#include "Folder/HisDevValueId.h"
#include "FoldersService.h"
#include "EItemType.h"
#include "Common/CUUID.h"
#include "death_handler.h"
#include "StringBuilder.h"
#include "PoppyDebugTools.h"
#include "ExpressionsService.h"
#include "microhttpd.h"

FoldersService::FoldersService(HisDevices & dev,
		HisDevFolderRoot & root,
		IUserManager* userManager,
		IHisDevFactory* factory,
		webserver* ws_i) :
		ServiceBase::ServiceBase(factory,userManager),
	logger(CLogger::GetLogger()),
	root(root),
	devices(dev)
{
	ws_i->register_resource(string("api/folders"), this, true);
	ws_i->register_resource(string("api/folder"), this, true);
	ws_i->register_resource(string("api/folders/{id}"), this, true);
	ws_i->register_resource(string("api/folder/allitems/{id}"), this, true);
	ws_i->register_resource(string("api/folder/{id}"), this, true);
	ws_i->register_resource(string("api/folder/valueid/{folderId}"), this, true);
}

FoldersService::~FoldersService(void)
{

}

const std::shared_ptr<http_response> FoldersService::GET(const http_request& req)
{
	STACK
	Document respjsondoc;
	respjsondoc.SetArray();
	int response_code = MHD_HTTP_FORBIDDEN;

	string strid = req.get_arg(JSON_ID);
	HisDevFolder* folder = NULL;
	string path = req.get_path();
	if(!strid.empty())
	{
		try
		{
			CUUID id = CUUID::Parse(strid);
			folder = dynamic_cast<HisDevFolder*>(root.GetFolder()->Find(id));
		}
		catch(...)
		{
			folder = root.GetFolder();
		}
	}
	//vrati vsechny polozky ve slozce
	if (path.find("/api/folder/allitems")!=string::npos)
	{
		if (folder!=NULL)
		{
			STACK_SECTION("/api/folder/allitems");
			FoldersToJson(root,folder,respjsondoc,false);
		}
		response_code = MHD_HTTP_OK;
	}
	else if (path.find("/api/folders")!=string::npos)
	{
		if (folder!=NULL)
		{
			STACK_SECTION("/api/folders");
			FoldersToJson(root,folder,respjsondoc,true);
		}
		response_code = MHD_HTTP_OK;
	}
	else if (path.find("/api/folder")!=string::npos)
	{
		STACK_SECTION("/api/folder");
		if (folder!=NULL)
		{
			FolderToJson(root,folder->GetParent(), folder,respjsondoc);
			response_code = MHD_HTTP_OK;
		}
		else
		{
			response_code = MHD_HTTP_BAD_REQUEST;
		}

	}
	else
	{
		response_code = MHD_HTTP_NOT_FOUND;
	}

	string json = DocumentToString(respjsondoc);

	return CreateResponseString(json,response_code);
}

void FoldersService::FolderToJson(HisDevFolderRoot & root,IHisBase *pParentFolder, IHisBase *pFolder, Document & respjsondoc)
{
	STACK

	Value d(kObjectType);
	string strvalue = pFolder->GetName();
	Value jsonvalue;
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_NAME,jsonvalue, respjsondoc.GetAllocator());

	strvalue = pFolder->GetCreateDateTime().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_CREATETIME,jsonvalue, respjsondoc.GetAllocator());

	strvalue = pFolder->GetModifyDateTime().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_MODIDFYTIME,jsonvalue, respjsondoc.GetAllocator());

	strvalue = (const char*)pFolder->GetNodeName();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_NODENAME,jsonvalue, respjsondoc.GetAllocator());

	if (pParentFolder!=NULL)
	{
		strvalue = pParentFolder->GetRecordId().ToString();
		jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
		d.AddMember(JSON_PARENTID,jsonvalue, respjsondoc.GetAllocator());
		strvalue = pParentFolder->GetName();
		jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
		d.AddMember(JSON_PARENTNAME,jsonvalue, respjsondoc.GetAllocator());
	}

	strvalue = pFolder->GetRecordId().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_ID,jsonvalue, respjsondoc.GetAllocator());

	if (pFolder->GetParent()!=NULL)
	{
		if (pFolder->GetParent()->GetRecordId()!=root.GetFolder()->GetRecordId())
		{
			strvalue = pFolder->GetParent()->GetRecordId().ToString();
			jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
			d.AddMember(JSON_PARENTID,jsonvalue, respjsondoc.GetAllocator());
		}
	}
	respjsondoc.PushBack(d, respjsondoc.GetAllocator());
}

void FoldersService::FoldersToJson(HisDevFolderRoot & root,HisDevFolder *parentFolder, Document & respjsondoc,bool foldersOnly)
{
	STACK
	vector<HisBase*> folders;
	folders = parentFolder->GetItems<HisBase>();

	//FolderToJson(root,parentFolder,respjsondoc);

	for(size_t i=0;i<folders.size();i++)
	{
		HisDevFolder* pFolder = dynamic_cast<HisDevFolder*>(folders[i]);
		HisDevValueId* pValueId = dynamic_cast<HisDevValueId*>(folders[i]);
		LuaExpression* pExpression = dynamic_cast<LuaExpression*>(folders[i]);

		if (pFolder!=NULL)
			FolderToJson(root,parentFolder,pFolder,respjsondoc);
		else if (!foldersOnly)
		{
			if (pValueId!=NULL)
			{
				Value d(kObjectType);
				HisDevValueBase* devValue = devices.FindValue(pValueId->GetDeviceValueId());
				if (devValue!=NULL)
				{
					DevicesService::DevValueToJson(d,pValueId,devValue,respjsondoc);
					respjsondoc.PushBack(d, respjsondoc.GetAllocator());
				}
			}
			else if (pExpression!=NULL)
			{
				Value d(kObjectType);
				ExpressionService::ExpressionToJson(&devices, d,pExpression, respjsondoc);
				respjsondoc.PushBack(d,respjsondoc.GetAllocator());
			}
		}
	}
}

bool FoldersService::AddValueIdToFolder(string strFolderId, string strJson,string & message)
{
	STACK
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.

	if (document.Parse<0>((char*)strJson.c_str()).HasParseError())
		return NULL;

	if (document.HasMember(JSON_DEVVALUEID) && document[JSON_DEVVALUEID].IsString())
	{
		string strDevValueId = document[JSON_DEVVALUEID].GetString();
		CUUID devValueId = CUUID::Parse(strDevValueId);
		HisDevValueBase* valueBase = devices.FindValue(devValueId);
		if (valueBase!=NULL)
		{
			CUUID folderId = CUUID::Parse(strFolderId);
			HisDevFolder* folder = dynamic_cast<HisDevFolder*>(root.GetFolder()->Find(folderId));
			if (folder!=NULL)
			{
				HisDevValueId* valueId = new HisDevValueId(valueBase,GetFactory());
				folder->Add(valueId);
				root.Save();
				return true;
			}
		}
	}
	else
	{
		message = "AddValueIdToFolder. JSON does not contain valid DevValueId.";
	}
	return false;
}

const std::shared_ptr<http_response> FoldersService::POST(const http_request& req)
{
	STACK
	string message = "";
	int response_code = MHD_HTTP_FORBIDDEN;
	std::string content = req.get_content();

	if (CreateFolder(content,message))
	{
		response_code = MHD_HTTP_OK;
	}

	return CreateResponseString(message,response_code);
}

bool FoldersService::UpdateFolder(string strid, string strJson,string & message)
{
	STACK
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.


		if (document.Parse<0>((char*)strJson.c_str()).HasParseError())
			return false;

		CUUID id;

		id = CUUID::Parse(strid);

		CUUID parentid;
		if (document.HasMember(JSON_PARENTID) && document[JSON_PARENTID].IsString())
		{
			parentid = CUUID::Parse(document[JSON_PARENTID].GetString());
		}
		else
		{
			message = "Json does not conatin valid ParentId field";
			return false;
		}

		string name;
		if (document.HasMember(JSON_NAME) && document[JSON_NAME].IsString())
		{
			name = document[JSON_NAME].GetString();
		}
		else
		{
			message = "Json does not conatin valid name field";
			return false;
		}

		HisDevFolder* folder = dynamic_cast<HisDevFolder*>(root.GetFolder()->Find(id));

		if (folder!=NULL)
		{
			bool changed = false;
			if (folder->GetName()!=name)
			{
				changed = true;
				folder->SetName(name);
			}
			HisDevFolder* parent = dynamic_cast<HisDevFolder*>(folder->GetParent());
			if (parent->GetRecordId()!=parentid)
			{
				HisDevFolder* newparent = dynamic_cast<HisDevFolder*>(root.GetFolder()->Find(parentid));
				if (newparent)
				{
					changed = true;
					parent->Remove(id);
					newparent->Add(folder);
				}
			}

			if (changed) root.Save();
			return true;
		}
		return false;
}

bool FoldersService::CreateFolder(string strJson,string & message)
{
	STACK
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.

	if (document.Parse<0>((char*)strJson.c_str()).HasParseError())
		return false;

	CUUID parentid = CUUID::Empty();
	if (document.HasMember(JSON_PARENTID) && document[JSON_PARENTID].IsString())
	{
		try
		{
			parentid = CUUID::Parse(document[JSON_PARENTID].GetString());
		}
		catch(...)
		{
			parentid = CUUID::Empty();
		}
	}

	HisDevFolder* newFolder = NULL;
	if (document.HasMember(JSON_NAME) && document[JSON_NAME].IsString())
	{
		string name = document[JSON_NAME].GetString();
		newFolder = new HisDevFolder(name,GetFactory());
	}
	else
	{
		message = "Json does not contain valid name property";
		return false;
	}

	HisDevFolder* parent = dynamic_cast<HisDevFolder*>(root.GetFolder()->Find(parentid));
	if (parent!=NULL)
		parent->Add(newFolder);
	else
		root.GetFolder()->Add(newFolder);
	root.Save();

	return true;
}


const std::shared_ptr<http_response> FoldersService::PUT(const http_request& req)
{
	STACK
	string message;
	int response_code = MHD_HTTP_FORBIDDEN;

	string strid = req.get_arg(JSON_ID);
	string content = req.get_content();
	string path = req.get_path();

	if (path.find("/api/folder/valueid")!=string::npos)
	{
		string strfolderid = req.get_arg(JSON_FOLDERID);
		//vytvori id datoveho bodu ve slozce
		if (AddValueIdToFolder(strfolderid,content,message))
		{
			response_code = MHD_HTTP_OK;
		}
	}
	else if (UpdateFolder(strid,content,message))
	{
		response_code = MHD_HTTP_OK;
	}

	return CreateResponseString(message,response_code);
}

string FoldersService::DeleteDevValue(string strDevValueRecordId)
{
	STACK
	CUUID devValueId = CUUID::Parse(strDevValueRecordId);
	HisDevValueBase* devValue = devices.FindValue(devValueId);
	if (devValue==NULL)
	{
		string msg = StringBuilder::Format("OneWireDevicesService::DeleteDevValue | Doesn't exists");
		logger.Error(msg.c_str());
		return msg;
	}
	HisDevBase* device = dynamic_cast<HisDevBase*>(devValue->GetParent());

	HisDevValueId* usedIn = root.FindValueId(devValueId);
	if (usedIn!=NULL)
	{
		HisDevFolder* folder = dynamic_cast<HisDevFolder*>(usedIn->GetParent());
		if (folder!=NULL)
		{
			string msg = StringBuilder::Format("OneWireDevicesService::DeleteDevValue | Cant delete node %s used in folder %s",strDevValueRecordId.c_str(),folder->GetName().c_str());
			logger.Error(msg.c_str());
			return msg;
		}
	}
	else
	{
		IHisBase* devvalue = device->Remove(devValueId);
		if (devvalue!=NULL)
		{
			delete(devvalue);
			devvalue = NULL;
		}
		else
		{
			string msg = StringBuilder::Format("OneWireDevicesService::DeleteDevValue | Error while delete.");
			logger.Error(msg.c_str());
			return msg;
		}
	}
	return "";
}

const std::shared_ptr<http_response> FoldersService::DELETE(const http_request& req)
{
	STACK
	int response_code = MHD_HTTP_FORBIDDEN;
	string message;

	string strid = req.get_arg(JSON_ID);
	CUUID id = CUUID::Parse(strid);
	HisDevFolder* folder = dynamic_cast<HisDevFolder*>(root.GetFolder()->Find(id));

	if (folder!=NULL)
	{
		HisDevFolder* parentfolder = dynamic_cast<HisDevFolder*>(folder->GetParent());
		parentfolder->Remove(id);
		delete(folder);
		root.Save();

		response_code = MHD_HTTP_OK;
		message = "OK";
	}

	return CreateResponseString(message,response_code);
}
