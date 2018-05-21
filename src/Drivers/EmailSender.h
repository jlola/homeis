/*
 * EmailSender.h
 *
 *  Created on: 22. 4. 2018
 *      Author: pc
 */

#ifndef SRC_DRIVERS_EMAILSENDER_H_
#define SRC_DRIVERS_EMAILSENDER_H_

#include <string>
#include <vector>
#include "HomeIsConfig.h"
#include "IEmailSender.h"

using namespace std;

typedef struct {
	  vector<string> payload;
	  size_t lines_read;
	} upload_status;

class EmailSender : public IEmailSender {
	string smtp;
	string username;
	string password;
public:
	EmailSender(string smtp,string username,string password);
	EmailSender(SSmtpSettings settings);
	bool Send(string fromaddr,string toaddr, string subject, string body);
	static vector<string> CreatePayLoad(string fromaddr,string toaddr, string subject, string body);
	static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp);
	virtual ~EmailSender();
};

#endif /* SRC_DRIVERS_EMAILSENDER_H_ */
