/*
 * FileController.cpp
 *
 *  Created on: Apr 16, 2013
 *      Author: root
 */
#include <signal.h>
#include <string>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "File.h"
#include "PoppyDebugTools.h"
#include "FileController.h"
#include "File.h"

FileController::FileController(webserver* ws_i,
		IUserManager* userManager,
		IHisDevFactory* factory
		)
	: ServiceBase::ServiceBase(factory,userManager)

{
	ws_i->register_resource(string(""), this, true);
	ws_i->register_resource(string("files/{path}"), this, true);
}

const std::shared_ptr<http_response> FileController::GET(const http_request& r)
{
	STACK
	File file;
	std::string filepar = r.get_arg("path");
	std::string strfile = file.getexepath();
	strfile += "/files/";

	if (r.get_path()=="/")
		strfile += "index.html";
	else
		strfile += r.get_path();
	if (strfile.find(".css")!= std::string::npos)
	{
		//*res = new http_file_response(file, 200, "text/css");
		std::shared_ptr<http_response> resp = std::shared_ptr<http_response>(new file_response(strfile, 200, "text/css"));
		return resp;
	}
	else
	{
		//*res = new http_file_response(file, 200, "text/html");
		std::shared_ptr<http_response> resp = std::shared_ptr<http_response>(new file_response(strfile, 200, "text/html"));
		return resp;
	}
}
