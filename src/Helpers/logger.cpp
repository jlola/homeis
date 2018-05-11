/*
 * logger.cpp
 *
 *  Created on: Feb 17, 2013
 *      Author: root
 */

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <limits.h>
#include <unistd.h>
#include <stdarg.h>
#include <sstream>

#include "linuxcs.h"
#include "ScopedLock.h"
#include "File.h"
#include "Directory.h"
#include "PoppyDebugTools.h"
#include "logger.h"

using namespace std;
#define BUFFER_SIZE 2048

static CLogger logger;

CLogger::CLogger()
{
	logLevel = ELogLevel::ELogLevelTrace;
	InitializeCriticalSection(&cs);
}

ILogger & CLogger::GetLogger()
{
	return logger;
}

void CLogger::SetLogLevel(ELogLevel level)
{
	Info("Set LogLevel=%s",LogLevelToString(level).c_str());
	logLevel = level;
}

string CLogger::LogLevelToString(ELogLevel logLevel)
{
	switch(logLevel)
	{
	case ELogLevelError:
		return "Error";
	case ELogLevelInfo:
		return "Info";
	case ELogLevelTrace:
	default:
		return "Trace";
	}
}

ELogLevel CLogger::GetLogLevel()
{
	return logLevel;
}

std::string CLogger::getStrTime()
{
  char      tmpStr[200];
  //time_t    curtime;
  struct tm *loctime;

  std::string line;

  timeval tv;
  gettimeofday(&tv, 0);
  loctime = localtime(&tv.tv_sec);

  sprintf(tmpStr,"%02d.%02d.%04d %02d:%02d:%02d.%03d",loctime->tm_mday,loctime->tm_mon+1,loctime->tm_year+1900, loctime->tm_hour,loctime->tm_min,loctime->tm_sec,(int)tv.tv_usec);
  //fprintfMulti( inExtraStream, inStdStream, "%s [%d]: ", tmpStr, LOW_platformMisc::getThreadID());
  line = tmpStr;

  return line;
}

std::string CLogger::getStrDate()
{
  char      tmpStr[200];
  //time_t    curtime;
  struct tm *loctime;

  std::string line;

  timeval tv;
  gettimeofday(&tv, 0);
  loctime = localtime(&tv.tv_sec);

  sprintf(tmpStr,"%04d%02d%02d",loctime->tm_year+1900,loctime->tm_mon+1,loctime->tm_mday);

  line = tmpStr;

  return line;
}

void CLogger::WriteToFile(string subDir,string filestr,string line)
{
	string path = file.getexepath();
	path += "/" + subDir;

	if (!directory.Exists(path)) {
	    directory.Create(path);
	}

	path += "/" + filestr;

	FILE* fp = fopen(path.c_str(), "a");
	if (fp == NULL) {
	 printf("I couldn't open %s for writing.\n",filestr.c_str());
	 return;
	}

	fprintf(stderr,line.c_str());
	fflush(stderr);
	fprintf(fp,line.c_str());

	fflush(fp);

	fclose(fp);
}

void CLogger::Log(string type, const char * text, va_list args)
{
	ScopedLock lock(cs);

	char buffer[BUFFER_SIZE];
	vsnprintf (buffer,sizeof(buffer),text,args);

	string line = getStrTime();
	line += "|" + type;
	line += "|";
	line += buffer;
	line += "\n";
	WriteToFile("Log","homeisLog"+  getStrDate() + ".txt",line);
}

void CLogger::Info(const char * text, ...)
{
	if (logLevel>=ELogLevel::ELogLevelInfo)
	{
		va_list args;
		va_start (args, text);
		logger.Log("INFO",text,args);
		va_end (args);
	}
}

void CLogger::Trace(const char * text, ...)
{
	STACK
	if (logLevel>=ELogLevel::ELogLevelTrace)
	{
		va_list args;
		va_start (args, text);
		logger.Log("TRACE",text,args);
		va_end (args);
	}
}

void CLogger::Error(const char * text, ...)
{
	STACK
	if (logLevel>=ELogLevel::ELogLevelError)
	{
		va_list args;
		va_start (args, text);
		logger.Log("ERROR",text,args);
		va_end (args);
	}
}
