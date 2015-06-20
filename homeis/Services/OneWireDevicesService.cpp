/*
 * OneWireDevicesService.cpp
 *
 *  Created on: Dec 31, 2012
 *      Author: root
 */

#include "HisDevices.h"
#include "document.h"		// rapidjson's DOM-style API
#include "prettywriter.h"
#include "stringbuffer.h"
#include "filestream.h"	// wrapper of C stream for prettywriter as output
#include "HisDevValue.h"
#include "EItemType.h"
#include "homeis/Helpers/StringBuilder.h"
#include "HisDallas.h"

#include "OneWireDevicesService.h"

using namespace rapidjson;

const char json[] = "[{\"name\":\"Bečka horní\",\"id\":4,\"type\":1,\"value\":1.5},  \
                         {\"name\":\"Bečka prostřední\",\"id\":0,\"type\":0,\"value\":12.5},  \
                         {\"name\":\"Bečka spodní\",\"id\":5,\"type\":0,\"value\":80}, \
                         {\"name\":\"Podlahový okruh\",\"id\":1,\"type\":1,\"value\":23}, \
                         {\"name\":\"Radiatorový okruh\",\"id\":3,\"type\":0,\"value\":45.4}]";

OneWireDevicesService::OneWireDevicesService(HisDevices & dev,HisDevFolderRoot & folder) :
		devices(dev),rootFolder(folder)
{
}


OneWireDevicesService::~OneWireDevicesService(void)
{
}

void OneWireDevicesService::render_GET(const http_request& req, http_response** res)
{
	Document respjsondoc;
	respjsondoc.SetArray();
	string path = req.get_path();
	//CLogger::Info(path.c_str());
	//Value a(kArrayType);

	if (path.find("/api/onewiredevices/folder")!=string::npos)
	{
		string strFolderId = req.get_arg("id");
		HisDevFolder* folder = NULL;
		if(!strFolderId.empty())
		{
			CUUID folderId = CUUID::Parse(strFolderId);
			folder = dynamic_cast<HisDevFolder*>(rootFolder.GetFolder()->Find(folderId));
		}
		if (folder==NULL)
		{
			folder = rootFolder.GetFolder();
		}
		//ToJson(folder,respjsondoc);
		FillFolderDevicesToJson(folder,respjsondoc,devices);
	}
	else if (path.find("/api/onewiredevices")!=string::npos)
	{
		string strDevId = req.get_arg("id");
		if (strDevId.length()>0)
		{
			CUUID devId = CUUID::Parse(strDevId);
			int index = devices.Find(devId);
			if (index>=0)
			{
				Value devjson(kObjectType);
				FillDeviceToJson(devjson,devices[index],respjsondoc);
				respjsondoc.PushBack(devjson, respjsondoc.GetAllocator());
			}
		}
		else
		{
			for(uint16_t i=0;i<devices.Size();i++)
			{
				Value devjson(kObjectType);
				FillDeviceToJson(devjson,devices[i],respjsondoc);
				respjsondoc.PushBack(devjson, respjsondoc.GetAllocator());

			}
		}
	}

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	respjsondoc.Accept(wr);
	std::string json = buffer.GetString();
	*res = new http_string_response(json, 200, "application/json");
}

void OneWireDevicesService::FillFolderDevicesToJson(HisDevFolder* folder,Document & respjsondoc,HisDevices & devices)
{
	vector<HisDevValueId*> valueIds = folder->GetItems<HisDevValueId>();
	for(size_t i=0;i<valueIds.size();i++)
	{
		Value d(kObjectType);
		CUUID valueId = valueIds[i]->GetDeviceValueId();
		HisDevValueBase* val = devices.FindValue(valueId);
		if (val!=NULL)
		{
			DevValueToJson(d,valueIds[i],val,respjsondoc);
			respjsondoc.PushBack(d, respjsondoc.GetAllocator());
		}
	}
}

