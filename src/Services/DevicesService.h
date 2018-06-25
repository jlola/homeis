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
#include "HttpHeadersProvider.h"
#include "IHisDevFactory.h"
#include "logger.h"
#include "ParamsNames.h"

using namespace std;
using namespace httpserver;
using namespace rapidjson;

class DevicesService : public http_resource
{
	ILogger & logger;
	HisDevices & devices;
	HisDevFolderRoot & rootFolder;
	IHttpHeadersProvider & headersProvider;
	IHisDevFactory* factory;

	bool UpdateDevValue(CUUID devValueId, string strjson);
	//bool SetForce(string address, string force);
	HisDevValueBase* CreateVirtualDevValue(string strjson,string & message);
	HisDevVirtual* CreateVirtualDevice(string strjson,string & message);
	HisDevBase* UpdateDevice(string strjson,string strDevId);
	bool DeleteValueId(string strValueId, string & message);
	bool DeleteDevValue(string strDevValueRecordId, string & msg);
	bool DeleteDev(string strDevValueRecordId, string & msg);
public:
	DevicesService(HisDevices & dev,
			HisDevFolderRoot & folder,
			IHttpHeadersProvider & headersProvider,
			IHisDevFactory* factory,
			webserver* ws_i);
	~DevicesService(void);
	static void FillFolderDevicesToJson(HisDevFolder* folder,Document & respjsondoc, HisDevices & dev);
	const http_response render_OPTIONS(const http_request& req);
	const http_response render_GET(const http_request&);
	const http_response render_POST(const http_request& r);
	const http_response render_PUT(const http_request& req);
	const http_response render_DELETE(const http_request& req);
	static void FillDeviceToJson(Value & d, HisDevBase* dev,Document & respjsondoc);
	static void DevValueToJson(Value & d, HisDevValueId* valueId,HisDevValueBase* devValue,Document & respjsondoc);
};

#endif /* ONEWIREDEVICESSERVICE_H_ */
