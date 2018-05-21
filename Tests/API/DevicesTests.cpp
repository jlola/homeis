/*
 * DevicesTests.cpp
 *
 *  Created on: 19. 3. 2017
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
#include "DevicesTests.h"
#include "IEmailSender.h"
#include "ModbusProvider.h"
#include "File.h"

namespace AF {

DevicesTests::DevicesTests() :
	server(NULL),client(SERVER_NAME,SERVER_PORT),modbusProvider(NULL)
{
}

void DevicesTests::SetUp()
{
	modbussim.Driver = "modbussimulator";
	modbussim.Name = "modbussimulator";
	modbussim.Port = "";
	std::vector<SSerPortConfig> serports;
	serports.push_back(modbussim);
	modbusProvider = new ModbusProvider(serports);
	server = new HomeIsServer(*modbusProvider,NULL,SERVER_PORT,"");
	server->Start(false);
}

void DevicesTests::TearDown()
{
	server->Stop();
	delete(server);

	delete modbusProvider;
	modbusProvider = NULL;
	File file;

	string foldersfile = StringBuilder::Format("%s/folders.xml",file.getexepath().c_str());
	if (file.Exists(foldersfile))
		file.Delete(foldersfile);
	string devicesfile = StringBuilder::Format("%s/devices.xml",file.getexepath().c_str());
	if (file.Exists(devicesfile))
		file.Delete(devicesfile);
}

TEST_F(DevicesTests,CreateFolderReturnsSameFolder)
{
	std::string reqest = StringBuilder::Format("api/folder");
	string response;
	string json = "{\"type\":null, \"name\":\"subtest2\",\"parentId\":null}";
	long http_code = 0;
	CURLcode cresp = client.Post(reqest,json,response,http_code);
	ASSERT_EQ(cresp, CURLE_OK);
	ASSERT_EQ(http_code, MHD_HTTP_OK);

	reqest = StringBuilder::Format("/api/folder/allitems");
	//string json = "{\"type\":null, \"name\":\"subtest2\",\"parentId\":null}";
	cresp = client.Get(reqest,response,http_code);
	ASSERT_EQ(cresp, CURLE_OK);
	ASSERT_EQ(http_code, MHD_HTTP_OK);

	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	ASSERT_EQ(document.Parse<0>((char*)response.c_str()).HasParseError(),false);
	ASSERT_EQ(document.IsArray(),true);

	const Value& folders = document;
	ASSERT_EQ(folders.Size(),1);
	for(SizeType ob = 0 ;ob < folders.Size();ob++)
	{
		ASSERT_EQ(folders[ob].IsObject(),true);
		ASSERT_EQ(folders[ob].HasMember("name") && folders[ob]["name"].IsString(),true);
		string name = folders[ob]["name"].GetString();
		ASSERT_EQ(name,"subtest2");
	}
}

TEST_F(DevicesTests,GetDevValueTest)
{
	string response;
	long http_code = 0;

	//api/modbus/scan/{connectorname}/{address}
	std::string scandevice = StringBuilder::Format("api/modbus/scan/%s/%d",modbussim.Name.c_str(),1);
	CURLcode cresp = client.Get(scandevice,response,http_code);
	ASSERT_EQ(cresp, CURLE_OK);
	ASSERT_EQ(http_code,MHD_HTTP_OK);

	std::string reqest = StringBuilder::Format("api/devices");
	cresp = client.Get(reqest,response,http_code);
	ASSERT_EQ(cresp, CURLE_OK);
	ASSERT_EQ(http_code, MHD_HTTP_OK);

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

}

} /* namespace AF */

#endif
