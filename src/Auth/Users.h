/*
 * Users.h
 *
 *  Created on: 23. 11. 2018
 *      Author: pc
 */

#ifndef SRC_AUTH_USERS_H_
#define SRC_AUTH_USERS_H_

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
#include "User.h"

class Users : public HisCollectionBase<IUser>
{
	IHisDevFactory* factory;
public:
	Users(string fileName);
	string GetRootNodeName();
	void Load(IHisDevFactory* factory);
	size_t FindUser(CUUID userid);
	IUser* FindByUserName(string userName);
	int FindUserIndex(string userName);
	IUser* GetUser(size_t index);
	IUser* LoadItem(xmlNodePtr cur);
	virtual ~Users();
};

#endif /* SRC_AUTH_USERS_H_ */
