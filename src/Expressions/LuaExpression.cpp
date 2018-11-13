/*
 * LuaExpression.cpp
 *
 *  Created on: Jul 28, 2013
 *      Author: root
 */
#include <stdio.h>
#include <iostream>
#include <fstream>      // std::ifstream
#include <sstream>

#include "srutil/event/event.hpp"
#include "srutil/delegate/delegate.hpp"

#include "Helpers/File.h"
#include "Helpers/Directory.h"
#include "Helpers/StringBuilder.h"
#include "Helpers/HisDateTime.h"
#include "Common/HisException.h"

#include "Expressions/LuaExpression.h"

//#define luac_c
//#define LUA_CORE
//#define lua_c
//extern "C" {
//
//#include "lua.h"                                /* Always include this when calling Lua */
//#include "lauxlib.h"                            /* Always include this when calling Lua */
//#include "lualib.h"                             /* Always include this when calling Lua */
//
//}
#include "PoppyDebugTools.h"
#include "LuaExpression.h"

LuaExpression* LuaExpression::ActualExpression=NULL;

LuaExpression::LuaExpression(xmlNodePtr pnode,
		HisDevices* hisDevices,
		IExpressionRuntime *pExpressionRuntime,
		IHisDevFactory* factory) :
	HisBase(pnode,factory),
	logger(CLogger::GetLogger()),
	started(false)
{
	STACK

	if (hisDevices==NULL)
	{
		throw ArgumentNullException("hisDevices");
	}
	if (pExpressionRuntime==NULL)
	{
		throw ArgumentNullException("pExpressionRuntime");
	}
	if (factory==NULL)
	{
		throw ArgumentNullException("factory");
	}
	setRunningMutex = HisLock::CreateMutex();
	evaluateMutex = HisLock::CreateMutex();
	expressionRuntime = pExpressionRuntime;
	devices = hisDevices;
	nextTime = 0;
	inEvalFunc = false;
	runningAllowed = false;
	CreateFolder();
	running = false;
	L = NULL;
	cL = NULL;
	folder = NULL;
}

LuaExpression::LuaExpression(HisDevFolder* folder,
		HisDevices* devices,
		string expressionName,
		IExpressionRuntime *expressionRuntime,
		IHisDevFactory* factory) :
	HisBase(factory),
	logger(CLogger::GetLogger()),
	started(false)
{
	STACK
	if (folder==NULL)
	{
		logger.Error("LuaExpression | Constructor can not be called with empty folder");
		throw ArgumentNullException("folder");
	}
	if (devices==NULL)
	{
		throw ArgumentNullException("devices");
	}
	if (expressionRuntime==NULL)
	{
		throw ArgumentNullException("expressionRuntime");
	}
	if (factory==NULL)
	{
		throw ArgumentNullException("factory");
	}

	CreateFolder();
	setRunningMutex = HisLock::CreateMutex();
	evaluateMutex = HisLock::CreateMutex();
	this->expressionRuntime = expressionRuntime;
	inEvalFunc = false;
	nextTime = 0;
	runningAllowed = false;

	this->folder = folder;
	this->devices = devices;
	SetName(expressionName);
	SetParent(folder);
	running = false;
	L = NULL;
	cL = NULL;
}

CUUID LuaExpression::GetRecordId()
{
	STACK
	return HisBase::GetRecordId();
}

void LuaExpression::SetName(string name)
{
	STACK
	if (oldName!=name)
	{
		if (!ExistsName(name))
			HisBase::SetName(name);
		else throw HisException("Expression name: " + name + "already exists",__FILE__, __LINE__);
	}
}

string LuaExpression::GetLastEvaluateError()
{
	return lastEvaluateError;
}

vector<string> LuaExpression::GetLogs()
{
	return this->logs;
}

void LuaExpression::SetRunning(bool blRunning)
{
	STACK
	HisLock lock(setRunningMutex);

	if (runningAllowed!=blRunning || !started)
	{
		runningAllowed = blRunning;
		if (blRunning) {
			ReloadValues();
			//StartListening();
			expressionRuntime->Add(this);
			for(size_t i=0;i<expressionDevices.size();i++)
				expressionDevices[i]->AddExpression(this);
		}
		else
		{
			//ukoncim vlakno
			this->Evaluate();
			expressionRuntime->Remove(this);
			for(size_t i=0;i<expressionDevices.size();i++)
				expressionDevices[i]->RemoveExpression(this);
		}
	}
}

