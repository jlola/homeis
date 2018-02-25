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
#include "logger.h"

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
	ILogger & logger;
	string filename;
	Config cfg;

public:

	HomeIsConfig(string name);

	virtual ~HomeIsConfig();

	int GetServerPort();

	ELogLevel GetLogLevel();

	string GetAllowOrigin();

	vector<SSerPortConfig> GetSerialPorts();
};

#endif /* PRINTSERVERCONFIG_H_ */
