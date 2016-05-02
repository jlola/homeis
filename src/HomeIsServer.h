/*
 * HomeIsServer.h
 *
 *  Created on: Feb 17, 2013
 *      Author: Josef Lola
 */

#ifndef HOMEISSERVER_H_
#define HOMEISSERVER_H_

#include <string>
#include "LOW_network.h"
#include "LOW_linkPassiveSerial.h"
#include "LOW_linkDS2480B.h"
#include "LOW_linkDS2490.h"
#include "LOW_helper_msglog.h"
#include "LOW_objectSynchronizer.h"

#include "LOW_devDS1820.h"
#include <httpserver.hpp>
#include "Devices/Folder/HisDevFolderRoot.h"
#include "Expressions/ExpressionRuntime.h"
#include "HomeIsConfig.h"

using namespace httpserver;
using namespace std;

class HomeIsServer
{
	vector<SSerPortConfig> serports;
	LOW_network  oneWireNet;
	HisDevRuntime* devruntime;
	HisDevFolderRoot* rootFolder;
	ExpressionRuntime* expressionRuntime;
	create_webserver cw;
	HisDevices* devs;
	bool InitOneWireLib(vector<SSerPortConfig> pserports);
	bool Init();
	bool InitHisDevices();
	void InitWebServer();
public:
	HomeIsServer(vector<SSerPortConfig> pserports,int tcpPort);
	void Start();
	void Stop();
};

#endif /* HOMEISSERVER_H_ */
