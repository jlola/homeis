/*
 * User.h
 *
 *  Created on: 11. 10. 2018
 *      Author: pc
 */

#ifndef SRC_AUTH_USER_H_
#define SRC_AUTH_USER_H_

#include <string>
#include "HisBase.h"
#include "IUser.h"
#include "IHisDevFactory.h"

#define PROP_USERNAME BAD_CAST "UserName"
#define PROP_PASSWORDHASH BAD_CAST "PasswordHash"
#define PROP_FIRSTNAME BAD_CAST "FirstName"
#define PROP_LASTNAME BAD_CAST "LastName"

using namespace std;

#define NODE_USER "User"

class User : public HisBase, public IUser
{
	string passwordhash;
	string firstName;
	string lastName;
public:
	User(IHisDevFactory* factory, string userName);
	User(xmlNodePtr pnode, IHisDevFactory* factory);
	CUUID GetRecordId();
	string GetFirstName();
	string GetLastName();
	bool IsAdmin();
	void SetFirstName(string firstName);
	void SetLastName(string lastName);
	string sha256(const string str);
	virtual ~User();
	void DoInternalSave(xmlNodePtr & node);
	void DoInternalLoad(xmlNodePtr & node);
	const xmlChar* GetNodeNameInternal();
	bool IsPasswordValid(string passwordHash);
	bool SetPassword(string oldPassword, string newPassword, bool loggedAdmin);
	string GetUserName();
	void SetUserName(string userName);
	IHisBase* GetParent();
};

#endif /* SRC_AUTH_USER_H_ */
