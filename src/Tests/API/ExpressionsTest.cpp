/*
 * ExpressionsTest.cpp
 *
 *  Created on: 13. 3. 2017
 *      Author: pc
 */

#ifdef HOMEISTEST

#include <vector>
#include "Tests/TestConfig.h"
#include "HomeIsConfig.h"
#include "HomeIsServer.h"
#include "StringBuilder.h"
#include "homeis.h"
#include "gtest/gtest.h"
#include "TestsSupport/Client.h"
#include <Tests/API/ExpressionsTest.h>

namespace AF {

ExpressionsTest::ExpressionsTest():
	client(SERVER_NAME,SERVER_PORT),server(NULL)
{
}

void ExpressionsTest::SetUp()
{
	modbussim.Driver = "modbussimulator";
	modbussim.Name = "modbussimulator";
	modbussim.Port = "";
	std::vector<SSerPortConfig> serports;
	serports.push_back(modbussim);

	server = new HomeIsServer(serports,SERVER_PORT,false,"","");
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
1 end\",\"errormessage\":null,\"name\":\"newExpression\",\"id\":null,\"NodeName\":\"expression\"}";
	long http_code = 0;
	CURLcode cresp = client.Put(reqest,json,response,http_code);
	ASSERT_EQ(cresp, CURLE_OK);
	if (http_code!=200)
		CLogger::Error("CreateExpressionTest:%s",response.c_str());
	ASSERT_EQ(http_code, 200);
	string exprefile = StringBuilder::Format("%s/Expressions/%s.lua",File::getexepath().c_str(),"newExpression");
	ASSERT_EQ(File::Exists(exprefile), true);
	if (File::Exists(exprefile))
		File::Delete(exprefile);
}



ExpressionsTest::~ExpressionsTest() {
	string foldersfile = StringBuilder::Format("%s/folders.xml",File::getexepath().c_str());
	if (File::Exists(foldersfile))
		File::Delete(foldersfile);
	string devicesfile = StringBuilder::Format("%s/devices.xml",File::getexepath().c_str());
	if (File::Exists(devicesfile))
		File::Delete(devicesfile);

	server->Stop();
	delete(server);

}
} /* namespace AF */

#endif
