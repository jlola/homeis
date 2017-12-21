/*
 * HisDevices.cpp
 *
 *  Created on: Feb 16, 2013
 *      Author: Josef Lola
 */

#include <iostream>
#include <algorithm>    // std::sort
#include <string>
#include <vector>
#include <unistd.h>

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <semaphore.h>
#include "HisDevBase.h"
#include "HisDevValue.h"
#include "HisDevFactory.h"
#include "VirtualDevices/HisDevVirtual.h"
#include "PoppyDebugTools.h"
#include "HisDevModbus.h"

#include "HisDevices.h"

HisDevices::HisDevices(string fileName ,IModbusProvider* modbusProvider)
{
	this->modbusProvider = modbusProvider;
	devicesFileName = fileName;
	doc = NULL;
	this->__devRefreshMutex = HisLock::CreateMutex();
	onRefreshdelegate = OnRefreshDelegate::from_method<HisDevices, &HisDevices::AddToRefreshQueue>(this);
}

void HisDevices::AddToRefreshQueue(HisDevBase* hisDevBase)
{
	STACK
	__devRefreshMutex->lock();
	//CLogger::Info("Add %s to refresh queue." ,hisDevBase->GetName().c_str());
	devqueue.push(hisDevBase);
	__devRefreshMutex->unlock();
}

HisDevBase *HisDevices::operator[](unsigned int i)
{
	if (devices.size()>i)
	{
		return devices[i];
	}
	return NULL;
}

HisDevices::~HisDevices()
{
	STACK
	for(uint32_t i=0;i<devices.size();i++)
		delete(devices[i]);
	devices.clear();

	/*free the document */
	xmlFreeDoc(doc);

	 /*
	 *Free the global variables that may
	 *have been allocated by the parser.
	 */
	xmlCleanupParser();
}

void HisDevices::Load()
{
	STACK
	xmlNodePtr root_node = NULL;/* node pointers */

	LIBXML_TEST_VERSION;

	if (devices.size()>0)
	{
		for(int i = devices.size() - 1; i >= 0; i--)
		{
			this->Delete(i);
		}
	}

	if (doc!=NULL)
	{
		/*free the document */
		xmlFreeDoc(doc);

		 /*
		 *Free the global variables that may
		 *have been allocated by the parser.
		 */
		xmlCleanupParser();

		doc = NULL;
	}

	/*
	 * Creates a new document, a node and set it as a root node
	 */
	doc = xmlReadFile(devicesFileName.c_str(), NULL, 0);
	if (doc == NULL) {
		doc = xmlNewDoc(BAD_CAST "1.0");
	}

	root_node = xmlDocGetRootElement(doc);
	if (root_node==NULL)
	{
		root_node = xmlNewNode(NULL, BAD_CAST "devices");
		xmlDocSetRootElement(doc, root_node);
	}
	//load saved devices
	LOW_deviceID id;
	xmlNodePtr cur = root_node->children;
	HisDevBase* newhisdev = NULL;

	while (cur != NULL) {
		//LOW_device* dev = NULL;

		try
		{
			if (cur->type==XML_ELEMENT_NODE)
			{
				if (HisDevModbus::Resolve(cur))
				{
					newhisdev = (HisDevBase*)new HisDevModbus(cur,modbusProvider);
				}
				else if (HisDevVirtual::IsInternal(cur))
				{
					newhisdev = (HisDevBase*)new HisDevVirtual(cur);
				}
				if (newhisdev!=NULL)
				{
					newhisdev->Load();
					newhisdev->OnRefresh = onRefreshdelegate;
					devices.push_back(newhisdev);
				}
			}
		}
		catch(...)
		{
			string message = "General error on element: ";
			message += (const char*)cur->name;
			cout <<  message;
			CLogger::Info(message.c_str());
		}

		cur = cur->next;
	}
}

void HisDevices::Add(HisDevBase *hisdev)
{
	STACK
	xmlNodePtr root_node = xmlDocGetRootElement(doc);
	if (root_node==NULL)
	{
		root_node = xmlNewNode(NULL, BAD_CAST "devices");
		xmlDocSetRootElement(doc, root_node);
	}

	xmlAddChild(root_node,hisdev->GetNodePtr());
	hisdev->OnRefresh = onRefreshdelegate;
	devices.push_back(hisdev);
}

size_t HisDevices::Size()
{

	return devices.size();
}

void HisDevices::Delete(uint16_t index)
{
	STACK
	uint16_t size = devices.size();
	if (size>index)
	{
		HisDevBase* dev = devices[index];
		//xmlNodePtr node = dev->GetNodePtr();
		devices.erase(devices.begin() + index);
		delete(dev);
	}
}

HisDevValueBase* HisDevices::FindValue(string address)
{
	STACK
	for(uint16_t i=0;i<devices.size();i++)
	{
		vector<HisDevValueBase*> tagvalues = devices[i]->GetValues();
		for(uint16_t v=0;v<tagvalues.size();v++)
		{
			HisDevValueBase* devValue = tagvalues[v];
			if (devValue->GetAddress()==address)
			{
				return devValue;
			}
		}
	}
	return NULL;
}

HisDevValueBase* HisDevices::FindValue(CUUID valueId)
{
	//STACK
	for(uint16_t i=0;i<devices.size();i++)
	{
		vector<HisDevValueBase*> tagvalues = devices[i]->GetValues();
		for(uint16_t v=0;v<tagvalues.size();v++)
		{
			HisDevValueBase* devValue = tagvalues[v];
			if (devValue->GetRecordId()==valueId)
			{
				return devValue;
			}
		}
	}
	return NULL;
}

void HisDevices::Refresh()
{
	HisDevBase* dev = NULL;
	STACK
	for(uint i=0;i<devices.size();i++)
	{
		if (devices[i]->IsEnabled())
			devices[i]->Refresh(false);

		while (devqueue.size()>0)
		{
			__devRefreshMutex->lock();
			dev = devqueue.front();
			devqueue.pop();
			__devRefreshMutex->unlock();
			//CLogger::Info("Refresh from queue: %s" ,dev->GetName().c_str());
			if (dev->IsEnabled())
				dev->Refresh(false);

		}
		usleep(1000);
	}
}

int HisDevices::FindModbusDev(int addressId)
{
	for(uint i=0;i<devices.size();i++)
	{
		HisDevModbus* modbusdev = dynamic_cast<HisDevModbus*>(devices[i]);
		if (modbusdev!=NULL)
		{
			if (modbusdev->GetAddress()==addressId)
				return i;
		}
	}
	return -1;
}

int HisDevices::Find(CUUID recordId)
{
	STACK
	for(uint i=0;i<devices.size();i++)
	{
		if (devices[i]->GetRecordId()==recordId) return i;
	}

	return -1;
}

void HisDevices::Save()
{
	STACK
	for(uint i=0;i<devices.size();i++)
	{
		devices[i]->Save();
	}

	int result = xmlSaveFormatFileEnc( devicesFileName.c_str(), doc,"UTF-8", 1);

	if (result<0)
	{
		CLogger::Error("Error while writing devices.xml");
		throw std::domain_error("Error while writing devices.xml");
	}
}
