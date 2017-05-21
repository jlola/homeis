/*
 * Client.cpp
 *
 *  Created on: 16. 3. 2017
 *      Author: pc
 */


#ifdef HOMEISTEST

#include <TestsSupport/Client.h>
#include "StringBuilder.h"

namespace AF
{

Client::Client(string serverName,int port) {
	this->serverName = serverName;
	this->port = port;
}

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* s) {
    s->append((char*) ptr, size * nmemb);
    return size * nmemb;
}

CURLcode Client::Get(string requestApi,string & response, long & http_code)
{
	std::string s;
	CURLcode res;

	CURL *curl = curl_easy_init();
	if(curl) {
	  std::string url = StringBuilder::Format("http://%s:%d/%s",
			  serverName.c_str(),port,requestApi.c_str());
	  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	  curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
	  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
	  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
	  res = curl_easy_perform(curl);
	  curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
	  curl_easy_cleanup(curl);
	}
	response = s;
	return res;
}

CURLcode Client::Post(string requestApi,string message,string & response, long & http_code)
{
	std::string s;
	CURLcode res;

	CURL *curl = curl_easy_init();
	if(curl) {
	  std::string url = StringBuilder::Format("http://%s:%d/%s",
			  serverName.c_str(),port,requestApi.c_str());
	  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	  curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1L);
	  curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
	  curl_easy_setopt(curl, CURLOPT_USERPWD, "a:a"); //Your credentials goes here
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message.c_str());
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, message.length());
	  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
	  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
	  res = curl_easy_perform(curl);
	  curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
	  curl_easy_cleanup(curl);
	}
	response = s;
	return res;
}

CURLcode Client::Put(string requestApi,string message,string & response,long &http_code)
{
	std::string s;
	CURLcode res;

	CURL *curl = curl_easy_init();
	if(curl) {
	  std::string url = StringBuilder::Format("http://%s:%d/%s",
			  serverName.c_str(),port,requestApi.c_str());

	  struct curl_slist *headers=NULL;

	  headers = curl_slist_append(headers, "pragma:");

	   if (headers == NULL)
	     return CURLcode::CURLE_FAILED_INIT;

	  headers = curl_slist_append(headers, "Content-Type: application/json");

	  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"); /* !!! */

	  curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
	  curl_easy_setopt(curl, CURLOPT_USERPWD, "a:a"); //Your credentials goes here

	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message.c_str());
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, message.length());
	  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
	  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
	  res = curl_easy_perform(curl);
	  curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
	  curl_easy_cleanup(curl);
	}
	response = s;
	return res;
}

Client::~Client() {
	// TODO Auto-generated destructor stub
}

} /* namespace AF */

#endif
