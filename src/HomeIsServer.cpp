/*
 * HomeIsServer.cpp
 *
 *  Created on: Feb 17, 2013
 *      Author: Josef Lola
 */

//#include "stdafx.h"
#include <signal.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <Services/DevicesService.h>
#include <unistd.h>

#include "HisDevices.h"
#include "File.h"
#include "logger.h"
#include "Tags/TagBase.h"

//#include "EDataType.h"
#include "HisDevValue.h"
#include "document.h"		// rapidjson's DOM-style API
#include "Expressions/LuaExpression.h"


#include "HisDevFactory.h"
#include "LOWdevLCD.h"
#include "HomeIsServer.h"
#include "HttpHeadersProvider.h"

bool HomeIsServer::Init(bool blocking)
{
	if (!InitHisDevices())
	{
		CLogger::Error("Error init HisDevices");
		return false;
	}

	InitWebServer(blocking);

	return true;
}

void HomeIsServer::Stop()
{
	CLogger::Info("Stop server");
	devruntime->Stop();
	expressionRuntime->Stop();
	ws_i->stop();
}

void HomeIsServer::Start(bool blocking)
{
	Init(blocking);
}

void  HomeIsServer::AddModbus(IModbus* m)
{
	modbusProvider.Add(m);
}

HomeIsServer::HomeIsServer(vector<SSerPortConfig> & pserports,int TcpPort, string allowOrigin) :
		headersProvider(allowOrigin),devruntime(NULL),rootFolder(NULL),expressionRuntime(NULL),
		cw(create_webserver(TcpPort)),devs(NULL),modbusProvider(pserports),serports(pserports),
		fc(NULL),owds(NULL),foldersService(NULL),expressionService(NULL), modbusDevService(NULL),
		modbusservice(NULL), connectorsService(NULL),logservice(NULL)
{
	ws_i = new webserver(cw);
}

void HomeIsServer::InitWebServer(bool blocking)
{
	fc = new FileController();
	owds = new DevicesService(*devs,*rootFolder, headersProvider);
	foldersService = new FoldersService(*devs,*rootFolder,headersProvider);
	expressionService = new ExpressionService(rootFolder,expressionRuntime, devs, headersProvider);
	modbusDevService = new ModbusDeviceService(devs,&modbusProvider,headersProvider);
	modbusservice = new ModbusService(&modbusProvider,headersProvider);
	connectorsService = new ConnectorsService(serports, headersProvider);
	logservice = new LogService(headersProvider);

	//ws_i = cw;
	ws_i->register_resource(string("files/{path}"), fc, true);
	ws_i->register_resource(string(""), fc, true);

	ws_i->register_resource(string("api/logs"), logservice, true);
	ws_i->register_resource(string("api/logs/{log}"), logservice, true);
	ws_i->register_resource(string("api/onewiredevices"), owds, true);
	ws_i->register_resource(string("api/devices"), owds, true);
	ws_i->register_resource(string("api/onewiredevices/{devid}"), owds, true);
	ws_i->register_resource(string("api/devices/{devid}"), owds, true);
	ws_i->register_resource(string("api/onewiredevices/devvalue/{id}"), owds, true);
	ws_i->register_resource(string("api/devices/{devid}/devvalues/{valueid}"),owds,true);
	ws_i->register_resource(string("api/onewiredevices/folder/{id}"), owds, true);

	//run all expressions in folder
	ws_i->register_resource(string("api/expression/run/{id}"), expressionService, true);
	ws_i->register_resource(string("api/expression/folder/{id}"), expressionService, true);
	ws_i->register_resource(string("api/expression/debuglog/{id}"), expressionService, true);
	ws_i->register_resource(string("api/expression/{id}"), expressionService, true);
	ws_i->register_resource(string("api/expression"), expressionService, true);

	ws_i->register_resource(string("api/folders"), foldersService, true);
	ws_i->register_resource(string("api/folder"), foldersService, true);
	ws_i->register_resource(string("api/folders/{id}"), foldersService, true);
	ws_i->register_resource(string("api/folder/allitems/{id}"), foldersService, true);
	ws_i->register_resource(string("api/folder/{id}"), foldersService, true);
	ws_i->register_resource(string("api/folder/valueid/{folderid}"), foldersService, true);

	ws_i->register_resource(string("api/modbus/scan/{connectorname}/{address}"),modbusDevService,true);

	///{connectorname}/{devaddress}/{baseaddress}/{count}
	ws_i->register_resource(string("api/modbus/registers/{connectorname}/{devaddress}/{baseaddress}/{value}"),modbusservice,true);
	ws_i->register_resource(string("api/connectors"),connectorsService, true);
	CLogger::Info("Start webserver with %s blocking", blocking ? "enabled" : "disabled");
	ws_i->start(blocking);
}

bool HomeIsServer::InitHisDevices()
{
	expressionRuntime = new ExpressionRuntime();
	devs = new HisDevices(File::getexepath() + "/devices.xml",&modbusProvider);
	devs->Load();
	HisDevFactory::Instance().SetDevices(devs);

	HisDevFactory::Instance().SetExpressionRuntime(expressionRuntime);

	rootFolder = new HisDevFolderRoot(File::getexepath() + "/folders.xml");
	rootFolder->Load();
	HisDevFactory::Instance().SetRooFolder(rootFolder);

	devruntime = new HisDevRuntime(*devs);

	expressionRuntime->Start();
	devruntime->Start();
	return true;
}

HomeIsServer::~HomeIsServer()
{
	delete(ws_i);
	ws_i = NULL;
	delete(rootFolder);
	rootFolder = NULL;

	delete fc;
	fc = NULL;
	delete(owds);
	owds = NULL;
	delete foldersService;
	foldersService = NULL;
	delete expressionService;
	expressionService = NULL;
	delete modbusDevService;
	modbusDevService = NULL;
	delete modbusservice;
	modbusservice = NULL;
	delete connectorsService;
	connectorsService = NULL;
	delete logservice;
	logservice = NULL;
}
