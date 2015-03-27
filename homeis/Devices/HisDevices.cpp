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

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "LOW_netSegment.h"
#include "LOW_network.h"
#include "LOW_device.h"
#include "LOW_deviceFactory.h"

#include "HisDevBase.h"
#include "HisDevValue.h"
#include "HisDallas.h"
#include "HisDevFactory.h"
#include "VirtualDevices/HisDevVirtual.h"
#include "HisDevices.h"

void HisDevices::Scan()
{
	network->searchDevices<LOW_device>(false);
	AddScanned();
}

HisDevices::HisDevices(string fileName,LOW_network* pnetwork)
{
	devicesFileName = fileName;
	doc = NULL;
	network=pnetwork;
	this->__expressionMutex = HisLock::CreateMutex();
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
	xmlNodePtr root_node = NULL;/* node pointers */
	//xmlDtdPtr dtd = NULL;       /* DTD pointer */

	LIBXML_TEST_VERSION;

	if (devices.size()>0)
	{
		for(int i = devices.size() - 1; i >= 0; i--)
		{
			this->Delete(i);
		}
	}
	//network->searchDevices<LOW_device>(false);

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
		LOW_device* dev = NULL;

		try
		{
			if (!HisDevVirtual::IsInternal(cur))
			{
				id = HisDevDallas::GetId(cur);
				if (!network->ContainsDevice<LOW_device>(id))
				{
					cout << "test"+id.getRomIDString();
					dev = LOW_deviceFactory::new_SpecificDevice(*(network->getSegments()[0]),id);
				}
				else
				{
					cout << id.getRomIDString() + "\n";
					dev = network->getDevice<LOW_device>(id);
				}
				newhisdev = HisDevFactory::Instance().Create(cur,dev);
			}
			else
			{
				newhisdev = (HisDevBase*)new HisDevVirtual(cur);
			}
			newhisdev->Load();
			devices.push_back(newhisdev);
		}
		catch(LOW_netSegment::noDevice_error & ex)
		{
			cout << "Error "+id.getRomIDString();
			CLogger::Info(string("No device error id: "+id.getRomIDString()).c_str());
		}
		catch(...)
		{
			cout << "Error "+id.getRomIDString();
			CLogger::Info(string("General error id: "+id.getRomIDString()).c_str());
		}

		cur = cur->next;
	}
}

void HisDevices::Add(HisDevBase *hisdev)
{
	xmlNodePtr root_node = xmlDocGetRootElement(doc);
	//xmlNodePtr cur = NULL;
	if (root_node==NULL)
	{
		root_node = xmlNewNode(NULL, BAD_CAST "devices");
		xmlDocSetRootElement(doc, root_node);
	}

	xmlAddChild(root_node,hisdev->GetNodePtr());
	devices.push_back(hisdev);
}

void HisDevices::AddScanned()
{

	std::vector<LOW_device*> lowdevices = network->getDevices<LOW_device>();
	size_t found = lowdevices.size();
	//create new devices
	for(size_t i=0;i<found;i++)
	{
		LOW_device* pdev = lowdevices[i];
		try
		{
			int finded = Find(pdev->getID());
			if (finded<0)
			{
				HisDevDallas* hisdev = (HisDevDallas*)HisDevFactory::Instance().Create(pdev);
				if (hisdev!=NULL)
					Add(hisdev);
				else
					CLogger::Error("Unsupported family code: %i",pdev->getFamilyCode());
			}
		}
		catch(...)
		{
			CLogger::Info("Error add scaned device");

		}
	}
}

size_t HisDevices::Size()
{
	return devices.size();
}

void HisDevices::Delete(uint16_t index)
{

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
	for(uint i=0;i<devices.size();i++)
	{
		devices[i]->Refresh();
		usleep(1000);
	}
}

int HisDevices::Find(LOW_deviceID pid)
{
	for(uint i=0;i<devices.size();i++)
	{
		HisDevDallas* dallas = dynamic_cast<HisDevDallas*>(devices[i]);
		if (dallas!=NULL)
		{
			LOW_deviceID id =  dallas->GetId();
			if (id==pid) return i;
		}
	}

	return -1;
}

int HisDevices::Find(CUUID recordId)
{
	for(uint i=0;i<devices.size();i++)
	{
		if (devices[i]->GetRecordId()==recordId) return i;
	}

	return -1;
}

long HisDevices::GetNextDelayTimeMS()
{
	SortByNextScanTime();
	timeval start = devices[0]->GetNextScanTime();
	timeval stop = HisDateTime::Now();
	timeval result;
	timersub(&stop,&start,&result);
	return HisDateTime::timeval_to_usec(result);
}

//bool comparerecordid (HisDevBase* i,HisDevBase* j) { return (i->GetRecordId()<j->GetRecordId()); }

bool compareNextScanTime (HisDevBase* i,HisDevBase* j) {
	timeval it = i->GetNextScanTime();
	timeval jt = j->GetNextScanTime();
	return timercmp(&it, &jt, > );
}

//void HisDevices::SortByRecordId()
//{
//	sort(devices.begin(),devices.end(),comparerecordid);
//}

void HisDevices::SortByNextScanTime()
{
	sort(devices.begin(),devices.end(),compareNextScanTime);
}

void HisDevices::Save()
{
	for(uint i=0;i<devices.size();i++)
	{
		devices[i]->Save();
	}

	FILE* f = fopen(devicesFileName.c_str(),"w");
	if (f != NULL)
	{
		xmlDocDump( f, doc);
		fclose( f );
	}
}
