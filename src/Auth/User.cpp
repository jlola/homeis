/*
 * User.cpp
 *
 *  Created on: 11. 10. 2018
 *      Author: pc
 */
#include "openssl/sha.h"
#include "User.h"

User::User(xmlNodePtr node, IHisDevFactory* factory) :
	HisBase::HisBase(node,factory)
{

}

User::User(IHisDevFactory* factory, string userName) :
	HisBase::HisBase(factory)
{
	SetName(userName);
}

void User::DoInternalSave(xmlNodePtr & node)
{
	if (node==NULL)	node = xmlNewNode(NULL,this->GetNodeName());

	xmlSetProp(node, PROP_USERNAME,BAD_CAST GetUserName().c_str());
	xmlSetProp(node, PROP_PASSWORDHASH,BAD_CAST passwordhash.c_str());
	xmlSetProp(node, PROP_FIRSTNAME,BAD_CAST firstName.c_str());
	xmlSetProp(node, PROP_LASTNAME,BAD_CAST lastName.c_str());
}

void User::DoInternalLoad(xmlNodePtr & node)
{
	if (node!=NULL)
	{
		if (!xmlStrcmp(node->name,GetNodeName()))
		{
			string strtemp;
			xmlChar* prop;

			if (xmlHasProp(node,BAD_CAST PROP_PASSWORDHASH))
			{
				prop = xmlGetProp(node,BAD_CAST PROP_PASSWORDHASH);
				passwordhash = (const char*)prop;
				xmlFree(prop);
			}

			if (xmlHasProp(node,BAD_CAST PROP_FIRSTNAME))
			{
				prop = xmlGetProp(node,BAD_CAST PROP_FIRSTNAME);
				firstName = (const char*)prop;
				xmlFree(prop);
			}

			if (xmlHasProp(node,BAD_CAST PROP_LASTNAME))
			{
				prop = xmlGetProp(node,BAD_CAST PROP_LASTNAME);
				lastName = (const char*)prop;
				xmlFree(prop);
			}
		}
	}
}

string User::sha256(const string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

string User::GetUserName()
{
	return HisBase::GetName();
}
void User::SetUserName(string userName)
{
	HisBase::SetName(userName);
}
const xmlChar* User::GetNodeNameInternal()
{
	return (const xmlChar*)NODE_USER;
}

string User::GetFirstName()
{
	return firstName;
}

string User::GetLastName()
{
	return lastName;
}

void User::SetFirstName(string firstName)
{
	this->firstName = firstName;
}
void User::SetLastName(string lastName)
{
	this->lastName = lastName;
}

IHisBase* User::GetParent()
{
	return NULL;
}

bool User::IsPasswordValid(string passwordHash)
{
	return passwordHash==this->passwordhash;
}
void User::SetPassword(string password)
{
	this->passwordhash = sha256(password);
}

User::~User()
{
}

