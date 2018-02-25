/*
 * ILogger.h
 *
 *  Created on: 11. 2. 2018
 *      Author: pc
 */

#ifndef SRC_HELPERS_ILOGGER_H_
#define SRC_HELPERS_ILOGGER_H_

enum ELogLevel
{
	ELogLevelError,
	ELogLevelInfo,
	ELogLevelTrace
};

class CLogger;

class ILogger
{
public:
	virtual void SetLogLevel(ELogLevel level)=0;
	virtual ELogLevel GetLogLevel()=0;
	virtual void Info(const char * text, ...)=0;
	virtual void Trace(const char * text, ...)=0;
	virtual void Error(const char * text, ...)=0;
};



#endif /* SRC_HELPERS_ILOGGER_H_ */
