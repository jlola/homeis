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

	if (0 != access(dirpath.c_str(), F_OK)) {
	  if (ENOENT == errno) {
		 return false;
	  }
	//  if (ENOTDIR == errno) {
	//     // not a directory
	//  }
	}

	return true;
}

bool Directory::Create(string path)
{
	if (mkdir(path.c_str(), 0777)==0)
		return true;
	return false;
}
