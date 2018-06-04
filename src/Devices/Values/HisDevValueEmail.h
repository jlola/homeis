/*
 * HisDevValueEmail.h
 *
 *  Created on: 21. 4. 2018
 *      Author: pc
 */

#ifndef SRC_DEVICES_VALUES_HISDEVVALUEEMAIL_H_
#define SRC_DEVICES_VALUES_HISDEVVALUEEMAIL_H_

#include "IEmailSender.h"
#include "HisDevValue.h"

#define NODE_FROMADDR BAD_CAST "fromaddr"
#define NODE_RECEIVERS BAD_CAST "receivers"

using namespace std;

class HisDevValueEmail : public HisDevValue<string>
{
	string fromAddr;
	string receivers;
	IEmailSender* emailSender;
public:
	HisDevValueEmail(IWriteToDevice* deviceWriter,
			IHisDevFactory* factory);
	HisDevValueEmail(xmlNodePtr pnode,
				IWriteToDevice* deviceWriter,
				IHisDevFactory* factory);
	void SetValue(string pValue);
	void SetFromAddr(string fromAddr);
	string GetFromAddr();
	void SetReceivers(string receivers);
	string GetReceivers();
	void SetSubject(string subject);
	void ForceValue(string pValue);
	string GetSubject();
	void DoInternalSave(xmlNodePtr & node);
	void DoInternalLoad(xmlNodePtr & node);
	virtual ~HisDevValueEmail();
};

#endif /* SRC_DEVICES_VALUES_HISDEVVALUEEMAIL_H_ */
