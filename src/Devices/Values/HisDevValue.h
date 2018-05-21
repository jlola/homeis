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
#include "IWriteToDevice.h"

#include "HisDevValueBase.h"

//#ifdef SRUTIL_DELEGATE_PREFERRED_SYNTAX
//typedef srutil::delegate<void (ValueChangedEventArgs)> OnEditDelegate;
//#else
// typedef srutil::delegate1<void,ValueChangedEventArgs> WriteToDeviceRequestDelegate;
//#endif

 template<class T> class HisDevValue;

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

protected:
	virtual void DoInternalSave(xmlNodePtr & node)
	{
		HisDevValueBase::DoInternalSave(node);
	}

	virtual void DoInternalLoad(xmlNodePtr & node)
	{
		HisDevValueBase::DoInternalLoad(node);
	}
public:
	/*
	 * addr - addres of device
	 * dircet - direction
	 * pinnumer - unicat number of pin
	 * */
	HisDevValue(std::string addr,
			EHisDevDirection direct,
			EDataType pdatatype,
			string pPinNumber,
			T defaultValue,
			string loadType,
			IHisDevFactory* factory,
			IWriteToDevice* deviceWriter
			) :
		HisDevValueBase::HisDevValueBase(
				addr,
				direct,
				pdatatype,
				pPinNumber,
				loadType,
				factory,
				deviceWriter),
				value(defaultValue),
				oldValue(defaultValue)
	{

	}

	HisDevValue(std::string addr,
			EHisDevDirection direct,
			EDataType pdatatype,
			int pPinNumber,
			T defaultValue,
			string loadType,
			IHisDevFactory* factory,
			IWriteToDevice* deviceWriter) :
		HisDevValue::HisDevValue(
				addr,
				direct,
				pdatatype,
				Converter::itos(pPinNumber),
				defaultValue,
				loadType,
				factory,
				deviceWriter)
	{

	}

	HisDevValue(xmlNodePtr pnode,
			T defaultValue,
			IHisDevFactory* factory,
			IWriteToDevice* devHandler) :
		HisDevValueBase::HisDevValueBase(pnode,factory,devHandler),
		value(defaultValue),
		oldValue(defaultValue)
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
	virtual void SetValue(T pValue)
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
				if (deviceWriter!=NULL) deviceWriter->WriteToDevice(args);
			}
		}
	}

	/*
	 * forcing output and new value will not be historized
	 */
	virtual void ForceValue(T pValue)
	{
		//STACK
		//if (allowForceOutput)
		{
			value = pValue;
			valueChangeTime = DateTime::Now();
			if (GetDirection()==EHisDevDirection::Write || GetDirection()==EHisDevDirection::ReadWrite)
			{
				ValueChangedEventArgs args(this);
				if (deviceWriter!=NULL) deviceWriter->WriteToDevice(args);
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

	virtual void ReadedValueFromDevice(T pvalue, bool perror)
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
