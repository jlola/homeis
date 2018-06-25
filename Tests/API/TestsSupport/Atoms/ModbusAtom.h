/*
 * ModbusAtom.h
 *
 *  Created on: 17. 6. 2018
 *      Author: pc
 */

#ifndef TESTS_API_TESTSSUPPORT_ATOMS_MODBUSATOM_H_
#define TESTS_API_TESTSSUPPORT_ATOMS_MODBUSATOM_H_

#include "HomeIsStarter.h"

class ModbusAtom {
	HomeIsStarter & homeisStarter;
	long http_code;
	CURLcode urlCode;
public:
	ModbusAtom(HomeIsStarter & homeisStarter);
	string Scan(string providerName, int address);
	virtual ~ModbusAtom();
};

#endif /* TESTS_API_TESTSSUPPORT_ATOMS_MODBUSATOM_H_ */
