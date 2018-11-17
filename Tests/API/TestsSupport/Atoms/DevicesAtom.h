/*
 * DeviceAtoms.h
 *
 *  Created on: 1. 6. 2018
 *      Author: pc
 */

#ifndef TESTS_API_TESTSSUPPORT_ATOMS_DEVICESATOM_H_
#define TESTS_API_TESTSSUPPORT_ATOMS_DEVICESATOM_H_

#include "HomeIsStarter.h"

class DevicesAtom {
	HomeIsStarter & homeisStarter;
	long http_code;
	CURLcode urlCode;
public:
	DevicesAtom(HomeIsStarter & homeisStarter);
	string CreateDevice(string name);
	Document GetDevices();
	Document GetDevice(string id);
	void UpdateDevicePropertyUint(string devid,string name,uint32_t value);
	void UpdateDeviceProperty(string id,string name, Value & value);
	string CreateEmailTag(string deviceId, string name,string sender, string receivers);
	string GetDevicePropertyString(string devieId,string name);
	int GetDevicePropertyInt(string deviceId,string name);
	int GetDevicePropertyUInt(string deviceId,string name);
	string FindTagId(string deviceId, string tagName);
	string GetDeviceId();
	void WriteToTag(string tagId,string value);
	Document GetTag(string tagId);
	virtual ~DevicesAtom();
};

#endif /* TESTS_API_TESTSSUPPORT_ATOMS_DEVICESATOM_H_ */
