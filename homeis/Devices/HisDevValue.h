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

#include "logger.h"
#include "converter.h"
#include "EDataType.h"
#include "EHisDevDirection.h"
#include "ValueEventArgs.h"
#include "homeis/Common/HisBase.h"

#define NODE_VALUE BAD_CAST "value"
#define PAR_PINNO BAD_CAST "pinno"
#define PAR_PINNAME BAD_CAST"pinname"
#define PAR_VALUE BAD_CAST"value"
#define PAR_DATATYPE BAD_CAST"DataType"
#define PAR_DEV_ADDR BAD_CAST "devaddr"
#define PAR_DIRECTION BAD_CAST "direction"
#define PAR_UNIT BAD_CAST "unit"

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
	uint16_t pinNumber;
	EHisDevDirection direction;
	EDataType datatype;
	OnValueChangedDelegate ValueChanged;
	map<void*,OnValueChangedDelegate> delegatesMap;
protected:
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
	HisDevValueBase(std::string pdevname, EHisDevDirection direct, EDataType pdatatype,int ppinNumber);

	HisDevValueBase(xmlNodePtr pnode);

	HisDevValueBase(HisDevValueBase & src);

	void FireOnValueChanged(ValueChangedEventArgs args);

	void Register(OnValueChangedDelegate delegate, void* owner);

	void UnRegister(void* owner);

	std::string GetAddress();

	EHisDevDirection GetDirection();

	EDataType GetDataType();

	uint16_t GetPinNumber();

	string GetUnit();

	void SetUnit(string pUnit);

	virtual ~HisDevValueBase();

	std::string GetPinName();

	std::string GetStringValue();

	bool ForceStringValue(string strvalue);

	void SetPinName(std::string pname);

	bool GetDeviceError();

	bool GetForceOutput();

	void SetForceOutput(bool force);

	static HisDevValueBase* Create(xmlNodePtr pNode);
};

template<class T>
class HisDevValue : public HisDevValueBase
{
private:
	T value;
	T oldValue;
public:
	HisDevValue(std::string addr, EHisDevDirection direct, EDataType pdatatype,int pPinNumber,T defaultValue) :
		HisDevValueBase::HisDevValueBase(addr, direct, pdatatype,pPinNumber),value(defaultValue),oldValue(defaultValue)
	{

	}

	HisDevValue(xmlNodePtr pnode,T defaultValue) :
		HisDevValueBase::HisDevValueBase(pnode),value(defaultValue),oldValue(defaultValue)
	{
	}

	/*
	 * read value from this object to world
	 */

	T GetValue()
	{
		return value;
	}



	/*
	 * set value from application to this object and should be written to device
	 */

	void SetValue(T pValue)
	{

		if (allowForceOutput)
		{
			CLogger::Info("allowForce is enabled. New value will be ignored");
			return;
		}
		if (value!=pValue)
		{
			oldValue = value;
			value = pValue;
			if (GetDirection()==EHisDevDirection::Write || GetDirection()==EHisDevDirection::ReadWrite)
			{
				ValueChangedEventArgs args(this);
				if (delegateWrite) delegateWrite(args);
			}
			else
			{
				CLogger::Error("Try to write to not writable tag");
			}
		}
	}

	/*
	 * working only if allowForceOutput = true
	 * forcing output and new value will not be historized
	 */
	void ForceValue(T pValue)
	{
		if (allowForceOutput)
		{
			value = pValue;
			if (GetDirection()==EHisDevDirection::Write || GetDirection()==EHisDevDirection::ReadWrite)
			{
				ValueChangedEventArgs args(this);
				if (delegateWrite) delegateWrite(args);
			}
			else if (GetDirection()==EHisDevDirection::Read)
			{
				SetValueFromDevice(value,false);
			}
			else
			{
				CLogger::Error("Try to write to not writable tag");
			}
		}
	}

	/*
	 * reads from this object value and write it to device
	 * this is called by device
	 */

	T GetValueFromDevice()
	{
		return value;
	}

	/*
	 * reads from device and write it to this object, this is called by device
	 * if is value changed there is fired event
	 */

	void SetValueFromDevice(T pvalue, bool perror)
	{
		deviceError = perror;
		if (allowForceOutput) return;
		if (!perror)
			value = pvalue;
		if (perror && !deviceError)
		{
			std::string msg = "Error on dev " + devaddr + "Pin: " + this->GetName();
			CLogger::Error(msg.c_str());
		}
		else if (value!=pvalue)
		{
			ValueChangedEventArgs args(this);
			FireOnValueChanged(args);
		}
	}
};

#endif /* HISDEVVALUE_H_ */
