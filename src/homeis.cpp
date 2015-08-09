/*
 * homeis.cpp
 *
 *  Created on: Feb 17, 2013
 *      Author: Josef Lola
 */

#include <unistd.h>

//#include "HisDevBase.h"
//#include "HisDevices.h"
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

#include "lua.h"                                /* Always include this when calling Lua */
#include "lauxlib.h"                            /* Always include this when calling Lua */
#include "lualib.h"                             /* Always include this when calling Lua */

//#include "stack_trace.h"
//#include "libcrash-master/crash.h"
}


//char buf[128];

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
	//string logfile = File::getexepath() + "/stdout.txt";
	//redirect_stdout(logfile.c_str());

	printf("Home information system v.1.0.9\n");
	printf("-------------------------------\n");
	CLogger::Info("Home information system v.1.0.9\n");
	CLogger::Info("-------------------------------\n");

	HomeIsServer server("",81);
	server.Start();
	server.Stop();

	exit(EXIT_SUCCESS);
}
