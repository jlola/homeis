/***************************************************************************
                          LOW_exception.h  -  description
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

#ifndef LOW_EXCEPTION_H
#define LOW_EXCEPTION_H

  
#include <string>

#include "LOW_platformMiscFactory.h"
#include "LOW_objectSynchronizerMacros.h"

class LOW_thread_rwlock;


/** Macro for easy subclassing of LOW_exception.
 */
#define class_DERIVE_FROM_EXCEPTION(DERIVED,SUPER) \
  class DERIVED : public SUPER { \
    public: DERIVED( const std::string inMsg, const std::string inFile, const int inLine) : SUPER( inMsg, inFile, inLine) {};  \
            DERIVED( const int inErrNum, const std::string inMsg, const std::string inFile, const int inLine) : SUPER( inErrNum, inMsg, inFile, inLine) {}; \
  }


            
/** Base class for all exception of this library.

    This class is thread-safe.

    @author Harald Roelle
 */
class LOW_exception {

//=======================================================================================
public: 

  //=====================================================================================
  //
  // attributes
  //
  
  const int          errNum;   /**< OS error number. */
  const std::string  message;  /**< Descriptive message. */
  const std::string  file;     /**< File from which exception was thrown. */
  const int          line;     /**< Line number where exception was thrown. */
  
    
  //=====================================================================================
  //
  // constructors
  //

  /** Default constructor.
   */  
  LOW_exception();

  /** Constructor.
      @param  inMsg   Message describing the exception.
      @param  inFile  File from which exception is thrown.
      @param  inLine  Line number where exception is thrown.
   */
  LOW_exception( const std::string inMsg, const std::string inFile, const int inLine);

  /** Constructor for OS errors.

      The textual OS description (perror) is added automatically when exception is logged.
  
      @param  inErrNum  OS error number.
      @param  inMsg     Message describing the exception.
      @param  inFile    File from which exception is thrown.
      @param  inLine    Line number where exception is thrown.
   */
  LOW_exception( const int inErrNum, const std::string inMsg, const std::string inFile, const int inLine);
  
  /** Destructor.
   */
  virtual ~LOW_exception();

  
  //=====================================================================================
  //
  // static methods
  //

  /** Set wether exceptions should be logged automatically when created.
      @param  inLogOnCreation   Wether exceptions should be logged automatically.
   */
  static void setLogOnCreation( const bool inLogOnCreation);

  /** Get wether exceptions are logged automatically when created.
      @return   Wether exceptions are logged automatically.
   */
  static const bool getLogOnCreation();
  
  
  //=====================================================================================
  //
  // methods
  //

  /** Log the exception via LOW_helper_msglog::printError()
      @param  inPrefix  Prefix to prepend to the log message.
   */
  virtual void logException( const std::string inPrefix = "");
      

//=======================================================================================
protected:

  //=====================================================================================
  //
  // static attributes
  //

  __LOW_SYNCHRONIZE_DEFINE_PROTECTED_LOCK__

  /** Indicates wether exception should be logged automatically when the are created. */
  static bool logOnCreation;
  
};

#endif
