/*
 * IUser.h
 *
 *  Created on: 11. 10. 2018
 *      Author: pc
 */

#ifndef SRC_AUTH_IUSER_H_
#define SRC_AUTH_IUSER_H_

#include <string>
#include "IHisBase.h"

using namespace std;

#define ADMIN_USERNAME "admin"

class IUser
{
public:
	virtual bool IsAdmin()=0;
	virtual CUUID GetRecordId()=0;
	virtual string GetUserName()=0;
	virtual void SetUserName(string userName)=0;
	virtual string GetFirstName()=0;
	virtual string GetLastName()=0;
	virtual void SetFirstName(string firstName)=0;
	virtual void SetLastName(string lastName)=0;
	virtual bool IsPasswordValid(string passwordHash)=0;
	virtual bool SetPassword(string oldPassword, string newPassword, bool loggedAdmin)=0;
	virtual string sha256(const string str)=0;
	virtual ~IUser() {}
};

#endif /* SRC_AUTH_IUSER_H_ */
