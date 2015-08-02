/***************************************************************************
                          LOW_helper_msglog.cpp  -  description
                             -------------------
    begin                : Sun Jul 21 2002
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


#include <memory>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>


#include "LOW_helper_msglog.h"
#include "LOW_platformMisc.h"
#include "LOW_thread_Factory.h"
#include "logger.h"

//=====================================================================================
//
// static attributes
//

bool LOW_helper_msglog::errorOccured = false;
bool LOW_helper_msglog::debugLevels[all_dl];

bool LOW_helper_msglog::useStdMsgStream     = true;
bool LOW_helper_msglog::useStdWarnStream    = true;
bool LOW_helper_msglog::useStdErrStream     = true;
bool LOW_helper_msglog::useStdDebugStream   = true;
bool LOW_helper_msglog::useExtraMsgStream   = false;
bool LOW_helper_msglog::useExtraWarnStream  = false;
bool LOW_helper_msglog::useExtraErrStream   = false;
bool LOW_helper_msglog::useExtraDebugStream = false;
  
FILE* LOW_helper_msglog::stdOutStream     = stdout;
FILE* LOW_helper_msglog::stdWarnStream    = stderr;
FILE* LOW_helper_msglog::stdErrStream     = stderr;
FILE* LOW_helper_msglog::stdDebugStream   = stderr;
FILE* LOW_helper_msglog::extraOutStream   = NULL;
FILE* LOW_helper_msglog::extraWarnStream  = NULL;
FILE* LOW_helper_msglog::extraErrStream   = NULL;
FILE* LOW_helper_msglog::extraDebugStream = NULL;



//=====================================================================================
//
// constructors
//

LOW_helper_msglog::LOW_helper_msglog()
{
};


LOW_helper_msglog::~LOW_helper_msglog()
{
};



//=====================================================================================
//
// static methods
//

void LOW_helper_msglog::setDebugLevelEnabled( const debugLevel_t inLevel, const bool isEnabled)
{
  msgLock lock = msgLock();
  debugLevels[inLevel] = isEnabled;
}


bool LOW_helper_msglog::getDebugLevelEnabled( const debugLevel_t inLevel)
{
  return debugLevels[inLevel];
}


//=====================================================================================
//
// static log methods
//

/**
 * Print error message
 */
void LOW_helper_msglog::printPerror( const int inErrno, const char *inFormat, ...)
{
  va_list inParamList;
  int     length = 0;
  char    dummy;
  char    *buffer;

  // calculate length
  va_start( inParamList, inFormat);
  length += vsnprintf( &dummy, 0, inFormat, inParamList);
  va_end( inParamList);
  if ( inErrno > 0 && inErrno < sys_nerr-1 ) {
    length += strlen( ": ");
    length += strlen( sys_errlist[inErrno]);
  }
  length ++; // the null char
  
  buffer = new char[length];
  
  va_start( inParamList, inFormat);
  vsprintf( buffer, inFormat, inParamList);
  va_end( inParamList);
  if ( inErrno > 0 && inErrno < sys_nerr-1 ) {
    strcat( buffer, ": ");
    strcat( buffer, sys_errlist[inErrno]);
  }

  printError( "%s (Error# %d)", buffer, inErrno);
  
  delete[] buffer;
}


/**
 * Print error message
 */
void LOW_helper_msglog::printError( const char *inFormat, ...)
{
  va_list inParamList;

  va_start( inParamList, inFormat);
  va_printToLog( err_log, inFormat, inParamList);
  va_end( inParamList);
}


/**
 * Print warning
 */
void LOW_helper_msglog::printWarning( const char *inFormat, ...)
{
  va_list inParamList;

  va_start( inParamList, inFormat);
  va_printToLog( warn_log, inFormat, inParamList);
  va_end( inParamList);
}


/**
 * Print message
 */
void LOW_helper_msglog::printMessage( const char *inFormat, ...)
{
  va_list inParamList;

  va_start( inParamList, inFormat);
  va_printToLog( msg_log, inFormat, inParamList);
  va_end( inParamList);
}


/**
 * Print debug message
 */
