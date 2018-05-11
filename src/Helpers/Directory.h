/*
 * Directory.h
 *
 *  Created on: Sep 1, 2013
 *      Author: root
 */

#ifndef DIRECTORY_H_
#define DIRECTORY_H_

#include <vector>

#include "IDirectory.h"

using namespace std;

class Directory : public IDirectory
{
public:
	bool Exists(string path);
	bool Create(string path);
	vector<string> GetFileList(string dir);
};

#endif /* DIRECTORY_H_ */
