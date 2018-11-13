/*
 * DateTime.cpp
 *
 *  Created on: 4. 6. 2017
 *      Author: pc
 */

#include <Common/DateTime.h>
#include <stdexcept>

#define DATE_TIME_SYNTAX "%FT%T.00Z"

DateTime::DateTime() : _time(0)
{

}

DateTime::DateTime(tm tmstruct)
{
	_time = mktime(&tmstruct);
}

DateTime::DateTime(time_t timet)
{
	this->_time = timet;
}

DateTime::DateTime(const DateTime & time)
{
	this->_time = time._time;
}

DateTime::DateTime(std::string strTime)
{
	DateTime dt = DateTime::Parse(strTime);
	this->_time = dt._time;
}

//%Y-%m-%d %H:%M:%S
std::string DateTime::ToString()
{
	char buf[300];

	tm* timeptr;

	timeptr = localtime (&_time);
	//%Y-%m-%d %H:%M:%S
	//2012-04-23T18:25:43.511Z
	strftime(buf,sizeof(buf), DATE_TIME_SYNTAX,timeptr);

	return buf;
}

bool DateTime::TryParse(std::string strTime, DateTime & dt)
{
	tm result;
	if(strptime(strTime.c_str(), DATE_TIME_SYNTAX,&result) == NULL)
		 return false;
	dt._time = mktime(&result);
	return true;
}

DateTime DateTime::Parse(std::string strTime)
{
	tm result;
	if(strptime(strTime.c_str(), DATE_TIME_SYNTAX,&result) == NULL)
	     throw std::runtime_error("struct tm *timeptr;");
	DateTime dt;
	dt._time = mktime(&result);
	return dt;
}

DateTime DateTime::operator = (const DateTime& dt_)
{
	this->_time = dt_._time;
	return *this;
}

DateTime DateTime::Now()
{
	time_t now = time(0);
	DateTime times(now);
	return times;
}

bool DateTime::operator < (const DateTime& dt_)
{
	time_t thisTime = this->_time;
	time_t thatTime = dt_._time;
	return thisTime < thatTime;
}

bool DateTime::operator > (const DateTime& dt_)
{
	time_t thisTime = this->_time;
	time_t thatTime = dt_._time;
	return thisTime > thatTime;
}

bool DateTime::operator == (const DateTime& dt_)
{
	time_t thisTime = this->_time;
	time_t thatTime = dt_._time;
	return thisTime == thatTime;
}

bool DateTime::operator != (const DateTime& dt_)
{
	time_t thisTime = this->_time;
	time_t thatTime = dt_._time;
	return thisTime != thatTime;
}

double DateTime::operator - (const DateTime& dt_)
{
	return difftime(_time,dt_._time);
}

DateTime::~DateTime() {
	// TODO Auto-generated destructor stub
}
