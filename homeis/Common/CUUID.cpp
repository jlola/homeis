/*
 * CUUID.cpp
 *
 *  Created on: May 17, 2013
 *      Author: root
 */

#include "CUUID.h"

CUUID::CUUID()
{
	uuid_generate(uuid);
}

CUUID CUUID::Empty()
{
	CUUID uuido;
	for(uint8_t i=0;i<16;i++)
	{
		uuido.uuid[i] = 0x00;
	}
	return uuido;
}

CUUID::CUUID(const CUUID &rhs)
{
	uuid_copy(uuid,rhs.uuid);
}


bool CUUID::operator==(const CUUID& rhs)
{
	/* do actual comparison */
	return uuid_compare(uuid,rhs.uuid)==0 ? true : false;
}

bool CUUID::operator!=(const CUUID& rhs)
{
	/* do actual comparison */
	return uuid_compare(uuid,rhs.uuid)==0 ? false : true;
}

CUUID& CUUID::operator=(const CUUID & rhs)
{
	uuid_copy(uuid,rhs.uuid);
	return *this;
}

std::string CUUID::ToString()
{
	char charuuid[36];
	uuid_unparse(uuid,charuuid);
	return charuuid;
}

CUUID CUUID::Parse(std::string struuid)
{
	CUUID result;
	if (uuid_parse(struuid.c_str(),result.uuid)==-1)
		throw "CUUID::Parse | Error parse id";
	return result;
}
