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
#include "Modbus/ModbusWrapper.h"
#include "Modbus/ModbusSimulator.h"

bool HomeIsServer::Init(bool blocking)
{
	if (!InitModbus()) {
		CLogger::Error("Error init modbus HomeIsServer");
		return false;
	}

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
	ws_i->stop();
	devruntime->Stop();
}

void HomeIsServer::Start(bool blocking)
{
	Init(blocking);
}

void  HomeIsServer::AddModbus(IModbus* m)
{
	momanager.Add(m);
}

bool HomeIsServer::InitModbus()
{
	for (size_t i=0;i<serports.size();i++)
	{
		std::string driver = Converter::strtolower((serports[i].Driver));
		if (driver=="modbus")
		{
			IModbus* m = new Modbus(serports[i]);
			if (!m->Init()) return false;
			momanager.Add(m);
		}
		if (driver=="modbussimulator")
		{
			IModbus* m = new ModbusSimulator(serports[i]);
			if (!m->Init()) return false;
			momanager.Add(m);
		}
	}

	return true;
}

HomeIsServer::HomeIsServer(vector<SSerPortConfig> & pserports,int TcpPort,bool useHttps,
		string httpsKey, string httpsCert) :
		devruntime(NULL),rootFolder(NULL),expressionRuntime(NULL),
		cw(create_webserver(TcpPort)),devs(NULL),serports(pserports),
		fc(NULL),owds(NULL),foldersService(NULL),expressionService(NULL), modbusDevService(NULL),
		modbusservice(NULL), connectorsService(NULL),logservice(NULL)
{
	if (useHttps)
	{
		cw.max_threads(5).use_ssl().https_mem_key(File::getexepath()+"/"+httpsKey).https_mem_cert(File::getexepath()+"/"+httpsCert);
	}
	ws_i = new webserver(cw);

	string strkey = File::getexepath()+"/server.key";
	string strcert = File::getexepath()+"/server.crt";
	CLogger::Info( "%s , %s",strkey.c_str(), strcert.c_str());
}

void HomeIsServer::InitWebServer(bool blocking)
{
	fc = new FileController();
	owds = new DevicesService(*devs,*rootFolder);
	foldersService = new FoldersService(*devs,rootFolder);
	expressionService = new ExpressionService(rootFolder,expressionRuntime, devs);
	modbusDevService = new ModbusDeviceService(devs,&momanager);
	modbusservice = new ModbusService(&momanager);
	connectorsService = new ConnectorsService(serports);
	logservice = new LogService();

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
	ws_i->register_resource(string("api/connectors"),connectorsService,true);
	CLogger::Info("Start webserver with blocking %d", blocking ? 1 : 0);
	ws_i->start(blocking);
}

bool HomeIsServer::InitHisDevices()
{
	expressionRuntime = new ExpressionRuntime();
	devs = new HisDevices(File::getexepath() + "/devices.xml",&oneWireNet,&momanager);
	devs->Load();
	devs->AddScanned();
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

bool HomeIsServer::InitOneWireLib(vector<SSerPortConfig> & pserports)
{
	// stuff for the passive adapter
	//LOW_linkPassiveSerial  *passiveLink = 0;
	LOW_linkDS2480B *ds2480Link = 0;
	LOW_linkDS2490 *ds2490Link = 0;
	try {
		LOW_exception::setLogOnCreation( false );

		for(size_t s=0;s<pserports.size();s++)
		{
			LOW_portSerialFactory::portSpecifier_t  ttyS = LOW_portSerialFactory::portSpecifier_t( pserports[s].Name );
			ds2480Link = new LOW_linkDS2480B(ttyS,LOW_linkDS2480B::RXPOL_val_t::RXPOL_NORM,true,false);
			oneWireNet.addLink(ds2480Link);
		}

		LOW_portUsb_Factory::usbDevSpecVec_t adapters = LOW_portUsb_Factory::getPortSpecifiers(LOW_linkDS2490::usbVendorID,LOW_linkDS2490::usbProductID);
		for(size_t i=0;i<adapters.size();i++)
		{
			const char* adapterpath = adapters[i].c_str();
			CLogger::Info("Found adapter DS2490 at: %s\n",adapterpath);
			printf("Found adapter DS2490 at: %s\n",adapterpath);
			LOW_portUsb_Factory::usbDeviceSpecifier_t usb = LOW_portUsb_Factory::usbDeviceSpecifier_t(adapters[0]);
			ds2490Link = new LOW_linkDS2490(usb,true,false);
			oneWireNet.addLink(ds2490Link);
			printf("Adapter DS2490 successfully initialized\n");
			CLogger::Info("Adapter DS2490 successfully initialized\n");
		}
		//oneWireLinks.push_back( passiveLink);
	}
	catch( LOW_exception & ex) {
		ex.logException();
		//return false;
	}
	return true;
}

HomeIsServer::~HomeIsServer()
{
	delete(ws_i);
	ws_i = NULL;
	delete(rootFolder);
	rootFolder = NULL;
}
