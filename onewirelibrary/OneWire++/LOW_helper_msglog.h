/***************************************************************************
                          LOW_helper_msglog.h  -  description
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

#ifndef LOW_HELPER_MSGLOG_H
#define LOW_HELPER_MSGLOG_H


#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>


#include "LOW_thread_mutex.h"


/** Static class for message logging.
    Distinguishes error/warning/normal and debugging messages.
    Debugging support arbitrary debug levels.
    
    This class is thread-safe.
    
    @todo Finish implementation of class, namely additional output streams.

    @bug printXXX( "\n") gives "(NULL)" output.

    @author Harald Roelle
 */
class LOW_helper_msglog {

//=======================================================================================
public:

  //=====================================================================================
  //
  // type definitions
  //

  /** Type defining debug levels. */
  typedef enum {
    portSerial_dl,           /**< Debugging of serial port communication. */
    objSync_getLock_dl,      /**< Debugging of successfully obtaining a lock in LOW_objectSynchronizer. */
    objSync_lockFailed_dl,   /**< Debugging of failing to obtain a lock in LOW_objectSynchronizer. */
    linkDS2490_dl,           /**< Debugging of DS2490. */
    all_dl                   /**< This one MUST be the last! */
  } debugLevel_t;

  
  //=====================================================================================
  //
  // static methods
  //

  /** Enable/disable logging of debug messages of a certain level.
      @param  inLevel    Debug level to set logging for.
      @param  isEnabled  Whether to enable or disable logging.
   */
  static void setDebugLevelEnabled( const debugLevel_t inLevel, const bool isEnabled = true);

  
  /** Get whether logging of debug messages of a certain level is enabled.
      @param   inLevel  Debug level to query.
      @return  True if logging is enabled.
   */
  static bool getDebugLevelEnabled( const debugLevel_t inLevel);

  
  /** Print error message.

      @param inErrno   OS error number.
      @param inFormat  Format string as in ANSI-C printf().
      @param ...       Variable argument list as in ANSI-C printf().
   */
  static void printPerror( const int inErrno, const char *inFormat, ...);

  
  /** Print error message.

      @param inFormat  Format string as in ANSI-C printf().
      @param ...       Variable argument list as in ANSI-C printf().
   */
  static void printError( const char *inFormat, ...);

  
  /** Print warning message.

      @param inFormat  Format string as in ANSI-C printf().
      @param ...       Variable argument list as in ANSI-C printf().
   */
  static void printWarning( const char *inFormat, ...);

    
  /** Print message.

      @param inFormat  Format string as in ANSI-C printf().
      @param ...       Variable argument list as in ANSI-C printf().
   */
  static void printMessage( const char *inFormat, ...);

    
  /** Print message.

      @param inLevel   Debug level the message belongs to.
      @param inFormat  Format string as in ANSI-C printf().
      @param ...       Variable argument list as in ANSI-C printf().
   */
  static void printDebug( const debugLevel_t inLevel, const char *inFormat, ...);


//=======================================================================================
private:
  
  //=====================================================================================
  //
  // locks
  //
  
  /** Locking class to ensure exclusive access to output streams.

      The class is intended to be used in a "locking is creation" design pattern.
      On creation an exclusive lock is optained, and on destruction the lock is released.

      Uses a recursive mutex, i.e. does not distinguish reads/writes but my be called
      multiple times by the same thread without blocking.

      Class is inlined for performance reasons.

      @see LOW_thread_mutex
   */
  class msgLock {
    public:
      /** Obtain the lock.
       */
      inline msgLock()
      {
        msgMutex->lock();
      };
      
      /** Release the lock.
       */
      inline ~msgLock()
      {
        msgMutex->unlock();
      };
    
    private:
      static LOW_thread_mutex  *msgMutex; /**< Recursive mutex used for locking. */
  };

  
  //=====================================================================================
  //
  // type definitions
  //
  
  /** Internal type to mark type of message. */
  typedef enum { msg_log, warn_log, err_log, debug_log} logType_t;
  
  
  //=====================================================================================
  //
  // constructors
  //

  /** Default constructor.
      It is private to prevent creating objects from this class as
      this is a static helper class.
   */
  LOW_helper_msglog();

  /** Destructor.
      It is private to prevent creating objects from this class as
      this is a static helper class.
   */
  ~LOW_helper_msglog();


  //=====================================================================================
  //
  // static attributes
  //
  
  static bool errorOccured;
  static bool debugLevels[all_dl];

  static bool useStdMsgStream;
  static bool useStdWarnStream;
  static bool useStdErrStream;
  static bool useStdDebugStream;
  static bool useExtraMsgStream;
  static bool useExtraWarnStream;
  static bool useExtraErrStream;
  static bool useExtraDebugStream;
  
  static FILE* stdOutStream;
  static FILE* stdWarnStream;
  static FILE* stdErrStream;
  static FILE* stdDebugStream;
  static FILE* extraOutStream;
  static FILE* extraWarnStream;
  static FILE* extraErrStream;
  static FILE* extraDebugStream;
  
  
  //=====================================================================================
  //
  // static methods
  //
  
  static void va_printToLog( const logType_t inLogType, const char *inFormat, va_list inParamList);
  static unsigned int fprintLogHeader( FILE *inExtraStream, FILE *inStdStream);
  static void fprintfMulti( FILE *inExtraStream, FILE *inStdStream, const char *inFormat, ...);
  static void vfprintfMulti( FILE *inExtraStream, FILE *inStdStream, const char *inFormat, va_list inAp);
  static void* callocCheck( const size_t inSize);

};

#endif

