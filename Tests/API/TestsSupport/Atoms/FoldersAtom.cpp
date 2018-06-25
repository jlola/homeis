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
#include "HisException.h"

using namespace std;
using namespace rapidjson;

#include "FoldersAtom.h"

using namespace AF;

FoldersAtom::FoldersAtom(HomeIsStarter & homeisStarter)
	: homeisStarter(homeisStarter)
{
}


void FoldersAtom::CreateFolder(string name, CUUID parentId)
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
	long int http_code;
	CURLcode urlCode = homeisStarter.GetClient().Post("/api/folder",json,response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);

//	Document documentResponse;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
//	if (documentResponse.Parse<0>((char*)response.c_str()).HasParseError() == true)
//	{
//		throw ArgumentNullException("response");
//	}

	//return documentResponse;
}

Document FoldersAtom::GetFolders()
{
	long http_code;
	CURLcode urlCode;
	string response;
	urlCode = homeisStarter.GetClient().Get("api/folder/allitems",response,http_code);
	Client::AssertCurlResponse(http_code,urlCode);

	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.
	if (document.Parse<0>((char*)response.c_str()).HasParseError() == true)
		throw ArgumentNullException("response");

	return document;
}

FoldersAtom::~FoldersAtom() {
}

#endif

