/*
 * HomeIsServer.h
 *
 *  Created on: Feb 17, 2013
 *      Author: Josef Lola
 */

#ifndef HOMEISSERVER_H_
#define HOMEISSERVER_H_

#include <string>
#include <httpserver.hpp>
#include <Modbus/ModbusProvider.h>
#include "Devices/Folder/HisDevFolderRoot.h"
#include "Expressions/ExpressionRuntime.h"
#include "HomeIsConfig.h"
#include "modbus.h"
#include "HisDevRunTime.h"
#include <algorithm>

#include "HomeIsServer.h"
#include "Services/FileController.h"
#include "Services/FoldersService.h"
#include "Services/ExpressionsService.h"
#include "Services/ModbusDeviceService.h"
#include "Services/ModbusService.h"
#include "Services/ConnectorsService.h"
#include "Services/LogService.h"

using namespace httpserver;
using namespace std;

class HomeIsServer
{
	//LOW_network  oneWireNet;
	HttpHeadersProvider headersProvider;
	HisDevRuntime* devruntime;
	HisDevFolderRoot* rootFolder;
	ExpressionRuntime* expressionRuntime;
	create_webserver cw;
	HisDevices* devs;
	ModbusProvider modbusProvider;
	vector<SSerPortConfig> & serports;
	webserver* ws_i;

	FileController* fc;
	DevicesService* owds;
	FoldersService* foldersService;
	ExpressionService* expressionService;
	ModbusDeviceService* modbusDevService;
	ModbusService* modbusservice;
	ConnectorsService* connectorsService;
	LogService* logservice;

	//bool InitOneWireLib(vector<SSerPortConfig> & pserports);
	bool Init(bool blocking);
	bool InitHisDevices();
	void InitWebServer(bool blocking);
public:
	HomeIsServer(vector<SSerPortConfig> & serports,int tcpPort, string allowOrigin);
	void AddModbus(IModbus* m);
	void Start(bool blocking);
	void Stop();
	~HomeIsServer();
};

#endif /* HOMEISSERVER_H_ */
