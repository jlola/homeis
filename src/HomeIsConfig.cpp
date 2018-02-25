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
#include "HomeIsConfig.h"


using namespace std;

const char configFileDefault[] = "\
\n\
#Server port\n\
Port=81;\n\
AllowOrigin=\"\"\n\
SerialPorts = (\
		#{\n\
		#Port=\"/dev/ttyUSB0\"		#name of port\n\
		#Driver=\"Modbus\"			#driver: modbus\n\
		#Name=\"Main\"\n\
		#},\
		{\n\
		Port=\"SimulatedPort\"		#name of port\n\
		Driver=\"ModbusSimulator\"	#driver: ModbusSimulator\n\
		Name=\"Simulator\"\n\
		}\n\
		);\n\
		LogLevel=\"Info\"				# Error,Info,Trace\n\
";

HomeIsConfig::HomeIsConfig(string pfilename) :
	logger(CLogger::GetLogger())
{
	filename = pfilename;

	// Read the file. If there is an error, report it and exit.
	try
	{
		string configFilePath = File::getexepath() + "/" + filename;
		//if (File::Exists(configFilePath)) File::Delete(configFilePath);
		if (!File::Exists(configFilePath))
		{
			logger.Info("File %s not exists and will be created with default params",configFilePath.c_str());

			FILE* fp = fopen(configFilePath.c_str(), "w+");
			if (fp == NULL) {
				logger.Error("I couldn't open %s for writing.\n",configFilePath.c_str());
				return;
			}

			fprintf(fp,configFileDefault);

			fclose(fp);
		}

		cfg.readFile(configFilePath.c_str());
	}
	catch(const FileIOException &fioex)
	{
		logger.Error("I/O error while reading file. %s", fioex.what());
		throw;
	}
	catch(const ParseException &pex)
	{
		logger.Error("Parse error at : in file %s - at line: %d %s", pex.getFile(), pex.getLine(),pex.getError());
		throw;
	}
}

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

ELogLevel HomeIsConfig::GetLogLevel()
{
	string result = "";
	Setting& root = cfg.getRoot();
	if (root.lookupValue("LogLevel",result))
	{
		if (result=="Info")
			return ELogLevelInfo;
		else if (result=="Error")
			return ELogLevelError;
		else if (result=="Trace")
			return ELogLevelTrace;
	}
	return ELogLevelTrace;
}

string HomeIsConfig::GetAllowOrigin()
{
	string result = "";
	Setting& root = cfg.getRoot();
	if (root.lookupValue("AllowOrigin",result))
	{
		return result;
	}
	return "";
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

HomeIsConfig::~HomeIsConfig()
{

}

