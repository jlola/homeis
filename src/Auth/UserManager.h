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


using namespace std;

class UserManager : protected HisCollectionBase<IUser>, public IUserManager
{
	IHisDevFactory* factory;
	vector<Session*> sessions;
	ILogger & logger;
	double expireTimeSeconds;

public:
	UserManager(string fileName,int expireSessionSec);
	void RecycleOldSessions(DateTime now);
	void AddToSessions(IUser* user, string hash, string ip,DateTime now);
	Session* FindSession(string sessionHash, string ip);
	/**/
	bool Authentize(string userName, string password, string sessionId,string ip, string & sessionHash,DateTime now);
	bool AuthorizeSession(string sessionHash,string ip,DateTime now);
	string GetRootNodeName();
	void Load(IHisDevFactory* factory);
	IUser* LoadItem(xmlNodePtr cur);
	int FindUserIndex(string userName);
	size_t FindUser(CUUID userid);
	IUser* FindByUserName(string userName);
	IUser* GetUser(size_t index);
	size_t Size();
	void Add(IUser* user);
	IUser* Delete(size_t index);
	void Save();
	virtual ~UserManager();
};

#endif /* SRC_AUTH_USERMANAGER_H_ */
