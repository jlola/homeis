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

HisDevValueEmail::HisDevValueEmail(string subject,string fromAddr,string receivers,
		IWriteToDevice* deviceWriter, IHisDevFactory* factory	) :
		HisDevValue::HisDevValue(subject,
				EHisDevDirection::Write,
				EDataType::Email,
				0,
				"",
				"Email",
				factory,
				deviceWriter)
{
	this->emailSender = factory->GetEmailSender();
	if (this->emailSender==NULL)
		throw ArgumentNullException("emailSender");
	this->fromAddr = fromAddr;
	this->receivers = receivers;
}

HisDevValueEmail::HisDevValueEmail(xmlNodePtr node,
			IWriteToDevice* deviceWriter,
			IHisDevFactory* factory) :
					HisDevValue::HisDevValue(node,
							"",
							factory,
							deviceWriter)
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

void HisDevValueEmail::SetFromAddr(string fromAddr)
{
	this->fromAddr = fromAddr;
}

string HisDevValueEmail::GetFromAddr()
{
	return fromAddr;
}

void HisDevValueEmail::SetReceivers(string receivers)
{
	this->receivers = receivers;
}

string HisDevValueEmail::GetReceivers()
{
	return receivers;
}

void HisDevValueEmail::SetSubject(string subject)
{
	SetName(subject);
}

string HisDevValueEmail::GetSubject()
{
	return GetName();
}

void HisDevValueEmail::SetValue(string body)
{

	if ( GetValue()!=body && body.length() > 0)
	{
		HisDevValue<string>::SetValue(body);
		emailSender->Send(this->fromAddr,receivers,GetName(),body);
	}
}

void HisDevValueEmail::ForceValue(string body)
{
	if ( GetValue()!=body && body.length() > 0)
	{
		HisDevValue<string>::ForceValue(body);
		emailSender->Send(this->fromAddr,receivers,GetName(),body);
	}
}

HisDevValueEmail::~HisDevValueEmail() {
}

