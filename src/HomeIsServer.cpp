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

#include "HisException.h"

bool HomeIsServer::Init(bool blocking,string devicesxml,string foldersxml)
{
	if (!InitHisDevices(devicesxml,foldersxml))
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
	Init(blocking, devicesxml, foldersxml);
}

void HomeIsServer::Start(bool blocking,string devicesxml,string foldersxml)
{
	Init(blocking,devicesxml,foldersxml);
}

void  HomeIsServer::AddModbus(IModbus* m)
{
	modbusProvider.Add(m);
}

HomeIsServer::HomeIsServer(IModbusProvider & modbusprovider,
		IEmailSender* emailSender,
		int TcpPort,
		string allowOrigin) :
		logger(CLogger::GetLogger()),
		factory(NULL),
		headersProvider(allowOrigin),
		devruntime(NULL),
		rootFolder(NULL),
		expressionRuntime(NULL),
		cw(create_webserver(TcpPort)),
		devs(NULL),
		modbusProvider(modbusprovider),
		//serports(pserports),
		fc(NULL),
		owds(NULL),
		foldersService(NULL),
		expressionService(NULL),
		modbusDevService(NULL),
		modbusservice(NULL),
		connectorsService(NULL),
		logservice(NULL),
		emailSender(emailSender)
{
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

	fc = new FileController( ws_i );
	owds = new DevicesService( *devs, *rootFolder, headersProvider, factory, ws_i );
	foldersService = new FoldersService( *devs,*rootFolder, headersProvider, factory, ws_i );
	expressionService = new ExpressionService( rootFolder, expressionRuntime, devs, headersProvider, factory, ws_i );
	modbusDevService = new ModbusDeviceService( devs, &modbusProvider, headersProvider, factory, ws_i );
	modbusservice = new ModbusService( &modbusProvider, headersProvider, ws_i );
	connectorsService = new ConnectorsService( modbusProvider, headersProvider, ws_i );
	logservice = new LogService(headersProvider, ws_i );

	logger.Info("Start homeis webserver %s blocking starting thread", blocking ? "with" : "without");
	ws_i->start(blocking);
}

bool HomeIsServer::InitHisDevices(string devicesxml,string foldersxml)
{
	expressionRuntime = new ExpressionRuntime();
	devs = new HisDevices(devicesxml,&modbusProvider);

	factory = new HisDevFactory(expressionRuntime,devs,emailSender,&file,&directory);
	devs->Load(factory);

	rootFolder = new HisDevFolderRoot(foldersxml,factory);
	rootFolder->Load();

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
