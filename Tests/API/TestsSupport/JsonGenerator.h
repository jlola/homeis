/*
 * JsonGenerator.h
 *
 *  Created on: 16. 5. 2018
 *      Author: pc
 */

#ifndef TESTS_API_TESTSSUPPORT_JSONGENERATOR_H_
#define TESTS_API_TESTSSUPPORT_JSONGENERATOR_H_

#include "CUUID.h"

class JsonGenerator {
public:
	JsonGenerator();
	string CreateFolder(string name, CUUID parentId);
	virtual ~JsonGenerator();
};

#endif /* TESTS_API_TESTSSUPPORT_JSONGENERATOR_H_ */
