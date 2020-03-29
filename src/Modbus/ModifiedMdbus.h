/*
 * ModifiedMdbus.h
 *
 *  Created on: 10. 10. 2019
 *      Author: pc
 */

#ifndef SRC_MODBUS_MODIFIEDMDBUS_H_
#define SRC_MODBUS_MODIFIEDMDBUS_H_

#include "ISerialReceiver.h"
#include "SerialPort.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "Event.h"
#include <chrono>
#include "IModbus.h"
#include "HomeIsConfig.h"
#include "ILogger.h"
#include "Common/HisLock.h"
#include "linuxcs.h"
#include "IBlockingQueue.h"

using namespace mn;
using namespace CppLinuxSerial;

#define FUNC_GETHOLDINGS 0x03
#define FUNC_SETHOLDING 0x06
#define FUNC_EVENT 0xF8

#define OFFSET_FUNC 1
#define OFFSET_COUNT 2

class ModifiedMdbus : public ISerialReceiver, public IModbus
{
	LPCRITICAL_SECTION modbusmutex;  /**< Mutex for exclusive access. */
	ILogger & logger;
	Event event;
	SSerPortConfig config;
	SerialPort serialPort;
	vector<uint8_t> buffer;
	vector<uint8_t> getholdingsBuffer;
	vector<IDeviceEventConsumer*> consumers;
	IBlockingQueue<int>* events;
	bool Send(uint8_t* data,uint16_t len,const int timeoutMs);
	bool receiving;
public:
	static string DriverName;

	void Stop();

	ModifiedMdbus(SSerPortConfig config);

	void OnData(std::vector<uint8_t> data);

	bool DataCompleted(bool & valid, uint8_t & func);

	bool Init();

	string GetDriverName();
	string GetName();
	string GetPort();

	vector<uint8_t> CreateHoldingsRequest();

	bool checkFrameCrc(const uint8_t *p, uint8_t length);

	bool setBitInput(uint16_t address,uint16_t index, uint16_t state);
	bool getBitInput(uint16_t address,uint16_t index,bool & bit);

	bool getShortInput(uint16_t address,uint16_t index,uint16_t & input);

	bool setCoil(uint16_t address,uint16_t index, uint16_t state);
	bool getCoil(uint16_t address,uint16_t index,bool & coil);

	bool setHolding(uint16_t address,uint16_t index, uint16_t val);
	bool getHolding(uint16_t address,uint16_t index,uint16_t* holding);
	bool getHolding(uint16_t address,uint16_t index,uint16_t* holding, uint32_t timeOut);
	bool getHoldings(uint16_t address,uint16_t offset,uint16_t count,uint16_t* target);
	bool getHoldings(uint16_t address,uint16_t offset,uint16_t count,uint16_t* target, uint32_t timeOutMs);

	int WaitForAddress();

	void AddConsumer(IDeviceEventConsumer* consumer);

	virtual ~ModifiedMdbus();
};

#endif /* SRC_MODBUS_MODIFIEDMDBUS_H_ */