void LOW_helper_msglog::printDebug( const debugLevel_t inLevel, const char *inFormat, ...)
{
  va_list inParamList;

  if ( debugLevels[inLevel] || debugLevels[all_dl] ) {
    va_start( inParamList, inFormat);
    va_printToLog( debug_log, inFormat, inParamList);
    va_end( inParamList);
  }
}



//=====================================================================================
//
// locks
//

LOW_thread_mutex*  LOW_helper_msglog::msgLock::msgMutex = LOW_thread_Factory::new_mutex( LOW_thread_mutex::mutexKind_recursive);



//=====================================================================================
//
// private methods
//

void LOW_helper_msglog::va_printToLog( const logType_t inLogType, const char *inFormat, va_list inParamList)
{
  FILE    *extraStream = 0, *stdStream = 0;
  char    dummy, *buffer, *indentStr, *tmpPtr;
  int     headLen, length;

  msgLock lock = msgLock();
  
  switch ( inLogType ) {
    case msg_log:
      stdStream   = useStdMsgStream   ? stdOutStream   : NULL;
      extraStream = useExtraMsgStream ? extraOutStream : NULL;
      break;
    case warn_log:
      stdStream   = useStdWarnStream   ? stdWarnStream   : NULL;
      extraStream = useExtraWarnStream ? extraWarnStream : NULL;
      break;
    case err_log:
      stdStream   = useStdErrStream   ? stdErrStream   : NULL;
      extraStream = useExtraErrStream ? extraErrStream : NULL;
      break;
    case debug_log:
      stdStream   = useStdDebugStream   ? stdDebugStream   : NULL;
      extraStream = useExtraDebugStream ? extraDebugStream : NULL;
      break;
  }
  
  headLen = fprintLogHeader( extraStream, stdStream);

  // build string for indention
  indentStr = new char[headLen+1];
  memset( indentStr, ' ', headLen);
  indentStr[headLen] = '\0';

  if ( inLogType==err_log )
    fprintfMulti( extraStream, stdStream, "ERROR -- ");
    
  if ( inLogType==debug_log )
    fprintfMulti( extraStream, stdStream, "DEBUG -- ");

  // detect length and reserve buffer
  length = vsnprintf( &dummy, 0, inFormat, inParamList);  // detect length
  buffer = new char[length+1];  // get buffer (remember ther \0!)

  vsprintf( buffer, inFormat, inParamList);  // print into buffer
  tmpPtr = strtok( buffer, "\n"); 
  fprintfMulti( extraStream, stdStream, "%s\n", tmpPtr); // first line without indent
  while( (tmpPtr=strtok( NULL, "\n")) != NULL ) {  // subsequent lines with indent
    fprintfMulti( extraStream, stdStream, "%s%s\n", indentStr, tmpPtr);
  }

  delete[] buffer;
  delete[] indentStr;
}


unsigned int LOW_helper_msglog::fprintLogHeader( FILE *inExtraStream, FILE *inStdStream)
{
  char      tmpStr[200];
  time_t    curtime;
  struct tm *loctime;

  curtime = time( NULL);
  loctime = localtime( &curtime);
  strftime( tmpStr, 200, "%a %Y/%m/%d %H:%M:%S", loctime);
  fprintfMulti( inExtraStream, inStdStream, "%s [%d]: ", tmpStr, LOW_platformMisc::getThreadID());
  return( strlen( tmpStr));
}


void LOW_helper_msglog::fprintfMulti( FILE *inExtraStream, FILE *inStdStream, const char *inFormat, ...)
{

  va_list inParamList;
  va_start( inParamList, inFormat);
  CLogger::Info(inFormat,inParamList);
  vfprintfMulti( inExtraStream, inStdStream, inFormat, inParamList);
  va_end( inParamList);
}


void LOW_helper_msglog::vfprintfMulti( FILE *inExtraStream, FILE *inStdStream, const char *inFormat, va_list inAp)
{
  if ( inExtraStream )  vfprintf( inExtraStream, inFormat, inAp);
  if ( inStdStream )    vfprintf( inStdStream, inFormat, inAp);
}


void* LOW_helper_msglog::callocCheck( const size_t inSize)
{
  void* retPtr;

  if ( (retPtr=calloc( 1, inSize)) == NULL ) {
    LOW_helper_msglog::printPerror( errno, "Error in LOW_helper_msglog::callocCheck()");
    std::terminate();
  }

  return retPtr;
}
