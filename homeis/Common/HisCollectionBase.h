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

using namespace std;

class HisCollectionBase
{
	xmlDocPtr doc;       /* document pointer */
	string fileName;
	vector<HisBase*> items;
protected:
	virtual string GetRootNodeName()=0;
	virtual HisBase* LoadItem(xmlNodePtr cur)=0;
public:
	size_t FindIndex(CUUID Id);
	void Delete(size_t index);
	virtual void Add(HisBase *hisBase);
	virtual void Load();
	virtual void Save();
	size_t Size();
	HisCollectionBase(std::string fileName);
	HisBase *operator[](size_t i);
	~HisCollectionBase();
};

#endif /* HISCOLLECTIONBASE_H_ */
