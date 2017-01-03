/*
 * converter.h
 *
 *  Created on: Mar 1, 2013
 *      Author: root
 */

#ifndef CONVERTER_H_
#define CONVERTER_H_

#include <stdint.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <cctype>
#include <wchar.h>


using namespace std;

class Converter
{
	typedef unsigned char base;

public:
	static string strtoupper(string str)
	{
	    int leng=str.length();
	    for(int i=0; i<leng; i++)
	        if (97<=str[i]&&str[i]<=122)//a-z
	            str[i]-=32;
	    return str;
	}
	static string strtolower(string str)
	{
	    int leng=str.length();
	    for(int i=0; i<leng; i++)
	        if (65<=str[i]&&str[i]<=90)//A-Z
	            str[i]+=32;
	    return str;
	}

	static std::vector<uint8_t> stobytes(std::string s)
	{
		string strbyte;
		std::vector<uint8_t> result;

		for(uint16_t i=0; i<s.size();i+=2)
		{
			strbyte.clear();
			strbyte.push_back(s[i]);
			strbyte.push_back(s[i+1]);
			uint8_t byte = stoui(strbyte,16);
			result.insert(result.begin(),byte);
		}
		return result;
	}

	static long stoi(std::string s)
	{
		return std::atol( s.c_str() );
	}

	static unsigned long stoui(std::string s, base b = 10)
	{
		std::string Mask = "0123456789ABCDEF";
		b = b > 1 && b <= 16 ? b : 10;
		unsigned long no = 0;
		for (uint16_t i = 0, pos = 0; i < s.length(); i++)
			no = (pos = Mask.find(toupper(s[i]))) >= 0 ? b * no + pos : no;
		return no;
	}

	static unsigned long stoi(std::wstring s, base b = 10)
	{
		return wcstol (s.c_str(),NULL,10);
	}

	static std::string itos(int i, base b = 10)
	{
		std::string Mask = "0123456789ABCDEF";
		b = b > 1 && b <= 16 ? b : 10;
		std::string s = "";
		return (s += i < b ? "" : itos(i / b, b)) + Mask[i % b];
	}


};

#endif /* CONVERTER_H_ */
