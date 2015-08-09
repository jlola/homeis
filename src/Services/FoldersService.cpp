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
#include "ExpressionsService.h"

FoldersService::FoldersService(HisDevices & dev,HisDevFolderRoot* proot) :
	root(proot),devices(dev)
{
}

FoldersService::~FoldersService(void)
{

}

void FoldersService::render_GET(const http_request& req, http_response** res)
{
	Document respjsondoc;
	respjsondoc.SetArray();

	string strid = req.get_arg("id");
	HisDevFolder* folder = NULL;
	string path = req.get_path();
	if(!strid.empty())
	{
		try
		{
			CUUID id = CUUID::Parse(strid);
			folder = dynamic_cast<HisDevFolder*>(root->GetFolder()->Find(id));
		}
		catch(...)
		{
			folder = root->GetFolder();
		}
	}
	//vrati vsechny polozky ve slozce
	if (path.find("/api/folder/allitems")!=string::npos)
	{
		if (folder!=NULL)
		{
			FoldersToJson(root,folder,respjsondoc,false);
		}
	}
	else if (path.find("/api/folders")!=string::npos)
	{
		if (folder!=NULL)
		{
			FoldersToJson(root,folder,respjsondoc,true);
		}
	}
	else if (path.find("/api/folder")!=string::npos)
	{
		if (folder!=NULL)
		{
			FolderToJson(root, folder,respjsondoc);
		}
	}


	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	respjsondoc.Accept(wr);
	std::string json = buffer.GetString();
	//*res = new http_string_response(json, 200, "application/json");
	*res = new http_response(http_response_builder(json, 200,"application/json").string_response());
}

void FoldersService::FolderToJson(HisDevFolderRoot* root, HisBase *pFolder, Document & respjsondoc)
{
	Value d(kObjectType);
	string strvalue = pFolder->GetName();
	Value jsonvalue;
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember("name",jsonvalue, respjsondoc.GetAllocator());

	strvalue = (const char*)pFolder->GetNodeName();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember("NodeName",jsonvalue, respjsondoc.GetAllocator());

	strvalue = pFolder->GetRecordId().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember("id",jsonvalue, respjsondoc.GetAllocator());

	if (pFolder->GetParent()!=NULL)
	{
		if (pFolder->GetParent()->GetRecordId()!=root->GetFolder()->GetRecordId())
		{
			strvalue = pFolder->GetParent()->GetRecordId().ToString();
			jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
			d.AddMember("parentId",jsonvalue, respjsondoc.GetAllocator());
		}
	}
	respjsondoc.PushBack(d, respjsondoc.GetAllocator());
}

void FoldersService::FoldersToJson(HisDevFolderRoot* root,HisDevFolder *parentFolder, Document & respjsondoc,bool foldersOnly)
{
	vector<HisBase*> folders;
	folders = parentFolder->GetItems<HisBase>();

	for(size_t i=0;i<folders.size();i++)
	{
		HisDevFolder* pFolder = dynamic_cast<HisDevFolder*>(folders[i]);
		HisDevValueId* pValueId = dynamic_cast<HisDevValueId*>(folders[i]);
		LuaExpression* pExpression = dynamic_cast<LuaExpression*>(folders[i]);

		if (pFolder!=NULL)
			FolderToJson(root,pFolder,respjsondoc);
		else if (!foldersOnly)
		{
			if (pValueId!=NULL)
			{
				Value d(kObjectType);
				HisDevValueBase* devValue = devices.FindValue(pValueId->GetDeviceValueId());
				if (devValue!=NULL)
				{
					OneWireDevicesService::DevValueToJson(d,pValueId,devValue,respjsondoc);
					respjsondoc.PushBack(d, respjsondoc.GetAllocator());
				}
			}
			else if (pExpression!=NULL)
			{
				ExpressionService::ExpressionToJson(pExpression, respjsondoc);
			}
		}
	}
}

bool FoldersService::AddValueIdToFolder(string strFolderId, string strJson)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.

	if (document.Parse<0>((char*)strJson.c_str()).HasParseError())
		return NULL;

	if (document.HasMember("DevValueId"))
	{
		string strDevValueId = document["DevValueId"].GetString();
		CUUID devValueId = CUUID::Parse(strDevValueId);
		HisDevValueBase* valueBase = devices.FindValue(devValueId);
		if (valueBase!=NULL)
		{
			CUUID ValueId = valueBase->GetRecordId();
			CUUID folderId = CUUID::Parse(strFolderId);
			HisDevFolder* folder = dynamic_cast<HisDevFolder*>(root->GetFolder()->Find(folderId));
			if (folder!=NULL)
			{
				HisDevValueId* valueId = new HisDevValueId(ValueId);
				folder->Add(valueId);
				root->Save();
				return true;
			}
		}
	}
	return false;
}

void FoldersService::render_POST(const http_request& req, http_response** res)
{
	std::string content = req.get_content();

	if (req.get_user()=="a" && req.get_pass()=="a")
	{

		//string address = req.get_arg("id");

		if (CreateFolder(content))
		{
			//*res = new http_string_response("", 200, "application/json");
			*res = new http_response(http_response_builder("", 200,"application/json").string_response());
			return;
		}
	}
	else
	{
		string message = "Autentication error";
		//*res = new http_string_response(message.c_str(), 401, "application/json");
		*res = new http_response(http_response_builder(message, 401,"application/json").string_response());
		return;
	}

	//*res = new http_string_response("", 403, "application/json");
	*res = new http_response(http_response_builder("", 403,"application/json").string_response());
}