bool LuaExpression::GetRunning()
{
	return runningAllowed;
}

string LuaExpression::GetDescription()
{
	return description;
}

void LuaExpression::SetDescription(string pDescription)
{
	description = pDescription;
}

void LuaExpression::SetExpression(string expression)
{
	this->luaContent = expression;
}

string LuaExpression::GetExpression()
{
	return this->luaContent;
}

void LuaExpression::CreateFolder()
{
	STACK
	std::string dir = GetFactory()->GetFile()->getexepath() + "/"+ EXPRESSION_FOLDER;
	if (!GetFactory()->GetDirectory()->Exists(dir))
	{
		GetFactory()->GetDirectory()->Create(dir);
	}
}

void LuaExpression::SaveExpressionToFile()
{
	STACK

	if (oldName!=GetName())
	{
		if (GetFactory()->GetFile()->Exists(GetFilePath(oldName)))
			GetFactory()->GetFile()->Delete(GetFilePath(oldName));
	}
	FILE* fp = fopen(GetFilePath(GetName()).c_str(), "w+");
	if (fp == NULL) {
		string file = this->GetName()+".lua";
		printf("I couldn't open %s for writing.\n",file.c_str());
		logger.Error("I couldn't open %s for writing.\n",file.c_str());
		return;
	}

	fprintf(fp,luaContent.c_str());

	fclose(fp);
}

void LuaExpression::LoadExpressionFromFile()
{
	STACK
	if (GetFactory()->GetFile()->Exists(GetFilePath(GetName())))
	{
		std::ifstream in(GetFilePath(GetName()));

		stringstream buffer;
		buffer << in.rdbuf();

		luaContent = buffer.str();
	}
}

string LuaExpression::GetFilePath(string pFileName)
{
	STACK
	return GetLuaFilesPath() + "/" + pFileName + ".lua";
}

string LuaExpression::GetLuaFilesPath()
{
	return GetFactory()->GetFile()->getexepath() + "/"+ EXPRESSION_FOLDER;
}

bool LuaExpression::ExistsName(string pName)
{
	STACK
	string path;
	path = GetFilePath(pName);
	return GetFactory()->GetFile()->Exists(path);
}

bool LuaExpression::IsInExpressionDevices(HisDevBase* pdev)
{
	for(size_t i=0;i<expressionDevices.size();i++)
	{
		if (expressionDevices[i]==pdev)
		{
			return true;
		}
	}
	return false;
}

void LuaExpression::ReloadValues()
{
	STACK
	values.clear();
	HisDevFolder* parent = dynamic_cast<HisDevFolder*>(GetParent());
	vector<HisDevValueId*> items = parent->GetItems<HisDevValueId>();
	for(size_t i=0;i<items.size();i++)
	{
		HisDevValueId* valueId = items[i];
		if (valueId!=NULL)
		{
			HisDevValueBase* value = devices->FindValue(valueId->GetDeviceValueId());
			if (value!=NULL)
			{
				values.push_back(value);
				HisDevBase* dev = dynamic_cast<HisDevBase*>(value->GetParent());
				if (dev!=NULL)
				{
					//vlozim do seznamu ktery provede refresh v metode GetGlobals
					if (!IsInExpressionDevices(dev))
					{
						expressionDevices.push_back(dev);
					}
				}
			}
		}
	}
}

size_t LuaExpression::GetCountInputs()
{
	STACK
	size_t countInputs = 0;
	for(size_t i=0;i<values.size();i++)
	{
		if (values[i]->GetDirection()==EHisDevDirection::Read ||
			values[i]->GetDirection()==EHisDevDirection::ReadWrite)
		{
			countInputs++;
		}
	}
	return countInputs;
}

size_t LuaExpression::GetCountOutputs()
{
	STACK
	size_t countOutputs = 0;
	for(size_t i=0;i<values.size();i++)
	{
		if (values[i]->GetDirection()==EHisDevDirection::Write ||
			values[i]->GetDirection()==EHisDevDirection::ReadWrite)
		{
			countOutputs++;
		}
	}
	return countOutputs;
}

