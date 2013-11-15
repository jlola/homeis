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
#include "homeis/Devices/Folder/HisDevFolderRoot.h"
#include "Expressions/ExpressionRuntime.h"

using namespace httpserver;
using namespace std;

class HomeIsServer
{
	string serialPort;
	LOW_network  oneWireNet;
	HisDevRuntime* runtime;
	HisDevFolderRoot* rootFolder;
	ExpressionRuntime* expressionRuntime;
	webserver ws_i;
	HisDevices* devs;
	bool InitOneWireLib(string port);
	bool Init();
	bool InitHisDevices();
	void InitWebServer();
public:
	HomeIsServer(string SerialPort,int tcpPort);
	void Start();
	void Stop();
};

#endif /* HOMEISSERVER_H_ */
