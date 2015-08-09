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
#include "Helpers/logger.h"
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

#include "LuaExpression.h"



LuaExpression::LuaExpression(xmlNodePtr pnode,HisDevices* hisDevices,ExpressionRuntime *pExpressionRuntime) :
	HisBase(pnode)
{
	mutex = HisLock::CreateMutex();
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

LuaExpression::LuaExpression(HisDevFolder* pfolder,HisDevices* hisDevices, string expressionName,ExpressionRuntime *pExpressionRuntime) :
	HisBase()
{
	mutex = HisLock::CreateMutex();
	expressionRuntime = pExpressionRuntime;
	inEvalFunc = false;
	nextTime = 0;
	runningAllowed = false;
	if (pfolder==NULL)
	{
		CLogger::Fatal("LuaExpression | Constructor can not be called with empty folder");
		throw HisException("LuaExpression | Constructor can not be called with empty folder");
	}
	folder = pfolder;
	devices = hisDevices;
	SetName(expressionName);
	running = false;
	L = NULL;
	cL = NULL;
}

CUUID LuaExpression::GetRecordId()
{
	return HisBase::GetRecordId();
}

void LuaExpression::SetName(string name)
{
	if (oldName!=name)
	{
		if (!ExistsName(name))
			HisBase::SetName(name);
		else throw HisException("Expression name: " + name + "already exists");
	}
}

string LuaExpression::GetLastEvaluateError()
{
	return lastEvaluateError;
}

void LuaExpression::SetRunning(bool blRunning)
{
	HisLock lock(mutex);

	if (runningAllowed!=blRunning)
	{
		runningAllowed = blRunning;
		if (blRunning) {
			ReloadValues();
			//StartListening();
			expressionRuntime->Add(this);
		}
		else
		{
			//ukoncim vlakno
			expressionRuntime->Evaluate();
			expressionRuntime->Remove(this);
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
	std::string dir = File::getexepath() + "/"+ EXPRESSION_FOLDER;
	if (!Directory::Exists(dir))
	{
		Directory::Create(dir);
	}
}

void LuaExpression::SaveExpressionToFile()
{
	if (oldName!=GetName())
	{
		if (File::Exists(GetFileName(oldName)))
			File::Delete(GetFileName(oldName));
	}
	FILE* fp = fopen(GetFileName(GetName()).c_str(), "w+");
	if (fp == NULL) {
		string file = this->GetName()+".lua";
		printf("I couldn't open %s for writing.\n",file.c_str());
		CLogger::Error("I couldn't open %s for writing.\n",file.c_str());
		return;
	}

	fprintf(fp,luaContent.c_str());

	fclose(fp);
}

void LuaExpression::LoadExpressionFromFile()
{
	if (File::Exists(GetFileName(GetName())))
	{
		std::ifstream in(GetFileName(GetName()));

		stringstream buffer;
		buffer << in.rdbuf();

		luaContent = buffer.str();
	}
}

string LuaExpression::GetFileName(string pFileName)
{
	return File::getexepath() + "/"+ EXPRESSION_FOLDER + "/" + pFileName + ".lua";
}

bool LuaExpression::ExistsName(string pName)
{
	string path;
	path = GetFileName(pName);
	return File::Exists(path);
}



void LuaExpression::ReloadValues()
{
	values.clear();
	HisDevFolder* parent = dynamic_cast<HisDevFolder*>(GetParent());
	vector<HisDevValueId*> items = parent->GetItems<HisDevValueId>();
	for(size_t i=0;i<items.size();i++)
	{
		HisDevValueId* valueId = items[i];
		if (valueId!=NULL)
		{
			HisDevValueBase* value = devices->FindValue(valueId->GetDeviceValueId());
			if (value!=NULL) values.push_back(value);
		}
	}
}

size_t LuaExpression::GetCountInputs()
{
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

	for(size_t i=0;i<values.size();i++)
	{
		if (values[i]->GetDirection()==EHisDevDirection::Write||
			values[i]->GetDirection()==EHisDevDirection::ReadWrite)
		{
			switch(values[i]->GetDataType())
			{
				case EDataType::Int:
				{
					HisDevValue<int>* value = dynamic_cast<HisDevValue<int>*>(values[i]);

					lua_getglobal(L,value->GetPinName().c_str());
					int number = lua_tointeger(L,-1);
					value->SetValue(number);
					break;
				}
				case EDataType::Double:
				{
					HisDevValue<double>* value = dynamic_cast<HisDevValue<double>*>(values[i]);

					lua_getglobal(L,value->GetPinName().c_str());
					double number = lua_tonumber(L,-1);
					value->SetValue(number);
					break;
				}
				case EDataType::Uint:
				{
					HisDevValue<uint32_t>* value = dynamic_cast<HisDevValue<uint32_t>*>(values[i]);
					lua_getglobal(L,value->GetPinName().c_str());
					unsigned int number = lua_tointeger(L,-1);
					value->SetValue(number);
					break;
				}
				case EDataType::String:
				{
					HisDevValue<string>* value = dynamic_cast<HisDevValue<string>*>(values[i]);
					//lua_pushstring(L,value->GetValue().c_str());
					lua_getglobal(L,value->GetPinName().c_str());
					value->SetValue(lua_tostring(L,-1));
					break;
				}
				case EDataType::Bool:
				{
					HisDevValue<bool>* value = dynamic_cast<HisDevValue<bool>*>(values[i]);
					lua_getglobal(L,value->GetPinName().c_str());
					value->SetValue(lua_toboolean(L,-1));
					break;
				}
				case EDataType::Unknown:
					break;
			}
		}
	}
}

void LuaExpression::SetGlobals(lua_State* L)
{
	for(size_t i=0;i<values.size();i++)
	{
		switch(values[i]->GetDataType())
		{
			case EDataType::Int:
			{
				HisDevValue<int>* value = dynamic_cast<HisDevValue<int>*>(values[i]);
				lua_pushinteger(L,value->GetValue());
				lua_setglobal( L, value->GetPinName().c_str() );
				break;
			}
			case EDataType::Double:
			{
				HisDevValue<double>* value = dynamic_cast<HisDevValue<double>*>(values[i]);
				lua_pushnumber(L,value->GetValue());
				lua_setglobal( L, value->GetPinName().c_str() );
				break;
			}
			case EDataType::Uint:
			{
				HisDevValue<uint32_t>* value = dynamic_cast<HisDevValue<uint32_t>*>(values[i]);
				lua_pushinteger(L,value->GetValue());
				lua_setglobal( L, value->GetPinName().c_str() );
				break;
			}
			case EDataType::String:
			{
				HisDevValue<string>* value = dynamic_cast<HisDevValue<string>*>(values[i]);
				lua_pushstring(L,value->GetValue().c_str());
				lua_setglobal( L, value->GetPinName().c_str() );
				break;
			}
			case EDataType::Bool:
			{
				HisDevValue<bool>* value = dynamic_cast<HisDevValue<bool>*>(values[i]);
				lua_pushboolean(L,value->GetValue());
				lua_setglobal( L, value->GetPinName().c_str() );
				break;
			}
			case EDataType::Unknown:
				break;
		}
	}
}

void LuaExpression::open_libs(lua_State *L)
{
  luaopen_io(L);
  luaopen_base(L);
  luaopen_table(L);
  luaopen_string(L);
  luaopen_math(L);
  //luaopen_loadlib(L);
}

string LuaExpression::GetLuaCodeInFuncion(string funcName, string luaCodeFilePath)
{
	string code = File::ReadWholeFile(luaCodeFilePath);
	return "function " + funcName +"() \
			" + code + " \
			end";
}

int LuaExpression::delays;

int lua_sleep(lua_State *L) {
	LuaExpression::delays = lua_tointeger(L, -1);      /* Get the single number arg */
    //printf("lua_sleep called %d s\n",LuaExpression::delays);
    return lua_yield(L,0);
}

bool LuaExpression::ForceEvaluate()
{
	bool saveRunning = runningAllowed;
	runningAllowed = true;
	bool result = Evaluate();
	runningAllowed = saveRunning;
	result &= Evaluate();
	return result;
}

bool LuaExpression::Evaluate()
{
	if (inEvalFunc) return false;
	inEvalFunc = true;
	this->ReloadValues();

	if (!ExistsName(GetName()))
	{
		inEvalFunc = false;
		return false;
	}

	if (!running && runningAllowed)
	{
		lastEvaluateError.clear();
		LuaExpression::delays = -1;
		nextTime = 0;
		/* initialize Lua */
		L = luaL_newstate();                        /* Create Lua state variable */

		/* load Lua base libraries */
		luaL_openlibs(L);

		lua_register(L, "sleep", lua_sleep);
		lua_register(L, "delay", lua_sleep);

		/* add code to function */
		string code = GetLuaCodeInFuncion("runExpression",GetFileName(GetName()));

		/* load string and check syntax */
		if (luaL_dostring(L,code.c_str()))
		{
			lastEvaluateError = StringBuilder::Format("Error load Lua script: %s\n",lua_tostring(L, -1));
			CLogger::Error(lastEvaluateError.c_str());
			//throw HisException(lastEvaluateError);
			inEvalFunc = false;
			return false;
		}


		/**/
		//cL = lua_newthread(L);

		/* set golabl variables */
		SetGlobals(L);

		/* load func name to top stack */
		lua_getglobal(L,"runExpression");

		running = true;

		//call function
//		if (lua_pcall(L, 0, 0, 0))
//		{
//			lastEvaluateError = StringBuilder::Format("Error running Lua script: %s",  lua_tostring(L, -1));
//			CLogger::Error(lastEvaluateError.c_str());    /* Error out if Lua file has an error */
//			throw HisException(lastEvaluateError);
//		}
	}

	if (running)
	{
		if (runningAllowed)
		{
			uint64_t curTimeUs = HisDateTime::timeval_to_usec(HisDateTime::Now());

			if (curTimeUs >= nextTime)
			{
				//SetGlobals(L);
				int status = lua_resume(L,NULL,0);

				if (status == LUA_YIELD && LuaExpression::delays != -1)
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
		}
		else
		{
			lua_close(L);
			running = false;
		}
	}
	inEvalFunc = false;
	return true;
}

void LuaExpression::OnValueChaned(ValueChangedEventArgs args)
{
	//Evaluate();
}

void LuaExpression::StartListening()
{
//	HisDevFolder* folder = dynamic_cast<HisDevFolder*>(GetParent());
//	if (folder!=NULL)
//	{
//		vector<HisDevValueId*> ids = folder->GetItems<HisDevValueId>();
//		for(size_t i=0;i<ids.size();i++)
//		{
//			HisDevValueBase* value = devices->FindValue(ids[i]->GetDeviceValueId());
//			OnValueChangedDelegate delegate = OnValueChangedDelegate::from_method<LuaExpression, &LuaExpression::OnValueChaned>(this);
//			value->Register(delegate,this);
//		}
//
//	}
}

void LuaExpression::StopListening()
{
//	HisDevFolder* folder = dynamic_cast<HisDevFolder*>(GetParent());
//	if (folder!=NULL)
//	{
//		vector<HisDevValueId*> ids = folder->GetItems<HisDevValueId>();
//		for(size_t i=0;i<ids.size();i++)
//		{
//			HisDevValueBase* value = devices->FindValue(ids[i]->GetDeviceValueId());
//			value->UnRegister(this);
//		}
//	}
}

void LuaExpression::DoInternalSave(xmlNodePtr & node)
{
	HisBase::DoInternalSave(node);
	SaveExpressionToFile();
	//pri loudovani si ulozim nazev
	oldName = GetName();
	xmlSetProp(node,BAD_CAST PROP_EXPRESSIONBODY, BAD_CAST luaContent.c_str());
	xmlSetProp(node,BAD_CAST PROP_DESCRIPTION, BAD_CAST description.c_str());
	xmlSetProp(node,BAD_CAST PROP_RUNNING, GetRunning() ? BAD_CAST "1" : BAD_CAST "0");
}

void LuaExpression::DoInternalLoad(xmlNodePtr & node)
{
	HisBase::DoInternalLoad(node);
	LoadExpressionFromFile();
	xmlChar* prop;
	prop = xmlGetProp(node,BAD_CAST PROP_DESCRIPTION);
	if (prop!=NULL)
		description = (const char*)prop;
	xmlFree(prop);
	prop = xmlGetProp(node,BAD_CAST PROP_RUNNING);
	if (prop!=NULL)
	{
		string strrunning = (const char*)prop;
		if (strrunning=="1") SetRunning(true);
		else SetRunning(false);
	}
	xmlFree(prop);
	//pri loudovani si ulozim nazev
	oldName = GetName();
}

xmlChar* LuaExpression::GetNodeNameInternal()
{
	return /*BAD_CAST*/ NODE_EXPRESSION;
}

LuaExpression::~LuaExpression()
{
	SetRunning(false);
	File::Delete(GetFileName(GetName()));
}
