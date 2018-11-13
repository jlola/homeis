/*
 * UsersAtom.h
 *
 *  Created on: 9. 11. 2018
 *      Author: pc
 */

#ifndef TESTS_API_TESTSSUPPORT_ATOMS_USERSATOM_H_
#define TESTS_API_TESTSSUPPORT_ATOMS_USERSATOM_H_

#include "HomeIsStarter.h"
#include "fakeit.hpp"

using namespace fakeit;


class UsersAtom {
	Client& client;
public:
	UsersAtom(HomeIsStarter* homeisStarter);
	UsersAtom(Client & client);
	int LoginUser(string userName, string password, string sessionId,string & hashPassword);
	string CreateUser(string userName,string firstName,string lastName,string sessionHash);
	void DeleteUser(CUUID userId);
	Document GetUser(CUUID userId);
	Document GetUsers();
	virtual ~UsersAtom();
};

#endif /* TESTS_API_TESTSSUPPORT_ATOMS_USERSATOM_H_ */
