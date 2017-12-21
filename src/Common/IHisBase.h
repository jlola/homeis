/*
 * IHisBase.h
 *
 *  Created on: 16. 12. 2017
 *      Author: pc
 */

#ifndef SRC_COMMON_IHISBASE_H_
#define SRC_COMMON_IHISBASE_H_

#include <string>

#include "DateTime.h"
#include "CUUID.h"

using namespace std;

class IHisBase
{
public:
	typedef bool FindProc(IHisBase* hisbase,void* args);
	virtual void Add(IHisBase *pitem)=0;
	virtual CUUID GetRecordId()=0;
	virtual string GetName()=0;
	virtual const xmlChar* GetNodeName()=0;
	virtual IHisBase* FindByName(string name)=0;
	virtual IHisBase* Find(FindProc proc,void* args)=0;
	virtual xmlNodePtr GetNodePtr()=0;
	virtual IHisBase* Remove(CUUID puuid)=0;
	virtual IHisBase* Find(CUUID id)=0;
	virtual IHisBase* GetParent()=0;
	virtual void SetParent(IHisBase* pParent)=0;
	virtual DateTime GetCreateDateTime()=0;
	virtual DateTime GetModifyDateTime()=0;
	virtual void Load()=0;
	virtual void Save()=0;

	virtual ~IHisBase(){}
};


#endif /* SRC_COMMON_IHISBASE_H_ */
