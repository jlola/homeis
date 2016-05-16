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
SerialPorts = ({\n\
		Name=\"/dev/ttyS2\"		#name of port\n\
		Driver=\"DS2480B\"	#driver: DS2480B or PassiveSerial\n\
		});\n\
USBCount=0;				#count of connected adapters\n\
";



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
		string name,driver;
		for(int i=0;i<count;i++)
		{
			const Setting &serialport = serialports[i];
			if (serialport.lookupValue("Name", name)&&
				serialport.lookupValue("Driver", driver))
			{
				serport.Driver = driver;
				serport.Name = name;
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

