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
#include "logger.h"
#include "Expressions/IExpression.h"
#include "Common/HisBase.h"
#include "Devices/Folder/HisDevFolder.h"
#include "Devices/HisDevices.h"
#include "Devices/ValueEventArgs.h"
#include "IExpressionRuntime.h"
#include "IHisDevFactory.h"

#define luac_c
#define LUA_CORE
//#define lua_c
extern "C" {

#include "lua.h"                                /* Always include this when calling Lua */
#include "lauxlib.h"                            /* Always include this when calling Lua */
#include "lualib.h"                             /* Always include this when calling Lua */

}

#include "Common/HisLock.h"

using namespace std;

#define EXPRESSION_FOLDER "Expressions"

#define NODE_EXPRESSION BAD_CAST "expression"
#define FOLDERNAME "Expressions"
#define PROP_DESCRIPTION "description"
#define PROP_RUNNING "running"
#define PROP_EXPRESSIONBODY "expressionbody"
#define MAX_LOGS 10

class LuaExpression : public HisBase, public IExpression
{
public:
	static LuaExpression* ActualExpression;
private:
	ILogger & logger;
	vector<string> logs;
	LPCRITICAL_SECTION setRunningMutex;
	LPCRITICAL_SECTION evaluateMutex;
	IExpressionRuntime *expressionRuntime;
	bool inEvalFunc;
	uint64_t nextTime;
	bool runningAllowed;
	//priznak ze se prave vykonava
	bool running;
	//priznak ze byla funkce nastartovana, start je treba udelat az po loadu
	bool started;
	lua_State* L;
	lua_State* cL;
	string lastEvaluateError;
	HisDevFolder* folder;
	HisDevices* devices;
	vector<HisDevValueBase*> values;
	vector<HisDevBase*> expressionDevices;
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
	int setLuaPath( lua_State* L, const char* path );
	bool IsInExpressionDevices(HisDevBase* pdev);
public:
	void Start();
	string GetName();
	string GetLuaFilesPath();
	CUUID GetRecordId();
	static int delays;
	string GetLastEvaluateError();
	LuaExpression(HisDevFolder* folder,HisDevices* hisDevices, string expressionName,IExpressionRuntime *pExpressionRuntime,IHisDevFactory* factory);
	LuaExpression(xmlNodePtr pnode,HisDevices* hisDevices,IExpressionRuntime *pExpressionRuntime,IHisDevFactory* factory);
	void DebugLog(string ln);
	void DebugPrint(string ln);
	void ReloadValues();
	string GetFilePath(string pFileName);
	void SetName(string name);
	void SetExpression(string expression);
	string GetExpression();
	string GetDescription();
	void SetDescription(string pDescription);
	bool ExistsName(string pFileName);
	const xmlChar* GetNodeNameInternal();
	void DoInternalLoad(xmlNodePtr & node);
	void DoInternalSave(xmlNodePtr & node);
	bool Evaluate();
	bool ForceEvaluate();
	size_t GetCountInputs();
	size_t GetCountOutputs();
	bool GetRunning();
	vector<string> GetLogs();
	void SetRunning(bool running);
	~LuaExpression();
};

#endif /* LUAEXPRESSION_H_ */
