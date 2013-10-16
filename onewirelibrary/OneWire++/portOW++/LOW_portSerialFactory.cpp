/***************************************************************************
                          LOW_portSerialFactory.cpp  -  description
                             -------------------
    begin                : Tue Jul 30 2002
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

 
#include "LOW_portSerialFactory.h"
#include "LOW_portSerial_Linux.h"


//=====================================================================================
//
// constructors
//

LOW_portSerialFactory::LOW_portSerialFactory()
{
}


LOW_portSerialFactory::~LOW_portSerialFactory()
{
}


//=====================================================================================
//
// static factory
//

LOW_portSerial* LOW_portSerialFactory::new_portSerial( const portSpecifier_t inPortSpec)
{
#ifdef __linux__
  return new LOW_portSerial_Linux( inPortSpec);
#endif
}
