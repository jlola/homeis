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
#include "IEmailSender.h"

#include "HisDevices.h"
#include "File.h"
#include "Tags/TagBase.h"

//#include "EDataType.h"
#include "HisDevValue.h"
#include "document.h"		// rapidjson's DOM-style API
#include "Expressions/LuaExpression.h"


#include "HomeIsServer.h"
#include "HttpHeadersProvider.h"
#include "UserManager.h"
#include "HisException.h"

bool HomeIsServer::Init(bool blocking,string devicesxml,string foldersxml,string usersxml)
{
	if (!InitHisDevices(devicesxml,foldersxml,usersxml))
	{
		logger.Error("Error init HisDevices");
		return false;
	}

	InitWebServer(blocking);

	return true;
}

void HomeIsServer::Stop()
{
	logger.Info("Stop server");
	devruntime->Stop();
	expressionRuntime->Stop();
	ws_i->stop();
}

void HomeIsServer::Start(bool blocking)
{
	string devicesxml = file.getexepath() + "/devices.xml";
	string foldersxml = file.getexepath() + "/folders.xml";
	string usersxml = file.getexepath() + "/users.xml";
	Init(blocking, devicesxml, foldersxml,usersxml);
}

void HomeIsServer::Start(bool blocking,string devicesxml,string foldersxml,string usersxml)
{
	Init(blocking,devicesxml,foldersxml,usersxml);
}

void  HomeIsServer::AddModbus(IModbus* m)
{
	modbusProvider.Add(m);
}

HomeIsServer::HomeIsServer(IModbusProvider & modbusprovider,
		IEmailSender* emailSender,
		IConfig & config) :
		logger(CLogger::GetLogger()),
		factory(NULL),
		headersProvider(config.GetAllowOrigin()),
		devruntime(NULL),
		rootFolder(NULL),
		expressionRuntime(NULL),
		cw(create_webserver(config.GetServerPort())),
		devs(NULL),
		modbusProvider(modbusprovider),
		fc(NULL),
		owds(NULL),
		foldersService(NULL),
		expressionService(NULL),
		modbusDevService(NULL),
		modbusservice(NULL),
		connectorsService(NULL),
		logservice(NULL),
		usersService(NULL),
		versionService(NULL),
		deviceQueue(NULL),
		modbusProcessor(NULL),
		emailSender(emailSender),
		userManager(NULL)

{
	deviceQueue = new BlockingQueue<HisDevBase*>();
	modbusProcessor = new ModbusProcessor(deviceQueue);
	ws_i = new webserver(cw);
}

void HomeIsServer::InitWebServer(bool blocking)
{
	STACK

	if (devs==NULL)
		throw ArgumentNullException("devs");
	if (factory==NULL)
		throw ArgumentNullException("factory");
	if (expressionRuntime==NULL)
		throw ArgumentNullException("expressionRuntime");
	if (rootFolder==NULL)
		throw ArgumentNullException("rootFolder");
	if (userManager==NULL)
			throw ArgumentNullException("userManager");

	fc = new FileController( ws_i,userManager, factory );
	owds = new DevicesService( *devs, *rootFolder, userManager, factory, ws_i );
	foldersService = new FoldersService( *devs,*rootFolder, userManager, factory, ws_i );
	expressionService = new ExpressionService( rootFolder, expressionRuntime, devs, userManager, factory, ws_i );
	modbusDevService = new ModbusDeviceService( devs, &modbusProvider, userManager, factory, ws_i );
	modbusservice = new ModbusService( &modbusProvider, userManager, factory, ws_i );
	connectorsService = new ConnectorsService( modbusProvider, userManager, factory, ws_i );
	logservice = new LogService(ws_i,userManager, factory);
	usersService = new UsersService(ws_i,userManager, factory);
	versionService = new VersionService(ws_i,userManager,factory);

	logger.Info("Start homeis webserver %s blocking starting thread", blocking ? "with" : "without");
	ws_i->start(blocking);
}

bool HomeIsServer::InitHisDevices(string devicesxml,string foldersxml,string usersxml)
{

	expressionRuntime = new ExpressionRuntime();
	devs = new HisDevices(devicesxml,&modbusProvider,deviceQueue);
	userManager = new UserManager(usersxml,300);

	factory = new HisDevFactory(expressionRuntime,
			devs,
			emailSender,
			&file,
			&directory,
			&headersProvider
			);

	userManager->Load(factory);
	devs->Load(factory);

	rootFolder = new HisDevFolderRoot(foldersxml,factory);
	rootFolder->Load();

	devruntime = new HisDevRuntime(*devs);
	expressionRuntime->Start();
	//consumer blocking queue
	modbusProcessor->Start();
	//time consumer + events from expressions + alarm consumer from bus
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
	delete usersService;
	usersService = NULL;
}
