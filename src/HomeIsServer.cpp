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
#include "LOWdevLCD.h"
#include "LOW_devDS2408.h"
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
	//ws_i.stop();
	devruntime->Stop();
}

void HomeIsServer::Start()
{
	Init();
}

HomeIsServer::HomeIsServer(string address,int TcpPort) :
		serialPort(address),devruntime(NULL),rootFolder(NULL),
		expressionRuntime(NULL),cw(create_webserver(TcpPort).max_threads(5)),devs(NULL)
{
	const char *key="key.pem";
	const char *cert="cert.pem";
	//cw.use_ssl().https_mem_key(key).https_mem_cert(cert);
}

void HomeIsServer::InitWebServer()
{
	FileController fc = FileController();
	OneWireDevicesService owds = OneWireDevicesService(*devs,*rootFolder);

	FoldersService foldersService= FoldersService(*devs,rootFolder);
	ExpressionService expressionService = ExpressionService(rootFolder,expressionRuntime, devs);
	webserver ws_i = cw;
	ws_i.register_resource(string("files/{path}"), &fc, true);
	ws_i.register_resource(string(""), &fc, true);

	ws_i.register_resource(string("api/onewiredevices"), &owds, true);
	ws_i.register_resource(string("api/onewiredevices/{id}"), &owds, true);
	ws_i.register_resource(string("api/onewiredevices/devvalue/{id}"), &owds, true);
	ws_i.register_resource(string("api/onewiredevices/folder/{id}"), &owds, true);

	//run all expressions in folder
	ws_i.register_resource(string("api/expression/run/{id}"), &expressionService, true);
	ws_i.register_resource(string("api/expression/folder/{id}"), &expressionService, true);
	ws_i.register_resource(string("api/expression/{id}"), &expressionService, true);
	ws_i.register_resource(string("api/expression"), &expressionService, true);

	ws_i.register_resource(string("api/folders"), &foldersService, true);
	ws_i.register_resource(string("api/folder"), &foldersService, true);
	ws_i.register_resource(string("api/folders/{id}"), &foldersService, true);
	ws_i.register_resource(string("api/folder/allitems/{id}"), &foldersService, true);
	ws_i.register_resource(string("api/folder/{id}"), &foldersService, true);
	ws_i.register_resource(string("api/folder/valueid/{folderid}"), &foldersService, true);
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

	devruntime = new HisDevRuntime(*devs);

	expressionRuntime->Start();
	devruntime->Start();
	return true;
}

bool HomeIsServer::InitOneWireLib(string port)
{
	// stuff for the passive adapter
	//LOW_linkPassiveSerial  *passiveLink = 0;
	LOW_linkDS2480B *ds2480Link = 0;
	LOW_linkDS2490 *ds2490Link = 0;
	try {
		LOW_exception::setLogOnCreation( false );
		//LOW_helper_msglog::printMessage( "Harald's predefined setup: Adding passive adapter to network.\n");
		//"/dev/ttyAMA0"

		LOW_portSerialFactory::portSpecifier_t  ttyS = LOW_portSerialFactory::portSpecifier_t( serialPort );
		ds2480Link = new LOW_linkDS2480B(ttyS,LOW_linkDS2480B::RXPOL_val_t::RXPOL_NORM,false,false);

		oneWireNet.addLink(ds2480Link);

		//std::vector<uint8_t> idbytes = Converter::stobytes("6500000016368729");
		//LOW_deviceID devid(idbytes);

		LOW_portUsb_Factory::usbDevSpecVec_t adapters = LOW_portUsb_Factory::getPortSpecifiers(LOW_linkDS2490::usbVendorID,LOW_linkDS2490::usbProductID);

		if (adapters.size()>0)
		{
			const char* adapterpath = adapters[0].c_str();
			CLogger::Info("Found adapter DS2490 at: %s\n",adapterpath);
			printf("Found adapter DS2490 at: %s\n",adapterpath);
			LOW_portUsb_Factory::usbDeviceSpecifier_t usb = LOW_portUsb_Factory::usbDeviceSpecifier_t(adapters[0]);
			ds2490Link = new LOW_linkDS2490(usb,true,false);
			//passiveLink = new LOW_linkPassiveSerial( ttyS1);
			//oneWireNet.addLink( passiveLink );
			//oneWireNet.addLink( ds2480Link );
			oneWireNet.addLink(ds2490Link);

			//LOW_devLCD* lcd = oneWireNet.getDevice<LOW_devLCD>(devid);
			//LOW_devDS2408* pio = oneWireNet.getDevice<LOW_devDS2408>(devid);
			//pio->WritePIO(0x00);
			//pio->WritePIO(0,true);
			//lcd->WriteToLCD("test1",0x00);
			//lcd->WriteToLCD("test2",0x40);


			printf("Adapter DS2490 successfully initialized\n");
			CLogger::Info("Adapter DS2490 successfully initialized\n");
		}
		else
		{
			printf("Error DS2490 not found.\n");
			CLogger::Info("Error DS2490 not found.\n");
			return false;
		}


		//oneWireLinks.push_back( passiveLink);
	}
	catch( LOW_exception & ex) {
		ex.logException();
		return false;
	}
	return true;
}
