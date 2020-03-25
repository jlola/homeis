/*
 * StringBuilder.h
 *
 *  Created on: Sep 11, 2013
 *      Author: root
 */

#ifndef STRINGBUILDER_H_
#define STRINGBUILDER_H_

#include <string>
#include <string.h>
#include <stdarg.h>
#include <vector>

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

	static string join(vector<string> v, string separator)
	{
		string result;
		for(size_t i=0;i<v.size();i++)
		{
			result += Format("%s%s",v[i].c_str(),i<v.size()-1?separator.c_str():"");
		}
		return result;
	}

	static string join(vector<uint8_t> v, string separator)
		{
			string result;
			for(size_t i=0;i<v.size();i++)
			{
				result += Format("%02X%s",v[i],i<v.size()-1?separator.c_str():"");
			}
			return result;
		}

	static std::vector<std::string> split(const std::string &text, char sep) {
		std::vector<std::string> tokens;
		std::size_t start = 0, end = 0;
		while ((end = text.find(sep, start)) != std::string::npos) {
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
		}
		tokens.push_back(text.substr(start));
		return tokens;
	}
};

#endif /* STRINGBUILDER_H_ */
