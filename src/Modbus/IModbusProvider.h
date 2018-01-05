/*
 * IModbusProvider.h
 *
 *  Created on: 16. 12. 2017
 *      Author: pc
 */

#ifndef SRC_MODBUS_IMODBUSPROVIDER_H_
#define SRC_MODBUS_IMODBUSPROVIDER_H_

#include <string>
#include <vector>
#include "IModbus.h"

using namespace std;

class IModbusProvider
{
public:
	virtual IModbus* Find(string name)=0;
	virtual void Add(IModbus* connector)=0;
	virtual vector<IModbus*> & GetConnectors()=0;
	virtual ~IModbusProvider(){}
};



#endif /* SRC_MODBUS_IMODBUSPROVIDER_H_ */
