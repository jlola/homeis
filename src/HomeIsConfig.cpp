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
#include "HisException.h"
#include "StringBuilder.h"
#include "File.h"
#include "logger.h"

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
\n\
SmtpServer=\"smtp.seznam.cz\"\n\
SmtpUserName=\"\"\n\
SmtpPassword=\"\"\
";

HomeIsConfig::HomeIsConfig(string text, bool isFileName) :
	logger(CLogger::GetLogger())
{
	// Read the file. If there is an error, report it and exit.
	try
	{
		if (isFileName)
		{
			cfg.readFile(GetOrCreateFilePath(text).c_str());
		}
		else
		{
			cfg.readString(text);
		}
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

string HomeIsConfig::GetOrCreateFilePath(string filename)
{
	File file;
	string configFilePath = file.getexepath() + "/" + filename;
	//if (File::Exists(configFilePath)) File::Delete(configFilePath);
	if (!file.Exists(configFilePath))
	{
		logger.Info("File %s not exists and will be created with default params",configFilePath.c_str());

		FILE* fp = fopen(configFilePath.c_str(), "w+");
		if (fp == NULL) {
			logger.Error("I couldn't open %s for writing.\n",configFilePath.c_str());
			throw MException(StringBuilder::Format("I couldn't open %s for writing.\n").c_str());
		}

		fprintf(fp,configFileDefault);

		fclose(fp);
	}
	return configFilePath;
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

SSmtpSettings HomeIsConfig::GetSmtpSettings()
{
	SSmtpSettings result;
	Setting& root = cfg.getRoot();
	root.lookupValue("SmtpServer",result.SMTP);
	root.lookupValue("SmtpPassword",result.Password);
	root.lookupValue("SmtpUserName",result.UserName);
	ILogger & logger = CLogger::GetLogger();
	if (result.SMTP.length()<=0)
	{
		logger.Info("SmtpServer is not configured. Email sender is disabled.");
	}
	else
	{
		logger.Info("SmtpServer: %s",result.SMTP.c_str());
		logger.Info("SmtpUserName: %s",result.UserName.c_str());
	}
	return result;
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

