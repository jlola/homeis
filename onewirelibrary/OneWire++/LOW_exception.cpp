/***************************************************************************
                          LOW_exception.cpp  -  description
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


#include <typeinfo>
 

#include "LOW_exception.h"
#include "LOW_objectSynchronizer.h"
#include "LOW_helper_msglog.h"
#include "PoppyDebugTools.h"

//=====================================================================================
//
// static attributes
//

__LOW_SYNCHRONIZE_INIT_PROTECTED_LOCK__(LOW_exception)

bool LOW_exception::logOnCreation = false;


//=====================================================================================
//
// constructors
//

LOW_exception::LOW_exception() :
  errNum( 0),
  message( ""),
  line(-1)
{
}


LOW_exception::LOW_exception( const std::string inMsg, const std::string inFile, const int inLine) : 
  errNum( 0),
  message( inMsg),
  file( inFile),
  line( inLine)
{
  if ( logOnCreation )
    logException( "[auto-logged on creation] ");
}


LOW_exception::LOW_exception( const int inErrNum, const std::string inMsg, const std::string inFile, const int inLine) : 
  errNum( inErrNum),
  message( inMsg),
  file( inFile),
  line( inLine)
{
  if ( logOnCreation )
    logException( "[auto-logged on creation] ");
}


LOW_exception::~LOW_exception()
{
}
  
  
//=====================================================================================
//
// public static methods
//

void LOW_exception::setLogOnCreation( const bool inLogOnCreation)
{
  __LOW_SYNCHRONIZE_STATIC_WRITE__

  logOnCreation = inLogOnCreation;
}


const bool LOW_exception::getLogOnCreation()
{
  __LOW_SYNCHRONIZE_STATIC_READ__

  return logOnCreation;
}
    

//=====================================================================================
//
// public methods
//

void LOW_exception::logException( const std::string inPrefix)
{
	STACK
  if ( errNum == 0 )
    LOW_helper_msglog::printError( "%s%s {file: %s, line: %d}", inPrefix.c_str(), message.c_str(), file.c_str(), line);
  else
    LOW_helper_msglog::printPerror( errNum, "%s%s {file: %s, line: %d}", inPrefix.c_str(), message.c_str(), file.c_str(), line);
}
