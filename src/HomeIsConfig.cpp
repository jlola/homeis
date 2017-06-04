/*
 * PrintserverConfig.cpp
 *
 *  Created on: 24. 8. 2015
 *      Author: jlola
 */

#include <stdio.h>
#include <iostream>
#include <fstream>      // std::ifstream
#include <sstream>
#include <vector>
#include "Helpers/logger.h"
#include "HomeIsConfig.h"


using namespace std;

const char configFileDefault[] = "\
\n\
#Server port\n\
Port=81;\n\
\n\
UseHttps=true;\n\
#relative path to homeis dir\n\
https_key=\"server.key\"\n\
https_cert=\"server.crt\"\n\
\n\
SerialPorts = (\
		#{\n\
		#Port=\"/dev/ttyUSB0\"		#name of port\n\
		#Driver=\"Modbus\"	#driver: DS2480B or PassiveSerial\n\
		#Name=\"Main\"\n\
		#},\
		{\n\
		Port=\"SimulatedPort\"		#name of port\n\
		Driver=\"ModbusSimulator\"	#driver: DS2480B or PassiveSerial\n\
		Name=\"Simulator\"\n\
		}\
		);\n\
";


bool HomeIsConfig::UseHTTPS()
{
	Setting& root = cfg.getRoot();
	if (root.exists("SerialPorts"))
	{
		bool useHttps = false;
		if (root.lookupValue("UseHttps",useHttps))
		{
			return useHttps;
		}
	}
	return false;
}

string HomeIsConfig::HTTPSCert()
{
	Setting& root = cfg.getRoot();
	if (root.exists("SerialPorts"))
	{
		string httpscert;
		if (root.lookupValue("https_cert",httpscert))
		{
			return httpscert;
		}
	}
	return "server.crt";
}

string HomeIsConfig::HTTPSKey()
{
	Setting& root = cfg.getRoot();
	if (root.exists("SerialPorts"))
	{
		string httpskey;
		if (root.lookupValue("https_key",httpskey))
		{
			return httpskey;
		}
	}
	return "server.key";
}

HomeIsConfig::HomeIsConfig(string pfilename)
{
	filename = pfilename;

	// Read the file. If there is an error, report it and exit.
	try
	{
		string configFilePath = File::getexepath() + "/" + filename;
		//if (File::Exists(configFilePath)) File::Delete(configFilePath);
		if (!File::Exists(configFilePath))
		{
			CLogger::Info("File %s not exists and will be created with default params",configFilePath.c_str());

			FILE* fp = fopen(configFilePath.c_str(), "w+");
			if (fp == NULL) {
				CLogger::Error("I couldn't open %s for writing.\n",configFilePath.c_str());
				return;
			}

			fprintf(fp,configFileDefault);

			fclose(fp);
		}

		cfg.readFile(configFilePath.c_str());
	}
	catch(const FileIOException &fioex)
	{
		CLogger::Error("I/O error while reading file. %s", fioex.what());
		throw;
	}
	catch(const ParseException &pex)
	{
		CLogger::Error("Parse error at : %s - %d %s", pex.getFile(), pex.getLine(),pex.getError());
		throw;
	}
}

//string HomeIsConfig::GetPrinterGadgetCMD()
//{
//	string result;
//	Setting& root = cfg.getRoot();
//	if (!root.lookupValue("PrinterGadgetCMD",result))
//	{
//		CLogger::Info("Config file does not contains key: GetPrinterGadgetCMD will be used default wihthout parametters.");
//		//idVendor=0x03f0 idProduct=0x3217 iSerialNum=2 iPNPstring=\"MFG:Hewlett-Packard;CMD:L,PML,MLC,POSTSCRIPT,PCLXL,PCL;MDL:HP P LaserJet 3050;CLS:PRINTER;DES:Hewlett-Packa LaserJet 3050EMEM:MEM=53MB;12.4.4DL:4d,4e,1;1;CMEMENT:RES=1200x1;\"
//		result = "modprobe g_printer";
//	}
//	return result;
//}

vector<SSerPortConfig> HomeIsConfig::GetSerialPorts()
{
	vector<SSerPortConfig> serports;
	Setting& root = cfg.getRoot();
	if (root.exists("SerialPorts"))
	{
		const Setting &serialports = root["SerialPorts"];
		int count = serialports.getLength();
		SSerPortConfig serport;
		string name,driver,port;
		for(int i=0;i<count;i++)
		{
			const Setting &serialport = serialports[i];
			if (serialport.lookupValue("Name", name)&&
				serialport.lookupValue("Driver", driver)&&
				serialport.lookupValue("Port", port))
			{
				serport.Driver = driver;
				serport.Name = name;
				serport.Port = port;
				serports.push_back(serport);
			}
		}
	}
	return serports;
}

int HomeIsConfig::GetServerPort()
{
	int intresult = 0;
	Setting& root = cfg.getRoot();
	if (root.lookupValue("Port",intresult))
	{
		return intresult;
	}
	return 81;
}

//string HomeIsConfig::GetMountMassScript()
//{
//	string result;
//	Setting& root = cfg.getRoot();
//	//if (!root.lookupValue("MountMassScript",result))
//	return result;
//}
//
//string HomeIsConfig::GetMountFolderScript()
//{
//	string result;
//	Setting& root = cfg.getRoot();
////	if (!root.lookupValue("MountFolderScript",result))
////		throw PrintServerException("Config file does not contains key: MountFolderScript");
//	return result;
//}
//
//string HomeIsConfig::GetUmountFolderScript()
//{
//	string result;
//	Setting& root = cfg.getRoot();
////	if (!root.lookupValue("UmountScript",result))
////		throw PrintServerException("Config file does not contains key: UmountScript");
//	return result;
//}
//
//string HomeIsConfig::GetUmountMassScript()
//{
//	string result;
//	Setting& root = cfg.getRoot();
////	if (!root.lookupValue("UmountMassScript",result))
////		throw PrintServerException("Config file does not contains key: UmountMassScript");
//	return result;
//}
//
//string HomeIsConfig::GetTargetDir()
//{
//	string result;
//	Setting& root = cfg.getRoot();
////	if (!root.lookupValue("TargetDir",result))
////		throw PrintServerException("Config file does not contains key: TargetDir");
//	return result;
//}
//
//string HomeIsConfig::GetRefreshScript()
//{
//	string result;
//	Setting& root = cfg.getRoot();
////	if (!root.lookupValue("RefreshScript",result))
////		throw PrintServerException("Config file does not contains key: RefreshScript");
//	return result;
//}
//
//string HomeIsConfig::GetFileStoragePath()
//{
//	string result;
//	Setting& root = cfg.getRoot();
////	if (!root.lookupValue("FileStorageName",result))
////		throw PrintServerException("Config file does not contains key: FileStorageName");
//	return result;
//}

HomeIsConfig::~HomeIsConfig()
{

}

