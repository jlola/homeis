/*
 * UserManagerTests.cpp
 *
 *  Created on: 19. 10. 2018
 *      Author: pc
 */

#ifdef HOMEISTEST

#include <Auth/UserManagerTests.h>
#include "Auth/User.h"
#include "Auth/UserManager.h"

using namespace fakeit;

UserManagerTests::UserManagerTests() {
	File file;
	usersxml = file.getexepath() + "/users.xml";
}

void UserManagerTests::SetUp()
{

}

void UserManagerTests::TearDown()
{
	File file;
	if (file.Exists(usersxml))
		file.Delete(usersxml);
}

TEST_F(UserManagerTests,CreatNewCollectionAddUserSaveUserThenLoad)
{
	Mock<IHisDevFactory> factoryMock;
	Mock<IUser> userMock;

	UserManager manager(usersxml,300);
	manager.Load(&factoryMock.get());
	string userName = "userName";
	IUser* user = new User(&factoryMock.get(),userName);

	manager.Add(user);
	manager.Save();
	ASSERT_EQ((size_t)2,manager.Size());
	auto success = manager.Delete(user);
	delete user;
	ASSERT_EQ((size_t)1,manager.Size());
	UserManager manager2(usersxml,300);
	manager2.Load(&factoryMock.get());
	ASSERT_EQ((size_t)1,manager.Size());
}

TEST_F(UserManagerTests,AddUserAndFindByNameAndCheckPassword)
{
	Mock<IHisDevFactory> factoryMock;
	Mock<IUser> userMock;

	UserManager manager(usersxml,300);
	manager.Load(&factoryMock.get());
	string userName = "userName";
	IUser* user = new User(&factoryMock.get(),userName);
	user->SetUserName("userName");
	user->SetPassword("","password",true);
	manager.Add(user);
	string sessionhash;
	string ip = "192.168.89.254";
	bool result = manager.Authentize(user->GetUserName(),"password", "sessionId",ip, sessionhash,DateTime::Now());
	ASSERT_TRUE(result);
	result = manager.AuthorizeSession(sessionhash,ip,DateTime::Now());
	ASSERT_TRUE(result);
}

TEST_F(UserManagerTests,AddUserAndRecycleSessionThenAuthorizeSessionFail)
{
	Mock<IHisDevFactory> factoryMock;
	Mock<IUser> userMock;
	int expireSessionMin = 5;
	int expireSessionSec = 60*expireSessionMin;
	UserManager manager(usersxml,expireSessionSec);
	manager.Load(&factoryMock.get());
	string userName = "userName";
	IUser* user = new User(&factoryMock.get(),userName);
	user->SetPassword("","password",true);
	manager.Add(user);
	string sessionhash;
	string ip = "192.168.89.254";
	tm stm;
	stm.tm_sec = 10;
	stm.tm_min = 10;
	stm.tm_hour = 9;
	stm.tm_year = 2017-1900;
	stm.tm_mon = 7-1;
	stm.tm_mday = 2;
	DateTime dtAuthentize(stm);
	stm.tm_min += 1;
	DateTime dtAuthorizePass(stm);
	stm.tm_min += expireSessionMin+1;
	DateTime dtAuthorizeExpired(stm);
	bool result = manager.Authentize(user->GetUserName(),"password", "sessionId",ip, sessionhash,dtAuthentize);
	ASSERT_TRUE(result);
	result = manager.AuthorizeSession(sessionhash,ip,dtAuthorizePass);
	ASSERT_TRUE(result);
	result = manager.AuthorizeSession(sessionhash,ip,dtAuthorizeExpired);
	ASSERT_FALSE(result);
}

UserManagerTests::~UserManagerTests() {

}

#endif
