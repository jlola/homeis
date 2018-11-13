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

class IUser
{
public:
	virtual string GetUserName()=0;
	virtual void SetUserName(string userName)=0;
	virtual string GetFirstName()=0;
	virtual string GetLastName()=0;
	virtual void SetFirstName(string firstName)=0;
	virtual void SetLastName(string lastName)=0;
	virtual bool IsPasswordValid(string passwordHash)=0;
	virtual void SetPassword(string password)=0;
	virtual string sha256(const string str)=0;
	virtual ~IUser() {}
};

#endif /* SRC_AUTH_IUSER_H_ */
