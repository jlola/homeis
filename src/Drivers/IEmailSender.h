/*
 * IEmailSender.h
 *
 *  Created on: 30. 4. 2018
 *      Author: pc
 */

#ifndef SRC_DRIVERS_IEMAILSENDER_H_
#define SRC_DRIVERS_IEMAILSENDER_H_

#include <string>
#include <vector>

using namespace std;

class IEmailSender
{
public:
	virtual bool Send(string fromaddr,string toaddr, string subject, string body)=0;
};



#endif /* SRC_DRIVERS_IEMAILSENDER_H_ */
