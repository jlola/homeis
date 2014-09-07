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
#include "homeis/Devices/Folder/HisDevFolder.h"
#include "homeis/Devices/Folder/HisDevFolderRoot.h"

//#include "homeis/Expressions/LuaExpression.h"
#include "linuxservice.h"

#define luac_c
#define LUA_CORE
//#define lua_c
extern "C" {

#include "lua.h"                                /* Always include this when calling Lua */
#include "lauxlib.h"                            /* Always include this when calling Lua */
#include "lualib.h"                             /* Always include this when calling Lua */

#include "stack_trace.h"
#include "libcrash-master/crash.h"

}

std::string code = "i = 1 \
function loop() \
	print(\"loop.lua\");	\
\
	while i < 4 \
	do \
		print(\"lua_loop iteration\"); \
		sleep(10);\
\
	i = i + 1; \
	end \
end ";

//bool running = true;
//
//int lua_finish(lua_State *) {
//	    running = false;
//	    printf("lua_finish called\n");
//	    return 0;
//	}
//int rtrn;
//	int lua_sleep(lua_State *L) {
//		rtrn = lua_tointeger(L, -1);      /* Get the single number arg */
//	    //printf("lua_sleep called %d ms\n",rtrn);
//	    return lua_yield(L,1);
//	}

//	int status;
//	    lua_State* L = luaL_newstate();
//	    luaL_openlibs(L);
//
//	    lua_register(L, "sleep", lua_sleep);
//	    lua_register(L, "finish", lua_finish);
//	    status = luaL_dostring(L,code.c_str());
//	    if ( status != LUA_OK ) {
//	    	printf("isstring: %s\n", lua_tostring(L, -1));
//	    	return 0;
//	    }
//	    lua_pcall(L, 0, 0, 0);
//
//	    lua_State* cL = lua_newthread(L);
//
//	    lua_getglobal(cL, "loop");
//
//	    while (running) {
//
//	        status = lua_resume(cL,NULL,0);
//	        if (status == LUA_YIELD) {
//	            printf("loop yielding %d ms\n",rtrn);
//	        } else {
//	            running=false; // you can't try to resume if it didn't yield
//	            // catch any errors below
//	            if (status == LUA_ERRRUN && lua_isstring(cL, -1)) {
//	                printf("isstring: %s\n", lua_tostring(cL, -1));
//	                lua_pop(cL, -1);
//	            }
//	        }
//	    }
//
//	    lua_close(L);

char buf[128];

int main(int argc, char **argv)
{
	//startservice();
	redirect_stdout("~/log.txt");

	printf("Home information system v.1.0.5\n");
	printf("-------------------------------\n");
	int ret = register_crash_handler(argv[0],(unsigned char *)&buf);
	assert(ret==0);
	//set_signal_handler(argv[0]);

	//"/dev/ttyAMA0"
	HomeIsServer server("",81);
	server.Start();
	server.Stop();

	exit(EXIT_SUCCESS);
}
