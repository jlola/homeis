/*
 * HisDevTemperature.h
 *
 *  Created on: Mar 10, 2013
 *      Author: Josef Lola
 */

#ifndef HISDEVTEMP18B20_H_
#define HISDEVTEMP18B20_H_
#include <unistd.h>
#include "LOW_device.h"
#include "LOW_devDS1820.h"
#include "LOW_exception.h"
#include "logger.h"

#include "HisDevBase.h"
#include "HisDallas.h"
#include "HisDevValue.h"
#include "PoppyDebugTools.h"
#include "Common/CUUID.h"


class HisDevTemp18B20 : public HisDevDallas
{
	class_DERIVE_FROM_EXCEPTION( data_error, LOW_exception);

	LOW_devDS1820 *tempdev;
	HisDevValue<double>* tempValue;
	bool doConversion;

	public:	HisDevTemp18B20(LOW_devDS1820* pdev) :
		HisDevDallas::HisDevDallas(pdev)
	{
		STACK
		doConversion = true;
		tempdev = pdev;
		CreateDataPoints();
	}

	public:	HisDevTemp18B20(xmlNodePtr node,LOW_devDS1820* pdev) :
			HisDevDallas::HisDevDallas(node,pdev)
	{
		STACK
		tempdev = pdev;
		doConversion = true;
		tempValue = NULL;
	}

	private: void CreateDataPoints()
	{
		STACK
		std::string strid = tempdev->getID().getRomIDString();
		//create data points
		//xmlNodePtr childNode = GetOrCreateNode(0,GetNodePtr());
		tempValue = new HisDevValue<double>(strid,EHisDevDirection::Read,EDataType::Double,0,0);
		WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevTemp18B20, &HisDevTemp18B20::WriteToDevice>(this);
		tempValue->delegateWrite = delegate;
		tempValue->Load();
		Add(tempValue);
	}

	const xmlChar* GetNodeNameInternal()
	{
		return (const xmlChar*)"HisDevTemp18B20";
	}

	private: void WriteToDevice(ValueChangedEventArgs args)
	{

	}

	public: static double GetDouble(uint16_t pValue)
	{
		STACK
		uint8_t signedflag = pValue >> 12;

		if (signedflag)
			pValue = 0xFFFF - pValue + 1;

		double result = pValue >>4;

		if (pValue & 0x08) result += 0.5;
		if (pValue & 0x04) result += 0.25;
		if (pValue & 0x02) result += 0.125;
		if (pValue & 0x01) result += 0.0625;

		if (signedflag) result = result*-1;

		return result;
	}

	//=====================================================================================
	//
	// methods
	//

	double getTemperature( const bool inDoConversion)
	{
		STACK
	  LOW_devDS1820::scratchpadDS1820_t  scratchpad;


	  if ( inDoConversion )
	  {
		  tempdev->cmd_ConvertT();
	  }

	  tempdev->cmd_ReadScratchpad( &scratchpad);


	  if ( scratchpad.tempLSB==0xaa && scratchpad.tempMSB==0x00 )
	  {
		  std::string message = "DS18B20 - Illeagal data in scratchpad ";
		  message += tempdev->getID().getRomIDString();
		  throw data_error(message.c_str() , __FILE__, __LINE__);
	  }

	  //int16_t halfDegBit = scratchpad.tempLSB & 0x01;
	  int16_t intPart    = (scratchpad.tempMSB<<8) | scratchpad.tempLSB;//(scratchpad.tempMSB==0x00?0x00:0x80) | (scratchpad.tempLSB>>1);
	  //float   floatPart  = -0.25 + (static_cast<float>(scratchpad.cntPerCelsius-scratchpad.cntRemain))/(static_cast<float>(scratchpad.cntPerCelsius));

	  //return (static_cast<float>(intPart))+floatPart;
	  return GetDouble(intPart);
	}

	void DoInternalLoad(xmlNodePtr & node)
	{
		STACK
		HisDevDallas::DoInternalLoad(node);
		WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevTemp18B20, &HisDevTemp18B20::WriteToDevice>(this);

		vector<HisDevValue<double>*> values = GetItems<HisDevValue<double>>();

		for(size_t i=0;i<values.size();i++)
		{
			HisDevValue<double> *value = values[i];
			value->delegateWrite = delegate;
			tempValue = value;
		}
	}

	protected: virtual void DoInternalRefresh(bool alarm)
	{
		STACK
		double tempvalue = 0;
		try
		{
			if (doConversion) doConversion = false;
			else doConversion = true;

			if (alarm)
			{
				int8_t high = 127;
				int8_t low = -127;
				tempdev->cmd_WriteScratchpad(low,high);
			}
			tempvalue = getTemperature(doConversion);
			tempValue->ReadedValueFromDevice(tempvalue,false);
			SetError(false);
		}
		catch(LOW_exception & ex)
		{
			string msg = GetName() + " | " + ex.message;
			//CLogger::Error(msg.c_str());
			SetError(true);
			tempValue->ReadedValueFromDevice(tempvalue,true);
		}
	}

	protected: virtual ~HisDevTemp18B20()
	{
	}
};

#endif /* HISDEVTEMP18B20_H_ */
