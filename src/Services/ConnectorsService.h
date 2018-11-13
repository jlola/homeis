/*
 * ConnectorsService.h
 *
 *  Created on: 15. 11. 2016
 *      Author: pc
 */

#ifndef SRC_SERVICES_CONNECTORSSERVICE_H_
#define SRC_SERVICES_CONNECTORSSERVICE_H_

#include <httpserver.hpp>
#include "Devices/Folder/HisDevFolder.h"
#include "Devices/Folder/HisDevFolderRoot.h"
#include "Devices/Folder/HisDevValueId.h"
#include "VirtualDevices/HisDevVirtual.h"
#include "HomeIsConfig.h"
#include "IHisDevFactory.h"
#include "IModbusProvider.h"
#include "ServiceBase.h"

using namespace std;
using namespace httpserver;
using namespace rapidjson;

class ConnectorsService : public ServiceBase
{
	//vector<SSerPortConfig> & serports;
	IModbusProvider & provider;
public:
	ConnectorsService(IModbusProvider & provider,
			IUserManager* userManager,
			IHisDevFactory* factory,
			webserver* ws_i);
	const http_response GET(const http_request& r);
	virtual ~ConnectorsService();
};

#endif /* SRC_SERVICES_CONNECTORSSERVICE_H_ */