void LuaExpression::GetGlobals(lua_State* L)
{
	STACK
	for(size_t i=0;i<values.size();i++)
	{
		if (values[i]->GetDirection()==EHisDevDirection::Write||
			values[i]->GetDirection()==EHisDevDirection::ReadWrite)
		{
			switch(values[i]->GetDataType())
			{
				case EDataType::Enum:
					break;
				case EDataType::Int:
				{
					STACK_BLOCK("Int")
					HisDevValue<int>* value = dynamic_cast<HisDevValue<int>*>(values[i]);

					lua_getglobal(L,value->GetAddressName().c_str());
					int number = lua_tointeger(L,-1);
					value->SetValue(number);
					break;
				}
				case EDataType::Double:
				{
					STACK_BLOCK("Double")
					HisDevValue<double>* value = dynamic_cast<HisDevValue<double>*>(values[i]);

					lua_getglobal(L,value->GetAddressName().c_str());
					double number = lua_tonumber(L,-1);
					value->SetValue(number);
					break;
				}
				case EDataType::Uint:
				{
					STACK_BLOCK("Uint")
					HisDevValue<uint32_t>* value = dynamic_cast<HisDevValue<uint32_t>*>(values[i]);
					lua_getglobal(L,value->GetAddressName().c_str());
					unsigned int number = lua_tointeger(L,-1);
					value->SetValue(number);
					break;
				}
				case EDataType::Email:
				case EDataType::String:
				{
					STACK_BLOCK("String")
					HisDevValue<string>* value = dynamic_cast<HisDevValue<string>*>(values[i]);
					//lua_pushstring(L,value->GetValue().c_str());
					lua_getglobal(L,value->GetAddressName().c_str());
					const char* val = lua_tostring(L,-1);
					if (val!=NULL)
						value->SetValue(val);
					else
						value->SetValue("");
					break;
				}
				case EDataType::Bool:
				{
					STACK_BLOCK("Bool")
					HisDevValue<bool>* value = dynamic_cast<HisDevValue<bool>*>(values[i]);
					lua_getglobal(L,value->GetAddressName().c_str());
					value->SetValue(lua_toboolean(L,-1));
					break;
				}

				case EDataType::Unknown:
					break;
			}
		}
	}
	for(size_t i=0;i<expressionDevices.size();i++)
	{
		expressionDevices[i]->NeedRefresh();
	}
}

void LuaExpression::SetGlobals(lua_State* L)
{
	STACK
	for(size_t i=0;i<values.size();i++)
	{
		switch(values[i]->GetDataType())
		{
			case EDataType::Enum:
				break;
			case EDataType::Int:
			{
				//STACK_SECTION("Int")
				HisDevValue<int>* value = dynamic_cast<HisDevValue<int>*>(values[i]);
				lua_pushinteger(L,value->GetValue());
				lua_setglobal( L, value->GetAddressName().c_str() );

				string errvalue = value->GetAddressName()+"Err";
				lua_pushboolean(L,value->GetDeviceError());
				lua_setglobal(L, errvalue.c_str());
				break;
			}
			case EDataType::Double:
			{
				//STACK_SECTION("Double")
				HisDevValue<double>* value = dynamic_cast<HisDevValue<double>*>(values[i]);
				lua_pushnumber(L,value->GetValue());
				lua_setglobal( L, value->GetAddressName().c_str() );

				string errvalue = value->GetAddressName()+"Err";
				lua_pushboolean(L,value->GetDeviceError());
				lua_setglobal(L, errvalue.c_str());
				break;
			}
			case EDataType::Uint:
			{
				STACK_SECTION("Uint")
				HisDevValue<uint32_t>* value = dynamic_cast<HisDevValue<uint32_t>*>(values[i]);
				lua_pushinteger(L,value->GetValue());
				lua_setglobal( L, value->GetAddressName().c_str() );

				string errvalue = value->GetAddressName()+"Err";
				lua_pushboolean(L,value->GetDeviceError());
				lua_setglobal(L, errvalue.c_str());
				break;
			}
			case EDataType::Email:
			case EDataType::String:
			{
				//STACK_SECTION("String")
				HisDevValue<string>* value = dynamic_cast<HisDevValue<string>*>(values[i]);
				lua_pushstring(L,value->GetValue().c_str());
				lua_setglobal( L, value->GetAddressName().c_str() );

				string errvalue = value->GetAddressName()+"Err";
				lua_pushboolean(L,value->GetDeviceError());
				lua_setglobal(L, errvalue.c_str());
				break;
			}
			case EDataType::Bool:
			{
				STACK_SECTION("Bool")
				HisDevValue<bool>* value = dynamic_cast<HisDevValue<bool>*>(values[i]);
				lua_pushboolean(L,value->GetValue());
				lua_setglobal( L, value->GetAddressName().c_str() );

				string errvalue = value->GetAddressName()+"Err";
				lua_pushboolean(L,value->GetDeviceError());
				lua_setglobal(L, errvalue.c_str());
				break;
			}
			case EDataType::Unknown:
				break;
		}
	}
}

