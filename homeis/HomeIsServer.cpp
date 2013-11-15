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
#include <unistd.h>

#include "HisDevices.h"
#include "File.h"
#include "logger.h"
#include "Tags/TagBase.h"
#include "HisDevRunTime.h"
#include "EDataType.h"
#include "HisDevValue.h"
#include "document.h"		// rapidjson's DOM-style API
#include "Services/FileController.h"
#include "Services/OneWireDevicesService.h"
#include "Services/FoldersService.h"
#include "Expressions/LuaExpression.h"
#include "Services/ExpressionsService.h"
#include "HisDevFactory.h"

//#include "OneWireDevicesService.h"
//#include "EchoService.h"
//#include "LogInService.h"
//#include "Server.h"

#include "HomeIsServer.h"

bool HomeIsServer::Init()
{
	if (!InitOneWireLib(serialPort)) {
		CLogger::Error("Error init HomeIsServer");
		return false;
	}

	if (!InitHisDevices())
	{
		CLogger::Error("Error init HisDevices");
		return false;
	}

	InitWebServer();

	return true;
}

void HomeIsServer::Stop()
{
	ws_i.stop();
	runtime->Stop();
}

void HomeIsServer::Start()
{
	Init();
}

HomeIsServer::HomeIsServer(string SerialPort,int TcpPort) :
		serialPort(SerialPort),ws_i(create_webserver(TcpPort).max_threads(5))
{
}

void HomeIsServer::InitWebServer()
{
	FileController fc = FileController();
	OneWireDevicesService owds = OneWireDevicesService(*devs,*rootFolder);
	FoldersService foldersService= FoldersService(rootFolder);
	ExpressionService expressionService = ExpressionService(rootFolder);
	ws_i.register_resource(string("files/{path}"), &fc, true);

	ws_i.register_resource(string("api/onewiredevices"), &owds, true);
	ws_i.register_resource(string("api/onewiredevices/{id}"), &owds, true);
	ws_i.register_resource(string("api/onewiredevices/folder/{id}"), &owds, true);

	//run all expressions in folder
	ws_i.register_resource(string("api/expression/folderrun/{id}"), &expressionService, true);
	ws_i.register_resource(string("api/expression/folder/{id}"), &expressionService, true);
	ws_i.register_resource(string("api/expression/{id}"), &expressionService, true);
	ws_i.register_resource(string("api/expression"), &expressionService, true);

	ws_i.register_resource(string("api/folders"), &foldersService, true);
	ws_i.register_resource(string("api/folder"), &foldersService, true);
	ws_i.register_resource(string("api/folders/{id}"), &foldersService, true);
	ws_i.register_resource(string("api/folder/{id}"), &foldersService, true);
	ws_i.register_resource(string("api/folder/valueid/{deviceid}"), &foldersService, true);
	ws_i.start(true);
}

bool HomeIsServer::InitHisDevices()
{
	devs = new HisDevices(File::getexepath() + "/devices.xml",&oneWireNet);
	devs->Load();
	devs->AddScanned();
	HisDevFactory::Instance().SetDevices(devs);

	expressionRuntime = new ExpressionRuntime();
	HisDevFactory::Instance().SetExpressionRuntime(expressionRuntime);

	rootFolder = new HisDevFolderRoot(File::getexepath() + "/folders.xml");
	rootFolder->Load();
	HisDevFactory::Instance().SetRooFolder(rootFolder);

	runtime = new HisDevRuntime(*devs);

	expressionRuntime->Start();
	runtime->Start();
	return true;
}

bool HomeIsServer::InitOneWireLib(string port)
{
	// stuff for the passive adapter
	LOW_linkPassiveSerial  *passiveLink = 0;
	try {

		//LOW_helper_msglog::printMessage( "Harald's predefined setup: Adding passive adapter to network.\n");
		//"/dev/ttyAMA0"
		LOW_portSerialFactory::portSpecifier_t  ttyS1 = LOW_portSerialFactory::portSpecifier_t( serialPort );
		passiveLink = new LOW_linkPassiveSerial( ttyS1);
		oneWireNet.addLink( passiveLink);

		//oneWireLinks.push_back( passiveLink);
	}
	catch( LOW_exception ex) {
		ex.logException();
		return false;
	}
	return true;
}
