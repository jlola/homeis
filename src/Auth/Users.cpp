/*
 * Users.cpp
 *
 *  Created on: 23. 11. 2018
 *      Author: pc
 */

#include "Users.h"

Users::Users(string fileName)
	: HisCollectionBase<IUser>::HisCollectionBase<IUser>(fileName),
	  factory(NULL)
{
}

string Users::GetRootNodeName()
{
	return "users";
}

int Users::FindUserIndex(string userName)
{
	for(size_t i=0;i<HisCollectionBase::Size();i++)
	{
		IHisBase* base = dynamic_cast<IHisBase*>(operator [](i));
		if (base->GetName()==userName)
			return i;
	}
	return -1;
}

size_t Users::FindUser(CUUID userid)
{
	for(size_t i=0;i<HisCollectionBase::Size();i++)
	{
		IHisBase* base = dynamic_cast<IHisBase*>(operator [](i));
		if (base->GetRecordId()==userid)
			return i;
	}
	return -1;
}

IUser* Users::GetUser(size_t index)
{
	if (index<Size())
		return this->operator [](index);
	return NULL;
}

IUser* Users::FindByUserName(string userName)
{
	for(size_t i=0;i<Size();i++)
	{
		if (operator[](i)->GetUserName()==userName)
		{
			return operator[](i);
		}
	}
	return NULL;
}

void Users::Load(IHisDevFactory* factory)
{
	if (factory == NULL)
		throw ArgumentNullException("factory");

	this->factory = factory;
	HisCollectionBase<IUser>::Load();

	IUser* userAdmin = FindByUserName(ADMIN_USERNAME);
	if (userAdmin==NULL)
	{
		userAdmin = new User(factory,ADMIN_USERNAME);
		userAdmin->SetPassword("",ADMIN_USERNAME,true);
		Add(userAdmin);
	}
}

IUser* Users::LoadItem(xmlNodePtr cur)
{
	return new User(cur,factory);
}

Users::~Users() {
	// TODO Auto-generated destructor stub
}

