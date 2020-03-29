/*
 * homeis.cpp
 *
 *  Created on: Feb 17, 2013
 *      Author: Josef Lola
 */

#include <unistd.h>
#include <HisDevRunTime.h>
#include "HomeIsServer.h"
#include "logger.h"
#include "File.h"
#include "converter.h"
#include "string"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "Common/CUUID.h"
#include "Devices/Folder/HisDevFolder.h"
#include "Devices/Folder/HisDevFolderRoot.h"
#include "StringBuilder.h"
#include "Helpers/File.h"
#include "death_handler.h"
#include "linuxservice.h"
#include "Version.h"


#define luac_c
#define LUA_CORE
extern "C" {


}
#include <signal.h>
#include "PoppyDebugTools.h"

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <cxxabi.h>
#include "ModbusProvider.h"
#include "EmailSender.h"
#include "SerialPort.hpp"
#include "ModifiedMdbus.h"

#ifndef HOMEISTEST

using namespace mn;
using namespace CppLinuxSerial;

void ConfigureByArgs(int argc, char **argv,bool & debug, bool & printversion);
int main(int argc, char **argv)
{

	Debug::DeathHandler dh;
	bool debug = false;
	bool printversion = false;


	ConfigureByArgs(argc,argv,debug,printversion);

	char infomsg[] =
"\n\
---------------------------------------\n\
Home information system %d.%d.%8d\n\
---------------------------------------\n\
";
	ILogger & logger = CLogger::GetLogger();
	//printf(infomsg);
	logger.Info(infomsg,VERSION_MAIN,VERSION_SEC,VERSION_BUILD);

	HomeIsConfig config("homeis.cfg",true);

	logger.SetLogLevel(config.GetLogLevel());
	vector<SSerPortConfig> serports = config.GetSerialPorts();

	SSmtpSettings smtpsetttings = config.GetSmtpSettings();
	ModbusProvider provider(serports);
	EmailSender esender(smtpsetttings);
	HomeIsServer server(provider,&esender,config);

	server.Start(true);
	server.Stop();

	exit(EXIT_SUCCESS);
}



void ConfigureByArgs(int argc, char **argv,bool & debug, bool & printversion)
{
	if (argc >= 2)
		{
			for(int i=1;i<argc;i++)
			{
				if ( strcmp( argv[i], "DEBUG") == 0 )
				{
					debug = true;
				}
				if ( strcmp( argv[i], "-version") == 0 )
				{
					printversion = true;
				}
			}
			if (!debug && !printversion)
			{
				printf("Homeis was run with wrong arguments");
				exit(1);
			}
		}

		if (printversion)
		{
			printf("%d.%d.%8d\n",VERSION_MAIN,VERSION_SEC,VERSION_BUILD);
			exit(0);
		}

		if (!debug)
		{
			File file;
			string pid = StringBuilder::Format("/var/run/%s.pid",file.getexefile().c_str());
			daemonize2(pid.c_str());
			sleep(5);
		}
}

#endif

