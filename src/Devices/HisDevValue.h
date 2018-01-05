/*
 * HisDevValue.h
 *
 *  Created on: Mar 20, 2013
 *      Author: Josef Lola
 */

#ifndef HISDEVVALUE_H_
#define HISDEVVALUE_H_

#include <map>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "srutil/event/event.hpp"
#include "srutil/delegate/delegate.hpp"
#include "Common/DateTime.h"

#include "logger.h"
#include "converter.h"
#include "EDataType.h"
#include "EHisDevDirection.h"
#include "ValueEventArgs.h"
#include "Common/HisBase.h"
#include "PoppyDebugTools.h"

using namespace std;

#define NODE_VALUE BAD_CAST "value"
#define PAR_PINNO BAD_CAST "pinno"
#define PAR_PINNAME BAD_CAST"pinname"
#define PAR_VALUE BAD_CAST"value"
#define PAR_DATATYPE BAD_CAST"DataType"
#define PAR_DEV_ADDR BAD_CAST "devaddr"
#define PAR_DIRECTION BAD_CAST "direction"
#define PAR_UNIT BAD_CAST "unit"
#define PAR_LOADTYPE BAD_CAST	"loadtype"
#define PAR_ADDRESSNAME BAD_CAST "addressname"

#ifdef SRUTIL_DELEGATE_PREFERRED_SYNTAX
typedef srutil::delegate<void (ValueChangedEventArgs)> OnEditDelegate;
#else
 typedef srutil::delegate1<void, ValueChangedEventArgs> OnValueChangedDelegate;
#endif

#ifdef SRUTIL_DELEGATE_PREFERRED_SYNTAX
typedef srutil::delegate<void (ValueChangedEventArgs)> OnEditDelegate;
#else
 typedef srutil::delegate1<void,ValueChangedEventArgs> WriteToDeviceRequestDelegate;
#endif

 template<class T> class HisDevValue;

class HisDevValueBase : public HisBase
{
	string addressName;
	string pinNumber;
	EHisDevDirection direction;
	EDataType datatype;
	OnValueChangedDelegate ValueChanged;
	std::string loadtype;
	map<void*,OnValueChangedDelegate> delegatesMap;

protected:
	DateTime valueChangeTime;
	std::string unit;
	//std::string pinname;
	std::string devaddr;
	//xmlNodePtr node;
	bool deviceError;
	bool allowForceOutput;
	virtual void DoInternalSave(xmlNodePtr & node);
	virtual void DoInternalLoad(xmlNodePtr & node);
	virtual const xmlChar* GetNodeNameInternal();
public:	WriteToDeviceRequestDelegate delegateWrite;
public:
	HisDevValueBase(std::string pdevname, EHisDevDirection direct, EDataType pdatatype,string ppinNumber,string loadType,IHisDevFactory* factory);

	HisDevValueBase(xmlNodePtr pnode,IHisDevFactory* factory);

	HisDevValueBase(HisDevValueBase & src,IHisDevFactory* factory);

	void FireOnValueChanged(ValueChangedEventArgs args);

	void Register(OnValueChangedDelegate delegate, void* owner);

	void UnRegister(void* owner);

	DateTime GetValueChangeTime();

	std::string GetLoadType();

	void SetLoadType(std::string loadtype);

	std::string GetAddress();

	EHisDevDirection GetDirection();

	EDataType GetDataType();

	string GetPinNumber();

	std::string GetAddressName();

	void SetAddressName(string pAddressName);

	string GetUnit();

	void SetUnit(string pUnit);

	virtual ~HisDevValueBase();

	std::string GetPinName();

	std::string GetStringValue();

	bool ForceStringValue(string strvalue, bool checkChange);

	void SetPinName(std::string pname);

	bool GetDeviceError();

	bool GetForceOutput();

	void SetForceOutput(bool force);

	void SetError();

	static HisDevValueBase* Create(xmlNodePtr pNode,IHisDevFactory* factory);
};

template<class T>
class HisDevValue : public HisDevValueBase
{
private:
	/*actual value
	 * if !allowforceValue - reprezents hw value
	 *  else - reprezents internal value and can be set by user
	 * */
	T value;
	/*
	 * all time reprezents hw value
	 * */
	T extvalue;

	T oldValue;
public:
	/*
	 * addr - addres of device
	 * dircet - direction
	 * pinnumer - unicat number of pin
	 * */
	HisDevValue(std::string addr, EHisDevDirection direct, EDataType pdatatype,string pPinNumber,T defaultValue,string loadType,IHisDevFactory* factory) :
		HisDevValueBase::HisDevValueBase(addr, direct, pdatatype,pPinNumber,loadType,factory),value(defaultValue),oldValue(defaultValue)
	{

	}

	HisDevValue(std::string addr, EHisDevDirection direct, EDataType pdatatype,int pPinNumber,T defaultValue,string loadType,IHisDevFactory* factory) :
		HisDevValue::HisDevValue(addr, direct, pdatatype, Converter::itos(pPinNumber),defaultValue,loadType,factory)
	{

	}

	HisDevValue(xmlNodePtr pnode,T defaultValue,IHisDevFactory* factory) :
		HisDevValueBase::HisDevValueBase(pnode,factory),value(defaultValue),oldValue(defaultValue)
	{
	}

	/*
	 * read value from this object to world
	 */

	T GetValue()
	{
		return value;
	}

	T GetExtValue()
	{
		return extvalue;
	}

	/*
	 * set value from application to this object
	 * and should be written to device if is not allowForceOutput set
	 * else is value only saved
	 */
	void SetValue(T pValue)
	{
		//STACK
		if (value!=pValue)
		{
			valueChangeTime = DateTime::Now();
			oldValue = value;
			value = pValue;

			if (!allowForceOutput && (GetDirection()==EHisDevDirection::Write || GetDirection()==EHisDevDirection::ReadWrite))
			{
				extvalue = pValue;
				ValueChangedEventArgs args(this);
				if (delegateWrite) delegateWrite(args);
			}
		}
	}

	/*
	 * forcing output and new value will not be historized
	 */
	void ForceValue(T pValue)
	{
		//STACK
		//if (allowForceOutput)
		{
			value = pValue;
			valueChangeTime = DateTime::Now();
			if (GetDirection()==EHisDevDirection::Write || GetDirection()==EHisDevDirection::ReadWrite)
			{
				ValueChangedEventArgs args(this);
				if (delegateWrite) delegateWrite(args);
			}
			else
			{
				//CLogger::Error("Try to write to not writable tag");
			}
		}
	}

	/*
	 * reads from device and write it to this object, this is called by device
	 * if is value changed there is fired event
	 */

	void ReadedValueFromDevice(T pvalue, bool perror)
	{
		//STACK
		deviceError = perror;

		if (!perror )
		{
			if (allowForceOutput)
				extvalue = pvalue;
			else
			{
				if (value!=pvalue)
				{
					value = pvalue;
					valueChangeTime = DateTime::Now();
				}
			}
		}
	}
};

#endif /* HISDEVVALUE_H_ */
