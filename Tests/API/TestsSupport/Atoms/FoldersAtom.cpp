/*
 * FoldersAtom.cpp
 *
 *  Created on: 29. 5. 2018
 *      Author: pc
 */

#ifdef HOMEISTEST

#include <string>
#include "gtest_src/gtest/gtest.h"
#include "StringBuilder.h"

using namespace std;

#include "document.h"		// rapidjson's DOM-style API
#include "prettywriter.h"
#include "stringbuffer.h"
#include "ParamsNames.h"

using namespace std;
using namespace rapidjson;

#include "FoldersAtom.h"

using namespace AF;

FoldersAtom::FoldersAtom(HomeIsStarter & homeisStarter)
	: homeisStarter(homeisStarter)
{
}


string FoldersAtom::CreateFolder(string name, CUUID parentId, long & httpRespCode,CURLcode & urlCode)
{

	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	document.SetObject();

	document.AddMember(JSON_PARENTID,"parentid",document.GetAllocator());
	document.AddMember(JSON_NAME,name.c_str(),document.GetAllocator());

	StringBuffer buffer;
	PrettyWriter<StringBuffer> wr(buffer);
	document.Accept(wr);
	const std::string json(buffer.GetString());
	string response;
	urlCode = homeisStarter.GetClient().Post("/api/folder",json,response,httpRespCode);
	return response;
}

string FoldersAtom::GetFolders(long & httpRespCode, CURLcode & urlCode)
{
	string response;
	urlCode = homeisStarter.GetClient().Get("api/folder/allitems",response,httpRespCode);
	return response;
}

FoldersAtom::~FoldersAtom() {
}

#endif

