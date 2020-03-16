/*
 * ExpressionsTest.cpp
 *
 *  Created on: 13. 3. 2017
 *      Author: pc
 */

#ifdef HOMEISTEST

#include <vector>
#include "TestConfig.h"
#include "HomeIsConfig.h"
#include "HomeIsServer.h"
#include "StringBuilder.h"
#include "homeis.h"
#include "gtest/gtest.h"
#include "Client.h"
#include "DevicesAtom.h"
#include "Tests/API/ExpressionsTest.h"

#include "ModbusProvider.h"


namespace AF {

ExpressionsTest::ExpressionsTest():
	homeisStarter(NULL),
	client(NULL),
	expressionAtom(NULL),
	usersAtom(NULL)
{
}

void ExpressionsTest::SetUp()
{
	homeisStarter = new HomeIsStarter();
	homeisStarter->Start();
	client = &homeisStarter->GetClient();
	expressionAtom = new ExpressionAtom(homeisStarter);
	usersAtom = new UsersAtom(homeisStarter);
	usersAtom->LoginUser("admin","admin","sessionId",hashPassword);
}

TEST_F(ExpressionsTest,AddTagToExpressionThenRemove)
{
	DevicesAtom devAtom(*homeisStarter);
	Document devjson = devAtom.CreateDevice("testDevice");
	string strdevid =  devjson[JSON_ID].GetString();
	CUUID devId = CUUID::Parse(strdevid);

	Document tagdoc = devAtom.CreateTag(devId.ToString(),EDataType::Int,"tagtest","","");
	string strtagId = tagdoc[JSON_ID].GetString();

	string parentid="b004782a-ff08-4aee-b459-aa0a2d19ac0d";
	string name = "newExpression";
	string expression = "if(1== 1) then test=1 end";
	string nodeName = "expression";
	Document exprjson = expressionAtom->CreateExpression(parentid,false,name,"",expression,nodeName,hashPassword);
	string strExprId = exprjson[JSON_ID].GetString();
	expressionAtom->AddTagToExpression(strExprId,strtagId,hashPassword);
	Document exprjsonAddedTag = expressionAtom->GetExpression(strExprId);
	int tagsCount = exprjsonAddedTag[JSON_TAGS].Size();
	ASSERT_EQ(1,tagsCount);
	expressionAtom->RemoveTagFromExpression(strExprId,strtagId,"admin",hashPassword);
	Document exprjsonRemovedTag = expressionAtom->GetExpression(strExprId);
	tagsCount = exprjsonRemovedTag[JSON_TAGS].Size();
	ASSERT_EQ(0,tagsCount);
}

TEST_F(ExpressionsTest,ModbusReadTest)
{
	std::string reqest = StringBuilder::Format("api/modbus/registers/%s/1/0/52",
			homeisStarter->GetModbusConfig().Name.c_str());
	string response;
	long http_code;
	CURLcode cresp = client->Get(reqest,response,http_code);
	ASSERT_EQ(cresp, CURLE_OK);
}

TEST_F(ExpressionsTest,CreateExpressionTest)
{
	string parentid="b004782a-ff08-4aee-b459-aa0a2d19ac0d";
	string name = "newExpression";
	string expression = "if(1== 1) then test=1 end";
	string nodeName = "expression";
	expressionAtom->CreateExpression(parentid,false,name,"",expression,nodeName,hashPassword);
	string exprefile = StringBuilder::Format("%s/Expressions/%s.lua",file.getexepath().c_str(),"newExpression");
	ASSERT_EQ(file.Exists(exprefile), true);
	if (file.Exists(exprefile))
		file.Delete(exprefile);
}


void ExpressionsTest::TearDown()
{
	delete expressionAtom;
	delete usersAtom;
	string foldersfile = StringBuilder::Format("%s/folders.xml",file.getexepath().c_str());
	if (file.Exists(foldersfile))
		file.Delete(foldersfile);
	string devicesfile = StringBuilder::Format("%s/devices.xml",file.getexepath().c_str());
	if (file.Exists(devicesfile))
		file.Delete(devicesfile);

	delete homeisStarter;
}

ExpressionsTest::~ExpressionsTest() {
}
} /* namespace AF */

#endif
