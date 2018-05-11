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

FileController::FileController()
{

}

const http_response FileController::render_GET(const http_request& r)
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
		http_response resp(http_response_builder(strfile, 200, "text/css").file_response());
		return resp;
	}
	else
	{
		//*res = new http_file_response(file, 200, "text/html");
		http_response resp(http_response_builder(strfile, 200, "text/html").file_response());
		return resp;
	}
}
