/*
 * IModbusHandler.h
 *
 *  Created on: 16. 12. 2017
 *      Author: pc
 */

#ifndef SRC_DEVICES_HANDLERS_IMODBUSHANDLER_H_
#define SRC_DEVICES_HANDLERS_IMODBUSHANDLER_H_

#include "CUUID.h"

class IModbusHandler
{
public:
	virtual bool Scan(bool addnew)=0;
	virtual bool Remove(CUUID id)=0;
	virtual void RefreshOutputs(bool force)=0;
	virtual void Refresh(bool modbusSuccess)=0;
	virtual void Load()=0;
	virtual ~IModbusHandler() {	}
};

#endif /* SRC_DEVICES_HANDLERS_IMODBUSHANDLER_H_ */
