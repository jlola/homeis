/*
 * DeviceAtoms.h
 *
 *  Created on: 1. 6. 2018
 *      Author: pc
 */

#ifndef TESTS_API_TESTSSUPPORT_ATOMS_DEVICEATOMS_H_
#define TESTS_API_TESTSSUPPORT_ATOMS_DEVICEATOMS_H_

#include "HomeIsStarter.h"

class DeviceAtoms {
	HomeIsStarter & homeisStarter;
	long http_code;
	CURLcode urlCode;
public:
	DeviceAtoms(HomeIsStarter & homeisStarter);
	string CreateDevice(string name);
	string GetDevices();
	void AssertCurlResponse();
	string CreateEmailTag(string deviceId, string name);
	virtual ~DeviceAtoms();
};

#endif /* TESTS_API_TESTSSUPPORT_ATOMS_DEVICEATOMS_H_ */
