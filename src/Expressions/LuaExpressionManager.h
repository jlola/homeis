/*
 * LuaExpressionManager.h
 *
 *  Created on: Aug 6, 2013
 *      Author: root
 */

#ifndef LUAEXPRESSIONMANAGER_H_
#define LUAEXPRESSIONMANAGER_H_
#include <vector>
#include <iostream>
#include <algorithm>    // std::sort
#include <string>
#include <vector>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "LuaExpression.h"
#include "Common/HisCollectionBase.h"

#include "Devices/Folder/HisDevFolderRoot.h"

using namespace std;

class LuaExpressionManager : public HisCollectionBase
{
	xmlDocPtr doc;       /* document pointer */
	vector<LuaExpression*> expressions;
	string fileName;
public:
	LuaExpressionManager(string pFileName);
	//schould be unique name
	//bool Add(LuaExpression* newExpression);
};


#endif /* LUAEXPRESSIONMANAGER_H_ */
