/*
 * UsersTests.cpp
 *
 *  Created on: 10. 11. 2018
 *      Author: pc
 */

#ifdef HOMEISTEST

#include "CUUID.h"
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

TEST_F(UsersTests,ChangeUserAttributesSuccess)
{
	string sessionHash;
	auto http_code = usersAtom->LoginUser(ADMIN_USERNAME,"admin","sessionid",sessionHash);
	ASSERT_EQ(MHD_HTTP_OK, http_code);
	auto doc = usersAtom->FindUser(ADMIN_USERNAME);
	string strUserId = doc[JSON_ID].GetString();
	//CUUID userId = CUUID::Parse(strUserId);
	usersAtom->UpdateUser(strUserId,ADMIN_USERNAME,"adminFirstName","adminLastName",sessionHash);
	auto doc2 = usersAtom->FindUser(ADMIN_USERNAME);
	string userName = doc2[JSON_LASTNAME].GetString();
	ASSERT_EQ("adminLastName",userName);
}

TEST_F(UsersTests,ChangeUserPassword)
{
	string sessionHash;
	usersAtom->LoginUser("admin","admin","sessionid",sessionHash);
	Document jsondoc = usersAtom->CreateUser("userName","firstName","lastName",sessionHash);
	string userId = jsondoc[JSON_ID].GetString();
	usersAtom->UpdatePassword(userId,"","newpassword",sessionHash);
}

TEST_F(UsersTests,LoginAdminUserWrongPasswordFailed)
{
	string response;
	string wrongpassword = "admin1";
	auto http_code = usersAtom->LoginUser("admin",wrongpassword,"sessionid",response);
	ASSERT_EQ(MHD_HTTP_UNAUTHORIZED, http_code);
}

TEST_F(UsersTests,LoginAdminThenLogout)
{
	string sessionHash;
	auto http_code = usersAtom->LoginUser("admin","admin","sessionid",sessionHash);
	usersAtom->LogOutUser(sessionHash);
	ASSERT_EQ(MHD_HTTP_OK, http_code);
}

TEST_F(UsersTests,CreateUserLoginUserAndTryToDeleteNotSuccessLogoutThenDeleteSuccess)
{
	string sessionHash;
	usersAtom->LoginUser("admin","admin","sessionid",sessionHash);
	Document userdoc = usersAtom->CreateUser("userName","firstName","lastName",sessionHash);
	string userid = userdoc[JSON_ID].GetString();
	string sessionHash2;
	string pass = "pass";
	usersAtom->UpdatePassword(userid,"",pass,sessionHash);
	usersAtom->LoginUser("userName",pass,"sessionid2",sessionHash2);
	//should be not success because user is loggedin
	bool result = usersAtom->DeleteUser(userid,"admin",sessionHash);
	ASSERT_FALSE(result);
	usersAtom->LogOutUser(sessionHash2);
	result = usersAtom->DeleteUser(userid,"admin",sessionHash);
	ASSERT_TRUE(result);
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
