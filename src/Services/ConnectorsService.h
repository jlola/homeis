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

using namespace std;
using namespace httpserver;
using namespace rapidjson;

class ConnectorsService : public http_resource
{
	vector<SSerPortConfig> & serports;
public:
	ConnectorsService(vector<SSerPortConfig> & pserports);
	const http_response render_GET(const http_request& r);
	virtual ~ConnectorsService();
};

#endif /* SRC_SERVICES_CONNECTORSSERVICE_H_ */
