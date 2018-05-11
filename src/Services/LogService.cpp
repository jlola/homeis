/*
 * LogService.cpp
 *
 *  Created on: 23. 5. 2017
 *      Author: pc
 */
#include "document.h"		// rapidjson's DOM-style API
#include "prettywriter.h"
#include "stringbuffer.h"

#include "PoppyDebugTools.h"
#include "LogService.h"
#include "Helpers/Directory.h"
#include "Helpers/File.h"
#include "Helpers/StringBuilder.h"
#include <vector>
#include "File.h"

using namespace rapidjson;

LogService::LogService(IHttpHeadersProvider & httpHeadersProvider)
	: logger(CLogger::GetLogger()),
	httpHeadersProvider(httpHeadersProvider)
{

}

LogService::~LogService() {

}

const http_response LogService::render_GET(const http_request& req)
{
	STACK
	int response_code = MHD_HTTP_FORBIDDEN;
	Document document;
	document.SetObject();

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	Value jsonvalue;
	jsonvalue.SetObject();

	string path_api = req.get_path_piece(0);
	string path_log = req.get_path_piece(1);
	string path_filename = req.get_path_piece(2);
	string path_loglevel = req.get_path_piece(2);

	if (path_loglevel=="loglevel")
	{

	}
	else if (path_loglevel=="")
	{
		string logpath = StringBuilder::Format("%s/Log",file.getexepath().c_str());
		vector<std::string> list = directory.GetFileList(logpath.c_str());
		std::sort (list.begin(), list.end());
		document.SetArray();

		for (size_t i=0;i<list.size();i++)
		{
			if (list[i]!="." && list[i]!="..")
			{
				document.PushBack(list[i].c_str(),document.GetAllocator());
			}
		}
		document.Accept(wr);
		std::string json = buffer.GetString();
		response_code = MHD_HTTP_OK;
	}
	else
	{
		document.SetObject();
		string logpath = StringBuilder::Format("%s/Log/%s",file.getexepath().c_str(),path_filename.c_str());
		if (file.Exists(logpath))
		{
			string content = file.ReadWholeFile(logpath);
			jsonvalue.SetString(content.c_str(),document.GetAllocator());
			document.AddMember(path_filename.c_str(),jsonvalue,document.GetAllocator());
			document.Accept(wr);
			std::string json = buffer.GetString();
			vector<string> topics;
			topics.push_back(json);
			response_code = MHD_HTTP_OK;
		}
		else
		{
			jsonvalue.SetString("Not exists",document.GetAllocator());
			document.AddMember("Result",jsonvalue,document.GetAllocator());
			response_code = MHD_HTTP_NOT_FOUND;
		}
	}
	std::string json = buffer.GetString();
	http_response_builder response_builder(json, response_code,httpHeadersProvider.GetContentTypeAppJson());
	httpHeadersProvider.AddHeaders(response_builder);
	http_response resp(response_builder.string_response());
	return resp;
}

