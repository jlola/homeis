/*
 * HisCollectionBase.h
 *
 *	Persisted collection
 *
 *  Created on: Nov 7, 2013
 *      Author: Josef Lola
 */

#ifndef HISCOLLECTIONBASE_H_
#define HISCOLLECTIONBASE_H_

#include <vector>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "CUUID.h"
#include "IHisBase.h"
#include "StringBuilder.h"
#include "HisException.h"

using namespace std;

template<class T>
class HisCollectionBase
{
	xmlDocPtr doc;       /* document pointer */
	string fileName;
	vector<T*> items;
protected:
	virtual string GetRootNodeName()=0;
	virtual T* LoadItem(xmlNodePtr cur)=0;
public:
	size_t FindIndex(CUUID Id)
	{
		for(size_t i=0;i<items.size();i++)
		{
			if (items[i]->GetRecordId()==Id) return i;
		}
		return -1;
	}

	T* Delete(size_t index)
	{

		size_t size = items.size();
		if (size>index)
		{
			T* base = items[index];
			items.erase(items.begin() + index);
			return base;
		}
		else
			throw MException(StringBuilder::Format("Index %d is larger than size of collection",index));
	}

	virtual void Add(T *obj)
	{
		xmlNodePtr root_node = xmlDocGetRootElement(doc);
		if (root_node==NULL)
		{
			root_node = xmlNewNode(NULL, BAD_CAST GetRootNodeName().c_str());
			xmlDocSetRootElement(doc, root_node);
		}

		IHisBase* hisBase = dynamic_cast<IHisBase*>(obj);
		xmlAddChild(root_node,hisBase->GetNodePtr());
		items.push_back(obj);
	}

	virtual void Load()
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
			T* item = LoadItem(cur);
			IHisBase* hisBase = dynamic_cast<IHisBase*>(item);
			hisBase->Load();
			items.push_back(item);
			cur = cur->next;
		}
	}

	virtual void Save()
	{
		for(size_t i=0;i<items.size();i++)
		{
			IHisBase* hisBase = dynamic_cast<IHisBase*>(items[i]);
			hisBase->Save();
		}

		FILE* f = fopen(fileName.c_str(),"w");
		if (f != NULL)
		{
			xmlDocDump( f, doc);
			fclose( f );
		}
	}

	size_t Size()
	{
		return items.size();
	}

	HisCollectionBase(std::string fileName)
	{
		this->fileName = fileName;
		doc = NULL;
	}

	T *operator[](size_t i)
	{
		{
			if (items.size()>i)
			{
				return items[i];
			}
			return NULL;
		}
	}

	virtual ~HisCollectionBase()
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
};

#endif /* HISCOLLECTIONBASE_H_ */
