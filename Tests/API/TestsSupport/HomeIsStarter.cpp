/*
 * HomeIsStarter.cpp
 *
 *  Created on: 29. 5. 2018
 *      Author: pc
 */

#ifdef HOMEISTEST

#include "HomeIsStarter.h"

HomeIsStarter::HomeIsStarter()
	: server(NULL),client(SERVER_NAME,SERVER_PORT),modbusProvider(NULL)
{
	modbussim.Driver = "modbussimulator";
	modbussim.Name = "modbussimulator";
	modbussim.Port = "";
}

SSerPortConfig HomeIsStarter::GetConfig()
{
	return modbussim;
}

Client & HomeIsStarter::GetClient()
{
	return client;
}

void HomeIsStarter::Start()
{
	std::vector<SSerPortConfig> serports;
	serports.push_back(modbussim);

	emailSender = new EmailSender("","","");
	modbusProvider = new ModbusProvider(serports);
	server = new HomeIsServer(*modbusProvider,emailSender,SERVER_PORT,"");
	server->Start(false);
}

void HomeIsStarter::Stop()
{
	server->Stop();
	delete server;
	delete modbusProvider;
}

HomeIsStarter::~HomeIsStarter() {
	Stop();
}

#endif
