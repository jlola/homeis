/*
 * ExpressionAtom.h
 *
 *  Created on: 11. 12. 2018
 *      Author: pc
 */

#ifndef TESTS_API_TESTSSUPPORT_ATOMS_EXPRESSIONATOM_H_
#define TESTS_API_TESTSSUPPORT_ATOMS_EXPRESSIONATOM_H_

#include "HomeIsStarter.h"


using namespace AF;

class ExpressionAtom {
	HomeIsStarter * homeisStarter;
	Client * client;
public:
	void RemoveTagFromExpression(string expressionId,string tagId,string user, string hashPassword);
	void AddTagToExpression(string expressionId,string tagId,string hashPassword);
	Document GetExpression(string strExprId);
	ExpressionAtom(HomeIsStarter * homeisStarter);
	Document CreateExpression(string parentId,bool running, string name,string description, string expression,string nodeName, string hashPassword);
	virtual ~ExpressionAtom();
};

#endif /* TESTS_API_TESTSSUPPORT_ATOMS_EXPRESSIONATOM_H_ */
