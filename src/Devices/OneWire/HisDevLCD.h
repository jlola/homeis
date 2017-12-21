/*
 * HisDevLCD.h
 *
 *  Created on: 27. 7. 2015
 *      Author: pc
 */

#ifndef SRC_DEVICES_HISDEVLCD_H_
#define SRC_DEVICES_HISDEVLCD_H_

#include "HisDallas.h"

#define NODE_NAME_LCD "DEV_LCD"

#define LIGHTENABLED 	0
#define ID_ROW1 		1
#define ID_ROW1ADDR 	2
#define ID_ROW2 		3
#define ID_ROW2ADDR 	4
#define ID_ROW3 		5
#define ID_ROW3ADDR 	6
#define ID_ROW4 		7
#define ID_ROW4ADDR 	8
#define ID_INPUT1 		9
#define ID_INPUT2 		10
#define ID_INPUT3 		11
#define ID_INPUT4 		12
#define ID_INPUTCNT1 	13
#define ID_INPUTCNT2 	14
#define ID_INPUTCNT3 	15
#define ID_INPUTCNT4 	16


class HisDevLCD: public HisDevDallas {
	LOW_devLCD* dev;
	HisDevValue<bool>* lightEnabled;
	bool firstrefresh;
	bool refreshRow1;
	bool refreshRow2;
	bool refreshRow3;
	bool refreshRow4;
	bool refreshbuttoncnt1;
	bool refreshbuttoncnt2;
	bool refreshbuttoncnt3;
	bool refreshbuttoncnt4;
	bool refreshLihtEnabled;


	HisDevValue<int>* rowaddr1;
	HisDevValue<int>* rowaddr2;
	HisDevValue<int>* rowaddr3;
	HisDevValue<int>* rowaddr4;

	HisDevValue<string>* row1;
	HisDevValue<string>* row2;
	HisDevValue<string>* row3;
	HisDevValue<string>* row4;

	bool lcdOn;

	HisDevValue<bool>* input1;
	HisDevValue<bool>* input2;
	HisDevValue<bool>* input3;
	HisDevValue<bool>* input4;

	HisDevValue<uint32_t>* inputcnt1;
	HisDevValue<uint32_t>* inputcnt2;
	HisDevValue<uint32_t>* inputcnt3;
	HisDevValue<uint32_t>* inputcnt4;

	void CreateRow(HisDevValue<string>* & row, string name,WriteToDeviceRequestDelegate & delegate,int pinno);
	void CreateRowAddr(HisDevValue<int>* rowaddr,string name,WriteToDeviceRequestDelegate & delegate,int pinno,int adr);
	void CreateDataPoints();
	void CreateInput(HisDevValue<bool>* & input,string name,int pinno);
	void CreateInputCounter(HisDevValue<uint32_t>* & inputcnt, string name,int pinno);
	void CreateInputs();
	bool GetValue(byteVec_t counters, size_t index);
protected:
	virtual void DoInternalRefresh(bool alarm);
	//protected: virtual void DoInternalSave(xmlNodePtr & node);
	virtual void DoInternalLoad(xmlNodePtr & node);
	virtual void DoInternalSave(xmlNodePtr & node);
	const xmlChar* GetNodeNameInternal();
public:
	void WriteToDevice(ValueChangedEventArgs args);
	HisDevLCD(LOW_devLCD* pdev);
	HisDevLCD(xmlNodePtr node, LOW_devLCD* pdev);
	virtual ~HisDevLCD();
};

#endif /* SRC_DEVICES_HISDEVLCD_H_ */
