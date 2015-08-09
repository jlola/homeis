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

using namespace std;

class CLogger
{
private:
	static CRITICAL_SECTION cs;
	static void WriteToFile(string subDir,string file,string line);
	static std::string getStrTime();
	static std::string getStrDate();
public:
	CLogger();
	static void Info(const FILE * stream);
	static void Info(const char * text, ...);
	static void Info(const char * text, va_list args);
	static void Fatal(const char * text, ...);
	static void Error(const char * text, ...);
};

#endif /* HELPERS_H_ */
