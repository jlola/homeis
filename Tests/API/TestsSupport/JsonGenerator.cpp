/*
 * JsonGenerator.cpp
 *
 *  Created on: 16. 5. 2018
 *      Author: pc
 */

#include <string>
#include "StringBuilder.h"
#include "JsonGenerator.h"

using namespace std;

JsonGenerator::JsonGenerator()
{
}

string JsonGenerator::CreateFolder(string name, CUUID parentId)
{
	string struuid = parentId != CUUID::Empty() ? "\"" + parentId.ToString() + "\"" : "null";
	string json = StringBuilder::Format("\"type\":null, \"name\":\"%s\",\"parentId\":%s", name.c_str(), struuid.c_str());
	return json;
}

JsonGenerator::~JsonGenerator() {
}

