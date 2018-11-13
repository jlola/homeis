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
	: HisCollectionBase::HisCollectionBase(fileName),
	  factory(NULL),
	  logger(CLogger::GetLogger())
{
	expireTimeSeconds = expireSessionSec;
}

void UserManager::Load(IHisDevFactory* factory)
{
	this->factory = factory;
	HisCollectionBase<IUser>::Load();

	IUser* userAdmin = FindByUserName("admin");
	if (userAdmin==NULL)
	{
		userAdmin = new User(factory,"admin");
		userAdmin->SetPassword("admin");
		Add(userAdmin);
	}
}

void UserManager::Add(IUser* user)
{
	HisCollectionBase<IUser>::Add(user);
}

IUser* UserManager::Delete(size_t index)
{
	return HisCollectionBase<IUser>::Delete(index);
}

string UserManager::GetRootNodeName()
{
	return "users";
}

IUser* UserManager::FindByUserName(string userName)
{
	for(size_t i=0;i<Size();i++)
	{
		if (this->operator [](i)->GetUserName()==userName)
			return this->operator [](i);
	}
	return NULL;
}

size_t UserManager::Size()
{
	return HisCollectionBase::Size();
}

bool UserManager::Authentize(string userName, string password, string sessionId, string ip, string & sessionHash, DateTime now)
{
	 size_t index = FindUserIndex(userName);
	 if (index>=0)
	 {
		 IUser* user = this->operator [](index);
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

Session* UserManager::FindSession(string sessionHash, string ip)
{
	auto funcSearch = [ip,sessionHash](Session* obj){
				return obj->GetSessionHash() == sessionHash && obj->GetIp()==ip;
		};
	auto it = std::find_if(sessions.begin(), sessions.end(),funcSearch);
	//return if iterator points to end or not. It points to end then it means element
	// does not exists in list
	if (it != sessions.end())
	{
		return *it;
	}
	return NULL;
}

/* remove all expired sessions
 * find session by ip and sessionhash
 * if is ession finded there is reset session time
 * */
bool UserManager::AuthorizeSession(string sessionHash, string ip,DateTime now)
{
	RecycleOldSessions(now);

	auto session = FindSession(sessionHash,ip);
	if (session!=NULL)
	{
		session->ResetDate(now);
		return true;
	}
	return false;
}

IUser* UserManager::LoadItem(xmlNodePtr cur)
{
	return new User(cur,factory);
}

int UserManager::FindUserIndex(string userName)
{
	for(size_t i=0;i<HisCollectionBase::Size();i++)
	{
		IHisBase* base = dynamic_cast<IHisBase*>(operator [](i));
		if (base->GetName()==userName)
			return i;
	}
	return -1;
}

size_t UserManager::FindUser(CUUID userid)
{
	for(size_t i=0;i<HisCollectionBase::Size();i++)
	{
		IHisBase* base = dynamic_cast<IHisBase*>(operator [](i));
		if (base->GetRecordId()==userid)
			return i;
	}
	return -1;
}

IUser* UserManager::GetUser(size_t index)
{
	if (index<Size())
		return this->operator [](index);
	return NULL;
}

void UserManager::Save()
{
	HisCollectionBase::Save();
}

UserManager::~UserManager()
{

}

