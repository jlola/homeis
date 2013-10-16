/***************************************************************************
                          LOW_objectIDFactory.cpp  -  description
                             -------------------
    begin                : Sun Oct 26 2003
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

 
#include "LOW_objectIDFactory.h"


//=====================================================================================
//
// static attributes
//

__LOW_SYNCHRONIZE_INIT_PROTECTED_LOCK__(LOW_objectIDFactory)

LOW_objectIDFactory::objectID_t LOW_objectIDFactory::objIDCounter = 0;



//=====================================================================================
//
// constructors
//

LOW_objectIDFactory::LOW_objectIDFactory()
{
}


LOW_objectIDFactory::~LOW_objectIDFactory()
{
}



//=====================================================================================
//
// static methods
//

LOW_objectIDFactory::objectID_t LOW_objectIDFactory::getNewObjectID()
{
  __LOW_SYNCHRONIZE_STATIC_WRITE__

  return ++objIDCounter;
}
