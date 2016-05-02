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

//#include "homeis/Expressions/LuaExpression.h"
#include "death_handler.h"
#include "linuxservice.h"

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
//#include "HomeIsConfig.h"

int main(int argc, char **argv)
{
	Debug::DeathHandler dh;

	bool dodaemonize = true;

	if (argc >= 2)
	{
		if ( strcmp( argv[1], "DEBUG") == 0 )
		{
			dodaemonize = false;
		}
	}

	if (dodaemonize)
	{
		daemonize();
		sleep(10);
	}

	char infomsg[] =
"Home information system v.1.0.10\n \
-------------------------------\n";
	printf(infomsg);
	CLogger::Info(infomsg);

	HomeIsConfig config("homeis.cfg");

	vector<SSerPortConfig> sreports = config.GetSerialPorts();

	HomeIsServer server(sreports,config.GetServerPort());
	server.Start();
	server.Stop();

	exit(EXIT_SUCCESS);
}

