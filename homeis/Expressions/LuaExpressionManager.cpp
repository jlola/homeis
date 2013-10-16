/*
 * LuaExpressionManager.cpp
 *
 *  Created on: Aug 6, 2013
 *      Author: root
 */

#include "LuaExpressionManager.h"

LuaExpressionManager::LuaExpressionManager(HisDevFolderRoot *rootFolder)
{

}

void LuaExpressionManager::Load()
{

}

void LuaExpressionManager::Save()
{

}

LuaExpression* LuaExpressionManager::CreateExpression(string name)
{
	for(size_t i=0;i<expressions.size();i++)
	{
		if (expressions[i]->GetName()==name) return NULL;
	}


}