void OneWireDevicesService::FillDeviceToJson(Value & devjson, HisDevBase* dev,Document & respjsondoc)
{

	Value jsonvalue;
	string strvalue = dev->GetName();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	devjson.AddMember("Name",jsonvalue,respjsondoc.GetAllocator());

	strvalue = dev->GetRecordId().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	devjson.AddMember("Id",jsonvalue,respjsondoc.GetAllocator());

	HisDevDallas* devDallas = dynamic_cast<HisDevDallas*>(dev);
	if (devDallas!=NULL)
	{
		strvalue = devDallas->GetId().getRomIDString();
		jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
		devjson.AddMember("Address",jsonvalue,respjsondoc.GetAllocator());
	}

	HisDevVirtual* devVirtual = dynamic_cast<HisDevVirtual*>(dev);
	jsonvalue.SetBool(devVirtual!=NULL);
	devjson.AddMember("Internal",jsonvalue,respjsondoc.GetAllocator());

	Value tagsjson(kArrayType);
	vector<HisDevValueBase*> tagvalues = dev->GetValues();
	for(uint16_t v=0;v<tagvalues.size();v++)
	{
		Value d(kObjectType);
		DevValueToJson(d,NULL,tagvalues[v],respjsondoc);
		tagsjson.PushBack(d,respjsondoc.GetAllocator());
	}
	devjson.AddMember("Tags",tagsjson,respjsondoc.GetAllocator());


}

void OneWireDevicesService::DevValueToJson(Value & d, HisDevValueId* valueId,HisDevValueBase* devValue,Document & respjsondoc)
{
	string strvalue = devValue->GetPinName();
	Value jsonvalue;
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember( "name",jsonvalue, respjsondoc.GetAllocator());

	if (valueId!=NULL)
	{
		strvalue = valueId->GetRecordId().ToString();
		jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
		d.AddMember( "DirValueId",jsonvalue, respjsondoc.GetAllocator());
	}

	strvalue = devValue->GetRecordId().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember( "id",jsonvalue, respjsondoc.GetAllocator());

	strvalue = (const char*)devValue->GetNodeName();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember("NodeName",jsonvalue, respjsondoc.GetAllocator());

	strvalue = devValue->GetAddress();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember( "address",jsonvalue, respjsondoc.GetAllocator());

	d.AddMember( "type",devValue->GetDataType(), respjsondoc.GetAllocator());
	d.AddMember( "direction",devValue->GetDirection(), respjsondoc.GetAllocator());
	strvalue = devValue->GetStringValue();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember( "value",jsonvalue, respjsondoc.GetAllocator());

	d.AddMember("error",devValue->GetDeviceError(),respjsondoc.GetAllocator());
	d.AddMember("force",devValue->GetForceOutput(),respjsondoc.GetAllocator());
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember( "value",jsonvalue, respjsondoc.GetAllocator());

	strvalue = devValue->GetUnit();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember( "unit",jsonvalue, respjsondoc.GetAllocator());

	HisDevVirtual* virtdev = dynamic_cast<HisDevVirtual*>(devValue->GetParent());
	if (virtdev!=NULL)
		d.AddMember( "internal",true, respjsondoc.GetAllocator());
	else
		d.AddMember( "internal",false, respjsondoc.GetAllocator());
}

void OneWireDevicesService::render_POST(const http_request& req, http_response** res)
{
	std::string content = req.get_content();
	string path = req.get_path();

	string message;

	Document respjsondoc;
	respjsondoc.SetArray();
	//respjsondoc.SetObject();
	StringBuffer buffer;

	if (req.get_user()=="a" && req.get_pass()=="a")
	{
		if (path.find("/api/onewiredevices/devvalue")!=string::npos)
		{
			HisDevValueBase* devvalue = CreateVirtualDevValue(content);
			if (devvalue!=NULL)
			{
				if (UpdateDevValue(devvalue->GetRecordId(),content))
				{
					*res = new http_string_response("", 200, "application/json");
					return;
				}
			}
		}
		else if (path.find("/api/onewiredevices")!=string::npos)
		{
			HisDevVirtual* vritualdev = CreateVirtualDevice(content);
			if (vritualdev!=NULL)
			{
				devices.Add(vritualdev);
				devices.Save();
				string id = vritualdev->GetRecordId().ToString();
				Value d(kObjectType);
				//DevValueToJson(d,NULL,value,respjsondoc);
				Value jsonvalue;
				jsonvalue.SetString(id.c_str(),id.length(),respjsondoc.GetAllocator());
				d.AddMember( "id",jsonvalue, respjsondoc.GetAllocator());
				respjsondoc.PushBack(d,respjsondoc.GetAllocator());
				PrettyWriter<StringBuffer> wr(buffer);
				respjsondoc.Accept(wr);
				std::string json = buffer.GetString();
				*res = new http_string_response(json, 200, "application/json");
				return;
			}
		}
	}
	else
	{
		string message = "Authentication error";
		*res = new http_string_response(message.c_str(), 401, "application/json");
		return;
	}
	*res = new http_string_response("", 403, "application/json");
}

