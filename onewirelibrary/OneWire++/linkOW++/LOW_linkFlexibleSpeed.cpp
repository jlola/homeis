/***************************************************************************
                          LOW_linkFlexibleSpeed.cpp  -  description
                             -------------------
    begin                : Sat Oct 11 2003
    copyright            : (C) 2003 by Harald Roelle
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

#include "LOW_linkFlexibleSpeed.h"


//=====================================================================================
//
// constructors
//

LOW_linkFlexibleSpeed::LOW_linkFlexibleSpeed( const wireSpeed_t inWireSpeed, const pdSlewRate_t inPdSlewRate,
                                              const w1LowTime_t inW1LowTime, const soW0RecTime_t inSoW0RecTime) :
  wireSpeed( inWireSpeed),
  pdSlewRate( inPdSlewRate),
  w1LowTime( inW1LowTime),
  soW0RecTime( inSoW0RecTime)
{
}


LOW_linkFlexibleSpeed::~LOW_linkFlexibleSpeed()
{
}



//=====================================================================================
//
// public methods
//

void LOW_linkFlexibleSpeed::setWireSpeed( const wireSpeed_t inWireSpeed)
{
  wireSpeed = inWireSpeed;
}

    
void LOW_linkFlexibleSpeed::setPullDownSlewRate( const pdSlewRate_t inPDSR)
{
  if ( wireSpeed != flexible_speed )
    throw incompatibleSpeed_error( "setPullDownSlewRate(): Only configurable in flexible speed mode", __FILE__, __LINE__);

  pdSlewRate = inPDSR;
}


LOW_linkFlexibleSpeed::pdSlewRate_t LOW_linkFlexibleSpeed::getPullDownSlewRate()
{
  if ( wireSpeed != flexible_speed )
    throw incompatibleSpeed_error( "Only allowed in flexible speed mode", __FILE__, __LINE__);

  return pdSlewRate;
}


void LOW_linkFlexibleSpeed::setWrite1LowTime( const w1LowTime_t inW1LT)
{
  if ( wireSpeed != flexible_speed )
    throw incompatibleSpeed_error( "setWrite1LowTime(): Only configurable in flexible speed mode", __FILE__, __LINE__);

  w1LowTime = inW1LT;
}

    
LOW_linkFlexibleSpeed::w1LowTime_t LOW_linkFlexibleSpeed::getWrite1LowTime()
{
  if ( wireSpeed != flexible_speed )
    throw incompatibleSpeed_error( "Only allowed in flexible speed mode", __FILE__, __LINE__);

  return w1LowTime;
}


void LOW_linkFlexibleSpeed::setSampleOffsetWrite0Rec( const soW0RecTime_t inSOW0RT)
{
  if ( wireSpeed != flexible_speed )
    throw incompatibleSpeed_error( "setSampleOffsetWrite0Rec(): Only configurable in flexible speed mode", __FILE__, __LINE__);

  soW0RecTime = inSOW0RT;
}


LOW_linkFlexibleSpeed::soW0RecTime_t LOW_linkFlexibleSpeed::getSampleOffsetWrite0Rec()
{
  if ( wireSpeed != flexible_speed )
    throw incompatibleSpeed_error( "Only allowed in flexible speed mode", __FILE__, __LINE__);

  return soW0RecTime;
}


