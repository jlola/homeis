/*
 * IWriteToDevice.h
 *
 *  Created on: 23. 1. 2018
 *      Author: pc
 */

#ifndef SRC_DEVICES_IWRITETODEVICE_H_
#define SRC_DEVICES_IWRITETODEVICE_H_

#include "ValueEventArgs.h"

class IWriteToDevice
{
public:
	virtual void WriteToDevice(ValueChangedEventArgs & args)=0;
};


#endif /* SRC_DEVICES_IWRITETODEVICE_H_ */
