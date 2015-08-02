/*
 * HisDevLCD.h
 *
 *  Created on: 27. 7. 2015
 *      Author: pc
 */

#ifndef HOMEIS_DEVICES_HISDEVLCD_H_
#define HOMEIS_DEVICES_HISDEVLCD_H_

#define LIGHT_ENABLE 0

#define ROW1 1
#define ROW2 2
#define ROW3 3
#define ROW4 4

#define NODE_NAME_LCD "DEV_LCD"
#define NODE_PROP_ROW1ADDR "ROW1ADDR"
#define NODE_PROP_ROW2ADDR "ROW2ADDR"
#define NODE_PROP_ROW3ADDR "ROW3ADDR"
#define NODE_PROP_ROW4ADDR "ROW4ADDR"

class HisDevLCD: public HisDevDallas {
	LOW_devLCD* dev;
	HisDevValue<bool>* lightEnabled;
	bool firstrefresh;
	vector<HisDevValue<string>*> rows;
	vector<HisDevValue<int>*> rowaddrs;

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

#endif /* HOMEIS_DEVICES_HISDEVLCD_H_ */
