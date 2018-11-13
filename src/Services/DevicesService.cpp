/*
 * OneWireDevicesService.cpp
 *
 *  Created on: Dec 31, 2012
 *      Author: root
 */

#include "Services/DevicesService.h"
#include "HisDevices.h"
#include "document.h"		// rapidjson's DOM-style API
#include "prettywriter.h"
#include "stringbuffer.h"
#include "filestream.h"	// wrapper of C stream for prettywriter as output
#include "HisDevValue.h"
#include "EItemType.h"
#include "Helpers/StringBuilder.h"
#include "HisDevModbus.h"
#include "PoppyDebugTools.h"
#include "HisDevValueEmail.h"
#include "microhttpd.h"

using namespace rapidjson;

const char json[] = "[{\"name\":\"Bečka horní\",\"id\":4,\"type\":1,\"value\":1.5},  \
                         {\"name\":\"Bečka prostřední\",\"id\":0,\"type\":0,\"value\":12.5},  \
                         {\"name\":\"Bečka spodní\",\"id\":5,\"type\":0,\"value\":80}, \
                         {\"name\":\"Podlahový okruh\",\"id\":1,\"type\":1,\"value\":23}, \
                         {\"name\":\"Radiatorový okruh\",\"id\":3,\"type\":0,\"value\":45.4}]";

DevicesService::DevicesService(HisDevices & dev,
		HisDevFolderRoot & folder,
		IUserManager* userManager,
		IHisDevFactory* factory,
		webserver* ws_i) :
		ServiceBase::ServiceBase(factory, userManager),
		logger(CLogger::GetLogger()),
		devices(dev),
		rootFolder(folder),
		factory(factory)
{
	ws_i->register_resource(string("api/onewiredevices"), this, true);
	ws_i->register_resource(string("api/devices"), this, true);
	ws_i->register_resource(string("api/onewiredevices/{devid}"), this, true);
	ws_i->register_resource(string("api/onewiredevices/devvalue/{valueid}"), this, true);
	ws_i->register_resource(string("api/devices/devvalue/{valueid}"), this, true);
	ws_i->register_resource(string("api/devices/{devid}"), this, true);
	ws_i->register_resource(string("api/devices/{devid}/devvalues/{valueid}"),this,true);
	ws_i->register_resource(string("api/onewiredevices/folder/{id}"), this, true);
}


DevicesService::~DevicesService(void)
{
}

