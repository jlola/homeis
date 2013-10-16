/*
 * HisBase.h
 *
 *  Created on: May 19, 2013
 *      Author: root
 */

#ifndef HISBASE_H_
#define HISBASE_H_

#include <vector>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "homeis/Common/CUUID.h"
#include "document.h"		// rapidjson's DOM-style API

using namespace rapidjson;
using namespace std;

#define PROP_NAME BAD_CAST "name"
#define PROP_RECORDID  BAD_CAST "recordid"

class HisBase;

typedef bool FindProc(HisBase* hisbase,void* args);
//template<typename T>
//using FindProc = void (*)(GLRenderer*, const MotionEvent&, std::vector<T >);

class HisBase
{
	HisBase* parent;
	string name;
	CUUID recordId;
	xmlNodePtr node;
	bool isnew;
	bool isloaded;
	vector<HisBase*> items;
public:
	HisBase();
	//HisBase(xmlNodePtr pnode,CUUID recordid);
	HisBase(xmlNodePtr pnode);

protected:
	virtual void DoInternalSave(xmlNodePtr & node);
	virtual void DoInternalLoad(xmlNodePtr & node);
	virtual const xmlChar* GetNodeNameInternal()=0;
	const xmlChar* GetNodeName();
	void ClearIsNew();
	void FreeItems();
	vector<HisBase*> GetAllItems();
public:
	/*
	 * najde id ve vsech potomcich i vnorenych
	 */
	HisBase* Find(CUUID id);


	HisBase* Find(FindProc proc,void* args);
	/*
	 * najde index Id v deckach v jedne urovni, kdyz nenajde nic -1
	 */
	int FindIndex(CUUID id);
	void Add(HisBase *pitem);
	void Remove(CUUID puuid);
	void Load();
	void Save();
	xmlNodePtr GetNodePtr();
	//void SetNodePtr(xmlNodePtr pnode);
	CUUID GetRecordId();
	string GetName();
	virtual void SetName(string pname);
	virtual ~HisBase();
	HisBase* GetParent();
	void SetParent(HisBase* pParent);
	//template<class T> void GetItems(/*vector<T*> & result*/);
	template<class T>
	vector<T*> GetItems()
	{
		vector<T*> result;
		for(size_t i=0;i<items.size();i++)
		{
			T* it = dynamic_cast<T*>(items[i]);
			if (it!=NULL) result.push_back(it);
		}
		return result;
	}

};

#endif /* HISBASE_H_ */
