/*
 * EmailSender.cpp
 *
 *  Created on: 22. 4. 2018
 *      Author: pc
 */
#include "curl.h"
#include "StringBuilder.h"
#include "DateTime.h"
#include "CUUID.h"
#include "EmailSender.h"

EmailSender::EmailSender(SSmtpSettings settings)
	: EmailSender::EmailSender(settings.SMTP,settings.UserName,settings.Password)
{
}

EmailSender::EmailSender(string smtp,string username,string password) {
	this->smtp = smtp;
	this->username = username;
	this->password = password;
}

size_t EmailSender::payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
  upload_status *upload_ctx = (upload_status *)userp;
  const char *data;

  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
    return 0;
  }

  if(upload_ctx->lines_read < upload_ctx->payload.size()) {
	data = upload_ctx->payload[upload_ctx->lines_read].c_str();
    size_t len = strlen(data);
    memcpy(ptr, data, len);
    upload_ctx->lines_read++;

    return len;
  }

  return 0;
}

vector<string> EmailSender::CreatePayLoad(string fromaddr,vector<string> toaddr, string subject, string body)
{
	vector<string> result;
	CUUID uuid;
	result.push_back(StringBuilder::Format("Date: %s\r\n",DateTime::Now().ToString().c_str()));
	result.push_back(StringBuilder::Format("To:  %s \r\n",StringBuilder::join(toaddr,";").c_str()));
	result.push_back(StringBuilder::Format("From: %s \r\n",fromaddr.c_str()));
	result.push_back(StringBuilder::Format("Message-ID: <%s@seznam.cz>\r\n",uuid.ToString().c_str()));
	result.push_back(StringBuilder::Format("Subject: %s\r\n",subject.c_str()));
	result.push_back(StringBuilder::Format("\r\n"));
	result.push_back(StringBuilder::Format("%s\r\n",body.c_str()));
	result.push_back(StringBuilder::Format("\r\n"));
	return result;
}

bool EmailSender::Send(string fromaddr,vector<string> toaddr, string subject, string body)
{
	  CURL *curl;
	  CURLcode res = CURLE_OK;
	  curl_slist *recipients = NULL;
	  upload_status upload_ctx;

	  upload_ctx.payload = CreatePayLoad(fromaddr,toaddr,subject,body);
	  upload_ctx.lines_read = 0;

	  curl = curl_easy_init();
	  if(curl) {
		  curl_easy_setopt(curl,CURLOPT_USERNAME,username.c_str());
		  curl_easy_setopt(curl,CURLOPT_PASSWORD,password.c_str());

		  string url = StringBuilder::Format("smtp://%s",smtp.c_str());
	    /* This is the URL for your mailserver */
	    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	    /* Note that this option isn't strictly required, omitting it will result
	     * in libcurl sending the MAIL FROM command with empty sender data. All
	     * autoresponses should have an empty reverse-path, and should be directed
	     * to the address in the reverse-path which triggered them. Otherwise,
	     * they could cause an endless loop. See RFC 5321 Section 4.5.5 for more
	     * details.
	     */
	    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, fromaddr.c_str());

	    /* Add two recipients, in this particular case they correspond to the
	     * To: and Cc: addressees in the header, but they could be any kind of
	     * recipient. */
	    for(size_t i=0;i<toaddr.size();i++)
	    	recipients = curl_slist_append(recipients, toaddr[i].c_str());
	    //recipients = curl_slist_append(recipients, CC_ADDR);
	    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

	    /* We're using a callback function to specify the payload (the headers and
	     * body of the message). You could just use the CURLOPT_READDATA option to
	     * specify a FILE pointer to read from. */
	    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
	    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
	    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

	    /* Send the message */
	    res = curl_easy_perform(curl);

	    /* Check for errors */
	    if(res != CURLE_OK)
	    {
	      fprintf(stderr, "curl_easy_perform() failed: %s\n",
	              curl_easy_strerror(res));

	    }
	    /* Free the list of recipients */
	    curl_slist_free_all(recipients);

	    /* curl won't send the QUIT command until you call cleanup, so you should
	     * be able to re-use this connection for additional messages (setting
	     * CURLOPT_MAIL_FROM and CURLOPT_MAIL_RCPT as required, and calling
	     * curl_easy_perform() again. It may not be a good idea to keep the
	     * connection open for a very long time though (more than a few minutes
	     * may result in the server timing out the connection), and you do want to
	     * clean up in the end.
	     */
	    curl_easy_cleanup(curl);

	    return res == CURLE_OK;
	  }
	  return false;
}

EmailSender::~EmailSender() {
}