const http_response DevicesService::GET(const http_request& req)
{
	STACK
	Document respjsondoc;
	respjsondoc.SetArray();
	string path = req.get_path();

	logger.Trace("DevicesService::render_GET: %s.",path.c_str());

	int response_code = MHD_HTTP_FORBIDDEN;

	if (path.find("/api/devices/devvalue")!=string::npos)
	{
		string strvalueid = req.get_arg("valueid");
		CUUID valueid = CUUID::Parse(strvalueid);
		HisDevValueBase* tag = devices.FindValue(valueid);
		if (tag != NULL)
		{
			respjsondoc.SetObject();
			DevValueToJson(respjsondoc,NULL,tag,respjsondoc);
			response_code = MHD_HTTP_OK;
		}
		else
		{
			response_code = MHD_HTTP_NOT_FOUND;
		}
	} //devices/{devid}/devvalue/{valueid}
	else if ((path.find("/api/devices")!=string::npos ||
		path.find("/api/onewiredevices")!=string::npos) &&
			path.find("devvalue")!=string::npos)
	{
		string strdevid = req.get_arg("devid");
		string strvalueid = req.get_arg("valueid");
		CUUID devid = CUUID::Parse(strdevid);
		CUUID valueid = CUUID::Parse(strvalueid);

		int index = devices.Find(devid);
		if (index>=0)
		{
			//Value devjson(kObjectType);
			respjsondoc.SetObject();
			HisDevBase* dev = devices[index];
			HisDevValueBase* devvalue = dynamic_cast<HisDevValueBase*>(dev->Find(valueid));
			if (devvalue!=NULL)
			{
				DevValueToJson(respjsondoc,NULL,devvalue,respjsondoc);
			}
			response_code = MHD_HTTP_OK;
		}
		else
		{
			response_code = MHD_HTTP_NOT_FOUND;
		}
	}
	else if (path.find("/api/onewiredevices/folder")!=string::npos ||
			 path.find("/api/devices/folder")!=string::npos)
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
		response_code = MHD_HTTP_OK;
	}
	else if (path.find("/api/onewiredevices")!=string::npos ||
			 path.find("/api/devices")!=string::npos)
	{
		string strDevId = req.get_arg("devid");
		if (strDevId.length()>0)
		{
			CUUID devId = CUUID::Parse(strDevId);
			int index = devices.Find(devId);
			if (index>=0)
			{
				//Value devjson(kObjectType);
				respjsondoc.SetObject();
				FillDeviceToJson(respjsondoc,devices[index],respjsondoc);
				//respjsondoc.PushBack(devjson, respjsondoc.GetAllocator());
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
		response_code = MHD_HTTP_OK;
	}
	else
	{
		response_code = MHD_HTTP_NOT_FOUND;
	}

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	respjsondoc.Accept(wr);
	std::string json = buffer.GetString();

	return CreateResponseString(json,response_code);
}

void DevicesService::FillFolderDevicesToJson(HisDevFolder* folder,Document & respjsondoc,HisDevices & devices)
{
	STACK
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

void DevicesService::FillDeviceToJson(Value & devjson, HisDevBase* dev,Document & respjsondoc)
{

	Value jsonvalue;
	string strvalue = dev->GetName();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	devjson.AddMember(JSON_NAME,jsonvalue,respjsondoc.GetAllocator());

	strvalue = dev->GetCreateDateTime().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	devjson.AddMember(JSON_CREATETIME,jsonvalue, respjsondoc.GetAllocator());

	strvalue = dev->GetModifyDateTime().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	devjson.AddMember(JSON_MODIDFYTIME,jsonvalue, respjsondoc.GetAllocator());

	strvalue = dev->GetRecordId().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	devjson.AddMember(JSON_ID,jsonvalue,respjsondoc.GetAllocator());

	jsonvalue.SetUint(dev->GetScanPeriod());
	devjson.AddMember(JSON_SCANPERIODMS,jsonvalue,respjsondoc.GetAllocator());

	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	jsonvalue.SetBool(dev->IsEnabled());
	devjson.AddMember(JSON_ENABLED,jsonvalue,respjsondoc.GetAllocator());

	HisDevModbus* devModbus = dynamic_cast<HisDevModbus*>(dev);
	if (devModbus!=NULL)
	{
		strvalue = Converter::itos(devModbus->GetAddress(),10);
		jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
		devjson.AddMember(JSON_ADDRESSNAME,jsonvalue,respjsondoc.GetAllocator());

		IModbus* modbusDriver = devModbus->GetModbus();
		if (modbusDriver!=NULL)
		{
			jsonvalue.SetString(modbusDriver->GetName().c_str(),modbusDriver->GetName().length(),respjsondoc.GetAllocator());
		}
		else
		{
			jsonvalue.Clear();
		}
		devjson.AddMember(JSON_CONNECTIONNAME,jsonvalue,respjsondoc.GetAllocator());
	}

	HisDevVirtual* devVirtual = dynamic_cast<HisDevVirtual*>(dev);
	jsonvalue.SetBool(devVirtual!=NULL);
	devjson.AddMember(JSON_INTERNAL,jsonvalue,respjsondoc.GetAllocator());

	Value tagsjson(kArrayType);
	vector<HisDevValueBase*> tagvalues = dev->GetValues();
	for(uint16_t v=0;v<tagvalues.size();v++)
	{
		Value d(kObjectType);
		DevValueToJson(d,NULL,tagvalues[v],respjsondoc);
		tagsjson.PushBack(d,respjsondoc.GetAllocator());
	}
	devjson.AddMember(JSON_TAGS,tagsjson,respjsondoc.GetAllocator());
}

void DevicesService::DevValueToJson(Value & d, HisDevValueId* valueId,HisDevValueBase* devValue,Document & respjsondoc)
{
	string strvalue = devValue->GetPinName();
	Value jsonvalue;
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_NAME,jsonvalue, respjsondoc.GetAllocator());

	strvalue = devValue->GetCreateDateTime().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_CREATETIME,jsonvalue, respjsondoc.GetAllocator());

	strvalue = devValue->GetModifyDateTime().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_MODIDFYTIME,jsonvalue, respjsondoc.GetAllocator());


	if (valueId!=NULL)
	{
		strvalue = valueId->GetRecordId().ToString();
		jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
		d.AddMember( JSON_DIRVALUEID,jsonvalue, respjsondoc.GetAllocator());

		if (valueId->GetParent()!=NULL)
		{
			strvalue = valueId->GetParent()->GetRecordId().ToString();
			jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
			d.AddMember( JSON_PARENTID,jsonvalue, respjsondoc.GetAllocator());
		}
	}

	strvalue = devValue->GetRecordId().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_ID,jsonvalue, respjsondoc.GetAllocator());

	strvalue = (const char*)devValue->GetNodeName();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_NODENAME,jsonvalue, respjsondoc.GetAllocator());

	strvalue = devValue->GetAddress();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_ADDRESS,jsonvalue, respjsondoc.GetAllocator());

	d.AddMember(JSON_TYPE,devValue->GetDataType(), respjsondoc.GetAllocator());
	d.AddMember(JSON_DIRECTION,devValue->GetDirection(), respjsondoc.GetAllocator());

	strvalue = devValue->GetStringValue();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_VALUE,jsonvalue, respjsondoc.GetAllocator());

	strvalue = devValue->GetValueChangeTime().ToString();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember(JSON_VALUECHANGETIME,jsonvalue, respjsondoc.GetAllocator());

	d.AddMember(JSON_ERROR,devValue->GetDeviceError(),respjsondoc.GetAllocator());
	d.AddMember(JSON_FORCE,devValue->GetForceOutput(),respjsondoc.GetAllocator());

	strvalue = devValue->GetUnit();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember( JSON_UNIT,jsonvalue, respjsondoc.GetAllocator());

	strvalue = devValue->GetAddressName();
	jsonvalue.SetString(strvalue.c_str(),strvalue.length(),respjsondoc.GetAllocator());
	d.AddMember( JSON_ADDRESSNAME,jsonvalue, respjsondoc.GetAllocator());

	HisDevVirtual* virtdev = dynamic_cast<HisDevVirtual*>(devValue->GetParent());
	if (virtdev!=NULL)
		d.AddMember(JSON_INTERNAL,true, respjsondoc.GetAllocator());
	else
		d.AddMember(JSON_INTERNAL,false, respjsondoc.GetAllocator());
}

