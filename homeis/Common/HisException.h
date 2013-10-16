/*
 * HisException.h
 *
 *  Created on: Sep 3, 2013
 *      Author: root
 */

#ifndef HISEXCEPTION_H_
#define HISEXCEPTION_H_

class HisException : public std::exception
{
   std::string s;
public:
   HisException(std::string ss) : s(ss) {}
   HisException(HisException & inner,std::string ss) : s(inner.s + "|"+ss)
   {

   }
   ~HisException() throw () {} // Updated
   const char* what() const throw() { return s.c_str(); }
};

#endif /* HISEXCEPTION_H_ */
