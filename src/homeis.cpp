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

}
#include <signal.h>
#include "PoppyDebugTools.h"

#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <cxxabi.h>

static inline void print_stacktrace(FILE *out, unsigned int max_frames);

void signal_handler(int signal, siginfo_t *info, void *reserved){
	print_stacktrace(stderr,512);
    std::string stackTrace = Stack:: GetTraceString();
    //you can print or log the stack trace here
    CLogger::Error(stackTrace.c_str());
}

int main(int argc, char **argv)
{
	Debug::DeathHandler dh;

//	struct sigaction handler;
//	//handler.sa_mask = 0;
//	handler.sa_restorer = 0;
//	handler.sa_sigaction = signal_handler;
//	handler.sa_flags = SA_RESETHAND;
//	#define CATCHSIG(X) sigaction(X, &handler, NULL)
//	CATCHSIG(SIGILL);
//	CATCHSIG(SIGABRT);
//	CATCHSIG(SIGBUS);
//	CATCHSIG(SIGFPE);
//	CATCHSIG(SIGSEGV);
//	CATCHSIG(SIGSTKFLT);
//	CATCHSIG(SIGPIPE);

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

	HomeIsServer server("/dev/ttyS2",81);
	server.Start();
	server.Stop();

	exit(EXIT_SUCCESS);
}

// stacktrace.h (c) 2008, Timo Bingmann from http://idlebox.net/
// published under the WTFPL v2.0

/** Print a demangled stack backtrace of the caller function to FILE* out. */
void print_stacktrace(FILE *out = stderr, unsigned int max_frames = 63)
{
    fprintf(out, "stack trace:\n");

    // storage array for stack trace address data
    void* addrlist[max_frames+1];

    // retrieve current stack addresses
    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

    if (addrlen == 0) {
	fprintf(out, "  <empty, possibly corrupt>\n");
	return;
    }

    // resolve addresses into strings containing "filename(function+address)",
    // this array must be free()-ed
    char** symbollist = backtrace_symbols(addrlist, addrlen);

    // allocate string which will be filled with the demangled function name
    size_t funcnamesize = 256;
    char* funcname = (char*)malloc(funcnamesize);

    // iterate over the returned symbol lines. skip the first, it is the
    // address of this function.
    for (int i = 1; i < addrlen; i++)
    {
	char *begin_name = 0, *begin_offset = 0, *end_offset = 0;

	// find parentheses and +address offset surrounding the mangled name:
	// ./module(function+0x15c) [0x8048a6d]
	for (char *p = symbollist[i]; *p; ++p)
	{
	    if (*p == '(')
		begin_name = p;
	    else if (*p == '+')
		begin_offset = p;
	    else if (*p == ')' && begin_offset) {
		end_offset = p;
		break;
	    }
	}

	if (begin_name && begin_offset && end_offset
	    && begin_name < begin_offset)
	{
	    *begin_name++ = '\0';
	    *begin_offset++ = '\0';
	    *end_offset = '\0';

	    // mangled name is now in [begin_name, begin_offset) and caller
	    // offset in [begin_offset, end_offset). now apply
	    // __cxa_demangle():

	    int status;
	    char* ret = abi::__cxa_demangle(begin_name,
					    funcname, &funcnamesize, &status);
	    if (status == 0) {
		funcname = ret; // use possibly realloc()-ed string
		fprintf(out, "  %s : %s+%s\n",
			symbollist[i], funcname, begin_offset);
	    }
	    else {
		// demangling failed. Output function name as a C function with
		// no arguments.
		fprintf(out, "  %s : %s()+%s\n",
			symbollist[i], begin_name, begin_offset);
	    }
	}
	else
	{
	    // couldn't parse the line? print the whole line.
	    fprintf(out, "  %s\n", symbollist[i]);
	}
    }

    free(funcname);
    free(symbollist);
}

