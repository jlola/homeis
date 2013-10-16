/*
 * CUUID.h
 *
 *  Created on: May 17, 2013
 *      Author: root
 */

#ifndef CUUID_H_
#define CUUID_H_

#include "uuid.h"
#include <string>

class CUUID
{
	uuid_t uuid;
public:
	CUUID();
	CUUID(const CUUID &rhs);
	bool operator==(const CUUID& rhs);
	bool operator!=(const CUUID& rhs);
	CUUID& operator=(const CUUID& rhs);
	std::string ToString();
	static CUUID Parse(std::string struuid);
	static CUUID Empty();
};

#endif /* CUUID_H_ */
