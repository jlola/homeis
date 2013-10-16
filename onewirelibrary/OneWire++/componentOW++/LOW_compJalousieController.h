/***************************************************************************
                          LOW_compJalousieController.h  -  description
                             -------------------
    begin                : Wed Sep 4 2002
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

#ifndef LOW_COMPJALOUSIECONTROLLER_H
#define LOW_COMPJALOUSIECONTROLLER_H


#include "LOW_component.h"
#include "LOW_devDS2405.h"
#include "LOW_thread_mutex.h"


/**
    @todo Documentation of class.
    @todo Calibration methods.
    @todo Locking during angle moves.
    @todo Method for angle move only
    @todo PROVIDE ASSIGNMENT CONSTRUCTOR
    
    @author Harald Roelle
  */
class LOW_compJalousieController : public LOW_component  {

//=======================================================================================
public:
  
  //=====================================================================================
  //
  // exceptions
  //
 
  class_DERIVE_FROM_EXCEPTION( compJalousieController_error, component_error);
  
  
  
  //=====================================================================================
  //
  // constructors
  //
  
  LOW_compJalousieController( const LOW_devDS2405 &inPowerControl, const LOW_devDS2405 &inDirectionControl,
                              const float inClosingTime, const float inOpeningTime,
                              const float inAngleClosingTime, const float inAngleOpeningTime);
  
  ~LOW_compJalousieController();

  
  
//=======================================================================================
protected:

  //=====================================================================================
  //
  // locks
  //

  class jcLock {
  public:
    inline jcLock( LOW_compJalousieController &inJCComp) : jcComp(inJCComp)
    {
      jcComp.__linkRecMutex->lock();
    };
    inline ~jcLock()
    {
      jcComp.__linkRecMutex->unlock();
    };
  protected:
    const LOW_compJalousieController &jcComp;
  };



//=======================================================================================
public:

  //=====================================================================================
  //
  // classes
  //
  
  class manualMove : public jcLock { 
  //---------------------------------------------------------------------------------------
  public:
    //---------------------------------------------------------------------------------------
    // constructors
    manualMove( LOW_compJalousieController &inJCComp);
    ~manualMove();
  
    //---------------------------------------------------------------------------------------
    // methods
    void moveUp();
    void moveDown();
    void stopMove();

  //---------------------------------------------------------------------------------------
  protected:
    LOW_compJalousieController &jcComp;
  };



  //=====================================================================================
  //
  // methods
  //
  
  void setPosition( float inPos, float inAngle);

  float getClosingTime() const;

  void setClosingTime( const float inClosingTime);

  float getOpeningTime() const;

  void setOpeningTime( const float inOpeningTime);

  float getAngleClosingTime() const;

  void setAngleClosingTime( const float inAngleClosingTime);

  float getAngleOpeningTime() const;

  void setAngleOpeningTime( const float inAngleOpeningTime);

  //LOW_devDS2405& getPowerPIODevice();

  //LOW_devDS2405& getDirectionPIODevice();

  void measureTransitionDelays();

  void getTransitionDelays( unsigned int &outStop2upDelay, unsigned int &outUp2stopDelay,
                            unsigned int &outStop2downDelay, unsigned int &outDown2stopDelay,
                            unsigned int &outUp2downDelay, unsigned int &outDown2upDelay);

  void setTransitionDelays( const unsigned int inStop2upDelay, const unsigned int inUp2stopDelay,
                            const unsigned int inStop2downDelay, const unsigned int inDown2stopDelay,
                            const unsigned int inUp2downDelay, const unsigned int inDown2upDelay);

  
//=======================================================================================
protected:

  //=====================================================================================
  //
  // friends
  //
  
  friend class manualMove;
  friend class jcLock;
  
  
  
  //=====================================================================================
  //
  // constants
  //
  
  static const int fullMovementAdditionMillis = 500;

  
      
  //=====================================================================================
  //
  // attributes
  //
  
  const int            jcSerialNum;
  const LOW_devDS2405  &powerControl;
  const LOW_devDS2405  &directionControl;
  float                closingTime;
  float                openingTime;
  float                angleClosingTime;
  float                angleOpeningTime;
  unsigned int         stop2upDelay;
  unsigned int         up2stopDelay;
  unsigned int         stop2downDelay;
  unsigned int         down2stopDelay;
  unsigned int         up2downDelay;
  unsigned int         down2upDelay;
  LOW_thread_mutex     *__linkRecMutex;

  
  
  //=====================================================================================
  //
  // methods
  //
  
  void moveUp();
  
  void moveDown();
  
  void stopMove();

  void moveUpStop( const unsigned long inMillis);

  void moveDownStop( const unsigned long inMillis);

  
  
//=======================================================================================
private:
  
  //=====================================================================================
  //
  // static attributes
  //
  
  static int  jcCounter;

  
  
  //=====================================================================================
  //
  // attributes
  //
  
  bool powerIsOn;
  bool directionIsDown;
  
  
  
  //=====================================================================================
  //
  // methods
  //
  
  void setPowerOn();
  
  void setPowerOff();
  
  void setDirectionDown();
  
  void setDirectionUp();

  unsigned int averageMillis( const std::vector<unsigned int> &inVals);
};

#endif
