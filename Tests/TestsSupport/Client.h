/*
 * Client.h
 *
 *  Created on: 16. 3. 2017
 *      Author: pc
 */

#ifndef SRC_TESTSSUPPORT_CLIENT_H_
#define SRC_TESTSSUPPORT_CLIENT_H_

#include <string>
#include "curl.h"

using namespace std;

namespace AF {

class Client {
	string serverName;
	int port;
public:
	Client(string serverName,int port);
	CURLcode Get(string requestApi,string & response, long & http_code);
	CURLcode Post(string requestApi,string message,string & response, long & http_code);
	CURLcode Put(string requestApi,string message,string & response,long &http_code);
	Client();
	virtual ~Client();
};

} /* namespace AF */

#endif /* SRC_TESTSSUPPORT_CLIENT_H_ */
