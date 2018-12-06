/*
 * UserManager.h
 *
 *  Created on: 11. 10. 2018
 *      Author: pc
 */

#ifndef SRC_AUTH_USERMANAGER_H_
#define SRC_AUTH_USERMANAGER_H_

#include <HisCollectionBase.hpp>
#include <unordered_map>
#include <vector>
#include "IUserManager.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "IUser.h"
#include "IHisBase.h"
#include "logger.h"
#include "IHisDevFactory.h"
#include "Session.h"
#include "Users.h"

using namespace std;

class UserManager : public IUserManager
{
	Users users;
	IHisDevFactory* factory;
	vector<Session*> sessions;
	ILogger & logger;
	double expireTimeSeconds;
	IUser* admin;

public:
	UserManager(string fileName,int expireSessionSec);
	void RecycleOldSessions(DateTime now);
	int FindSessionIndex(string sessionHash, string ip);
	bool IsUserLogged(IUser* user);
	bool LogSessionOut(string sessionHash, string ip);
	void AddToSessions(IUser* user, string hash, string ip,DateTime now);
	Session* FindSession(string sessionHash, string ip);
	/**/
	IUser* GetAdmin();
	bool Authentize(string userName, string password, string sessionId,string ip, string & sessionHash,DateTime now);
	Session* AuthorizeSession(string sessionHash,string ip,DateTime now);
	void Load(IHisDevFactory* factory);
	int FindUserIndex(string userName);
	IUser* FindUser(CUUID userid);
	IUser* FindByUserName(string userName);
	IUser* GetUser(size_t index);
	size_t Size();
	void Add(IUser* user);
	bool Delete(IUser* user);
	void Save();
	virtual ~UserManager();
};

#endif /* SRC_AUTH_USERMANAGER_H_ */
