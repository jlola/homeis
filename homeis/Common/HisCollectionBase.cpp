/*
 * HisCollectionBase.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: Josef Lola
 */

#include <iostream>
#include <algorithm>    // std::sort
#include <string>
#include <vector>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "homeis/Common/HisBase.h"

#include "HisCollectionBase.h"

HisCollectionBase::HisCollectionBase(std::string pFileName)
{
	fileName = pFileName;
	doc = NULL;
}

HisBase *HisCollectionBase::operator[](size_t i)
{
	if (items.size()>i)
	{
		return items[i];
	}
	return NULL;
}

void HisCollectionBase::Save()
{
	for(size_t i=0;i<items.size();i++)
	{
		items[i]->Save();
	}

	FILE* f = fopen(fileName.c_str(),"w");
	if (f != NULL)
	{
		xmlDocDump( f, doc);
		fclose( f );
	}
}

void HisCollectionBase::Load()
{
	xmlNodePtr root_node = NULL;/* node pointers */
	//xmlDtdPtr dtd = NULL;       /* DTD pointer */

	LIBXML_TEST_VERSION;

	if (items.size()>0)
	{
		for(size_t i = items.size() - 1; i >= 0; i--)
		{
			this->Delete(i);
		}
	}

	if (doc!=NULL)
	{
		/*free the document */
		xmlFreeDoc(doc);

		 /*
		 *Free the global variables that may
		 *have been allocated by the parser.
		 */
		xmlCleanupParser();

		doc = NULL;
	}

	/*
	 * Creates a new document, a node and set it as a root node
	 */
	doc = xmlReadFile(fileName.c_str(), NULL, 0);
	if (doc == NULL) {
		doc = xmlNewDoc(BAD_CAST "1.0");
	}

	root_node = xmlDocGetRootElement(doc);
	if (root_node==NULL)
	{
		root_node = xmlNewNode(NULL, BAD_CAST GetRootNodeName().c_str());
		xmlDocSetRootElement(doc, root_node);
	}

	xmlNodePtr cur = root_node->children;
	while (cur != NULL) {
		HisBase* item = LoadItem(cur);
		item->Load();
		items.push_back(item);
		cur = cur->next;
	}
}

void HisCollectionBase::Add(HisBase *hisBase)
{
	xmlNodePtr root_node = xmlDocGetRootElement(doc);
	//xmlNodePtr cur = NULL;
	if (root_node==NULL)
	{
		root_node = xmlNewNode(NULL, BAD_CAST GetRootNodeName().c_str());
		xmlDocSetRootElement(doc, root_node);
	}

	xmlAddChild(root_node,hisBase->GetNodePtr());
	items.push_back(hisBase);
}

size_t HisCollectionBase::Size()
{
	return items.size();
}

size_t HisCollectionBase::FindIndex(CUUID Id)
{
	for(size_t i=0;i<items.size();i++)
	{
		if (items[i]->GetRecordId()==Id) return i;
	}
	return -1;
}

void HisCollectionBase::Delete(size_t index)
{

	size_t size = items.size();
	if (size>index)
	{
		HisBase* base = items[index];
		//xmlNodePtr node = base->GetNodePtr();
		items.erase(items.begin() + index);
		delete(base);
	}
}

HisCollectionBase::~HisCollectionBase()
{
	for(uint32_t i=0;i<items.size();i++)
		delete(items[i]);
	items.clear();

	/*free the document */
	xmlFreeDoc(doc);

	 /*
	 *Free the global variables that may
	 *have been allocated by the parser.
	 */
	xmlCleanupParser();
}
