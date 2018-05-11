/*
 * HisDevValueEmail.cpp
 *
 *  Created on: 21. 4. 2018
 *      Author: pc
 */

#include "PoppyDebugTools.h"
#include "HisException.h"
#include "StringBuilder.h"
#include "HisDevValueEmail.h"

HisDevValueEmail::HisDevValueEmail(std::string addr,
		IHisDevFactory* factory	) :
		HisDevValue::HisDevValue(addr,
				EHisDevDirection::Write,
				EDataType::Email,
				0,
				"",
				"Email",
				factory,
				devHandler)
{
	this->emailSender = factory->GetEmailSender();
	if (this->emailSender==NULL)
		throw ArgumentNullException("emailSender");
}

HisDevValueEmail::HisDevValueEmail(xmlNodePtr node,
			IHisDevFactory* factory,
			IWriteToDevice* devHandler) :
					HisDevValue::HisDevValue(node,
							"",
							factory,
							devHandler)
{
	this->emailSender = factory->GetEmailSender();
	if (this->emailSender==NULL)
		throw ArgumentNullException("emailSender");
}

void HisDevValueEmail::DoInternalSave(xmlNodePtr & node)
{
	STACK
	HisDevValue<string>::DoInternalSave(node);

	if (node != NULL)
	{
		xmlSetProp(node,NODE_FROMADDR,(xmlChar*)fromAddr.c_str());
		xmlSetProp(node,NODE_RECEIVERS,(xmlChar*)receivers.c_str());
	}
}

void HisDevValueEmail::DoInternalLoad(xmlNodePtr & node)
{
	STACK
	if (node != NULL)
	{
		HisDevValue<string>::DoInternalLoad(node);

		xmlChar* prop;

		if (xmlHasProp(node,NODE_FROMADDR))
		{
			prop = xmlGetProp(node,NODE_FROMADDR);
			fromAddr = (char*)prop;
			xmlFree(prop);
		}

		if (xmlHasProp(node,NODE_RECEIVERS))
		{
			prop = xmlGetProp(node,NODE_RECEIVERS);
			receivers = (char*)prop;
			xmlFree(prop);
		}
	}
}

void HisDevValueEmail::SetValue(string body)
{
	HisDevValue<string>::SetValue(body);

	emailSender->Send(this->fromAddr,StringBuilder::split(fromAddr,';'),GetName(),body);
}

HisDevValueEmail::~HisDevValueEmail() {
}

