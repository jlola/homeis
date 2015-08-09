/*
 * Directory.h
 *
 *  Created on: Sep 1, 2013
 *      Author: root
 */

#ifndef DIRECTORY_H_
#define DIRECTORY_H_

using namespace std;

class Directory
{
public:
	static bool Exists(string path);
	static bool Create(string path);
};

#endif /* DIRECTORY_H_ */
