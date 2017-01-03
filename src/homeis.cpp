/*
 * homeis.cpp
 *
 *  Created on: Feb 17, 2013
 *      Author: Josef Lola
 */

#include <unistd.h>

//#include "MemoryTrace.hpp"
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

#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
//#include "HomeIsConfig.h"

int SendTTy()
{
	int fd; // file descriptor
	int flags; // communication flags
	int rsl_len; // result size
	char message[100]; // message to send
	char result; // result to read

	flags = O_RDWR | O_NOCTTY; // Read and write, and make the job control for portability
	if ((fd = open("/dev/ttyUSB0", flags)) == -1 ) {
	  printf("Error while opening\n"); // Just if you want user interface error control
	  return -1;
	}
	// In this point your communication is already estabilished, lets send out something
	strcpy(message, "Hellofsdaf");
	message[0] = 2;
	message[1] = 3;
	if (rsl_len = write(fd, message, strlen(message)) < 0 ) {
	  printf("Error while sending message\n"); // Again just in case
	  return -2;
	}
	if (rsl_len = read(fd, &result, sizeof(result)) < 0 ) {
	  printf("Error while reading return\n");
	  return -3;
	}
	close(fd);

	return 0;
}

//using namespace leaktracer;

int main(int argc, char **argv)
{
//	MemoryTrace::GetInstance().startMonitoringAllThreads();

//	for(int i=0;i<10;i++)
//		SendTTy();
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
	}

	if (printversion)
	{
		printf("%d.%d.%8d\n",VERSION_MAIN,VERSION_SEC,VERSION_BUILD);
		return 0;
	}

	if (dodaemonize)
	{
		string path;
		daemonize("/var/run/homeisd.pid");
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


	HomeIsServer server(serports,config.GetServerPort());
	server.Start();
	server.Stop();

	exit(EXIT_SUCCESS);
}

