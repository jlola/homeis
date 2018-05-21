/*
 * HisDevValueBase.h
 *
 *  Created on: 21. 4. 2018
 *      Author: pc
 */

#ifndef SRC_DEVICES_HISDEVVALUEBASE_H_
#define SRC_DEVICES_HISDEVVALUEBASE_H_

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

class HisDevValueBase : public HisBase
{
	 ILogger & logger;
	string addressName;
	string pinNumber;
	EHisDevDirection direction;
	EDataType datatype;
	std::string loadtype;
	map<void*,OnValueChangedDelegate> delegatesMap;
protected:
	IWriteToDevice* deviceWriter;
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
//public:	WriteToDeviceRequestDelegate delegateWrite;
public:
	HisDevValueBase(std::string pdevname,
			EHisDevDirection direct,
			EDataType pdatatype,
			string ppinNumber,
			string loadType,
			IHisDevFactory* factory,
			IWriteToDevice* devHandler);

	HisDevValueBase(xmlNodePtr pnode,
			IHisDevFactory* factory,
			IWriteToDevice* devHandler);

	HisDevValueBase(HisDevValueBase & src,
			IHisDevFactory* factory,
			IWriteToDevice* devHandler);

	void SetDevice(IWriteToDevice* handler);

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

	static HisDevValueBase* Create(xmlNodePtr pNode,
			IHisDevFactory* factory,
			IWriteToDevice* handler);
};

#endif /* SRC_DEVICES_HISDEVVALUEBASE_H_ */
