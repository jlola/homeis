/*
 * DevicesTests.cpp
 *
 *  Created on: 19. 3. 2017
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
#include <Tests/API/DevicesTests.h>

namespace AF {

DevicesTests::DevicesTests() :
	server(NULL),client(SERVER_NAME,SERVER_PORT)
{
}

void DevicesTests::SetUp()
{
	modbussim.Driver = "modbussimulator";
	modbussim.Name = "modbussimulator";
	modbussim.Port = "";
	std::vector<SSerPortConfig> serports;
	serports.push_back(modbussim);

	server = new HomeIsServer(serports,SERVER_PORT,"");
	server->Start(false);
}

TEST_F(DevicesTests,CreateFolderTest)
{
	std::string reqest = StringBuilder::Format("api/folder");
	string response;
	string json = "{\"type\":null, \"name\":\"subtest2\",\"parentId\":null}";
	long http_code = 0;
	CURLcode cresp = client.Post(reqest,json,response,http_code);
	ASSERT_EQ(cresp, CURLE_OK);
	if (http_code!=200)
		CLogger::Error("CreateFolderTest:%s",response.c_str());
	ASSERT_EQ(http_code, 200);
}

TEST_F(DevicesTests,GetFolderTest)
{
	std::string reqest = StringBuilder::Format("/api/folder");
	string response;
	//string json = "{\"type\":null, \"name\":\"subtest2\",\"parentId\":null}";
	long http_code = 0;
	CURLcode cresp = client.Get(reqest,response,http_code);
	ASSERT_EQ(cresp, CURLE_OK);
	if (http_code!=200)
		CLogger::Error("GetFolderTest:%s",response.c_str());
	ASSERT_EQ(http_code, 200);
}

TEST_F(DevicesTests,GetDevValueTest)
{
	string response;
	long http_code = 0;

	//api/modbus/scan/{connectorname}/{address}
	std::string scandevice = StringBuilder::Format("api/modbus/scan/%s/%d",modbussim.Name.c_str(),1);
	CURLcode cresp = client.Get(scandevice,response,http_code);
	ASSERT_EQ(cresp, CURLE_OK);
	ASSERT_EQ(http_code,200);

	std::string reqest = StringBuilder::Format("api/devices");
	cresp = client.Get(reqest,response,http_code);
	ASSERT_EQ(cresp, CURLE_OK);
	ASSERT_EQ(http_code, 200);

	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	ASSERT_EQ(document.Parse<0>((char*)response.c_str()).HasParseError(),false);
	ASSERT_EQ(document.IsArray(),true);

	const Value& devicesarray = document;
	for(SizeType ob = 0 ;ob < devicesarray.Size();ob++)
	{
		//Value::ConstValueIterator itr = document.Begin();
		ASSERT_EQ(devicesarray[ob].IsObject(),true);
		ASSERT_EQ(devicesarray[ob].HasMember("Id") && devicesarray[ob]["Id"].IsString(),true);
		string tagid = devicesarray[ob]["Id"].GetString();
		ASSERT_EQ(devicesarray[ob].HasMember("Tags"),true);
		ASSERT_EQ(devicesarray[ob]["Tags"].IsArray(),true);
		const Value& tagsarray = devicesarray[ob]["Tags"];

		for(SizeType i = 0; i < tagsarray.Size();i++)
		{
			ASSERT_EQ(tagsarray[i].IsObject(),true);
			ASSERT_EQ(tagsarray[i].HasMember("id"),true);
			string strid = tagsarray[i]["id"].GetString();
		}
	}
}

DevicesTests::~DevicesTests() {
	server->Stop();
	delete(server);

	string foldersfile = StringBuilder::Format("%s/folders.xml",File::getexepath().c_str());
	if (File::Exists(foldersfile))
		File::Delete(foldersfile);
	string devicesfile = StringBuilder::Format("%s/devices.xml",File::getexepath().c_str());
	if (File::Exists(devicesfile))
		File::Delete(devicesfile);
}

} /* namespace AF */

#endif
