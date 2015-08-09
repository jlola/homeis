/*
 * DateTime.h
 *
 *  Created on: Mar 15, 2013
 *      Author: root
 */

#ifndef DATETIME_H_
#define DATETIME_H_

#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

class HisDateTime
{
public:
	static timeval Now()
	{
		timeval tv;
		gettimeofday(&tv, 0);
		return tv;
	}

	static uint64_t timeval_to_usec( const struct timeval tv )
	{
	    return( (uint64_t)tv.tv_sec * 1000000 + tv.tv_usec ) ;
	}

	static timeval usec_to_timeval( int64_t usec)
	{
		timeval tv;
	    tv.tv_sec = usec / 1000000 ;
	    tv.tv_usec = usec % 1000000 ;
	    return tv ;
	}

	/*
	 * return timediff in miliseconds
	 */
	static long timevaldiff(struct timeval *starttime, struct timeval *finishtime)
	{
	  long msec;
	  msec=(finishtime->tv_sec-starttime->tv_sec)*1000;
	  msec+=(finishtime->tv_usec-starttime->tv_usec)/1000;
	  return msec;
	}
};

#endif /* DATETIME_H_ */
