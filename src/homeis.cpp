/*
 * homeis.cpp
 *
 *  Created on: Feb 17, 2013
 *      Author: Josef Lola
 */

#include <unistd.h>
#include "HisDevRunTime.h"
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

//#include "homeis/Expressions/LuaExpression.h"
#include "death_handler.h"
#include "linuxservice.h"
#include "Version.h"

#define luac_c
#define LUA_CORE
//#define lua_c
extern "C" {

//#include "lua.h"                                /* Always include this when calling Lua */
//#include "lauxlib.h"                            /* Always include this when calling Lua */
//#include "lualib.h"                             /* Always include this when calling Lua */

}
#include <signal.h>
#include "PoppyDebugTools.h"

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <cxxabi.h>

int main(int argc, char **argv)
{
	Debug::DeathHandler dh;
	bool dodaemonize = true;
	bool printversion = false;

	if (argc >= 2)
	{
		for(int i=1;i<argc;i++)
		{
			if ( strcmp( argv[i], "DEBUG") == 0 )
			{
				dodaemonize = false;
			}
			if ( strcmp( argv[i], "-version") == 0 )
			{
				printversion = true;
			}
		}
		if (dodaemonize && !printversion)
		{
			printf("Homeis was run with wrong arguments");
			exit(1);
		}
	}

	if (printversion)
	{
		printf("%d.%d.%8d\n",VERSION_MAIN,VERSION_SEC,VERSION_BUILD);
		return 0;
	}

	if (dodaemonize)
	{
		string pid = StringBuilder::Format("/var/run/%s.pid",File::getexefile().c_str());
		daemonize2(pid.c_str());
		sleep(5);
	}

	char infomsg[] =
"\n\
---------------------------------------\n\
Home information system %d.%d.%8d\n\
---------------------------------------\n\
";
	//printf(infomsg);
	CLogger::Info(infomsg,VERSION_MAIN,VERSION_SEC,VERSION_BUILD);

	HomeIsConfig config("homeis.cfg");
	vector<SSerPortConfig> serports = config.GetSerialPorts();

	HomeIsServer server(serports,config.GetServerPort(),config.UseHTTPS(),
			config.HTTPSKey(),config.HTTPSCert());

	server.Start(true);
	server.Stop();

	exit(EXIT_SUCCESS);
}

