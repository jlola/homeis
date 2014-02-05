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
#include "homeis/Common/CUUID.h"


FoldersService::FoldersService(HisDevFolderRoot* proot)
{
	root = proot;
}

FoldersService::~FoldersService(void)
{

}

void FoldersService::render_GET(const http_request& req, http_response** res)
{
	Document respjsondoc;
	respjsondoc.SetArray();

	string strid = req.get_arg("id");
	HisDevFolder* folder = root->GetFolder();
	string path = req.get_path();
	if(!strid.empty())
	{
		CUUID id = CUUID::Parse(strid);
		folder = dynamic_cast<HisDevFolder*>(folder->Find(id));
	}

	if (path.find("/api/folders")!=string::npos)
	{
		if (folder!=NULL)
		{
			FoldersToJson(folder,respjsondoc);
		}
	}
	else if (path.find("/api/folder")!=string::npos)
	{
		if (folder!=NULL)
		{
			FolderToJson(folder,respjsondoc);
		}
	}


	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	respjsondoc.Accept(wr);
	std::string json = buffer.GetString();
	*res = new http_string_response(json, 200, "application/json");
}

void FoldersService::FolderToJson(HisBase *pFolder, Document & respjsondoc)
{
	Value d(kObjectType);
	string strvalue = pFolder->GetName();
	Value jsonvalue;
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember("name",jsonvalue, respjsondoc.GetAllocator());

	strvalue = pFolder->GetRecordId().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember("id",jsonvalue, respjsondoc.GetAllocator());

	if (pFolder->GetParent()->GetRecordId()!=root->GetFolder()->GetRecordId())
	{
		strvalue = pFolder->GetParent()->GetRecordId().ToString();
		jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
		d.AddMember("parentId",jsonvalue, respjsondoc.GetAllocator());
	}
	respjsondoc.PushBack(d, respjsondoc.GetAllocator());
}

void FoldersService::FoldersToJson(HisDevFolder *parentFolder, Document & respjsondoc)
{
	vector<HisDevFolder*> folders;
	folders = parentFolder->GetItems<HisDevFolder>();
	for(size_t i=0;i<folders.size();i++)
	{
		HisBase* pFolder = folders[i];
		FolderToJson(pFolder,respjsondoc);
	}
}

void FoldersService::render_POST(const http_request& req, http_response** res)
{
	std::string content = req.get_content();

	if (req.get_user()=="a" && req.get_pass()=="a")
	{

		//string address = req.get_arg("id");

		if (CreateFolder(content))
		{
			*res = new http_string_response("", 200, "application/json");
			return;
		}
	}
	else
	{
		string message = "Autentication error";
		*res = new http_string_response(message.c_str(), 401, "application/json");
		return;
	}

	*res = new http_string_response("", 403, "application/json");
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

	CUUID parentid;
	if (document.HasMember("parentId"))
	{
		parentid = CUUID::Parse(document["parentId"].GetString());
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

		if (UpdateFolder(strid,content))
		{
			*res = new http_string_response("", 200, "application/json");
			return;
		}
	}
	else
	{
		string message = "Autentication error";
		*res = new http_string_response(message.c_str(), 401, "application/json");
		return;
	}

	*res = new http_string_response("", 403, "application/json");

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

			*res = new http_string_response("", 200, "application/json");
			return;
		}
	}
	else
	{
		string message = "Autentication error";
		*res = new http_string_response(message.c_str(), 401, "application/json");
		return;
	}

	*res = new http_string_response("", 403, "application/json");

}
