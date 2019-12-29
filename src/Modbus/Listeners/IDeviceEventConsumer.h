/*
 * IDeviceEventConsumer.h
 *
 *  Created on: 23. 12. 2019
 *      Author: pc
 */

#ifndef SRC_MODBUS_LISTENERS_IDEVICEEVENTCONSUMER_H_
#define SRC_MODBUS_LISTENERS_IDEVICEEVENTCONSUMER_H_


class IDeviceEventConsumer {
public:
	virtual void FireEvent(int adr) = 0;

	virtual ~IDeviceEventConsumer(){

	}
};


#endif /* SRC_MODBUS_LISTENERS_IDEVICEEVENTCONSUMER_H_ */