const http_response DevicesService::POST(const http_request& req)
{
	std::string content = req.get_content();
	string path = req.get_path();

	string message;
	int response_code = MHD_HTTP_FORBIDDEN;

	Document respjsondoc;
	respjsondoc.SetArray();
	//respjsondoc.SetObject();
	StringBuffer buffer;


	if (path.find("/api/onewiredevices/devvalue")!=string::npos ||
		path.find("/api/devices/devvalue")!=string::npos)
	{
		HisDevValueBase* devvalue = CreateVirtualDevValue(content,message);
		if (devvalue!=NULL)
		{
			if (UpdateDevValue(devvalue->GetRecordId(),content))
			{
				response_code = MHD_HTTP_OK;
			}
		}
		else
		{
			message = GetErrorMessageJson("Not found");
			response_code = MHD_HTTP_NOT_FOUND;
		}
	}
	else if (path.find("/api/onewiredevices")!=string::npos ||
			 path.find("/api/devices")!=string::npos)
	{
		HisDevVirtual* vritualdev = CreateVirtualDevice(content,message);
		if (vritualdev!=NULL)
		{
			devices.Add(vritualdev);
			devices.Save();
			string id = vritualdev->GetRecordId().ToString();
			Value d(kObjectType);
			//DevValueToJson(d,NULL,value,respjsondoc);
			Value jsonvalue;
			jsonvalue.SetString(id.c_str(),id.length(),respjsondoc.GetAllocator());
			d.AddMember(JSON_ID,jsonvalue, respjsondoc.GetAllocator());
			respjsondoc.PushBack(d,respjsondoc.GetAllocator());
			PrettyWriter<StringBuffer> wr(buffer);
			respjsondoc.Accept(wr);
			std::string json = buffer.GetString();
			response_code = MHD_HTTP_OK;
			message = json;
		}
		else
		{
			message = GetErrorMessageJson("Not found");
			response_code = MHD_HTTP_NOT_FOUND;
		}
	}


	return CreateResponseString(message,response_code);
}

