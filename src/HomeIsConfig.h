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

typedef struct
{
	string Port;
	string Name;
	string Driver;
} SSerPortConfig;

typedef struct
{
	string SMTP;
	string UserName;
	string Password;
} SSmtpSettings;

class HomeIsConfig
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
