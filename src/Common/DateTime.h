/*
 * DateTime.h
 *
 *  Created on: 4. 6. 2017
 *      Author: pc
 */

#ifndef SRC_COMMON_DATETIME_H_
#define SRC_COMMON_DATETIME_H_

#include <string>
#include <time.h>

class DateTime
{
	time_t _time;
public:
	DateTime();

	DateTime(tm tmstruct);

	DateTime(time_t timet);

	DateTime(const DateTime & time);

	DateTime(std::string strTime);

	bool operator < (const DateTime& dt_);

	bool operator > (const DateTime& dt_);

	bool operator == (const DateTime& dt_);

	bool operator != (const DateTime& dt_);

	double operator - (const DateTime& dt_);

	DateTime operator = (const DateTime& dt_);

	static DateTime Now();

	//%Y-%m-%d %H:%M:%S
	std::string ToString();

	static DateTime Parse(std::string strTime);

	static bool TryParse(std::string strTime, DateTime & dt);

	virtual ~DateTime();
};

#endif /* SRC_COMMON_DATETIME_H_ */
