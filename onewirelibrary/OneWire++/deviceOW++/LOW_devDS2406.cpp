/***************************************************************************
                          LOW_devDS2406.cpp  -  description
                             -------------------
    begin                : Fri Aug 23 2002
    copyright            : (C) 2002 by Harald Roelle
    email                : roelle@informatik.uni-muenchen.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/



#include "LOW_devDS2406.h"
#include "LOW_platformMisc.h"
#include "LOW_helper_msglog.h"
#include "LOW_deviceFactory.h"
#include "LOW_netSegment.h"
#include "LOW_helper_crc.h"



//=====================================================================================
//
// static initializer
//

const std::string LOW_devDS2406::familyName = "DS2406";

int LOW_devDS2406::initHelper = initialize();
int LOW_devDS2406::initialize()
{
  LOW_deviceFactory::registerSpecificCtor( familyCode, &LOW_devDS2406::new_Instance);
  return 0;
}
        

        
//=====================================================================================
//
// static methods
//
  
LOW_device* LOW_devDS2406::new_Instance( LOW_netSegment &inNetSegment, const LOW_deviceID &inDevID)
{
  return new LOW_devDS2406( inNetSegment, inDevID);
}



//=====================================================================================
//
// constructors
//

LOW_devDS2406::LOW_devDS2406( LOW_netSegment &inSegment, const LOW_deviceID &inDevID) : 
  LOW_device( inSegment, inDevID, familyCode),isExternalPowered(false),hasPioB(false)
{
  // reset latches and get basic information
  //cmd_ChannelAccess infoGet = cmd_ChannelAccess( *this);
  //cmd_ChannelAccess::channelInfo_t info = infoGet.getChannelInfo();
  //isExternalPowered = info.isExternalPowered;
  hasPioB           = true;
}


LOW_devDS2406::~LOW_devDS2406()
{
}


//=====================================================================================
//
// methods
//

bool LOW_devDS2406::getIsExternalPowered() const
{
  return isExternalPowered;
}


bool LOW_devDS2406::getHasPioB() const
{
  return hasPioB;
}


void LOW_devDS2406::cmd_ReadStatus( LOW_devDS2406::statusRegister_t *outStatusRegister) const
{
  // locking done by cmd_ReadStatus()

  byteVec_t  condReg = byteVec_t( 1);

  cmd_ReadStatus( 0x07, condReg);
  
  outStatusRegister->activePolarity    = static_cast<activePolarity_t>(condReg[0]&0x01);
  outStatusRegister->sourceSelect      = static_cast<sourceSelect_t>((condReg[0]>>1)&0x03);
  outStatusRegister->channelSelect     = static_cast<chanSelect_t>((condReg[0]>>3)&0x03);
  outStatusRegister->channelFFQ_pioA   = static_cast<pioTransistor_t>((condReg[0]>>5)&0x01);
  outStatusRegister->channelFFQ_pioB   = static_cast<pioTransistor_t>((condReg[0]>>6)&0x01);
  outStatusRegister->isExternalPowered = (condReg[0]>>7)&0x01;
}

  
void LOW_devDS2406::cmd_WriteStatus( const LOW_devDS2406::chanSelect_t inChanSelect,
                                        const LOW_devDS2406::sourceSelect_t inSourceSelect, 
                                        const LOW_devDS2406::activePolarity_t inPolaritySelect,
                                        const LOW_devDS2406::pioTransistor_t inPioATrans, 
                                        const LOW_devDS2406::pioTransistor_t inPioBTrans) const
{
  if ( ! getHasPioB() && ( inChanSelect==chanBSelect || inChanSelect==chanBothSelect ) )
    throw devDS2406_error( "Channel B selected, but device has only PIO A", __FILE__, __LINE__);

  // locking done by cmd_WriteStatus()

  byteVec_t  condReg = byteVec_t( 1);

  condReg[0] = 0;
  condReg[0] |= inPolaritySelect;
  condReg[0] |= inSourceSelect<<1;
  condReg[0] |= inChanSelect<<3;
  condReg[0] |= inPioATrans<<5;
  condReg[0] |= inPioBTrans<<6;

  cmd_WriteStatus( 0x07, condReg);
}


void LOW_devDS2406::cmd_ReadMemory( const uint8_t inStartAddr, byteVec_t &outBytes) const
{
  // locking done by readMemUniversal()
  readMemUniversal( inStartAddr, outBytes, 128, ReadMemory_COMMAND);
}

    
void LOW_devDS2406::cmd_ReadStatus( const uint8_t inStartAddr, byteVec_t &outBytes) const
{
  // locking done by readMemUniversal()
  readMemUniversal( inStartAddr, outBytes, 8, ReadStatus_COMMAND);
}


void LOW_devDS2406::cmd_WriteStatus( const uint8_t inStartAddr, const byteVec_t &inWriteBytes) const
{
  if ( inStartAddr+inWriteBytes.size() > 8 )
    throw devDS2406_error( "Too many bytes to write", __FILE__, __LINE__);
  
  if ( inStartAddr==0x05 || inStartAddr==0x06 )
    throw devDS2406_error( "Address not writeable", __FILE__, __LINE__);
  
  // not yet supported registers
  if ( /*inStartAddr>=0x00 &&*/ inStartAddr<=0x04 )
    throw devDS2406_error( "Access to address not supported in this version", __FILE__, __LINE__);
  
  linkLock  lock( *this);

  // only address 7 remains, i.e. address must be 7 and length of inWriteBytes is 1
  
  byteVec_t  sendBytes = byteVec_t( 4);
  sendBytes[0] = WriteStatus_COMMAND;
  sendBytes[1] = inStartAddr&0xff;
  sendBytes[2] = inStartAddr>>8;
  sendBytes[3] = inWriteBytes[0];
  
  cmd_MatchROM();

  getLink().writeData( sendBytes);
    
  uint16_t expectedCrc16 = 0x0000;
  expectedCrc16 |= (getLink().readDataByte() ^ 0xff);      // NOTE: CRC bytzes are sent _inverted_!
  expectedCrc16 |= (getLink().readDataByte() ^ 0xff) << 8; // NOTE: CRC bytzes are sent _inverted_!
  if ( LOW_helper_CRC::calcCRC16( sendBytes) != expectedCrc16 )
    throw LOW_helper_CRC::crc_error( "CRC error in write operation", __FILE__, __LINE__);

  //getLink().writeData( static_cast<uint8_t>(0xff));

  // skip validation byte
  //uint8_t validationByte = getLink().readDataByte();

  //getLink().resetBus();
}



