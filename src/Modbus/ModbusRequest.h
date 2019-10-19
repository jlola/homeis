/*
 * ModbusRequest.h
 *
 *  Created on: 13. 10. 2019
 *      Author: pc
 */

#ifndef SRC_MODBUS_MODBUSREQUEST_H_
#define SRC_MODBUS_MODBUSREQUEST_H_

#include <stdint.h>

typedef struct
{
	uint8_t Address;
	uint8_t Function;
	uint16_t StartingAddress;
	uint16_t Count;
	uint16_t CRC;
} ModbusRequest;


#endif /* SRC_MODBUS_MODBUSREQUEST_H_ */
