/***************************************************************************
                          LOW_helper_crc.cpp  -  description
                             -------------------
    begin                : Sat Jul 6 2002
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

 
#include "LOW_helper_crc.h"


//=====================================================================================
//
// constants
//

const uint8_t LOW_helper_CRC::crc8Table[256] = {
     0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
   157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
    35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
   190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
    70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
   219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
   101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
   248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
   140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
    17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
   175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
    50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
   202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
    87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
   233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
   116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53,
};

const uint16_t LOW_helper_CRC::crc16Table[256] = {
    0x0000,  0xc0c1,  0xc181,  0x0140,  0xc301,  0x03c0,  0x0280,  0xc241,
    0xc601,  0x06c0,  0x0780,  0xc741,  0x0500,  0xc5c1,  0xc481,  0x0440,
    0xcc01,  0x0cc0,  0x0d80,  0xcd41,  0x0f00,  0xcfc1,  0xce81,  0x0e40,
    0x0a00,  0xcac1,  0xcb81,  0x0b40,  0xc901,  0x09c0,  0x0880,  0xc841,
    0xd801,  0x18c0,  0x1980,  0xd941,  0x1b00,  0xdbc1,  0xda81,  0x1a40,
    0x1e00,  0xdec1,  0xdf81,  0x1f40,  0xdd01,  0x1dc0,  0x1c80,  0xdc41,
    0x1400,  0xd4c1,  0xd581,  0x1540,  0xd701,  0x17c0,  0x1680,  0xd641,
    0xd201,  0x12c0,  0x1380,  0xd341,  0x1100,  0xd1c1,  0xd081,  0x1040,
    0xf001,  0x30c0,  0x3180,  0xf141,  0x3300,  0xf3c1,  0xf281,  0x3240,
    0x3600,  0xf6c1,  0xf781,  0x3740,  0xf501,  0x35c0,  0x3480,  0xf441,
    0x3c00,  0xfcc1,  0xfd81,  0x3d40,  0xff01,  0x3fc0,  0x3e80,  0xfe41,
    0xfa01,  0x3ac0,  0x3b80,  0xfb41,  0x3900,  0xf9c1,  0xf881,  0x3840,
    0x2800,  0xe8c1,  0xe981,  0x2940,  0xeb01,  0x2bc0,  0x2a80,  0xea41,
    0xee01,  0x2ec0,  0x2f80,  0xef41,  0x2d00,  0xedc1,  0xec81,  0x2c40,
    0xe401,  0x24c0,  0x2580,  0xe541,  0x2700,  0xe7c1,  0xe681,  0x2640,
    0x2200,  0xe2c1,  0xe381,  0x2340,  0xe101,  0x21c0,  0x2080,  0xe041,
    0xa001,  0x60c0,  0x6180,  0xa141,  0x6300,  0xa3c1,  0xa281,  0x6240,
    0x6600,  0xa6c1,  0xa781,  0x6740,  0xa501,  0x65c0,  0x6480,  0xa441,
    0x6c00,  0xacc1,  0xad81,  0x6d40,  0xaf01,  0x6fc0,  0x6e80,  0xae41,
    0xaa01,  0x6ac0,  0x6b80,  0xab41,  0x6900,  0xa9c1,  0xa881,  0x6840,
    0x7800,  0xb8c1,  0xb981,  0x7940,  0xbb01,  0x7bc0,  0x7a80,  0xba41,
    0xbe01,  0x7ec0,  0x7f80,  0xbf41,  0x7d00,  0xbdc1,  0xbc81,  0x7c40,
    0xb401,  0x74c0,  0x7580,  0xb541,  0x7700,  0xb7c1,  0xb681,  0x7640,
    0x7200,  0xb2c1,  0xb381,  0x7340,  0xb101,  0x71c0,  0x7080,  0xb041,
    0x5000,  0x90c1,  0x9181,  0x5140,  0x9301,  0x53c0,  0x5280,  0x9241,
    0x9601,  0x56c0,  0x5780,  0x9741,  0x5500,  0x95c1,  0x9481,  0x5440,
    0x9c01,  0x5cc0,  0x5d80,  0x9d41,  0x5f00,  0x9fc1,  0x9e81,  0x5e40,
    0x5a00,  0x9ac1,  0x9b81,  0x5b40,  0x9901,  0x59c0,  0x5880,  0x9841,
    0x8801,  0x48c0,  0x4980,  0x8941,  0x4b00,  0x8bc1,  0x8a81,  0x4a40,
    0x4e00,  0x8ec1,  0x8f81,  0x4f40,  0x8d01,  0x4dc0,  0x4c80,  0x8c41,
    0x4400,  0x84c1,  0x8581,  0x4540,  0x8701,  0x47c0,  0x4680,  0x8641,
    0x8201,  0x42c0,  0x4380,  0x8341,  0x4100,  0x81c1,  0x8081,  0x4040
};



//=====================================================================================
//
// constructors
//

LOW_helper_CRC::LOW_helper_CRC()
{
};


LOW_helper_CRC::~LOW_helper_CRC()
{
};



//=====================================================================================
//
// static methods
//

const uint8_t LOW_helper_CRC::calcCRC8( const uint8_t *inBuf, const unsigned int inLen, const uint8_t inPreloadCRC)
{ 
  uint8_t crc = inPreloadCRC;
   
  for( unsigned int a=0; a<inLen; a++)
    crc = crc8Table[ crc ^ *inBuf++ ];  
    
  return crc;
}


const uint8_t LOW_helper_CRC::calcCRC8( const byteVec_t &inVec, const uint8_t inPreloadCRC)
{
  uint8_t crc = inPreloadCRC;
   
  for( unsigned int a=0; a<inVec.size(); a++)
    crc = crc8Table[ crc ^ inVec[a] ];
    
  return crc;
}


const uint16_t LOW_helper_CRC::calcCRC16( const uint8_t *inBuf, const unsigned int inLen, const uint16_t inPreloadCRC)
{
  uint16_t crc = inPreloadCRC;

  for( unsigned int a=0; a<inLen; a++)
    crc = crc16Table[ (crc ^ *inBuf++) & 0xff ] ^ (crc>>8);
    
  return crc;
}


const uint16_t LOW_helper_CRC::calcCRC16( const byteVec_t &inVec, const uint16_t inPreloadCRC)
{
  uint16_t crc = inPreloadCRC;

  for( unsigned int a=0; a<inVec.size(); a++)
    crc = crc16Table[ (crc ^ inVec[a]) & 0xff ] ^ (crc>>8);

  return crc;
}
