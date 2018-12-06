/*
 * Session.h
 *
 *  Created on: 26. 10. 2018
 *      Author: pc
 */

#ifndef SRC_AUTH_SESSION_H_
#define SRC_AUTH_SESSION_H_

#include "IUser.h"

class Session
{
	IUser* user;
	string ip;
	string sessionHash;
	DateTime date;
public:
	Session(IUser* user, string ip,string sessionHash,DateTime now);
	IUser* GetUser();
	string GetIp();
	string GetSessionHash();
	DateTime GetDate();
	void ResetDate(DateTime now);
	virtual ~Session();
};

#endif /* SRC_AUTH_SESSION_H_ */
