/*
 * UserManager.cpp
 *
 *  Created on: 11. 10. 2018
 *      Author: pc
 */

#include "UserManager.h"
#include "PoppyDebugTools.h"

#include "User.h"

UserManager::UserManager(string fileName, int expireSessionSec)
	: users(fileName),
	  factory(NULL),
	  logger(CLogger::GetLogger()),
	  admin(NULL)
{
	expireTimeSeconds = expireSessionSec;
}

void UserManager::Load(IHisDevFactory* factory)
{
	this->factory = factory;
	users.Load(factory);
	admin = FindByUserName(ADMIN_USERNAME);
}

void UserManager::Add(IUser* user)
{
	if (user==NULL)
		throw ArgumentNullException("user");

	if (FindUser(user->GetRecordId())==NULL)
		users.Add(user);
	else
		throw MException("User already exists");
}

bool UserManager::Delete(IUser* user)
{
	if (user==NULL)
		throw ArgumentNullException("user");

	if (IsUserLogged(user))
	{
		CLogger::GetLogger().Error("User %s is logged and can not be deleted.",user->GetUserName().c_str());
		return false;
	}

	int index = users.FindIndex(user->GetRecordId());
	users.Delete(index);

	return true;
}

bool UserManager::IsUserLogged(IUser* user)
{
	for(int i=sessions.size()-1;i>=0;i--)
	{
		if (sessions[i]->GetUser()->GetRecordId()==user->GetRecordId())
			return true;
	}
	return false;
}

IUser* UserManager::FindByUserName(string userName)
{
	return users.FindByUserName(userName);
}

size_t UserManager::Size()
{
	return users.Size();
}

bool UserManager::Authentize(string userName, string password, string sessionId, string ip, string & sessionHash, DateTime now)
{
	 size_t index = FindUserIndex(userName);
	 if (index>=0)
	 {
		 IUser* user = this->users[index];
		 if (user!=NULL)
		 {
			 string passwordHash = user->sha256(password);
			 if (user->IsPasswordValid(passwordHash))
			 {
				 string hash = userName + sessionId;
				 sessionHash = user->sha256(hash);
				 AddToSessions(user, sessionHash, ip,now);
				 return true;
			 }
		 }
	 }
	 return false;
}

bool UserManager::LogSessionOut(string sessionHash, string ip)
{
	auto sessionIndex = FindSessionIndex(sessionHash,ip);
	if (sessionIndex>=0)
	{
		sessions.erase(sessions.begin()+sessionIndex);
		return true;
	}
	return false;
}

void UserManager::AddToSessions(IUser* user, string hash, string ip, DateTime now)
{
	auto session = new Session(user, ip, hash, now);
	sessions.push_back(session);
	if (sessions.size()>10)
		sessions.erase(sessions.end());
}

void UserManager::RecycleOldSessions(DateTime now)
{
	for(int i=sessions.size()-1;i>=0;i--)
	{
		DateTime addDate = sessions[i]->GetDate();
		double addTimeSec = now - addDate;
		if (addTimeSec > expireTimeSeconds)
		{
			Session* sess = sessions[i];
			sessions.erase(sessions.begin()+i);
			delete(sess);
		}
	}
}

int UserManager::FindSessionIndex(string sessionHash, string ip)
{
	if (sessionHash.empty())
		return -1;

	auto funcSearch = [ip,sessionHash](Session* obj){
					return obj->GetSessionHash() == sessionHash && obj->GetIp()==ip;
			};
	std::vector<Session*>::iterator it = std::find_if(sessions.begin(), sessions.end(),funcSearch);

	if (it == sessions.end())
	{
	  return -1;
	}
	else
	{
	  return std::distance(sessions.begin(), it);
	}
}

Session* UserManager::FindSession(string sessionHash, string ip)
{
	int index =  FindSessionIndex(sessionHash,ip);
	//return if iterator points to end or not. It points to end then it means element
	// does not exists in list
	if (index >= 0)
	{
		return sessions[index];
	}
	return NULL;
}

/* remove all expired sessions
 * find session by ip and sessionhash
 * if is ession finded there is reset session time
 * */
Session* UserManager::AuthorizeSession(string sessionHash, string ip,DateTime now)
{
	RecycleOldSessions(now);

	auto session = FindSession(sessionHash,ip);
	if (session!=NULL)
	{
		session->ResetDate(now);
		return session;
	}
	return NULL;
}

IUser* UserManager::GetAdmin()
{
	return admin;
}

int UserManager::FindUserIndex(string userName)
{
	for(size_t i=0;i<users.Size();i++)
	{
		IHisBase* base = dynamic_cast<IHisBase*>(users[i]);
		if (base->GetName()==userName)
			return i;
	}
	return -1;
}

IUser* UserManager::FindUser(CUUID userid)
{
	for(size_t i=0;i<users.Size();i++)
	{
		if (users[i]->GetRecordId()==userid)
			return users[i];
	}
	return NULL;
}

IUser* UserManager::GetUser(size_t index)
{
	if (index<users.Size())
		return users[index];
	return NULL;
}

void UserManager::Save()
{
	users.Save();
}

UserManager::~UserManager()
{

}

