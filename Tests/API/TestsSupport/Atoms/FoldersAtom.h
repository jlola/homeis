/*
 * FoldersAtom.h
 *
 *  Created on: 29. 5. 2018
 *      Author: pc
 */

#ifndef TESTS_API_TESTSSUPPORT_FOLDERSATOM_H_
#define TESTS_API_TESTSSUPPORT_FOLDERSATOM_H_

#include "HomeIsStarter.h"

class FoldersAtom {
	HomeIsStarter & homeisStarter;
public:
	FoldersAtom(HomeIsStarter & homeisStarter);
	string CreateFolder(string name, CUUID parentId, long & httpRespCode,CURLcode & urlCode);
	string GetFolders(long & httpRespCode,CURLcode & urlCode);
	virtual ~FoldersAtom();
};

#endif /* TESTS_API_TESTSSUPPORT_FOLDERSATOM_H_ */
