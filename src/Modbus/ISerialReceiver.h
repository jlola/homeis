/*
 * ISerialReceiver.h
 *
 *  Created on: 8. 10. 2019
 *      Author: pc
 */

#ifndef SRC_MODBUS_ISERIALRECEIVER_H_
#define SRC_MODBUS_ISERIALRECEIVER_H_

#include <vector>
#include "stdint.h"

class ISerialReceiver
{
public:
	virtual void OnData(std::vector<uint8_t> data)=0;

	virtual ~ISerialReceiver() {}
};



#endif /* SRC_MODBUS_ISERIALRECEIVER_H_ */
