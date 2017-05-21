/*
 * Directory.cpp
 *
 *  Created on: Sep 1, 2013
 *      Author: root
 */
#include <string>
#include <unistd.h>
#include <sys/stat.h>
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
