/*
 * IDirectory.h
 *
 *  Created on: 8. 5. 2018
 *      Author: pc
 */

#ifndef SRC_HELPERS_IDIRECTORY_H_
#define SRC_HELPERS_IDIRECTORY_H_

#include <string>
#include <vector>

using namespace std;

class IDirectory
{
public:
	virtual bool Exists(string path)=0;
	virtual bool Create(string path)=0;
	virtual vector<string> GetFileList(string dir)=0;
	virtual ~IDirectory()
	{

	}
};



#endif /* SRC_HELPERS_IDIRECTORY_H_ */
