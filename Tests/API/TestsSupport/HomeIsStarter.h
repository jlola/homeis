/*
 * HomeIsStarter.h
 *
 *  Created on: 29. 5. 2018
 *      Author: pc
 */

#ifndef TESTS_API_TESTSSUPPORT_HOMEISSTARTER_H_
#define TESTS_API_TESTSSUPPORT_HOMEISSTARTER_H_

#include "HomeIsServer.h"
#include "Client.h"
#include "TestConfig.h"
#include "HomeIsConfig.h"
#include "EmailSender.h"
#include "ModbusSimulator.h"
#include "gtest/gtest.h"
#include "fakeit.hpp"

using namespace fakeit;
using namespace AF;

class HomeIsStarter {
	IEmailSender* emailSender;
	SSerPortConfig modbussim;
	HomeIsServer* server;
	Client client;
	IModbusProvider* modbusProvider;
	void Stop();
public:
	HomeIsStarter();
	ModbusSimulator* GetModbusSimulator();
	IModbusProvider* GetModbusProvider();
	void Start();
	SSerPortConfig GetModbusConfig();
	Client & GetClient();
	virtual ~HomeIsStarter();
};

#endif /* TESTS_API_TESTSSUPPORT_HOMEISSTARTER_H_ */
