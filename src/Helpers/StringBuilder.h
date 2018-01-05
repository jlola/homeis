/*
 * StringBuilder.h
 *
 *  Created on: Sep 11, 2013
 *      Author: root
 */

#ifndef STRINGBUILDER_H_
#define STRINGBUILDER_H_

#include <string.h>
#include <stdarg.h>

#define MAX_BUFFER 5000

using namespace std;

class StringBuilder
{
public:
	static string Format ( const char *fmt, ... )
	{
	    char textString[MAX_BUFFER] = {'\0'};

	    // -- empty the buffer properly to ensure no leaks.
	    memset(textString, '\0', sizeof(textString));

	    va_list args;
	    va_start ( args, fmt );
	    vsnprintf ( textString, MAX_BUFFER, fmt, args );
	    va_end ( args );
	    std::string retStr = textString;
	    return retStr;
	}
};

#endif /* STRINGBUILDER_H_ */
