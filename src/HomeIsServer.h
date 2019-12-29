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
#include "ModbusProvider.h"
#include "Devices/Folder/HisDevFolderRoot.h"
#include "Expressions/ExpressionRuntime.h"
#include "HomeIsConfig.h"
#include "modbus.h"
#include "HisDevRunTime.h"
#include <algorithm>

#include "IEmailSender.h"
#include "logger.h"
#include "HomeIsServer.h"
#include "Services/FileController.h"
#include "Services/FoldersService.h"
#include "Services/ExpressionsService.h"
#include "Services/ModbusDeviceService.h"
#include "Services/ModbusService.h"
#include "Services/ConnectorsService.h"
#include "Services/LogService.h"
#include "Services/UsersService.h"
#include "VersionService.h"
#include "File.h"
#include "Directory.h"
#include "HisDevFactory.h"
#include "IConfig.h"
#include "IUserManager.h"
#include "IBlockingQueue.h"
#include "ModbusProcessor.h"

using namespace httpserver;
using namespace std;

class HomeIsServer
{
	Directory directory;
	File file;
	ILogger & logger;
	HisDevFactory* factory;
	HttpHeadersProvider headersProvider;
	HisDevRuntime* devruntime;
	HisDevFolderRoot* rootFolder;
	ExpressionRuntime* expressionRuntime;
	create_webserver cw;
	HisDevices* devs;
	IModbusProvider & modbusProvider;
	webserver* ws_i;

	FileController* fc;
	DevicesService* owds;
	FoldersService* foldersService;
	ExpressionService* expressionService;
	ModbusDeviceService* modbusDevService;
	ModbusService* modbusservice;
	ConnectorsService* connectorsService;
	LogService* logservice;
	UsersService* usersService;
	VersionService* versionService;
	IBlockingQueue<HisDevBase*>* deviceQueue;
	ModbusProcessor* modbusProcessor;

	IEmailSender* emailSender;
	IUserManager* userManager;

	bool Init(bool blocking,string devicesxml,string foldersxml,string usersxml);
	bool InitHisDevices(string devicesxml,string foldersxml,string usresxml);
	void InitWebServer(bool blocking);
public:
	HomeIsServer(IModbusProvider & modbusprovider,
			IEmailSender* emailSender,
			IConfig & config);
	void AddModbus(IModbus* m);
	void Start(bool blocking);
	void Start(bool blocking,string devicesxml,string foldersxml,string usresxml);
	void Stop();
	~HomeIsServer();
};

#endif /* HOMEISSERVER_H_ */
