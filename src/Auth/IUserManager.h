/*
 * IUserManager.h
 *
 *  Created on: 11. 10. 2018
 *      Author: pc
 */

#ifndef SRC_AUTH_IUSERMANAGER_H_
#define SRC_AUTH_IUSERMANAGER_H_

#include <string>
#include "CUUID.h"
#include "IUser.h"
#include "IHisDevFactory.h"
#include "Session.h"

using namespace std;

class IUserManager
{
public:
	virtual bool Authentize(string userName, string password, string sessionId, string ip, string & sessionHash,DateTime now)=0;
	virtual Session* AuthorizeSession(string sessionHash,string ip,DateTime now)=0;
	virtual bool LogSessionOut(string sessionHash, string ip)=0;
	virtual IUser* FindUser(CUUID userid)=0;
	virtual IUser* FindByUserName(string userName)=0;
	virtual IUser* GetUser(size_t index)=0;
	virtual bool IsUserLogged(IUser* user)=0;
	virtual size_t Size()=0;
	virtual IUser* GetAdmin()=0;
	virtual void Add(IUser* user)=0;
	virtual void Save()=0;
	virtual void Load(IHisDevFactory* factory)=0;
	virtual bool Delete(IUser* user)=0;
	virtual ~IUserManager(){}
};



#endif /* SRC_AUTH_IUSERMANAGER_H_ */
