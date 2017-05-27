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

using namespace rapidjson;

LogService::LogService() {

}

LogService::~LogService() {

}

void LogService::render_GET(const http_request& req, http_response** res)
{
	STACK

	Document document;
	document.SetObject();

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	Value jsonvalue;
	jsonvalue.SetObject();
	string logname = req.get_arg("log");

	if (logname.length()<=0)
	{
		string logpath = StringBuilder::Format("%s/Log",File::getexepath().c_str());
		vector<std::string> list = Directory::GetFileList(logpath.c_str());
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
		*res = new http_response(http_response_builder(json, 200,"application/json").string_response());
	}
	else
	{
		document.SetObject();
		string logpath = StringBuilder::Format("%s/Log/%s",File::getexepath().c_str(),logname.c_str());
		if (File::Exists(logpath))
		{
			string content = File::ReadWholeFile(logpath);
			jsonvalue.SetString(content.c_str(),document.GetAllocator());
			document.AddMember(logname.c_str(),jsonvalue,document.GetAllocator());
			document.Accept(wr);
			std::string json = buffer.GetString();
			*res = new http_response(http_response_builder(json, 200,"application/json").string_response());
		}
		else
		{
			jsonvalue.SetString("Not exists",document.GetAllocator());
			document.AddMember("Result",jsonvalue,document.GetAllocator());
			std::string json = buffer.GetString();
			*res = new http_response(http_response_builder(json, 400,"application/json").string_response());
		}
	}
}

