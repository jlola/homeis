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
#include "ParamsNames.h"
#include "curl.h"
#include "FoldersAtom.h"
#include "DeviceAtoms.h"


DevicesTests::DevicesTests()
{
}

void DevicesTests::SetUp()
{
	homeisStarter.Start();
}

void DevicesTests::TearDown()
{
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

	FoldersAtom foldersAtom(homeisStarter);
	string folderName = "subtest2";
	long http_code;
	CURLcode urlCode;
	foldersAtom.CreateFolder(folderName,CUUID::Empty(),http_code,urlCode);

	ASSERT_EQ(http_code, MHD_HTTP_OK);

	string response = foldersAtom.GetFolders(http_code,urlCode);
	ASSERT_EQ(CURLcode::CURLE_OK, urlCode);
	ASSERT_EQ(http_code, MHD_HTTP_OK);

	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	ASSERT_EQ(document.Parse<0>((char*)response.c_str()).HasParseError(),false);
	ASSERT_EQ(document.IsArray(),true);

	const Value& folders = document;
	ASSERT_EQ(folders.Size(),(size_t)1);
	for(SizeType ob = 0 ;ob < folders.Size();ob++)
	{
		ASSERT_EQ(folders[ob].IsObject(),true);
		ASSERT_EQ(folders[ob].HasMember("name") && folders[ob]["name"].IsString(),true);
		string name = folders[ob]["name"].GetString();
		ASSERT_EQ(name,folderName);
	}
}

TEST_F(DevicesTests,CreateEmailTagTest)
{
	DeviceAtoms deviceAtom(homeisStarter);
	string deviceName = "test";
	string expectedTagName = "tagName";

	string response = deviceAtom.CreateDevice(deviceName);

	response = deviceAtom.GetDevices();

	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	ASSERT_EQ(document.Parse<0>((char*)response.c_str()).HasParseError(),false);
	const Value& devices = document;
	ASSERT_EQ(devices.Size(),(size_t)1);
	for(SizeType ob = 0 ;ob < devices.Size();ob++)
	{
		ASSERT_EQ(devices[ob].HasMember(JSON_NAME) && devices[ob][JSON_NAME].IsString(),true);
		string name = devices[ob][JSON_NAME].GetString();
		ASSERT_EQ(deviceName,name);
		string parentId = devices[ob][JSON_ID].GetString();

		response = deviceAtom.CreateEmailTag(parentId,expectedTagName);
	}

	response = deviceAtom.GetDevices();
	ASSERT_EQ(document.Parse<0>((char*)response.c_str()).HasParseError(),false);
	ASSERT_EQ(devices.Size(),(size_t)1);
	for(SizeType ob = 0 ;ob < devices.Size();ob++)
	{
		ASSERT_TRUE(devices[ob].HasMember(JSON_TAGS));
		const Value& tags = devices[ob][JSON_TAGS];
		for(SizeType t = 0 ;t < tags.Size();t++)
		{
			string tagName = tags[t][JSON_NAME].GetString();
			ASSERT_EQ(expectedTagName,tagName);
		}
	}
}

TEST_F(DevicesTests,GetDevValueTest)
{
	string response;
	long http_code = 0;

	//api/modbus/scan/{connectorname}/{address}
	std::string scandevice = StringBuilder::Format("api/modbus/scan/%s/%d",homeisStarter.GetConfig().Name.c_str(),1);
	CURLcode cresp = homeisStarter.GetClient().Get(scandevice,response,http_code);
	ASSERT_EQ(cresp, CURLE_OK);
	ASSERT_EQ(http_code,MHD_HTTP_OK);

	std::string reqest = StringBuilder::Format("api/devices");
	cresp = homeisStarter.GetClient().Get(reqest,response,http_code);
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
		ASSERT_EQ(devicesarray[ob].HasMember(JSON_ID) && devicesarray[ob][JSON_ID].IsString(),true);
		string tagid = devicesarray[ob][JSON_ID].GetString();
		ASSERT_EQ(devicesarray[ob].HasMember(JSON_TAGS),true);
		ASSERT_EQ(devicesarray[ob][JSON_TAGS].IsArray(),true);
		const Value& tagsarray = devicesarray[ob][JSON_TAGS];

		for(SizeType i = 0; i < tagsarray.Size();i++)
		{
			ASSERT_EQ(tagsarray[i].IsObject(),true);
			ASSERT_EQ(tagsarray[i].HasMember(JSON_ID),true);
			string strid = tagsarray[i][JSON_ID].GetString();
		}
	}
}

DevicesTests::~DevicesTests() {

}


#endif
