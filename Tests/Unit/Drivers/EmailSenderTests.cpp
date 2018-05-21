/*
 * EmailSenderTests.cpp
 *
 *  Created on: 26. 4. 2018
 *      Author: pc
 */

#ifdef HOMEISTEST

#include "EmailSender.h"
#include "EmailSenderTests.h"


using namespace fakeit;
namespace AF {

EmailSenderTests::EmailSenderTests() {
}


TEST_F(EmailSenderTests,CreateResponseTest)
{
	EmailSender sender("smtp.seznam.cz","josef.lola@email.cz","");
	vector<string> recipients;
	recipients.push_back("josef.lola@gmail.com");
	//sender.Send("josef.lola@email.cz",recipients,"test","body test");
}

EmailSenderTests::~EmailSenderTests() {
}

}

#endif
