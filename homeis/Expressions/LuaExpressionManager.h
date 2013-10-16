/*
 * LuaExpressionManager.h
 *
 *  Created on: Aug 6, 2013
 *      Author: root
 */

#ifndef LUAEXPRESSIONMANAGER_H_
#define LUAEXPRESSIONMANAGER_H_
#include <vector>
#include "LuaExpression.h"
#include "homeis/Devices/Folder/HisDevFolderRoot.h"

class LuaExpressionManager
{
	vector<LuaExpression*> expressions;
public:
	LuaExpressionManager(HisDevFolderRoot *rootFolder);
	void Load();
	void Save();
	LuaExpression* Find(std::string expressionName);
	//schould be unique name
	bool Add(LuaExpression* newExpression);
	LuaExpression* CreateExpression(string name);
};


#endif /* LUAEXPRESSIONMANAGER_H_ */
