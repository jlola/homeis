/*
 * HisDevIO.h
 *
 *  Created on: Mar 10, 2013
 *      Author: Josef Lola
 */

#ifndef HISDEVIO2413_H_
#define HISDEVIO2413_H_

#include "LOW_devDS2413.h"
#include "HisDallas.h"
#include "logger.h"
#include "Common/CUUID.h"

#define PINNO_INPUTA 	0
#define PINNO_INPUTB	1
#define PINNO_OUTPUTA	2
#define PINNO_OUTPUTB 	3

class HisDevIO2413 : public HisDevDallas
{
	HisDevValue<bool>* valueAInput;
	HisDevValue<bool>* valueAOutput;

	HisDevValue<bool>* valueBInput;
	HisDevValue<bool>* valueBOutput;

	LOW_devDS2413 *dev;

	const xmlChar* GetNodeNameInternal();

	public:	HisDevIO2413(LOW_devDS2413* pdev);

	public:	HisDevIO2413(xmlNodePtr node,LOW_devDS2413* pdev);

	private: void CreateDataPoints();

	private : void WriteToDevice(ValueChangedEventArgs args);

	protected: virtual void DoInternalRefresh();

	//protected: virtual void DoInternalSave(xmlNodePtr & node);

	protected: virtual void DoInternalLoad(xmlNodePtr & node);

	protected: virtual ~HisDevIO2413();

};

#endif /* HISDEVIO2413_H_ */
