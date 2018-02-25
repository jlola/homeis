/*
 * Base64.h
 *
 *  Created on: 28. 1. 2018
 *      Author: pc
 */

#ifndef SRC_COMMON_BASE64_H_
#define SRC_COMMON_BASE64_H_

#include <string>

class Base64 {
	std::string b64encode(const void* data, const size_t len);
	std::string b64decode(const void* data, const size_t len);
public:
	std::string b64encode(const std::string& str64);
	std::string b64decode(const std::string& str64);
	Base64();
	virtual ~Base64();
};

#endif /* SRC_COMMON_BASE64_H_ */
