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
#include "logger.h"

using namespace std;
using namespace PushFramework;
#define BUFFER_SIZE 2048

CLogger logger;

CRITICAL_SECTION CLogger::cs;

CLogger::CLogger()
{
	InitializeCriticalSection(&CLogger::cs);
}

void CLogger::Info(const FILE * stream)
{
	stringstream strstream;
	strstream << stream;
	Info(strstream.str().c_str());
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

void CLogger::WriteToFile(string subDir,string file,string line)
{
	string path = File::getexepath();
	path += "/" + subDir;

	if (!Directory::Exists(path)) {
	    Directory::Create(path);
	}

	path += "/" + file;

	FILE* fp = fopen(path.c_str(), "a");
	if (fp == NULL) {
	 printf("I couldn't open %s for writing.\n",file.c_str());
	 return;
	}

	fprintf(fp,line.c_str());

	fclose(fp);
}

void CLogger::Info(const char * text, ...)
{
	ScopedLock lock(CLogger::cs);

	va_list args;
	va_start (args, text);
	Info(text,args);
	va_end (args);
}

void CLogger::Info(const char * text, va_list args)
{
	//ScopedLock lock(CLogger::cs);

	char buffer[BUFFER_SIZE];
	vsnprintf (buffer,sizeof(buffer),text,args);

	string line = getStrTime();
	line += " | ";
	line += buffer;
	line += "\n";
	WriteToFile("Log","info.txt",line);
}

void CLogger::Error(const char * text, ...)
{
	ScopedLock lock(CLogger::cs);

	char buffer[BUFFER_SIZE];
	va_list args;
	va_start (args, text);
	vsnprintf (buffer,sizeof(buffer),text,args);
	va_end (args);

	string line = getStrTime();
	line += " | ";
	line += buffer;
	line += "\n";
	WriteToFile("Log","info.txt",line);
}

void CLogger::Fatal(const char * text, ...)
{
	ScopedLock lock(CLogger::cs);

	char buffer[BUFFER_SIZE];
	va_list args;
	va_start (args, text);
	vsnprintf (buffer,sizeof(buffer),text,args);
	va_end (args);

	string line = getStrTime();
	line += " | ";
	line += buffer;
	line += "\n";
	WriteToFile("Log","info.txt",line);
}