const http_response DevicesService::PUT(const http_request& req)
{
	std::string content = req.get_content();
	string path = req.get_path();
	string message;
	int response_code = MHD_HTTP_FORBIDDEN;

	if (path.find("/api/onewiredevices/devvalue")!=string::npos ||
		path.find("/api/devices/devvalue")!=string::npos)
	{
		string strDevValueId = req.get_arg("valueid");
		CUUID devValueId = CUUID::Parse(strDevValueId);
		if (UpdateDevValue(devValueId,content))
		{
			response_code = MHD_HTTP_OK;
			message = "OK";
		}
	}
	else if (path.find("/api/onewiredevices")!=string::npos ||
			 path.find("/api/devices")!=string::npos)
	{
		string strDevId = req.get_arg("devid");
		if (UpdateDevice(strDevId,content))
		{
			message = "OK";
			response_code = MHD_HTTP_OK;
		}
	}

	return CreateResponseString(message,response_code);
}

const http_response DevicesService::DELETE(const http_request& req)
{
	std::string content = req.get_content();
	string path = req.get_path();
	int response_code = MHD_HTTP_FORBIDDEN;
	string msg;


	if (path.find("/api/onewiredevices/folder")!=string::npos)
	{
		string strValueIdRecordId = req.get_arg("id");

		if (DeleteValueId(strValueIdRecordId,msg))
		{
			msg = "OK";
			response_code = MHD_HTTP_OK;
		}
	}
	else if (path.find("/api/onewiredevices/devvalue")!=string::npos)
	{
		string strRecordId = req.get_arg("id");
		if (DeleteDevValue(strRecordId,msg))
		{
			response_code = MHD_HTTP_OK;
			msg = "OK";
		}
	}
	else if (path.find("/api/onewiredevices")!=string::npos)
	{
		string strRecordId = req.get_arg("devid");
		if (DeleteDev(strRecordId,msg))
		{
			response_code = MHD_HTTP_OK;
			msg = "OK";
		}
	}

	Document respjsondoc;
	respjsondoc.SetObject();
	StringBuffer buffer;

	Value jsonvalue;
	jsonvalue.SetString(msg.c_str(),msg.length(),respjsondoc.GetAllocator());
	respjsondoc.AddMember(JSON_MESSAGE,jsonvalue, respjsondoc.GetAllocator());

	PrettyWriter<StringBuffer> wr(buffer);
	respjsondoc.Accept(wr);
	std::string json = buffer.GetString();

	return CreateResponseString(json,response_code);
}

