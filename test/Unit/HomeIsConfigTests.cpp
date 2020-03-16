/*
 * HomeIsConfigTests.cpp
 *
 *  Created on: 1. 5. 2018
 *      Author: pc
 */

#ifdef HOMEISTEST

#include "gtest/gtest.h"
#include "HomeIsConfig.h"
#include "HomeIsConfigTests.h"

HomeIsConfigTests::HomeIsConfigTests() {
}

TEST_F(HomeIsConfigTests,SmtpConfigReadSmtpConfig)
{
	char configstr[] = "SmtpServer=\"smtp.seznam.cz\";\n\
SmtpUserName=\"userName\";\n\
SmtpPassword=\"password\";\
";
	HomeIsConfig config(configstr,false);
	SSmtpSettings settings = config.GetSmtpSettings();
	ASSERT_EQ(settings.SMTP,"smtp.seznam.cz");
	ASSERT_EQ(settings.UserName,"userName");
	ASSERT_EQ(settings.Password,"password");
}

HomeIsConfigTests::~HomeIsConfigTests() {
}

#endif
