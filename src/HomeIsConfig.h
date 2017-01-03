/*
 * PrintserverConfig.h
 *
 *  Created on: 24. 8. 2015
 *      Author: jlola
 */

#ifndef PRINTSERVERCONFIG_H_
#define PRINTSERVERCONFIG_H_

#include <string.h>
#include "File.h"
#include <libconfig.h++>
#include <vector>

using namespace std;
using namespace libconfig;

struct SSerPortConfig
{
	string Port;
	string Name;
	string Driver;
};

class HomeIsConfig
{

	string filename;
	Config cfg;

public:

	HomeIsConfig(string name);

	virtual ~HomeIsConfig();

//	string GetFileStoragePath();

	int GetServerPort();

	vector<SSerPortConfig> GetSerialPorts();

//	string GetMountMassScript();
//
//	string GetMountFolderScript();
//
//	string GetUmountFolderScript();
//
//	string GetUmountMassScript();
//
//	string GetRefreshScript();
//
//	string GetTargetDir();
//
//	string GetPrinterGadgetCMD();

};

#endif /* PRINTSERVERCONFIG_H_ */