bool DevicesService::DeleteDevValue(string strDevValueRecordId, string & msg)
{
	CUUID devValueId = CUUID::Parse(strDevValueRecordId);
	HisDevValueBase* devValue = devices.FindValue(devValueId);
	if (devValue==NULL)
	{
		msg += StringBuilder::Format("OneWireDevicesService::DeleteDevValue | Doesn't exists");
		logger.Error(msg.c_str());
		return false;
	}
	HisDevBase* device = dynamic_cast<HisDevBase*>(devValue->GetParent());

	HisDevValueId* usedIn = this->rootFolder.FindValueId(devValueId);
	if (usedIn!=NULL)
	{
		HisDevFolder* folder = dynamic_cast<HisDevFolder*>(usedIn->GetParent());
		if (folder!=NULL)
		{
			msg += StringBuilder::Format("OneWireDevicesService::DeleteDevValue | Cant delete node %s used in folder %s",strDevValueRecordId.c_str(),folder->GetName().c_str());
			logger.Error(msg.c_str());
			return false;
		}
	}
	else
	{
		IHisBase* devvalue = device->Remove(devValueId);
		if (devvalue!=NULL)
		{
			delete(devvalue);
			devvalue = NULL;
			devices.Save();
		}
		else
		{
			msg += StringBuilder::Format("OneWireDevicesService::DeleteDevValue | Error while delete.");
			logger.Error(msg.c_str());
			return false;
		}
	}
	return true;
}

bool DevicesService::DeleteDev(string strDevValueRecordId, string & msg)
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
					msg += StringBuilder::Format("OneWireDevicesService::DeleteDevice | Cant delete node %s used in folder %s",strDevValueRecordId.c_str(),folder->GetName().c_str());
					logger.Error(msg.c_str());
					return false;
				}
			}
		}

		devices.Delete(index);
		devices.Save();
		return true;
	 }

	msg = StringBuilder::Format("OneWireDevicesService::DeleteDevice | Can't delete not existed dev");
	logger.Error(msg.c_str());
	return false;
}

bool DevicesService::DeleteValueId(string strValueId, string & message)
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
	message = StringBuilder::Format("ValueId %s not found.",strValueId.c_str());
	return false;
}


HisDevVirtual* DevicesService::CreateVirtualDevice(string strjson,string & message)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.

	if (document.Parse<0>((char*)strjson.c_str()).HasParseError())
		return NULL;
	HisDevVirtual* virtualdev = NULL;
	if (document.HasMember(JSON_NAME) && document[JSON_NAME].IsString())
	{
		virtualdev = new HisDevVirtual(factory);
		virtualdev->SetName(document[JSON_NAME].GetString());
	}
	else
	{
		message = "Error creating VirtualDevice. Name should be filled in.";
	}
	return virtualdev;
}

HisDevValueBase* DevicesService::CreateVirtualDevValue(string strjson,string & message)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.

	if (document.Parse<0>((char*)strjson.c_str()).HasParseError())
		return NULL;

	if (document.HasMember(JSON_PARENTID) && document[JSON_PARENTID].IsString())
	{
		string strparentid = document[JSON_PARENTID].GetString();
		CUUID parentid = CUUID::Parse(strparentid);

		int index = devices.Find(parentid);
		if (index>=0)
		{
			HisDevVirtual* virtualdev = dynamic_cast<HisDevVirtual*>(devices[index]);
			if (document.HasMember(JSON_TYPE) && document[JSON_TYPE].IsInt())
			{
				EDataType type = (EDataType)document[JSON_TYPE].GetInt();
				return virtualdev->AddDevValue(type);

			}
			else
			{
				message = "Creating VirtualDevValue. Type shoould be filled in.";
			}
		 }
	}

	return NULL;
}

