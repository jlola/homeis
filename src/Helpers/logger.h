/*
 * helpers.h
 *
 *  Created on: Feb 17, 2013
 *      Author: root
 */

#ifndef HELPERS_H_
#define HELPERS_H_

#include <string>
#include "linuxcs.h"
#include "ILogger.h"
#include "File.h"
#include "Directory.h"
using namespace std;




class CLogger : public ILogger
{
private:
	Directory directory;
	File file;
	ELogLevel logLevel;
	CRITICAL_SECTION cs;
	void WriteToFile(string subDir,string file,string line);
	std::string getStrDate();
	void Log(string type, const char * text, va_list args);
	std::string getStrTime();
	string LogLevelToString(ELogLevel logLevel);
public:
	CLogger();
	void SetLogLevel(ELogLevel level);
	ELogLevel GetLogLevel();
	static ILogger & GetLogger();
	void Info(const char * text, ...);
	void Trace(const char * text, ...);
	void Error(const char * text, ...);
	virtual ~CLogger();
};

#endif /* HELPERS_H_ */
