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
#include "ILogger.h"
#include "IConfig.h"

using namespace std;
using namespace libconfig;

class HomeIsConfig : public IConfig
{
	ILogger & logger;
	Config cfg;

	string GetOrCreateFilePath(string filename);

public:
	HomeIsConfig(string text, bool isFile);

	virtual ~HomeIsConfig();

	int GetServerPort();

	ELogLevel GetLogLevel();

	string GetAllowOrigin();

	SSmtpSettings GetSmtpSettings();

	vector<SSerPortConfig> GetSerialPorts();
};

#endif /* PRINTSERVERCONFIG_H_ */
