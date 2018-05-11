/*
 * File.h
 *
 *  Created on: Feb 24, 2013
 *      Author: Josef Lola
 */

#ifndef FILE_H_
#define FILE_H_

#include <string>
#include "IFile.h"

class File : public IFile
{
public:
	bool Exists(std::string fileName);
	std::string getexepath();
	bool Delete(std::string fileName);
	std::string ReadWholeFile(std::string filePath);
	std::string getexefile();
};

#endif /* FILE_H_ */
