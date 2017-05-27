/*
 * Directory.h
 *
 *  Created on: Sep 1, 2013
 *      Author: root
 */

#ifndef DIRECTORY_H_
#define DIRECTORY_H_

#include <vector>

using namespace std;

class Directory
{
public:
	static bool Exists(string path);
	static bool Create(string path);
	static vector<string> GetFileList(string dir);
};

#endif /* DIRECTORY_H_ */