//=====================================================================================
//
// protected methods
//

void LOW_devDS2406::readMemUniversal( const uint16_t inStartAddr, byteVec_t &outBytes, 
                                      const uint16_t inMaxLen, const owCommand_t inCommand) const
{
  if ( inStartAddr >= inMaxLen )
    throw devDS2406_error( "Illegal address to read", __FILE__, __LINE__);
  
  if ( inStartAddr+outBytes.size() > inMaxLen )
    throw devDS2406_error( "Too many bytes to read", __FILE__, __LINE__);

  linkLock  lock( *this);
    
  byteVec_t  sendBytes = byteVec_t( 3);
  sendBytes[0] = inCommand;
  sendBytes[1] = inStartAddr&0xff;
  sendBytes[2] = inStartAddr>>8;
  
  cmd_MatchROM();

  getLink().writeData( sendBytes);
  getLink().readData( outBytes);
  
  if ( inStartAddr+outBytes.size() == inMaxLen ) { // read to end of mem => CRC16 checksumm is available
    uint16_t expectedCrc16 = 0x0000;
    expectedCrc16 |= (getLink().readDataByte() ^ 0xff);      // NOTE: CRC bytzes are sent _inverted_!
    expectedCrc16 |= (getLink().readDataByte() ^ 0xff) << 8; // NOTE: CRC bytzes are sent _inverted_!
    if ( LOW_helper_CRC::calcCRC16( outBytes, LOW_helper_CRC::calcCRC16( sendBytes)) != expectedCrc16 )
      throw LOW_helper_CRC::crc_error( "CRC error in read operation", __FILE__, __LINE__);
  }

  //getLink().resetBus();
}



//=====================================================================================
//
// class cmd_ChannelAccess
//

//LOW_devDS2406::cmd_ChannelAccess::cmd_ChannelAccess(
//    const LOW_devDS2406 &inDevice ) :
//  linkLock( inDevice),
//  device( inDevice)
//{
//	//SetChannel(inCRCtype,inChanSelect,inInterleaveMode,inToggleMode,inInitialMode,inALR);
//}