HisDevBase* DevicesService::UpdateDevice(string strDevId, string strjson)
{
	STACK
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
			if (document.HasMember(JSON_NAME) && document[JSON_NAME].IsString())
			{
				if (dev->GetName()!=document[JSON_NAME].GetString())
				{
					dev->SetName(document[JSON_NAME].GetString());
					changed = true;
				}
			}
			if (document.HasMember(JSON_ENABLED) && document[JSON_ENABLED].IsBool())
			{
				bool enabled = document[JSON_ENABLED].GetBool();
				if (dev->IsEnabled()!=enabled)
				{
					dev->Enable(enabled);
					changed = true;
				}
			}
			if (document.HasMember(JSON_SCANPERIODMS) && document[JSON_SCANPERIODMS].IsUint())
			{

				if (dev->GetScanPeriod()!=document[JSON_SCANPERIODMS].GetUint())
				{
					dev->SetScanPeriod(document[JSON_SCANPERIODMS].GetUint());
					changed = true;
				}
			}
			if (document.HasMember(JSON_TAGS))
			{
				const Value& a = document[JSON_TAGS];
				if (a.IsArray())
				{
					for (Value::ConstValueIterator itr = a.Begin(); itr != a.End(); ++itr)
					{
						if (itr->IsObject())
						{
							if (itr->HasMember(JSON_ID))
							{
								string tagname = itr->operator[](JSON_ID).GetString();
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

bool DevicesService::UpdateDevValue(CUUID devValueId, string strjson)
{
	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.

	if (document.Parse<0>((char*)strjson.c_str()).HasParseError())
		return false;
	bool saveReq = false;
	HisDevValueBase* devValue = devices.FindValue(devValueId);
	if (devValue != NULL)
	{
		if (document.HasMember(JSON_NAME) && document[JSON_NAME].IsString())
		{
			string name = document[JSON_NAME].GetString();
			if (name!=devValue->GetPinName())
			{
				devValue->SetPinName(name);
				saveReq = true;
			}
		}

		if (document.HasMember(JSON_ADDRESSNAME) && document[JSON_ADDRESSNAME].IsString())
		{
			string addressname = document[JSON_ADDRESSNAME].GetString();
			if (addressname!=devValue->GetAddressName())
			{
				devValue->SetAddressName(addressname);
				saveReq = true;
			}
		}

		if (document.HasMember(JSON_UNIT) && document[JSON_UNIT].IsString())
		{
			string strunit = document[JSON_UNIT].GetString();
			if (strunit!=devValue->GetUnit())
			{
				devValue->SetUnit(strunit);
				saveReq = true;
			}
		}

		if (document.HasMember(JSON_FORCE) && document[JSON_FORCE].IsBool())
		{
			bool force = document[JSON_FORCE].GetBool();
			devValue->SetForceOutput(force);
		}
		if (document.HasMember(JSON_VALUE) && document[JSON_VALUE].IsString())
		{
			string strvalue = document[JSON_VALUE].GetString();

			if (devValue->GetDirection()==EHisDevDirection::ReadWrite ||
				devValue->GetDirection()==EHisDevDirection::Write ||
				devValue->GetForceOutput())
			{
				devValue->ForceStringValue(strvalue,false);
				saveReq = true;
			}
			else
			{
				logger.Error("Try to write ReadOnly deviceValue %s",devValue->GetName().c_str());
			}
		}

		HisDevValueEmail* valueEmail = dynamic_cast<HisDevValueEmail*>(devValue);
		if (valueEmail!=NULL)
		{
			if (document.HasMember(JSON_SENDER) && document[JSON_SENDER].IsString())
			{
				valueEmail->SetFromAddr(document[JSON_SENDER].GetString());
				saveReq = true;
			}
			if (document.HasMember(JSON_RECEIVERS) && document[JSON_RECEIVERS].IsString())
			{
				valueEmail->SetFromAddr(document[JSON_RECEIVERS].GetString());
				saveReq = true;
			}
		}

		if (saveReq) devices.Save();

		return true;
	}

	return false;
}
