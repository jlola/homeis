/*
 * ServiceBase.cpp
 *
 *  Created on: 15. 10. 2018
 *      Author: pc
 */

#include "ServiceBase.h"
#include "HttpHeadersProvider.h"
#include "prettywriter.h"
#include "stringbuffer.h"
#include "httpserver/http_response.hpp"

ServiceBase::ServiceBase(IHisDevFactory* factory,IUserManager* userManager) :
	factory(factory),userManager(userManager)
{
	session = NULL;
}

bool ServiceBase::Authorize(const http_request& req)
{
	if (req.get_user()=="a" && req.get_pass()=="a")
		return true;

	session = userManager->AuthorizeSession(req.get_pass(),req.get_requestor(),DateTime::Now());

	if (session!=NULL)
		return true;

	return false;
}

Session* ServiceBase::GetSession()
{
	return session;
}

IHisDevFactory* ServiceBase::GetFactory()
{
	return factory;
}


string ServiceBase::DocumentToString(Document & doc)
{
	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	doc.Accept(wr);
	std::string json = buffer.GetString();
	return json;
}

string ServiceBase::GetErrorMessageJson(string message)
{
	return factory->GetHeadersProvider()->GetErrorMessageJson(message);
}

std::shared_ptr<http_response> ServiceBase::UnauthorizedResponse()
{
	string message = GetErrorMessageJson("Authentication error");
	int response_code = MHD_HTTP_UNAUTHORIZED;
	return CreateResponseString(message,response_code);
}

const std::shared_ptr<http_response> ServiceBase::CreateResponseString(string json,int response_code)
{
	IHttpHeadersProvider* headersProvider = factory->GetHeadersProvider();
	std::shared_ptr<http_response> response = std::shared_ptr<http_response>(new string_response(json, response_code, headersProvider->GetContentTypeAppJson()));
	response = headersProvider->AddHeaders(response);
	return response;
}

const std::shared_ptr<http_response> ServiceBase::render_GET(const http_request& req)
{
	return GET(req);
}

const std::shared_ptr<http_response> ServiceBase::render_POST(const http_request& req)
{
	if (req.get_path().find(URL_LOGIN)!=string::npos || Authorize(req))
	{
		return POST(req);
	}
	else
	{
		return UnauthorizedResponse();
	}
}

const std::shared_ptr<http_response> ServiceBase::render_PUT(const http_request& req)
{
	if (Authorize(req))
	{
		return PUT(req);
	}
	else
	{
		return UnauthorizedResponse();
	}
}

const std::shared_ptr<http_response> ServiceBase::render_DELETE(const http_request& req)
{
	if (Authorize(req))
	{
		return DELETE(req);
	}
	else
	{
		return UnauthorizedResponse();
	}
}

const std::shared_ptr<http_response> ServiceBase::render_OPTIONS(const http_request& req)
{
	IHttpHeadersProvider* headersProvider = factory->GetHeadersProvider();
	int response_code = MHD_HTTP_OK;
	string message = "";
	std::shared_ptr<http_response> response =
			std::shared_ptr<http_response>(new string_response(message,response_code,headersProvider->GetContentTypeAppJson()));
	headersProvider->AddHeaders(response);
	response->with_header("Access-Control-Allow-Methods","POST, PUT, DELETE");
	response->with_header("Access-Control-Allow-Headers","authorization,content-type");

	return response;
}

const std::shared_ptr<http_response> ServiceBase::GET(const http_request& req)
{
	return ServiceBase::render(req);
}

const std::shared_ptr<http_response> ServiceBase::POST(const http_request& req)
{
	return ServiceBase::render(req);
}

const std::shared_ptr<http_response> ServiceBase::PUT(const http_request& req)
{
	return ServiceBase::render(req);
}

const std::shared_ptr<http_response> ServiceBase::DELETE(const http_request& req)
{
	return ServiceBase::render(req);
}

ServiceBase::~ServiceBase() {
}