int LuaExpression::setLuaPath( lua_State* L, const char* path )
{
    lua_getglobal( L, "package" );
    lua_getfield( L, -1, "path" ); // get field "path" from table at top of stack (-1)
    std::string cur_path = lua_tostring( L, -1 ); // grab path string from top of stack
    cur_path.append( ";" ); // do your path magic here
    cur_path.append( path );
    lua_pop( L, 1 ); // get rid of the string on the stack we just pushed on line 5
    lua_pushstring( L, cur_path.c_str() ); // push the new one
    lua_setfield( L, -2, "path" ); // set the field "path" in table at -2 with value at top of stack
    lua_pop( L, 1 ); // get rid of package table from top of stack
    return 0; // all done!
}

void LuaExpression::open_libs(lua_State *L)
{
	STACK
  luaopen_io(L);
  luaopen_base(L);
  luaopen_table(L);
  luaopen_string(L);
  luaopen_math(L);
  //luaopen_loadlib(L);
}

string LuaExpression::GetLuaCodeInFuncion(string funcName, string luaCodeFilePath)
{
	STACK
	string code = GetFactory()->GetFile()->ReadWholeFile(luaCodeFilePath);
	return "function " + funcName +"()  \
			print=debuglog \
			" + code + " \
			end" ;
}

int LuaExpression::delays;

int lua_sleep(lua_State *L) {
	LuaExpression::delays = lua_tointeger(L, -1);      /* Get the single number arg */
    //printf("lua_sleep called %d s\n",LuaExpression::delays);
    return lua_yield(L,0);
}

int lua_debuglog(lua_State* L) {
	int nargs = lua_gettop(L);

	for (int i=1; i <= nargs; i++) {
		if (lua_isstring(L, i)) {
			/* Pop the next arg using lua_tostring(L, i) and do your print */
			if (LuaExpression::ActualExpression!=NULL)
			{
				string arg = lua_tostring(L, i);
				LuaExpression::ActualExpression->DebugLog(arg);
			}

		}
		else {
		/* Do something with non-strings if you like */
		}
	}
	return 0;
}

int lua_log(lua_State* L) {
    int nargs = lua_gettop(L);

    for (int i=1; i <= nargs; i++) {
        if (lua_isstring(L, i)) {
            /* Pop the next arg using lua_tostring(L, i) and do your print */
        	string arg = lua_tostring(L, i);
        	LuaExpression::ActualExpression->DebugLog(arg);
        	CLogger::GetLogger().Info("Expr. | %s",arg.c_str());
        }
        else {
        /* Do something with non-strings if you like */
        }
    }

    return 0;
}


bool LuaExpression::ForceEvaluate()
{
	STACK
	bool saveRunning = runningAllowed;
	runningAllowed = true;
	bool result = Evaluate();
	runningAllowed = saveRunning;
	result &= Evaluate();
	return result;
}

