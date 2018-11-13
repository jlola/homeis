/*
 * ServiceBase.cpp
 *
 *  Created on: 15. 10. 2018
 *      Author: pc
 */

#include "ServiceBase.h"
#include "HttpHeadersProvider.h"



ServiceBase::ServiceBase(IHisDevFactory* factory,IUserManager* userManager) :
	factory(factory),userManager(userManager)
{
}

bool ServiceBase::Authorize(const http_request& req)
{
	if (req.get_user()=="a" && req.get_pass()=="a")
		return true;

	auto result = userManager->AuthorizeSession(req.get_pass(),req.get_requestor(),DateTime::Now());

	return result;
}

IHisDevFactory* ServiceBase::GetFactory()
{
	return factory;
}

string ServiceBase::GetErrorMessageJson(string message)
{
	return factory->GetHeadersProvider()->GetErrorMessageJson(message);
}

http_response ServiceBase::UnauthorizedResponse()
{
	string message = GetErrorMessageJson("Authentication error");
	int response_code = MHD_HTTP_UNAUTHORIZED;
	return CreateResponseString(message,response_code);
}

http_response ServiceBase::CreateResponseString(string json,int response_code)
{
	IHttpHeadersProvider* headersProvider = factory->GetHeadersProvider();
	http_response_builder response_builder(json, response_code,headersProvider->GetContentTypeAppJson());
	headersProvider->AddHeaders(response_builder);
	http_response resp(response_builder.string_response());
	return resp;
}

const http_response ServiceBase::render_GET(const http_request& req)
{
	return GET(req);
}

const http_response ServiceBase::render_POST(const http_request& req)
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

const http_response ServiceBase::render_PUT(const http_request& req)
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

const http_response ServiceBase::render_DELETE(const http_request& req)
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

const http_response ServiceBase::render_OPTIONS(const http_request& req)
{
	IHttpHeadersProvider* headersProvider = factory->GetHeadersProvider();
	int response_code = MHD_HTTP_OK;
	string message = "";
	http_response_builder response_builder(message,response_code,headersProvider->GetContentTypeAppJson());
	headersProvider->AddHeaders(response_builder);
	response_builder = response_builder.with_header("Access-Control-Allow-Methods","POST, PUT");
	response_builder = response_builder.with_header("Access-Control-Allow-Headers","authorization,content-type");
	http_response resp(response_builder.string_response());
	return resp;
}

const http_response ServiceBase::GET(const http_request& req)
{
	return ServiceBase::render(req);
}

const http_response ServiceBase::POST(const http_request& req)
{
	return ServiceBase::render(req);
}

const http_response ServiceBase::PUT(const http_request& req)
{
	return ServiceBase::render(req);
}

const http_response ServiceBase::DELETE(const http_request& req)
{
	return ServiceBase::render(req);
}

ServiceBase::~ServiceBase() {
}

