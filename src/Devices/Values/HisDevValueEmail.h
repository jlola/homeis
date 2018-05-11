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

class HisDevValueEmail : public HisDevValue<string>
{
	string fromAddr;
	string receivers;
	IEmailSender* emailSender;
public:
	HisDevValueEmail(std::string addr, IHisDevFactory* factory);
	HisDevValueEmail(xmlNodePtr pnode,
				IHisDevFactory* factory,
				IWriteToDevice* devHandler);
	void SetValue(string pValue);
	void DoInternalSave(xmlNodePtr & node);
	void DoInternalLoad(xmlNodePtr & node);
	virtual ~HisDevValueEmail();
};

#endif /* SRC_DEVICES_VALUES_HISDEVVALUEEMAIL_H_ */