void OneWireDevicesService::render_PUT(const http_request& req, http_response** res)
{
	std::string content = req.get_content();
	string path = req.get_path();

	if (req.get_user()=="a" && req.get_pass()=="a")
	{
		if (path.find("/api/onewiredevices/devvalue")!=string::npos)
		{
			string strDevValueId = req.get_arg("id");
			CUUID devValueId = CUUID::Parse(strDevValueId);
			if (UpdateDevValue(devValueId,content))
			{
				*res = new http_string_response("", 200, "application/json");
				return;
			}
		}
		else if (path.find("/api/onewiredevices")!=string::npos)
		{
			string strDevId = req.get_arg("id");
			if (UpdateDevice(strDevId,content))
			{
				*res = new http_string_response("", 200, "application/json");
				return;
			}
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

void OneWireDevicesService::render_DELETE(const http_request& req, http_response** res)
{
	std::string content = req.get_content();
	string path = req.get_path();
	string msg;
	if (req.get_user()=="a" && req.get_pass()=="a")
	{
		if (path.find("/api/onewiredevices/folder")!=string::npos)
		{
			string strValueIdRecordId = req.get_arg("id");

			if (DeleteValueId(strValueIdRecordId))
			{
				*res = new http_string_response("", 200, "application/json");
				return;
			}
		}
		else if (path.find("/api/onewiredevices/devvalue")!=string::npos)
		{
			string strRecordId = req.get_arg("id");
			msg = DeleteDevValue(strRecordId);
			if (msg=="")
			{
				*res = new http_string_response("OK", 200, "application/json");
				return;
			}
		}
		else if (path.find("/api/onewiredevices")!=string::npos)
		{
			string strRecordId = req.get_arg("id");
			msg = DeleteDev(strRecordId);
			if (msg=="")
			{
				*res = new http_string_response("OK", 200, "application/json");
				return;
			}
		}
	}
	else
	{
		string message = "Autentication error";
		*res = new http_string_response(message.c_str(), 401, "application/json");
		return;
	}

	Document respjsondoc;
	//respjsondoc.SetArray();
	respjsondoc.SetObject();
	StringBuffer buffer;

	//Value d(kObjectType);
	Value jsonvalue;
	jsonvalue.SetString(msg.c_str(),msg.length(),respjsondoc.GetAllocator());
	respjsondoc.AddMember("message",jsonvalue, respjsondoc.GetAllocator());

	PrettyWriter<StringBuffer> wr(buffer);
	respjsondoc.Accept(wr);
	std::string json = buffer.GetString();
	*res = new http_string_response(json, 403, "application/json");
}

string OneWireDevicesService::DeleteDevValue(string strDevValueRecordId)
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

	HisDevValueId* usedIn = this->rootFolder.FindValueId(devValueId);
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

string OneWireDevicesService::DeleteDev(string strDevValueRecordId)
{
	CUUID devValueId = CUUID::Parse(strDevValueRecordId);
	 int index = devices.Find(devValueId);
	 if (index>=0)
	 {
		HisDevBase* device = dynamic_cast<HisDevBase*>(devices[index]);

		vector<HisDevValueBase*> values = device->GetItems<HisDevValueBase>();

		for(size_t i=0;i<values.size();i++)
		{
			HisDevValueId* usedIn = this->rootFolder.FindValueId(values[i]->GetRecordId());
			if (usedIn!=NULL)
			{
				HisDevFolder* folder = dynamic_cast<HisDevFolder*>(usedIn->GetParent());
				if (folder!=NULL)
				{
					string msg = StringBuilder::Format("OneWireDevicesService::DeleteDevice | Cant delete node %s used in folder %s",strDevValueRecordId.c_str(),folder->GetName().c_str());
					CLogger::Error(msg.c_str());
					return msg;
				}
			}
		}

		devices.Delete(index);
		devices.Save();
		return "";
	 }

	string msg = StringBuilder::Format("OneWireDevicesService::DeleteDevice | Cant delete not existed dev");
	CLogger::Error(msg.c_str());
	return msg;
}

bool OneWireDevicesService::DeleteValueId(string strValueId)
{

	CUUID valueIdRecordId = CUUID::Parse(strValueId);

	HisDevValueId* valueId = dynamic_cast<HisDevValueId*>(rootFolder.GetFolder()->FindValueId(valueIdRecordId));
	if (valueId!=NULL)
	{
		HisDevFolder* folder = dynamic_cast<HisDevFolder*>(valueId->GetParent());
		folder->Remove(valueId->GetRecordId());
		rootFolder.Save();
		return true;
	}
	return false;
}


HisDevVirtual* OneWireDevicesService::CreateVirtualDevice(string strjson)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.

	if (document.Parse<0>((char*)strjson.c_str()).HasParseError())
		return NULL;
	HisDevVirtual* virtualdev = new HisDevVirtual();
	if (document.HasMember("Name"))
	{
		virtualdev->SetName(document["Name"].GetString());
	}



	return virtualdev;
}

HisDevValueBase* OneWireDevicesService::CreateVirtualDevValue(string strjson)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.

	if (document.Parse<0>((char*)strjson.c_str()).HasParseError())
		return NULL;

	if (document.HasMember("parentid"))
	{
		string strparentid = document["parentid"].GetString();
		CUUID parentid = CUUID::Parse(strparentid);

		int index = devices.Find(parentid);
		if (index>=0)
		{
			HisDevVirtual* virtualdev = dynamic_cast<HisDevVirtual*>(devices[index]);
			if (document.HasMember("type"))
			{
				EDataType type = (EDataType)document["type"].GetInt();
				return virtualdev->AddDevValue(type);
			}
		 }
	}

	return NULL;
}

HisDevBase* OneWireDevicesService::UpdateDevice(string strDevId, string strjson)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.

	if (document.Parse<0>((char*)strjson.c_str()).HasParseError())
		return NULL;
	HisDevBase* dev = NULL;

	CUUID devId = CUUID::Parse(strDevId);
	bool changed = false;
	int index = devices.Find(devId);
	if (index>=0)
	{
		dev = dynamic_cast<HisDevBase*>(devices[index]);
		if (dev!=NULL)
		{
			if (document.HasMember("Name"))
			{
				if (dev->GetName()!=document["Name"].GetString())
				{
					dev->SetName(document["Name"].GetString());
					changed = true;
				}
			}
			if (document.HasMember("Tags"))
			{
				const Value& a = document["Tags"];
				if (a.IsArray())
				{
					for (Value::ConstValueIterator itr = a.Begin(); itr != a.End(); ++itr)
					{
						if (itr->IsObject())
						{
							if (itr->HasMember("id"))
							{
								string tagname = itr->operator[]("id").GetString();
							}
						}
					}
				}
			}
		}
		if (changed)
			devices.Save();
	}
	return dev;
}

