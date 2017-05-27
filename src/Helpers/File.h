/*
 * File.h
 *
 *  Created on: Feb 24, 2013
 *      Author: Josef Lola
 */

#ifndef FILE_H_
#define FILE_H_

#include <string>

class File
{
public:
	static bool Exists(std::string fileName);
	static std::string getexepath();
	static bool Delete(std::string fileName);
	static std::string ReadWholeFile(std::string filePath);
	static std::string getexefile();
};

#endif /* FILE_H_ */
