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
#include "FileController.h"

FileController::FileController()
{

}

void FileController::render_GET(const http_request& r, http_response** res)
{
	std::string filepar = r.get_arg("path");
	std::string file = File::getexepath();
	file += "/files/";

	if (r.get_path()=="/")
		file += "index.html";
	else
		file += r.get_path();
	if (file.find(".css")!= std::string::npos)
		//*res = new http_file_response(file, 200, "text/css");
		*res = new http_response(http_response_builder(file, 200, "text/css").file_response());
	else
		//*res = new http_file_response(file, 200, "text/html");
		*res = new http_response(http_response_builder(file, 200, "text/html").file_response());
}
