/*
 * UsersTests.cpp
 *
 *  Created on: 10. 11. 2018
 *      Author: pc
 */

#ifdef HOMEISTEST

#include "UsersTests.h"

UsersTests::UsersTests()
{
	homeisStarter = NULL;
	usersAtom = NULL;
	File file;
	usersxml = file.getexepath() + "/users.xml";
}

void UsersTests::SetUp()
{
	homeisStarter = new HomeIsStarter();
	usersAtom = new UsersAtom(homeisStarter->GetClient());
	homeisStarter->Start();
}

void UsersTests::TearDown()
{
	delete usersAtom;
	usersAtom = NULL;
	delete homeisStarter;
	homeisStarter = NULL;

	File file;
	if (file.Exists(usersxml))
		file.Delete(usersxml);
}

TEST_F(UsersTests,LoginAdminUserSuccess)
{
	string sessionHash;
	auto http_code = usersAtom->LoginUser("admin","admin","sessionid",sessionHash);
	ASSERT_EQ(MHD_HTTP_OK, http_code);
	ASSERT_TRUE(sessionHash.size()>0);
}

TEST_F(UsersTests,LoginAdminUserWrongPasswordFailed)
{
	string response;
	auto http_code = usersAtom->LoginUser("admin","admin1","sessionid",response);
	ASSERT_EQ(MHD_HTTP_UNAUTHORIZED, http_code);
}

TEST_F(UsersTests,CreateUserReturnsSameUser)
{
	string sessionHash;
	usersAtom->LoginUser("admin","admin","sessionid",sessionHash);
	usersAtom->CreateUser("userName","firstName","lastName",sessionHash);
	auto doc = usersAtom->GetUsers();

	const Value& users = doc;
	ASSERT_EQ(users.Size(),(size_t)2);
	CUUID userId;
	for(SizeType ob = 0 ;ob < users.Size();ob++)
	{
		string userName = users[ob][JSON_USERNAME].GetString();
		if (userName=="userName")
		{
			string firstName = users[ob][JSON_FIRSTNAME].GetString();
			ASSERT_EQ("firstName",firstName);
			string struserId = users[ob][JSON_ID].GetString();
			userId = CUUID::Parse(struserId);
		}
	}
	auto user = usersAtom->GetUser(userId);
	string userName = user[JSON_USERNAME].GetString();
	ASSERT_EQ("userName",userName);
}

UsersTests::~UsersTests() {
}

#endif
