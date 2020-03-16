/*
 * Client.cpp
 *
 *  Created on: 16. 3. 2017
 *      Author: pc
 */


#ifdef HOMEISTEST

#include "Client.h"
#include "StringBuilder.h"
#include "microhttpd.h"

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
	  printf("GET %s",url.c_str());
	  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	  curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
	  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
	  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
	  res = curl_easy_perform(curl);
	  curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
	  curl_easy_cleanup(curl);
	}
	response = s;
	printf("Response: %s\n",response.c_str());
	return res;
}


CURLcode Client::Post(string requestApi,string message, string sessionHash, string & response, long & http_code)
{
	return Post(requestApi,message,user,sessionHash,response,http_code);
}

CURLcode Client::Post(string requestApi,string message,string user, string sessionHash, string & response, long & http_code)
{
	std::string s;
	CURLcode res;

	CURL *curl = curl_easy_init();
	if(curl) {
	  std::string url = StringBuilder::Format("http://%s:%d/%s",
			  serverName.c_str(),port,requestApi.c_str());

	  printf("POST %s\n",url.c_str());

	  printf("Request body: %s\n",message.c_str());

	  string login = StringBuilder::Format("%s:%s", user.c_str(), sessionHash.c_str());

	  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	  curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1L);
	  curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
	  curl_easy_setopt(curl, CURLOPT_USERPWD, login.c_str()); //Your credentials goes here
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message.c_str());
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, message.length());
	  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
	  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
	  res = curl_easy_perform(curl);
	  curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
	  curl_easy_cleanup(curl);
	}
	response = s;
	printf("Response: %s\n",response.c_str());
	return res;
}

CURLcode Client::Put(string requestApi,string message,string sessionHash, string & response, long & http_code)
{
	return Put(requestApi,message,user,sessionHash,response,http_code);
}

CURLcode Client::Put(string requestApi,string message,string user, string sessionHash, string & response,long &http_code)
{
	std::string s;
	CURLcode res;

	CURL *curl = curl_easy_init();
	if(curl) {
	  std::string url = StringBuilder::Format("http://%s:%d/%s",
			  serverName.c_str(),port,requestApi.c_str());

	  printf("PUT %s\n",url.c_str());

	  printf(message.c_str());

	  struct curl_slist *headers=NULL;

	  headers = curl_slist_append(headers, "pragma:");

	   if (headers == NULL)
	     return CURLcode::CURLE_FAILED_INIT;

	   string login = StringBuilder::Format("%s:%s", user.c_str(), sessionHash.c_str());

	  headers = curl_slist_append(headers, "Content-Type: application/json");

	  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"); /* !!! */

	  curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
	  curl_easy_setopt(curl, CURLOPT_USERPWD, login.c_str()); //Your credentials goes here

	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message.c_str());
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, message.length());
	  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
	  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
	  res = curl_easy_perform(curl);
	  curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
	  curl_easy_cleanup(curl);
	}
	response = s;
	printf("Response: %s\n",response.c_str());
	return res;
}

CURLcode Client::Delete(string requestApi,string message,string user, string sessionHash, string & response,long &http_code)
{
	std::string s;
	CURLcode res;

	CURL *curl = curl_easy_init();
	if(curl) {
	  std::string url = StringBuilder::Format("http://%s:%d/%s",
			  serverName.c_str(),port,requestApi.c_str());

	  printf("DELETE %s",url.c_str());

	  printf(message.c_str());

	  struct curl_slist *headers=NULL;

	  headers = curl_slist_append(headers, "pragma:");

	   if (headers == NULL)
	     return CURLcode::CURLE_FAILED_INIT;

	   string login = StringBuilder::Format("%s:%s", user.c_str(), sessionHash.c_str());

	  headers = curl_slist_append(headers, "Content-Type: application/json");

	  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE"); /* !!! */

	  curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
	  curl_easy_setopt(curl, CURLOPT_USERPWD, login.c_str()); //Your credentials goes here

	  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, message.c_str());
	  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, message.length());

	  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
	  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
	  res = curl_easy_perform(curl);
	  curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
	  curl_easy_cleanup(curl);
	}
	response = s;
	printf("Response: %s",response.c_str());
	return res;
}

void Client::AssertCurlResponse(long http_code, CURLcode urlCode)
{
	if (CURLcode::CURLE_OK != urlCode)
		throw StringBuilder::Format("Assert urlCode: Expected: %d, Actual: %d",CURLcode::CURLE_OK,urlCode).c_str();
	if (MHD_HTTP_OK != http_code)
		throw StringBuilder::Format("Assert http_code: Expected: %d, Actual: %d",MHD_HTTP_OK,http_code).c_str();
}

Client::~Client() {
}

} /* namespace AF */

#endif
