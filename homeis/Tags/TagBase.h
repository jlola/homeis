/*
 * TagBase.h
 *
 *  Created on: Feb 15, 2013
 *      Author: root
 */

#ifndef TAGBASE_H_
#define TAGBASE_H_

using namespace std;

class TagValueChnagedEventArgs
{

};

#ifdef SRUTIL_DELEGATE_PREFERRED_SYNTAX
typedef srutil::delegate<void (ValueChnagedEventArgs)> OnEditDelegate;
#else
 typedef srutil::delegate1<void, TagValueChnagedEventArgs> OnTagValueChangedDelegate;
#endif


class TagBase {
private:
	bool enabled;
	string name;
	int deviceId;
	EDataType type;
	HisDevBase  *dev;
public:
	TagBase(HisDevBase  *pDev,EDataType dataType);
	void OnValueChanged(ValueChangedEventArgs args);
	OnTagValueChangedDelegate ValueChanged;
	void Enable(bool enable);
	void Refresh();
	void Save();
	void Load();
};

#endif /* TAGBASE_H_ */
