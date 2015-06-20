/*
 * LuaExpression.h
 *
 *  Created on: Jul 28, 2013
 *      Author: root
 */

#ifndef LUAEXPRESSION_H_
#define LUAEXPRESSION_H_

#include <string>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "homeis/Expressions/IExpression.h"
#include "homeis/Common/HisBase.h"
#include "homeis/Devices/Folder/HisDevFolder.h"
#include "homeis/Devices/HisDevices.h"
#include "homeis/Devices/ValueEventArgs.h"
#include "homeis/Expressions/ExpressionRuntime.h"

#define luac_c
#define LUA_CORE
//#define lua_c
extern "C" {

#include "lua.h"                                /* Always include this when calling Lua */
#include "lauxlib.h"                            /* Always include this when calling Lua */
#include "lualib.h"                             /* Always include this when calling Lua */

}

#include "homeis/Common/HisLock.h"

using namespace std;

#define EXPRESSION_FOLDER "Expressions"

#define NODE_EXPRESSION BAD_CAST "expression"
#define FOLDERNAME "Expressions"
#define PROP_DESCRIPTION "description"
#define PROP_RUNNING "running"
#define PROP_EXPRESSIONBODY "expressionbody"

class LuaExpression : public HisBase, public IExpression
{
private:
	LOW_thread_mutex* mutex;
	ExpressionRuntime *expressionRuntime;
	bool inEvalFunc;
	int64_t nextTime;
	bool runningAllowed;
	bool running;
	lua_State* L;
	lua_State* cL;
	string lastEvaluateError;
	HisDevFolder* folder;
	HisDevices* devices;
	vector<HisDevValueBase*> values;
	string luaContent;
	string oldName;
	string description;
	void CreateFolder();
	void SaveExpressionToFile();
	void LoadExpressionFromFile();
	void OnValueChaned(ValueChangedEventArgs args);
	void open_libs(lua_State *L);
	void SetGlobals(lua_State* L);
	void GetGlobals(lua_State* L);
	string GetLuaCodeInFuncion(string funcName, string luaCodeFilePath);
	void StartListening();
	void StopListening();
public:
	CUUID GetRecordId();
	static int delays;
	string GetLastEvaluateError();
	LuaExpression(HisDevFolder* folder,HisDevices* hisDevices, string expressionName,ExpressionRuntime *pExpressionRuntime);
	LuaExpression(xmlNodePtr pnode,HisDevices* hisDevices,ExpressionRuntime *pExpressionRuntime);
	void ReloadValues();
	string GetFileName(string pFileName);
	void SetName(string name);
	void SetExpression(string expression);
	string GetExpression();
	string GetDescription();
	void SetDescription(string pDescription);
	bool ExistsName(string pFileName);
	xmlChar* GetNodeNameInternal();
	void DoInternalLoad(xmlNodePtr & node);
	void DoInternalSave(xmlNodePtr & node);
	bool Evaluate();
	bool ForceEvaluate();
	size_t GetCountInputs();
	size_t GetCountOutputs();
	void SetRunning(bool running);
	bool GetRunning();
	~LuaExpression();
};

#endif /* LUAEXPRESSION_H_ */
