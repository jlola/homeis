/*
 * OneWireDevicesService.h
 *
 *  Created on: Dec 31, 2012
 *      Author: root
 */

#ifndef ONEWIREDEVICESSERVICE_H_
#define ONEWIREDEVICESSERVICE_H_

#include <httpserver.hpp>
#include "homeis/Devices/Folder/HisDevFolder.h"
#include "homeis/Devices/Folder/HisDevFolderRoot.h"
#include "homeis/Devices/Folder/HisDevValueId.h"
#include "VirtualDevices/HisDevVirtual.h"

using namespace std;
using namespace httpserver;
using namespace rapidjson;

class OneWireDevicesService : public http_resource<OneWireDevicesService>
{
	HisDevices & devices;
	HisDevFolderRoot & rootFolder;
	bool UpdateDevValue(string address, string strjson);
	//bool SetForce(string address, string force);
	HisDevVirtual* CreateVirtualDevice(string strjson);
	HisDevBase* UpdateDevice(string strjson,string strDevId);
	bool AddValueIdToFolder(string strFolerId, string strJson);
	bool DeleteValueId(string strValueId);
	string DeleteDevice(string strDevValueRecordId);
public:
	OneWireDevicesService(HisDevices & dev,HisDevFolderRoot & folder);
	~OneWireDevicesService(void);
	void render_GET(const http_request&, http_response**);
	void render_POST(const http_request& r, http_response** res);
	void render_PUT(const http_request& req, http_response** res);
	void render_DELETE(const http_request& req, http_response** res);
	void FillDeviceToJson(Value & d, HisDevBase* dev,Document & respjsondoc);
	void DevValueToJson(Value & d, HisDevValueId* valueId,HisDevValueBase* devValue,Document & respjsondoc);
};

#endif /* ONEWIREDEVICESSERVICE_H_ */
