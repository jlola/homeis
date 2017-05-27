/*
 * Directory.cpp
 *
 *  Created on: Sep 1, 2013
 *      Author: root
 */
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <iostream>
#include <exception>
#include "Helpers/StringBuilder.h"
#include "logger.h"
#include "Directory.h"

using namespace std;

bool Directory::Exists(string dirpath)
{
	struct stat sb;

	if (stat(dirpath.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Directory::Create(string path)
{
	if (mkdir(path.c_str(), 0777)==0)
		return true;
	return false;
}

/*function... might want it in some class?*/
vector<string> Directory::GetFileList(string dir)
{
	vector<std::string> files;
	DIR *dp;
	struct dirent *dirp;
	if((dp = opendir(dir.c_str())) == NULL)
	{
		string log = StringBuilder::Format("Error( %d ) opening %s ",errno,dir.c_str());
		CLogger::Info(log.c_str());
		return files;
	}

	while ((dirp = readdir(dp)) != NULL) {
		files.push_back(string(dirp->d_name));
	}
	closedir(dp);
	return files;
}

