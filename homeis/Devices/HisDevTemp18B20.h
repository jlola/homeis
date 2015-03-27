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
#include "homeis/Common/CUUID.h"


class HisDevTemp18B20 : public HisDevDallas
{
	class_DERIVE_FROM_EXCEPTION( data_error, LOW_exception);

	LOW_devDS1820 *tempdev;
	HisDevValue<double>* tempValue;
	bool doConversion;

	public:	HisDevTemp18B20(LOW_devDS1820* pdev) :
		HisDevDallas::HisDevDallas(pdev)
	{
		doConversion = true;
		tempdev = pdev;
		CreateDataPoints();
	}

	public:	HisDevTemp18B20(xmlNodePtr node,LOW_devDS1820* pdev) :
			HisDevDallas::HisDevDallas(node,pdev)
	{
		tempdev = pdev;
		doConversion = true;
		tempValue = NULL;
	}

	private: void CreateDataPoints()
	{
		std::string strid = tempdev->getID().getRomIDString();
		//create data points
		//xmlNodePtr childNode = GetOrCreateNode(0,GetNodePtr());
		tempValue = new HisDevValue<double>(strid,EHisDevDirection::Read,EDataType::Double,0);
		WriteToDeviceRequestDelegate delegate = WriteToDeviceRequestDelegate::from_method<HisDevTemp18B20, &HisDevTemp18B20::WriteToDevice>(this);
		tempValue->delegateWrite = delegate;
		tempValue->Load();
		Add(tempValue);
	}

	private: void WriteToDevice(ValueChangedEventArgs args)
	{

	}

	double GetDouble(uint16_t pValue)
	{
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
	  LOW_devDS1820::scratchpadDS1820_t  scratchpad;


	  if ( inDoConversion )
	  {
		  tempdev->cmd_ConvertT();
	  }

	  tempdev->cmd_ReadScratchpad( &scratchpad);


	  if ( scratchpad.tempLSB==0xaa && scratchpad.tempMSB==0x00 )
	    throw data_error( "Illeagal data in scratchpad", __FILE__, __LINE__);

	  //int16_t halfDegBit = scratchpad.tempLSB & 0x01;
	  int16_t intPart    = (scratchpad.tempMSB<<8) | scratchpad.tempLSB;//(scratchpad.tempMSB==0x00?0x00:0x80) | (scratchpad.tempLSB>>1);
	  //float   floatPart  = -0.25 + (static_cast<float>(scratchpad.cntPerCelsius-scratchpad.cntRemain))/(static_cast<float>(scratchpad.cntPerCelsius));

	  //return (static_cast<float>(intPart))+floatPart;
	  return GetDouble(intPart);
	}

	void DoInternalLoad(xmlNodePtr & node)
	{
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

	protected: virtual void DoInternalRefresh()
	{
		double tempvalue = 0;
		vector<HisDevValue<double>*> values = GetItems<HisDevValue<double>>();
		try
		{
			tempvalue = getTemperature(doConversion);

			if (doConversion) doConversion = false;
			else doConversion = true;

			for (size_t i=0;i<values.size();i++)
			{
				HisDevValue<double>* val = dynamic_cast<HisDevValue<double>*>(values[i]);
				val->SetValueFromDevice(tempvalue,false);
			}
		}
		catch(...)
		{
			for (uint16_t i=0;i<values.size();i++)
			{
				HisDevValue<double>* val = dynamic_cast<HisDevValue<double>*>(values[i]);
				val->SetValueFromDevice(tempvalue,true);
			}
		}
	}

	protected: virtual ~HisDevTemp18B20()
	{
	}
};

#endif /* HISDEVTEMP18B20_H_ */
