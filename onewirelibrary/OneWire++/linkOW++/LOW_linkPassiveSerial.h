/***************************************************************************
                          LOW_linkPassiveSerial.h  -  description
                             -------------------
    begin                : Sun Jul 7 2002
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

#ifndef LOW_LINKPASSIVESERIAL_H
#define LOW_LINKPASSIVESERIAL_H

 
#include "LOW_link.h"
#include "LOW_portSerialFactory.h"


    
/** Link class for simple passive serial 1-Wire adapters.

    As the passive adapter is not capable of real strong pullups with high current,
    it is emulated by providing high level on the bus for a relatively long time.

    This class is thread-safe.
    
    @todo PROVIDE ASSIGNMENT OPERATOR AND COPY CONSTRUCTOR
            
    @author Harald Roelle
*/
class LOW_linkPassiveSerial : public LOW_link  {

//=======================================================================================
public: 

  //=====================================================================================
  //
  // constructors
  //
  
  /** Constructor.

      @param  inSerPortSpec       Specification of serial port the adapter is on.
      @param  inAllowProgPulse    Wether the program pulse should be allowed.
   */
  LOW_linkPassiveSerial( const LOW_portSerialFactory::portSpecifier_t &inSerPortSpec,
                         const bool inAllowProgPulse = false);

  /** Destructor.
   */
  ~LOW_linkPassiveSerial();

  
  //=====================================================================================
  //
  // Standard methods required by LOW_Link
  //
  
  virtual std::string getLinkFamily() const { return "Passive serial link"; };

  //! @name Bus touch (write/read) methods required by LOW_Link
  //!@{
  virtual bool touchBit( const bool inSendBit, const strongPullup_t inPullup = pullUp_NONE);
  virtual uint8_t touchByte( const uint8_t inSendByte, const strongPullup_t inPullup = pullUp_NONE);
  //!@}

  //! @name Misc methods required by LOW_Link
  //!@{
  virtual void resetLinkAdapter();
  virtual bool resetBus();
  virtual void strongPullup( const unsigned long inMicroSecs);
  virtual void strongPullup( const strongPullup_t inPullupTime);
  virtual void programPulse( const unsigned long inMicroSecs);
  virtual void programPulse( const progPulse_t inPulseTime);
  //!@}

    
//=======================================================================================
private:

  //=====================================================================================
  //
  // constants
  //

  /** Time to wait for emulation of strong pullup in seconds. */
  static const int strongPullupEmuTime = 3;

    
  //=====================================================================================
  //
  // attributes
  //
  
  LOW_portSerial   *serialPort;   /**< The serial port the adapter is attached to. */

};

#endif
