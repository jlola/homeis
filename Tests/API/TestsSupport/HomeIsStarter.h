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
	void Start();
	SSerPortConfig GetConfig();
	Client & GetClient();
	virtual ~HomeIsStarter();
};

#endif /* TESTS_API_TESTSSUPPORT_HOMEISSTARTER_H_ */
