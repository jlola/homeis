/***************************************************************************
                          LOW_platformMiscFactory.cpp  -  description
                             -------------------
    begin                : Thu Aug 1 2002
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

#include "LOW_platformMisc.h"
#include "LOW_platformMiscFactory.h"
#include "LOW_platformMisc_Linux.h"



//=====================================================================================
//
// constructors
//

LOW_platformMiscFactory::LOW_platformMiscFactory()
{
}


LOW_platformMiscFactory::~LOW_platformMiscFactory()
{
}


//=====================================================================================
//
// static factory
//
  
LOW_platformMisc* LOW_platformMiscFactory::new_platformMisc()
{
#ifdef __linux__
  return new LOW_platformMisc_Linux();
#endif
}