LOW_devDS2406::channelInfo_t LOW_devDS2406::getChannel(const CRCtype_t inCRCtype, const chanSelect_t inChanSelect,
	    const interleaveMode_t inInterleaveMode, const toggleMode_t inToggleMode,
	    const initialMode_t inInitialMode, const activityLatchReset_t inALR)
{
	// locking done by initializer

	  if ( inChanSelect == noneSelect )
	    throw devDS2406_error( "At least one channel must be selected", __FILE__, __LINE__);

	  if ( ! getHasPioB() && ( inChanSelect==chanBSelect || inChanSelect==chanBothSelect ) )
	    throw devDS2406_error( "Channel B selected, but device has only PIO A", __FILE__, __LINE__);

	  if ( inChanSelect!=chanBothSelect && inInterleaveMode )
	    throw devDS2406_error( "Interleave mode only available when selected both channels", __FILE__, __LINE__);

	  byteVec_t  outBytes = byteVec_t( 3 );
	  outBytes[0] =  ChannelAccess_COMMAND;
	  outBytes[1] =  (inCRCtype & 0x03)                  |
	                 ((inChanSelect&0x3) <<2 )          |
	                 ((static_cast<uint8_t>(inInterleaveMode))<<4) |
	                 ((static_cast<uint8_t>(inToggleMode))<<5)     |
	                 ((static_cast<uint8_t>(inInitialMode))<<6)    |
	                 ((static_cast<int8_t>(inALR))<<7);
	  outBytes[2] =  0xff;  // reserved, 0xff sent as specified


	  cmd_MatchROM();
	  getLink().writeData( outBytes );

	  uint8_t infoByte = getLink().readDataByte();

	  LOW_devDS2406::channelInfo_t channelInfo;
	  channelInfo.channelFFQ_pioA    = (infoByte>>0)&0x01;
	  channelInfo.channelFFQ_pioB    = (infoByte>>1)&0x01;
	  channelInfo.sensedLevel_pioA   = (infoByte>>2)&0x01;
	  channelInfo.sensedLevel_pioB   = (infoByte>>3)&0x01;
	  channelInfo.activityLatch_pioA = (infoByte>>4)&0x01;
	  channelInfo.activityLatch_pioB = (infoByte>>5)&0x01;
	  channelInfo.hasPioB            = (infoByte>>6)&0x01;
	  channelInfo.isExternalPowered  = (infoByte>>7)&0x01;

//	  if (inCRCtype==CRCtype_t::CRC_after1Byte )
//	  {
//		  vector<uint8_t> crc(2);
//		  getLink().readData(crc);
//		  uint16_t expectedCrc16 =0;
//		  expectedCrc16 |= (crc[0] ^ 0xff);      // NOTE: CRC bytzes are sent _inverted_!
//		  expectedCrc16 |= (crc[1] ^ 0xff) << 8; // NOTE: CRC bytzes are sent _inverted_!
//		  if ( LOW_helper_CRC::calcCRC16( &infoByte,1 , expectedCrc16) != expectedCrc16 )
//			  throw LOW_helper_CRC::crc_error( "CRC error in read operation", __FILE__, __LINE__);
//	  }

	  return channelInfo;
}

//LOW_devDS2406::cmd_ChannelAccess::~cmd_ChannelAccess()
//{
//  // class locks device
//
//  //device.getLink().resetBus();
//}

    
//-------------------------------------------------------------------------------------
// methods
//

//LOW_devDS2406::cmd_ChannelAccess::channelInfo_t&  LOW_devDS2406::cmd_ChannelAccess::getChannelInfo()
//{
//  // class locks device
//  return channelInfo;
//}
//
//bool  LOW_devDS2406::cmd_ChannelAccess::readDataBit() const
//{
//  // class locks device
//  return device.getLink().readDataBit();
//}
//
//uint8_t  LOW_devDS2406::cmd_ChannelAccess::readDataByte() const
//{
//  // class locks device
//  return device.getLink().readDataByte();
//}
//
//void  LOW_devDS2406::cmd_ChannelAccess::readData( byteVec_t &outBytes) const
//{
//  // class locks device
//  device.getLink().readData( outBytes);
//}
//
//void  LOW_devDS2406::cmd_ChannelAccess::writeData( const bool inSendBit) const
//{
//  // class locks device
//  device.getLink().writeData( inSendBit);
//}
//
//void  LOW_devDS2406::cmd_ChannelAccess::writeData( const uint8_t inSendByte) const
//{
//  // class locks device
//  device.getLink().writeData( inSendByte);
//}
//
//void  LOW_devDS2406::cmd_ChannelAccess::writeData( const byteVec_t &inSendBytes) const
//{
//  // class locks device
//  device.getLink().writeData( inSendBytes);
//}
