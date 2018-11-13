/*
 * Session.cpp
 *
 *  Created on: 26. 10. 2018
 *      Author: pc
 */

#include "Session.h"

Session::Session(IUser* user,string ip,string sessionHash,DateTime now) :
	user(user),ip(ip),sessionHash(sessionHash)
{
	date = now;
}

string Session::GetIp()
{
	return ip;
}

string Session::GetSessionHash()
{
	return sessionHash;
}

void Session::ResetDate(DateTime now)
{
	date = now;
}

DateTime Session::GetDate()
{
	return date;
}

Session::~Session()
{

}