bool LuaExpression::Evaluate()
{
	STACK

	HisLock lock(evaluateMutex);

	if (inEvalFunc) return false;
	inEvalFunc = true;

	LuaExpression::ActualExpression = this;

	this->ReloadValues();
	if (!ExistsName(GetName()))
	{
		inEvalFunc = false;
		return false;
	}

	if (!running && runningAllowed)
	{
		LuaExpression::delays = -1;
		nextTime = 0;
		/* initialize Lua */
		L = luaL_newstate();                        /* Create Lua state variable */
		/* load Lua base libraries */
		luaL_openlibs(L);
		lua_register(L, "sleep", lua_sleep);
		lua_register(L, "delay", lua_sleep);
		lua_register(L, "log", lua_log);
		lua_register(L, "debuglog", lua_debuglog);

		string path = GetLuaFilesPath()+"/?.lua";
		this->setLuaPath(L,path.c_str());
		/* add code to function */
		string code = GetLuaCodeInFuncion("runExpression",GetFilePath(GetName()));

		/* load string and check syntax */
		if (luaL_dostring(L,code.c_str()))
		{
			string newEvaluateError = StringBuilder::Format("Error load Lua script: %s\n",lua_tostring(L, -1));
			if (newEvaluateError!=lastEvaluateError)
			{
				lastEvaluateError = newEvaluateError;
				logger.Error(lastEvaluateError.c_str());
			}
			inEvalFunc = false;
			return false;
		}

		/* load func name to top stack */
		lua_getglobal(L,"runExpression");

		running = true;
	}

	if (running)
	{
		//STACK_SECTION("running")
		if (runningAllowed)
		{
			uint64_t curTimeUs = HisDateTime::timeval_to_usec(HisDateTime::Now());

			if (curTimeUs >= nextTime)
			{
				SetGlobals(L);
				//STACK_SECTION("lua_resume")
				int status = lua_resume(L,NULL,0);


				if (status == LUA_YIELD)
				{
					lastEvaluateError.clear();
					if (LuaExpression::delays != -1)
					{
						nextTime = curTimeUs+LuaExpression::delays*1000000;
						running = true;
						//get global variables
						GetGlobals(L);
					}
					else
					{
						running = false;
						//get global variables
						GetGlobals(L);
						lua_close(L);
					}
				}
				else if (status == LUA_OK)
				{
					lastEvaluateError.clear();
					running = false;
					//get global variables
					GetGlobals(L);
					lua_close(L);
				}
				else
				{
					running = false;
					//get global variables
					//GetGlobals(L);
					string newEvaluateError = StringBuilder::Format("Error running Lua script: %s . Error no: %d, Stack: %s",GetName().c_str(), status, lua_tostring(L, -1));
					if (newEvaluateError != lastEvaluateError)
					{
						lastEvaluateError = newEvaluateError;
						logger.Error(lastEvaluateError.c_str());
					}
					lua_close(L);
				}
			}
		}
		else
		{
			//STACK_SECTION("lua_close")
			lua_close(L);
			running = false;
		}
	}
	inEvalFunc = false;
	LuaExpression::ActualExpression = NULL;
	return true;
}

string LuaExpression::GetName()
{
	return HisBase::GetName();
}

void LuaExpression::DebugLog(string ln)
{
	string msg = DateTime::Now().ToString();
	msg += " | ";
	msg += ln;
	logs.push_back(msg);

	if (logs.size() > MAX_LOGS)
		logs.erase(logs.begin());
}

void LuaExpression::OnValueChaned(ValueChangedEventArgs args)
{
	//Evaluate();
}

void LuaExpression::DoInternalSave(xmlNodePtr & node)
{
	STACK
	HisBase::DoInternalSave(node);
	SaveExpressionToFile();
	//pri loudovani si ulozim nazev
	oldName = GetName();
	xmlSetProp(node,BAD_CAST PROP_EXPRESSIONBODY, BAD_CAST luaContent.c_str());
	xmlSetProp(node,BAD_CAST PROP_DESCRIPTION, BAD_CAST description.c_str());
	xmlSetProp(node,BAD_CAST PROP_RUNNING, GetRunning() ? BAD_CAST "1" : BAD_CAST "0");
}

void LuaExpression::Start()
{
	if (runningAllowed)
		SetRunning(true);
	else
		SetRunning(false);
	started = true;
}

void LuaExpression::DoInternalLoad(xmlNodePtr & node)
{
	STACK
	HisBase::DoInternalLoad(node);
	LoadExpressionFromFile();
	xmlChar* prop;
	prop = xmlGetProp(node,BAD_CAST PROP_DESCRIPTION);
	if (prop!=NULL)
	{
		description = (const char*)prop;
		xmlFree(prop);
	}
	prop = xmlGetProp(node,BAD_CAST PROP_RUNNING);
	if (prop!=NULL)
	{
		string strrunning = (const char*)prop;
		if (strrunning=="1") runningAllowed = true;
		else runningAllowed = false;
		xmlFree(prop);
	}
	expressionRuntime->Add(this);
	//pri loudovani si ulozim nazev
	oldName = GetName();
}

const xmlChar* LuaExpression::GetNodeNameInternal()
{
	return /*BAD_CAST*/ NODE_EXPRESSION;
}

LuaExpression::~LuaExpression()
{
	STACK
	//SetRunning(false);
	GetFactory()->GetFile()->Delete(GetFilePath(GetName()));
}
