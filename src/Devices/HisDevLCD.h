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

#define LIGHTENABLED 0
#define ID_ROW1 1
#define ID_ROW1ADDR 2
#define ID_ROW2 3
#define ID_ROW2ADDR 4
#define ID_ROW3 5
#define ID_ROW3ADDR 6
#define ID_ROW4 7
#define ID_ROW4ADDR 8


class HisDevLCD: public HisDevDallas {
	LOW_devLCD* dev;
	HisDevValue<bool>* lightEnabled;
	bool firstrefresh;
	bool needRefresh;

	HisDevValue<int>* rowaddr1;
	HisDevValue<int>* rowaddr2;
	HisDevValue<int>* rowaddr3;
	HisDevValue<int>* rowaddr4;

	HisDevValue<string>* row1;
	HisDevValue<string>* row2;
	HisDevValue<string>* row3;
	HisDevValue<string>* row4;

	HisDevValue<string>* CreateRow(string name,WriteToDeviceRequestDelegate & delegate,int pinno);
	HisDevValue<int>* CreateRowAddr(string name,WriteToDeviceRequestDelegate & delegate,int pinno,int adr);
	void CreateDataPoints();
protected:
	virtual void DoInternalRefresh();
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
