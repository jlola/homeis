/*
 * ExpressionsTest.cpp
 *
 *  Created on: 13. 3. 2017
 *      Author: pc
 */

#ifdef HOMEISTEST

#include <vector>
#include "TestConfig.h"
#include "HomeIsConfig.h"
#include "HomeIsServer.h"
#include "StringBuilder.h"
#include "homeis.h"
#include "gtest/gtest.h"
#include "Client.h"
#include "Tests/API/ExpressionsTest.h"
#include "ModbusProvider.h"

namespace AF {

ExpressionsTest::ExpressionsTest():
	client(SERVER_NAME,SERVER_PORT),server(NULL),modbusprovider(NULL)
{
}

void ExpressionsTest::SetUp()
{
	modbussim.Driver = "modbussimulator";
	modbussim.Name = "modbussimulator";
	modbussim.Port = "";
	std::vector<SSerPortConfig> serports;
	serports.push_back(modbussim);
	modbusprovider = new ModbusProvider(serports);
	server = new HomeIsServer(*modbusprovider,NULL,SERVER_PORT,"");
	server->Start(false);
}


TEST_F(ExpressionsTest,ModbusReadTest)
{
	std::string reqest = StringBuilder::Format("api/modbus/registers/%s/1/0/52",
				  modbussim.Name.c_str());
	string response;
	long http_code;
	CURLcode cresp = client.Get(reqest,response,http_code);
	ASSERT_EQ(cresp, CURLE_OK);
}

TEST_F(ExpressionsTest,CreateExpressionTest)
{

	std::string reqest = StringBuilder::Format("api/expression");
	string response;
	string json = "{\"parentId\":\"b004782a-ff08-4aee-b459-aa0a2d19ac0d\",\"running\":false,\"description\":null,\"expression\":\"if(1\
== 1) then test=\
1 end\",\"errorMessage\":null,\"name\":\"newExpression\",\"id\":null,\"nodeName\":\"expression\"}";
	long http_code = 0;
	CURLcode cresp = client.Put(reqest,json,response,http_code);
	ASSERT_EQ(cresp, CURLE_OK);
	ASSERT_EQ(http_code, MHD_HTTP_OK);
	string exprefile = StringBuilder::Format("%s/Expressions/%s.lua",file.getexepath().c_str(),"newExpression");
	ASSERT_EQ(file.Exists(exprefile), true);
	if (file.Exists(exprefile))
		file.Delete(exprefile);
}


void ExpressionsTest::TearDown()
{
	string foldersfile = StringBuilder::Format("%s/folders.xml",file.getexepath().c_str());
	if (file.Exists(foldersfile))
		file.Delete(foldersfile);
	string devicesfile = StringBuilder::Format("%s/devices.xml",file.getexepath().c_str());
	if (file.Exists(devicesfile))
		file.Delete(devicesfile);

	server->Stop();
	delete(server);

	delete modbusprovider;
	modbusprovider = NULL;
}

ExpressionsTest::~ExpressionsTest() {
}
} /* namespace AF */

#endif