bool FoldersService::UpdateFolder(string strid, string strJson)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.

		if (document.Parse<0>((char*)strJson.c_str()).HasParseError())
			return false;

		CUUID id;

		id = CUUID::Parse(strid);

		CUUID parentid;
		if (document.HasMember("parentId"))
		{
			parentid = CUUID::Parse(document["parentId"].GetString());
		}

		string name;
		if (document.HasMember("name"))
		{
			name = document["name"].GetString();
		}

		HisDevFolder* folder = dynamic_cast<HisDevFolder*>(root->GetFolder()->Find(id));

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
				HisDevFolder* newparent = dynamic_cast<HisDevFolder*>(root->GetFolder()->Find(parentid));
				if (newparent)
				{
					changed = true;
					parent->Remove(id);
					newparent->Add(folder);
				}
			}

			if (changed) root->Save();
			return true;
		}
		return false;
}

bool FoldersService::CreateFolder(string strJson)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.

	if (document.Parse<0>((char*)strJson.c_str()).HasParseError())
		return false;

	CUUID parentid = CUUID::Empty();
	if (document.HasMember("parentId"))
	{
		try
		{
			if (document["parentId"].IsString())
				parentid = CUUID::Parse(document["parentId"].GetString());
		}
		catch(...)
		{
			parentid = CUUID::Empty();
		}
	}

	HisDevFolder* newFolder = NULL;
	if (document.HasMember("name"))
	{
		string name = document["name"].GetString();
		newFolder = new HisDevFolder(name);
	}

	HisDevFolder* parent = dynamic_cast<HisDevFolder*>(root->GetFolder()->Find(parentid));
	if (parent!=NULL)
		parent->Add(newFolder);
	else
		root->GetFolder()->Add(newFolder);
	root->Save();

	return true;
}


void FoldersService::render_PUT(const http_request& req, http_response** res)
{
	if (req.get_user()=="a" && req.get_pass()=="a")
	{
		string strid = req.get_arg("id");
		string content = req.get_content();
		string path = req.get_path();

		if (path.find("/api/folder/valueid")!=string::npos)
		{
			string strfolderid = req.get_arg("folderid");
			//vytvori id datoveho bodu ve slozce
			if (AddValueIdToFolder(strfolderid,content))
			{
				//*res = new http_string_response("", 200, "application/json");
				*res = new http_response(http_response_builder("", 200,"application/json").string_response());
				return;
			}
		}
		else if (UpdateFolder(strid,content))
		{
			//*res = new http_string_response("", 200, "application/json");
			*res = new http_response(http_response_builder("", 200,"application/json").string_response());
			return;
		}
	}
	else
	{
		string message = "Autentication error";
		//*res = new http_string_response(message.c_str(), 401, "application/json");
		*res = new http_response(http_response_builder(message, 401,"application/json").string_response());
		return;
	}

	//*res = new http_string_response("", 403, "application/json");
	*res = new http_response(http_response_builder("", 403,"application/json").string_response());

}

string FoldersService::DeleteDevValue(string strDevValueRecordId)
{
	CUUID devValueId = CUUID::Parse(strDevValueRecordId);
	HisDevValueBase* devValue = devices.FindValue(devValueId);
	if (devValue==NULL)
	{
		string msg = StringBuilder::Format("OneWireDevicesService::DeleteDevValue | Doesn't exists");
		CLogger::Error(msg.c_str());
		return msg;
	}
	HisDevBase* device = dynamic_cast<HisDevBase*>(devValue->GetParent());

	HisDevValueId* usedIn = root->FindValueId(devValueId);
	if (usedIn!=NULL)
	{
		HisDevFolder* folder = dynamic_cast<HisDevFolder*>(usedIn->GetParent());
		if (folder!=NULL)
		{
			string msg = StringBuilder::Format("OneWireDevicesService::DeleteDevValue | Cant delete node %s used in folder %s",strDevValueRecordId.c_str(),folder->GetName().c_str());
			CLogger::Error(msg.c_str());
			return msg;
		}
	}
	else
	{
		HisBase* devvalue = device->Remove(devValueId);
		if (devvalue!=NULL)
		{
			delete(devvalue);
			devvalue = NULL;
		}
		else
		{
			string msg = StringBuilder::Format("OneWireDevicesService::DeleteDevValue | Error while delete.");
			CLogger::Error(msg.c_str());
			return msg;
		}
	}
	return "";
}

void FoldersService::render_DELETE(const http_request& req, http_response** res)
{
	if (req.get_user()=="a" && req.get_pass()=="a")
	{
		string strid = req.get_arg("id");

		CUUID id = CUUID::Parse(strid);
		HisDevFolder* folder = dynamic_cast<HisDevFolder*>(root->GetFolder()->Find(id));

		if (folder!=NULL)
		{
			HisDevFolder* parentfolder = dynamic_cast<HisDevFolder*>(folder->GetParent());
			parentfolder->Remove(id);
			delete(folder);
			root->Save();

			//*res = new http_string_response("", 200, "application/json");
			*res = new http_response(http_response_builder("", 200,"application/json").string_response());
			return;
		}
	}
	else
	{
		string message = "Autentication error";
		//*res = new http_string_response(message.c_str(), 401, "application/json");
		*res = new http_response(http_response_builder(message, 401,"application/json").string_response());
		return;
	}

	//*res = new http_string_response("", 403, "application/json");
	*res = new http_response(http_response_builder("", 403,"application/json").string_response());

}
