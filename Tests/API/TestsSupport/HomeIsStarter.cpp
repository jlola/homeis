/*
 * HomeIsStarter.cpp
 *
 *  Created on: 29. 5. 2018
 *      Author: pc
 */

#ifdef HOMEISTEST


#include "HomeIsStarter.h"

using namespace fakeit;

HomeIsStarter::HomeIsStarter()
	: server(NULL),client(SERVER_NAME,SERVER_PORT),modbusProvider(NULL)
{
	modbussim.Driver = "modbussimulator";
	modbussim.Name = "modbussimulator";
	modbussim.Port = "";

	std::vector<SSerPortConfig> serports;
		serports.push_back(modbussim);

	emailSender = new EmailSender("","","");
	modbusProvider = new ModbusProvider(serports);
	Mock<IConfig> configMock;
	When(Method(configMock,GetServerPort)).AlwaysReturn(SERVER_PORT);
	When(Method(configMock,GetAllowOrigin)).AlwaysReturn("");
	server = new HomeIsServer(*modbusProvider,emailSender,configMock.get());
}

SSerPortConfig HomeIsStarter::GetModbusConfig()
{
	return modbussim;
}

Client & HomeIsStarter::GetClient()
{
	return client;
}

void HomeIsStarter::Start()
{
	server->Start(false);
}

IModbusProvider* HomeIsStarter::GetModbusProvider()
{
	return modbusProvider;
}

ModbusSimulator* HomeIsStarter::GetModbusSimulator()
{
	return dynamic_cast<ModbusSimulator*>(modbusProvider->Find(ModbusSimulator::DriverName));
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
