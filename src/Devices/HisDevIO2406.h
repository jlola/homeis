/*
 * HisDevIO2406.h
 *
 *  Created on: Nov 30, 2013
 *      Author: root
 */

#ifndef HISDEVIO2406_H_
#define HISDEVIO2406_H_

#include "LOW_devDS2406.h"
#include "HisDallas.h"
#include "logger.h"
#include "Common/CUUID.h"


#define PINNO_INPUTA 	0
#define PINNO_INPUTB	1
#define PINNO_OUTPUTA	2
#define PINNO_OUTPUTB 	3
#define PINNO_LATCHA 	4
#define PINNO_LATCHB 	5

class HisDevIO2406: public HisDevDallas {

	bool outputchanged;
	HisDevValue<bool>* valueAInput;
	HisDevValue<bool>* valueAOutput;
	HisDevValue<bool>* valueALatch;

	HisDevValue<bool>* valueBInput;
	HisDevValue<bool>* valueBOutput;
	HisDevValue<bool>* valueBLatch;

	LOW_devDS2406 *dev;

	void CreateDataPoints();
	void WriteToDevice(ValueChangedEventArgs args);
	void Init();
protected:
	virtual void DoInternalRefresh(bool alarm);
	//protected: virtual void DoInternalSave(xmlNodePtr & node);
	virtual void DoInternalLoad(xmlNodePtr & node);
	const xmlChar* GetNodeNameInternal();
public:
	HisDevIO2406(LOW_devDS2406* pdev);
	HisDevIO2406(xmlNodePtr node, LOW_devDS2406* pdev);
	virtual ~HisDevIO2406();
};

#endif /* HISDEVIO2406_H_ */
