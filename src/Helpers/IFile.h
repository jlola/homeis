/*
 * IFile.h
 *
 *  Created on: 8. 5. 2018
 *      Author: pc
 */

#ifndef SRC_HELPERS_IFILE_H_
#define SRC_HELPERS_IFILE_H_

#include <string>

using namespace std;

class IFile
{
public:
	virtual bool Exists(string fileName)=0;
	virtual string getexepath()=0;
	virtual bool Delete(string fileName)=0;
	virtual string ReadWholeFile(string filePath)=0;
	virtual string getexefile()=0;
};


#endif /* SRC_HELPERS_IFILE_H_ */