bool OneWireDevicesService::UpdateDevValue(CUUID devValueId, string strjson)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.

	if (document.Parse<0>((char*)strjson.c_str()).HasParseError())
		return false;
	bool saveReq = false;
	HisDevValueBase* devValue = devices.FindValue(devValueId);
	if (devValue != NULL)
	{
		if (document.HasMember("name"))
		{
			string name = document["name"].GetString();
			if (name!=devValue->GetPinName())
			{
				devValue->SetPinName(name);
				saveReq = true;
			}
		}

		if (document.HasMember("unit"))
		{
			string strunit = document["unit"].GetString();
			if (strunit!=devValue->GetUnit())
			{
				devValue->SetUnit(strunit);
				saveReq = true;
			}
		}

		if (document.HasMember("force"))
		{
			bool force = document["force"].GetBool();
			devValue->SetForceOutput(force);
		}
		if (document.HasMember("value"))
		{
			string strvalue = document["value"].GetString();

			if (devValue->GetDirection()==EHisDevDirection::ReadWrite ||
				devValue->GetDirection()==EHisDevDirection::Write)
			{
				devValue->ForceStringValue(strvalue);
			}
			else
			{
				CLogger::Error("Try to write not writable deviceValue");
			}
		}

		if (saveReq) devices.Save();

		return true;
	}

	return false;
}

//void OneWireDevicesService::handle( LogicalConnection* pClient, IncomingPacket* pRequest )
//{
//	WebsocketDataMessage& request = (WebsocketDataMessage&)(*pRequest);
//	WebsocketClient& client = (WebsocketClient&) (*pClient);
//
//	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
//
//	if (document.Parse<0>((char*)request.GetData().c_str()).HasParseError())//ParseInsitu<0>((char*)raw.c_str()).HasParseError())
//			return;
//
//	WebsocketDataMessage response(eOneWireDevice);
//
//	if (document.HasMember("Command"))
//	{
//		std::string command = document["Command"].GetString();
//		if (command == "OneWireDeviceList")
//		{
//			response.SetData(json);
//		}
//	}
//
//
//	client.PushPacket(&response);
//}
