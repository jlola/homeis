/*
 * File.cpp
 *
 *  Created on: Feb 24, 2013
 *      Author: Josef Lola
 */
#include <stdio.h>
#include <iostream>
#include <limits.h>
#include <unistd.h>
#include <fstream>
#include <cerrno>

#include "File.h"

using namespace std;

bool File::Exists(std::string fileName)
{
	FILE * file = fopen(fileName.c_str(), "r");
	if (file)
	{
		fclose(file);
		return true;
	}
	return false;
}

std::string File::getexepath()
{
	char result[ PATH_MAX ];
	ssize_t rcount = readlink( "/proc/self/exe", result, PATH_MAX );
	std::string path( result, (rcount > 0) ? rcount : 0 );

	int pos = path.find_last_of("/\\");
	path.erase(path.begin()+pos,path.end());
	return path;
}

std::string File::getexefile()
{
	char result[ PATH_MAX ];
	ssize_t rcount = readlink( "/proc/self/exe", result, PATH_MAX );
	std::string path( result, (rcount > 0) ? rcount : 0 );

	int pos = path.find_last_of("/\\");
	path.erase(path.begin(),path.begin()+pos+1);
	return path;
}

bool File::Delete(string fileName)
{
	int result = remove(fileName.c_str());
	if (result==0) return true;
	return false;
}

std::string File::ReadWholeFile(string filePath)
{
	std::ifstream in(filePath, std::ios::in | std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}
