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
#include "Common/CUUID.h"
#include "document.h"		// rapidjson's DOM-style API
#include "converter.h"
#include "DateTime.h"
#include "PoppyDebugTools.h"
#include "IHisBase.h"
#include "IHisDevFactory.h"

using namespace rapidjson;
using namespace std;

#define DEFAULTNAME "-------"

#define PROP_NAME BAD_CAST "name"
#define PROP_RECORDID  BAD_CAST "recordid"
#define PROP_CREATE_DATE BAD_CAST "createdate"
#define PROP_MODIFY_DATE BAD_CAST "modifydate"

//template<typename T>
//using FindProc = void (*)(GLRenderer*, const MotionEvent&, std::vector<T >);

class HisBase : public IHisBase
{
	IHisBase* parent;
	string name;
	CUUID recordId;
	xmlNodePtr node;
	bool isnew;
	bool isloaded;
	vector<IHisBase*> items;
	DateTime modifyDate;
	DateTime createDate;
	IHisDevFactory* factory;

	static bool FindProcByName(IHisBase* hisbase,void* args);
public:
	HisBase(IHisDevFactory* factory);
	HisBase(xmlNodePtr pnode, IHisDevFactory* factory);

protected:
	virtual void DoInternalSave(xmlNodePtr & node);
	virtual void DoInternalLoad(xmlNodePtr & node);
	virtual const xmlChar* GetNodeNameInternal() = 0;
	void ClearIsNew();
	void FreeItems();
	vector<IHisBase*> GetAllItems();
public:
	IHisDevFactory* GetFactory();
	const xmlChar* GetNodeName();
	/*
	 * najde id ve vsech potomcich i vnorenych
	 */
	IHisBase* Find(CUUID id);

	IHisBase* FindByName(string name);

	IHisBase* Find(FindProc proc,void* args);
	/*
	 * najde index Id v deckach v jedne urovni, kdyz nenajde nic -1
	 */
	int FindIndex(CUUID id);
	virtual void Add(IHisBase *pitem);
	virtual IHisBase* Remove(CUUID puuid);
	DateTime GetModifyDateTime();
	void SetModifyDateTime(DateTime modifyDate);
	DateTime GetCreateDateTime();
	void Load();
	void Save();
	xmlNodePtr GetNodePtr();
	//void SetNodePtr(xmlNodePtr pnode);
	CUUID GetRecordId();
	string GetName();
	virtual void SetName(string pname);
	virtual ~HisBase();
	IHisBase* GetParent();
	void SetParent(IHisBase* pParent);
	template<class T>
	vector<T*> GetItems()
	{
		//STACK
		vector<T*> result;
		for(size_t i=0;i<items.size();i++)
		{
			STACK_VAL("i",Converter::itos(i))
			T* it = dynamic_cast<T*>(items[i]);
			if (it!=NULL) result.push_back(it);
		}
		return result;
	}

};

#endif /* HISBASE_H_ */
