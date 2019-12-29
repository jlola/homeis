/*
 * IConfig.h
 *
 *  Created on: 9. 10. 2018
 *      Author: pc
 */

#ifndef SRC_ICONFIG_H_
#define SRC_ICONFIG_H_

#include <vector>

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

class IConfig
{
public:
	virtual int GetServerPort()=0;

	virtual ELogLevel GetLogLevel()=0;

	virtual string GetAllowOrigin()=0;

	virtual SSmtpSettings GetSmtpSettings()=0;

	virtual vector<SSerPortConfig> GetSerialPorts()=0;

	virtual ~IConfig() {

	}
};

#endif /* SRC_ICONFIG_H_ */
