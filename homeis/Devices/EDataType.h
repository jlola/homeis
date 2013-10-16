/*
 * EDataType.h
 *
 *  Created on: Mar 13, 2013
 *      Author: Josef Lola
 */

#ifndef EDATATYPE_H_
#define EDATATYPE_H_

#include <stdint.h>

typedef enum  {
	Unknown = -1,
	Int = 0,
	Uint = 1,
	Double = 2,
	String = 3,
	Bool = 4
} EDataType;


#endif /* EDATATYPE_H_ */
