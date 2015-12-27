/*
 * LOWdevLCD.h
 *
 *  Created on: 24. 7. 2015
 *      Author: pc
 */

#ifndef _LOWDEVLCD_H_
#define _LOWDEVLCD_H_

#include "LOW_device.h"




class LOW_devLCD :  public LOW_device
{
	static const owCommand_t  ReadScratchpad_COMMAND   = 0xbe; /**< 1-Wire command byte constant */
	static const owCommand_t  WriteScratchpad_COMMAND  = 0x4e; /**< 1-Wire command byte constant */

	static const owCommand_t  LcdOn_COMMAND 			 = 0x03; /**< 1-Wire command byte constant */
	static const owCommand_t  LcdOff_COMMAND 			 = 0x05; /**< 1-Wire command byte constant */
	static const owCommand_t  LightOff_COMMAND		 = 0x07; /**< 1-Wire command byte constant */
	static const owCommand_t  LightOn_COMMAND	  		 = 0x08; /**< 1-Wire command byte constant */
	static const owCommand_t  WriteToLCDRegMem_COMMAND = 0x10; /**< 1-Wire command byte constant */
	static const owCommand_t  CopyLCDRegMemToScratchPad_COMMAND = 0x11; /**< 1-Wire command byte constant */
	static const owCommand_t  WriteToLCDDataMem_COMMAND = 0x12; /**< 1-Wire command byte constant */
	static const owCommand_t  CopyLCDDataMemToScratchPad_COMMAND = 0x13; /**< 1-Wire command byte constant */
	static const owCommand_t  CopyScratchpadToLCD_COMMAND = 0x48; /**< 1-Wire command byte constant */
	static const owCommand_t  ClearLCD_COMMAND   		  = 0x49; /**< 1-Wire command byte constant */
	static const owCommand_t  ReadPowerSupply_COMMAND  = 0xb4; /**< 1-Wire command byte constant */

	bool isExternalPowered;
	 /** Family name of this specific device. */
	static const std::string familyName;
public:

	bool IsConnected();
	/** Exception base class for all exceptions thrown by LOW_devDS1820. */
	class_DERIVE_FROM_EXCEPTION( devLCD_error, LOW_exception);

	/** Family code of this specific device. */
    static const LOW_deviceIDRaw::devFamCode_t  familyCode = 0xFF;

	LOW_devLCD( LOW_netSegment &inSegment, const LOW_deviceID &inDevID);

	/** Static pseudo constructor for registering with LOW_deviceFactory.
	      @param  inSegment  Reference to network segment the device is on.
	      @param  inDevID    Reference to device's ID.
	      @return  New dynamic instance of specific device class.
	*/
	static LOW_device* new_Instance( LOW_netSegment &inNetSegment, const LOW_deviceID &inDevID);

	virtual ~LOW_devLCD();

	void LCDOn();
	bool WriteToLCD(const char* text, uint8_t address);

	void LightOn(bool enable);

	void cmd_WriteScratchpad( const uint8_t inTL, const uint8_t inTH) const;

	/** Get the device's family code.
	  <B>Note:</B> Subclasses must implement this method to return their specific family code.
	  @return Family name of the device.
	*/
	virtual const LOW_deviceIDRaw::devFamCode_t getFamilyCode() const { return familyCode; };

	//=======================================================================================
	private:
		byteVec_t oldDataRow1;
		byteVec_t oldDataRow2;
		byteVec_t oldDataRow3;
		byteVec_t oldDataRow4;
	//=====================================================================================
	//
	// static initializer
	//

	/** Needed for dirty little C++ hack to force static initialization on application start.
	  @see initialize()
	*/
	static int initHelper;

	/** Static inizializer to register the class with LOW_deviceFactory.
	  @see initHelper
	*/
	static int initialize();

};

#endif /* ONEWIRELIBRARY_ONEWIRE___DEVICEOW___LOWDEVLCD_H_ */
