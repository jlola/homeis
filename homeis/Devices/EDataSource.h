/*
 * ESourceValueType.h
 *
 *  Created on: May 10, 2013
 *      Author: root
 */

#ifndef ESOURCEVALUETYPE_H_
#define ESOURCEVALUETYPE_H_

#include <stdint.h>

typedef enum  {
	Const = 0,
	Time = 1,
	Storage = 2,
	DS18B20 = 3,
	DS2413 = 4
} EDataSource;

#endif /* ESOURCEVALUETYPE_H_ */
