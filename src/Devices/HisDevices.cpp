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
#include "VirtualDevices/HisDevVirtual.h"
#include "HisDevFactory.h"
#include "PoppyDebugTools.h"
#include "HisDevModbus.h"
#include "logger.h"
#include "HisDevices.h"
#include "ModbusSimulator.h"
#include "BlockingQueue.h"
#include "HisException.h"


HisDevices::HisDevices(string fileName ,IModbusProvider* modbusProvider,IBlockingQueue<HisDevBase*>* deviceQueue)
 : logger(CLogger::GetLogger())
{
	if (deviceQueue==NULL)
		throw ArgumentNullException("deviceQueue");
	if (modbusProvider==NULL)
			throw ArgumentNullException("modbusProvider");

	this->modbusProvider = modbusProvider;
	devicesFileName = fileName;
	doc = NULL;
	onRefreshdelegate = OnRefreshDelegate::from_method<HisDevices, &HisDevices::AddToRefreshQueue>(this);
	this->deviceQueue = deviceQueue;
	RegisterToModbus();
}

void HisDevices::RegisterToModbus()
{
	auto connectors = modbusProvider->GetConnectors();
	for(size_t i=0;i<connectors.size();i++)
	{
		connectors[i]->AddConsumer(this);
	}
}

void HisDevices::AddToRefreshQueue(HisDevBase* hisDevBase)
{
	STACK
	this->deviceQueue->Push(hisDevBase);
}

HisDevBase *HisDevices::operator[](unsigned int i)
{
	if (devices.size()>i)
	{
		return devices[i];
	}
	return NULL;
}

void HisDevices::FireEvent(int adr)
{
	STACK

	auto devIndex = FindModbusDev(adr);
	if (devIndex >=0)
	{
		auto dev = this->operator [](devIndex);
		dev->SetAlarm();
		this->deviceQueue->Push(dev);
	}

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

void HisDevices::Load(IHisDevFactory* factory)
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
					newhisdev = (HisDevBase*)new HisDevModbus(cur,modbusProvider,factory);
				}
				else if (HisDevVirtual::IsInternal(cur))
				{
					newhisdev = (HisDevBase*)new HisDevVirtual(cur,factory);
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
			logger.Info(message.c_str());
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
	STACK
	for(uint i=0;i<devices.size();i++)
	{
		if (devices[i]->IsEnabled())
			devices[i]->RefreshTime(deviceQueue,false);

//		HisDevBase* alarmedDevice = ReadAlarmDevice();
//		if (alarmedDevice!=NULL && alarmedDevice->IsEnabled())
//			alarmedDevice->Refresh(true);

//		while (devqueue.size()>0)
//		{
//			logger.Trace("Refresh before refreshMutex.Lock()");
//			refreshMutex.Lock();
//			logger .Trace("Refresh after refreshMutex.Lock()");
//			dev = devqueue.front();
//			devqueue.pop();
//			logger.Trace("Refresh before refreshMutex.Unlock()");
//			refreshMutex.Unlock();
//			logger.Trace("Refresh after refreshMutex.Unlock()");
//			logger.Trace("Refresh from queue: %s" ,dev->GetName().c_str());
//			if (dev->IsEnabled())
//				dev->Refresh(false);
//
//		}
		usleep(1000);
	}
}


//HisDevBase* HisDevices::ReadAlarmDevice(IModbus* modbus)
//{
//	auto connectors = modbusProvider->GetConnectors();
//	for (size_t i=0;i<connectors.size();i++)
//	{
//		if (connectors[i]->GetDriverName()!=ModbusSimulator::DriverName)
//		{
//			auto addr = connectors[i]->WaitForAddress();
//			if (addr > 1)
//			{
//				usleep(10000);
//				//reset by set 1
//				if (connectors[i]->setHolding(addr,CHANGE_FLAG,1))
//				{
//					CLogger::GetLogger().Info("Alarmed device addr: %d",addr);
//					int i = FindModbusDev(addr);
//					if (i>=0) return devices[i];
//				}
//			}
//		}
//	}
//	return NULL;
//}

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
		logger.Error("Error while writing devices.xml");
		throw std::domain_error("Error while writing devices.xml");
	}
}
