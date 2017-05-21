/*
 * OneWireDevicesService.h
 *
 *  Created on: Dec 31, 2012
 *      Author: root
 */

#ifndef ONEWIREDEVICESSERVICE_H_
#define ONEWIREDEVICESSERVICE_H_

#include <httpserver.hpp>
#include "Devices/Folder/HisDevFolder.h"
#include "Devices/Folder/HisDevFolderRoot.h"
#include "Devices/Folder/HisDevValueId.h"
#include "VirtualDevices/HisDevVirtual.h"
#include "HisDevices.h"

using namespace std;
using namespace httpserver;
using namespace rapidjson;

class DevicesService : public http_resource
{
	HisDevices & devices;
	HisDevFolderRoot & rootFolder;
	bool UpdateDevValue(CUUID devValueId, string strjson);
	//bool SetForce(string address, string force);
	HisDevValueBase* CreateVirtualDevValue(string strjson,string & message);
	HisDevVirtual* CreateVirtualDevice(string strjson,string & message);
	HisDevBase* UpdateDevice(string strjson,string strDevId);
	bool DeleteValueId(string strValueId);
	string DeleteDevValue(string strDevValueRecordId);
	string DeleteDev(string strDevValueRecordId);
public:
	DevicesService(HisDevices & dev,HisDevFolderRoot & folder);
	~DevicesService(void);
	static void FillFolderDevicesToJson(HisDevFolder* folder,Document & respjsondoc, HisDevices & dev);
	void render_GET(const http_request&, http_response**);
	void render_POST(const http_request& r, http_response** res);
	void render_PUT(const http_request& req, http_response** res);
	void render_DELETE(const http_request& req, http_response** res);
	static void FillDeviceToJson(Value & d, HisDevBase* dev,Document & respjsondoc);
	static void DevValueToJson(Value & d, HisDevValueId* valueId,HisDevValueBase* devValue,Document & respjsondoc);
};

#endif /* ONEWIREDEVICESSERVICE_H_ */
