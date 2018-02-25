/*
 * HisException.h
 *
 *  Created on: Sep 3, 2013
 *      Author: root
 */

#ifndef HISEXCEPTION_H_
#define HISEXCEPTION_H_

#include "converter.h"


class HisException : public std::exception
{
   std::string s;
public:
   HisException(std::string ss,std::string file,int line) : s(ss + " file: " + file + "line: " + Converter::itos(line,10)) {
   }
   HisException(HisException & inner,std::string ss,std::string file,int line) :
	   s(inner.s + "|"+ss + "file: " + file + "line: " + Converter::itos(line,10))
   {

   }
   ~HisException() throw () {} // Updated
   const char* what() const throw() { return s.c_str(); }
};

#define Exception(name) HisException(name,__FILE__,__LINE__)

#define ArgumentNullException(name) HisException("ArgumentNullException of " + std::string(name),__FILE__,__LINE__)

#endif /* HISEXCEPTION_H_ */
