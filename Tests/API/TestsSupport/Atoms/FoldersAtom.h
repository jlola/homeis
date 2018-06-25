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
	void CreateFolder(string name, CUUID parentId);
	Document GetFolders();
	virtual ~FoldersAtom();
};

#endif /* TESTS_API_TESTSSUPPORT_FOLDERSATOM_H_ */
