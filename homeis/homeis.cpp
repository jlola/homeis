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

#define luac_c
#define LUA_CORE
//#define lua_c
extern "C" {

#include "lua.h"                                /* Always include this when calling Lua */
#include "lauxlib.h"                            /* Always include this when calling Lua */
#include "lualib.h"                             /* Always include this when calling Lua */

}

///* the Lua interpreter */
//lua_State* L;
//
//int luaadd ( int x, int y )
//{
//	int sum;
//
//	lua_pushboolean(L,false);
//	lua_setglobal(L, "vstup");
//
//	lua_pushboolean(L,false);
//	lua_setglobal(L, "vystup");
//
//
//	/* the function name */
//	lua_getglobal(L, "add");
//
//	/* the first argument */
//	//lua_pushnumber(L, x);
//
//	/* the second argument */
//	//lua_pushnumber(L, y);
//
//	/* call the function with 2 arguments, return 1 result */
//	if (lua_pcall(L, 0, 0, 0))
//	{
//		printf("Error %s",lua_tostring(L,-1));
//	}
//
//	/* get the result */
//	//sum = (int)lua_tointeger(L, -1);
//	//lua_pop(L, 1);
//
//	lua_getglobal(L,"vystup");
//    bool vystup = lua_toboolean(L,-1);
//
//    lua_getglobal(L,"vstup");
//    bool vstup = lua_toboolean(L,-1);
//
//
//	return sum;
//}

int main(int argc, char **argv)
{

//		int sum;
//
//		/* initialize Lua */
//		L = luaL_newstate();                        /* Create Lua state variable */
//
//		/* load Lua base libraries */
//		luaL_openlibs(L);
//
//		/* load the script */
//		//luaL_dofile(L, "add.lua");
//		if (luaL_dostring(L,
//
//				" appName = \"test\"\
//				function add () \
//				vystup = not vstup\
//					 vstup = true\
//				  end \
//				"))
//		{
//			printf("error running function `f': %s\n",lua_tostring(L, -1));
//			return 1;
//		}
//
//		/* call the add function */
//		sum = luaadd( 10, 15 );
//
//		/* print the result */
//		printf( "The sum is %d\\n", sum );
//
//		/* cleanup Lua */
//		lua_close(L);


	HomeIsServer server("/dev/ttyAMA0",81);
	server.Start();
	server.Stop();

 	return 0;
}
