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
#include "ModbusAtom.h"
#include "OneWireHandler.h"
#include "ModbusSimulator.h"

DevicesTests::DevicesTests()
	: homeisStarter(NULL)
{
}

void DevicesTests::SetUp()
{
	homeisStarter = new HomeIsStarter();
}

void DevicesTests::TearDown()
{
	delete homeisStarter;
	homeisStarter = NULL;

	File file;

	string foldersfile = StringBuilder::Format("%s/folders.xml",file.getexepath().c_str());
	if (file.Exists(foldersfile))
		file.Delete(foldersfile);
	string devicesfile = StringBuilder::Format("%s/devices.xml",file.getexepath().c_str());
	if (file.Exists(devicesfile))
		file.Delete(devicesfile);
}



TEST_F(DevicesTests,TestLoadDeviceWithoutScanAndThenScan)
{
	ModbusAtom modbusAtom(*homeisStarter);
	//							   0						10	   1314     1718     2122  24 25 26          30       35         40
	uint16_t registers[] =        {4,3,10,3200,40,0,0,0,0,0,1,3,25,3,2,1,30,3,5,1,35,3,0,0,0,769,770,771,0,0,4,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//																																					44
	uint16_t registers_scaned[] = {4,3,10,3200,46,0,0,0,0,0,1,3,25,3,2,1,30,3,5,1,35,3,0,0,0,769,770,771,0,0,4,0,0,0,0,0,1,0,1,0,63016,2719,6,52224,2362,1,0,0,0,0,0};
	ModbusSimulator* modbusSimulator =  homeisStarter->GetModbusSimulator();
	modbusSimulator->SetRegisters(registers_scaned,sizeof(registers_scaned)/sizeof(uint16_t));
	homeisStarter->Start();
	//scan with termometer
	string scanResponse = modbusAtom.Scan(homeisStarter->GetModbusConfig().Name,1);
	delete homeisStarter;
	//start without thermometer
	homeisStarter = new HomeIsStarter();
	modbusSimulator =  homeisStarter->GetModbusSimulator();
	modbusSimulator->SetRegisters(registers,sizeof(registers)/sizeof(uint16_t));
	homeisStarter->Start();

	DevicesAtom deviceAtoms(*homeisStarter);
	string deviceId = deviceAtoms.GetDeviceId();
	ASSERT_NE(deviceId,"");
	deviceAtoms.UpdateDevicePropertyUint(deviceId,JSON_SCANPERIODMS,100);
	int period = deviceAtoms.GetDevicePropertyInt(deviceId,JSON_SCANPERIODMS);
	ASSERT_EQ(100,period);

	string ds18b20TagId = deviceAtoms.FindTagId(deviceId,DEFAULTNAME);
	ASSERT_NE(ds18b20TagId,"");

	string scanOneWireTagId = deviceAtoms.FindTagId(deviceId,SCAN_ONEWIRE_NAME);
	ASSERT_NE(scanOneWireTagId,"");
	uint16_t holdingRegister;
	modbusSimulator->getHolding(1,35,&holdingRegister);
	ASSERT_EQ(0,holdingRegister);
	//scan
	deviceAtoms.WriteToTag(scanOneWireTagId,"true");
	while(holdingRegister==0)
	{
		modbusSimulator->getHolding(1,35,&holdingRegister);
		sleep(1);
	}
	ASSERT_EQ(1,holdingRegister);
	modbusSimulator->SetRegisters(registers_scaned,sizeof(registers_scaned)/sizeof(uint16_t));
	//string tagValue = deviceAtoms.GetTag(scanOneWireTagId)[JSON_VALUE].GetString();
	//ASSERT_EQ(tagValue,"0");
	//int error = deviceAtoms.GetTag(ds18b20TagId)[JSON_ERROR].GetInt();
	//ASSERT_EQ(0,error);
}

TEST_F(DevicesTests,CreateFolderReturnsSameFolder)
{
	homeisStarter->Start();
	FoldersAtom foldersAtom(*homeisStarter);
	string folderName = "subtest2";

	foldersAtom.CreateFolder(folderName,CUUID::Empty());

	Document document = foldersAtom.GetFolders();

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
	homeisStarter->Start();
	DevicesAtom deviceAtom(*homeisStarter);
	string deviceName = "test";
	string expectedTagName = "tagName";

	deviceAtom.CreateDevice(deviceName);


	Document document = deviceAtom.GetDevices();
	string expectedSender = "sender";
	string expectedReceivers = "receiver1;receiver2";
	const Value& devices = document;
	ASSERT_EQ(devices.Size(),(size_t)1);
	for(SizeType ob = 0 ;ob < devices.Size();ob++)
	{
		ASSERT_EQ(devices[ob].HasMember(JSON_NAME) && devices[ob][JSON_NAME].IsString(),true);
		string name = devices[ob][JSON_NAME].GetString();
		ASSERT_EQ(deviceName,name);
		string parentId = devices[ob][JSON_ID].GetString();

		deviceAtom.CreateTag(parentId,EDataType::Email, expectedTagName,expectedSender,expectedReceivers);
	}

	Document document2 = deviceAtom.GetDevices();
	const Value& devices2 = document2;
	ASSERT_EQ(devices2.Size(),(size_t)1);
	for(SizeType ob = 0 ;ob < devices2.Size();ob++)
	{
		ASSERT_TRUE(devices2[ob].HasMember(JSON_TAGS));
		const Value& tags = devices2[ob][JSON_TAGS];
		string tagName;
		int tagIndex = -1;
		for(SizeType t = 0 ;t < tags.Size();t++)
		{
			 tagName = tags[t][JSON_NAME].GetString();
			if (tagName==expectedTagName)
			{
				tagIndex = t;
				break;
			}
		}
		ASSERT_NE(-1,tagIndex);
		ASSERT_EQ(expectedTagName,tagName);
		string sender = tags[tagIndex][JSON_SENDER].GetString();
		ASSERT_EQ(expectedSender,sender);
		string receivers = tags[tagIndex][JSON_RECEIVERS].GetString();
		ASSERT_EQ(expectedReceivers,receivers);
	}
}

//[4,3,10,3200,40,0,0,0,0,0,1,3,25,3,2,1,30,3,5,1,35,3,0,0,0,769,770,771,0,0,4,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0]

TEST_F(DevicesTests,GetDevValueTest)
{
	homeisStarter->Start();

	ModbusAtom modbusAtom(*homeisStarter);
	DevicesAtom deviceAtoms(*homeisStarter);

	string response = modbusAtom.Scan(homeisStarter->GetModbusConfig().Name,1);

	Document document = deviceAtoms.GetDevices();

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
