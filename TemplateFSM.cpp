

/****************************************************************************
  Module
  TemplateFSM.c

  Revision
  1.0.1

  Description
  This is a template file for implementing flat state machines under the        
  Gen2 Events and Services Framework.

  Notes

  History
  When           Who     What/Why
  -------------- ---     --------
  01/15/12 11:12 jec      revisions for Gen2 framework
  11/07/11 11:26 jec      made the queue static
  10/30/11 17:59 jec      fixed references to CurrentEvent in RunTemplateSM()
  10/23/11 18:20 jec      began conversion from SMTemplate.c (02/20/07 rev)
  ****************************************************************************/
/*----------------------------- Include Files -----------------------------*/
/* include header files for this state machine as well as any machines at the
next lower level in the hierarchy that are sub-machines to this machine
*/
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "TemplateFSM.h"
#include "Bluetooth.h"
#include "ES_Timers.h"

#include <Arduino.h>
#include <Wire.h>
#include <math.h>
#include <SparkFun_MMA8452Q.h>

/*----------------------------- Module Defines ----------------------------*/
#define LIGHT_THRESH        600
#define ACCEL_MOVING_THRESH 10
#define BLINK_TIME  200
/*---------------------------- Module Functions ---------------------------*/
/* prototypes for private functions for this machine.They should be functions
relevant to the behavior of this state machine
*/

/*---------------------------- Module Variables ---------------------------*/
// everybody needs a state variable, you may need others as well.
// type of state variable should match htat of enum in header file
static TemplateState_t CurrentState;

// with the introduction of Gen2, we need a module level Priority var as well
static uint8_t MyPriority;
static bool connection = false;
static bool LEDmode = false;
static bool LEDstate = true;
static bool night = false;
static bool moving = false;
static bool flash = false;

MMA8452Q accel;
int accel_vector;
int lightPin = A7;
int accelLED = 3;
int lightLED = 5;

/*------------------------------ Module Code ------------------------------*/
/****************************************************************************
Function
InitTemplateFSM

Parameters
uint8_t : the priorty of this service

Returns
bool, false if error in initialization, true otherwise

Description
Saves away the priority, sets up the initial transition and does any
other required initialization for this state machine
Notes

Author
J. Edward Carryer, 10/23/11, 18:55
****************************************************************************/
bool InitTemplateFSM ( uint8_t Priority )
{
  ES_Event ThisEvent;

  MyPriority = Priority;
// put us into the Initial PseudoState
  CurrentState = StartState;
//initialize inputs and outputs
  accel.init();
  pinMode(accelLED, OUTPUT);
  pinMode(lightLED, OUTPUT);
  while(accel.available() == false)Serial.println("connecting...");

  Serial.println("main code initialized");
// post the initial transition event
  ThisEvent.EventType = ES_INIT;
  if (ES_PostToService( MyPriority, ThisEvent) == true)
  {
    return true;
  }else
  {
    return false;
  }
}

/****************************************************************************
Function
PostTemplateFSM

Parameters
EF_Event ThisEvent , the event to post to the queue

Returns
boolean False if the Enqueue operation failed, True otherwise

Description
Posts an event to this state machine's queue
Notes

Author
J. Edward Carryer, 10/23/11, 19:25
****************************************************************************/
bool PostTemplateFSM( ES_Event ThisEvent )
{
  return ES_PostToService( MyPriority, ThisEvent);
}

/****************************************************************************
Function
RunTemplateFSM

Parameters
ES_Event : the event to process

Returns
ES_Event, ES_NO_EVENT if no error ES_ERROR otherwise

Description
add your description here
Notes
uses nested switch/case to implement the machine.
Author
J. Edward Carryer, 01/15/12, 15:23
****************************************************************************/
ES_Event RunTemplateFSM( ES_Event ThisEvent )
{
  ES_Event ReturnEvent;
  ReturnEvent.EventType = ES_NO_EVENT; // assume no errors
  ES_EventTyp_t NewEvent;
  NewEvent = ThisEvent.EventType;
  //  printf("incoming... ");
<<<<<<< HEAD
  if (NewEvent == Connected) connection = true;
  else if (NewEvent == Disconnected) connection = false;
  else if (NewEvent == Blink) LEDmode = true;
  else if (NewEvent == Solid) LEDmode = false;
  else if (NewEvent == Auto) LEDstate = true;
  else if (NewEvent == On) LEDstate = false;
  else if (NewEvent == Dark) night = true;
  else if (NewEvent == Bright) night = false;
  else if (NewEvent == Moving) moving = true;
  else if (NewEvent == NotMoving) moving = false;
=======
  if (NewEvent == ES_CONNECTED) Connection = true;
  else if (NewEvent == ES_DISCONNECTED) Connection = false;
  else if (NewEvent == ES_BLINK) Mode = true;                 //Blink mode = 1
  else if (NewEvent == ES_SOLID) Mode = false;                //solid mode = 0
  else if (NewEvent == ES_AUTO) State = true;                 //auto state = 1
  else if (NewEvent == ES_ON) State = false;                  //on state   = 0
  else if (NewEvent == ES_DARK) LightSensorState = true;      //dark state = 1
  else if (NewEvent == ES_BRIGHT) LightSensorState = false;   //light state= 0
  else if (NewEvent == ES_MOVING) AccelState = true;          //move state = 1
  else if (NewEvent == ES_NOTMOVING) AccelState = false;      //not moving = 0
  else if (NewEvent == 
>>>>>>> origin/master
  Serial.print("Connection: ");
  Serial.print(connection);
  Serial.print("\tLED Mode: ");
  Serial.print(LEDmode);
  Serial.print("\tLED State: ");
  Serial.print(LEDstate);
  Serial.print("\tLight Sensor: ");
  Serial.print(night);
  Serial.print("\tAccelerometer: ");
  Serial.println(moving);
  Serial.print("current state: ");
  Serial.println(CurrentState);
  //  else if (NewEvent == Error) printf("Something is wrong\n");
  switch ( CurrentState )
  {
    case StartState :       // If current state is initial Psedudo State
      if (connection && LEDmode && (!LEDstate || night) || (!connection && moving)) {
        CurrentState = BlinkingState; 
        Serial.println("Blinking State");
        //start the timer:
        ES_Timer_InitTimer(BlinkTimer, BLINK_TIME);
      }
    
      if (connection && !LEDmode && (!LEDstate || night)){
        CurrentState = SolidState;
        Serial.println("Solid State");
        //turn that shit on
        analogWrite(lightLED,255);
        analogWrite(accelLED,255);
      }
    
      if ((!connection && !moving) || (connection && (!night && LEDstate)))  {
        CurrentState = LEDOffState;
        Serial.println("LEDOff state");
        //turn that shit off
        analogWrite(lightLED,0);
        analogWrite(accelLED,0);
      }
      break;
  
    case BlinkingState :       // If current state is state one
      if (connection && !LEDmode && (!LEDstate || night)){
        CurrentState = SolidState;
        Serial.println("Solid State");
        //turn that shit on
        analogWrite(lightLED,255);
        analogWrite(accelLED,255);
      }
    
      if ((!connection && !moving) || (connection && (!night && LEDstate)))  {
        CurrentState = LEDOffState;
        Serial.println("LED Off State");
        //turn that shit off
        analogWrite(lightLED,0);
        analogWrite(accelLED,0);
      }
      if(ThisEvent.EventType == ES_TIMEOUT && ThisEvent.EventParam == BlinkTimer) {
        ES_Timer_InitTimer(BlinkTimer,BLINK_TIME);
        //flip the led
        if(flash) {
          Serial.println("blink off");
          flash = false;
          analogWrite(lightLED,0);
          analogWrite(accelLED,0);
        } else {
          Serial.println("blink on");
          flash = true;
          analogWrite(lightLED,255);
          analogWrite(accelLED,255);
        }
      }
      break;
  
    case SolidState :     
      if (connection && LEDmode && (!LEDstate || night) || (!connection && moving)) {
        CurrentState = BlinkingState; 
        Serial.println("Blinking State");
        //start the timer:
        ES_Timer_InitTimer(BlinkTimer,BLINK_TIME);
      }  
    
      if ((!connection && !moving) || (connection && (!night && LEDstate)))  {
        CurrentState = LEDOffState;
        Serial.println("LED Off State");
        //turn that shit off
        analogWrite(lightLED,0);
        analogWrite(accelLED,0);
      }
      break;
  
    case LEDOffState :       // If current state is state one
      if (connection && LEDmode && (!LEDstate || night) || (!connection && moving)) {
        CurrentState = BlinkingState; 
        Serial.println("Blinking State");
        //start the timer:
        ES_Timer_InitTimer(BlinkTimer,BLINK_TIME);
      }
    
      if (connection && !LEDmode && (!LEDstate || night)){
        CurrentState = SolidState;
        Serial.println("Solid State");
        //turn that shit on
        analogWrite(lightLED,255);
        analogWrite(accelLED,255);
      }
      break;
  }                                   // end switch on Current State
  return ReturnEvent;
}

/****************************************************************************
Function
QueryTemplateSM

Parameters
None

Returns
TemplateState_t The current state of the Template state machine

Description
returns the current state of the Template state machine
Notes

Author
J. Edward Carryer, 10/23/11, 19:21
****************************************************************************/
TemplateState_t QueryTemplateFSM ( void )
{
  return(CurrentState);
}

bool CheckAccelerometer(void) {
  static unsigned pastVals[10] = {0,0,0,0,0,0,0,0,0,0};
  static index = 0;
  static bool moving = false;
  ES_Event newEvent;
  if(accel.available()) {
    accel.read();
    accel_vector = sqrt(pow(accel.x,2) + pow(accel.y,2) + pow(accel.z,2));
    pastVals(index) = accel_vector;
    unsigned avg = 0;
    for(unsigned i = 0; i < 10; i++) {
      avg += pastVals(i);
    }
    avg /= 10;
    if(avg > ACCEL_MOVING_THRESHOLD && moving == false) {
      newEvent.EventType = ES_MOVING;
      PostTemplateFSM(newEvent);
      moving = true;
    }else if(avg <= ACCEL_MOVING_THRESHOLD && moving == true) {
      newEvent.EventType = ES_NOTMOVING;
      PostTemplateFSM(newEvent);
      moving = false;
    }
    index++;
    if(index >= 10) index-=10;
  }
  return false;
}

bool CheckAccel(void) {
  static bool isMoving = false;
  ES_Event newEvent;
  if(accel.available()) {
    accel.read();
    accel_vector = sqrt(pow(accel.x,2) + pow(accel.y,2) + pow(accel.z,2));
    int shakeLevel = map(accel_vector,1100,4095,0,255);
    if(shakeLevel > 255) shakeLevel = 255;
    else if(shakeLevel < 0) shakeLevel = 0;
    if(shakeLevel > ACCEL_MOVING_THRESH && isMoving == false) { //moving
      newEvent.EventType = Moving;
      PostTemplateFSM(newEvent);
      isMoving = true;
      return true;
    } else if(shakeLevel <= ACCEL_MOVING_THRESH && isMoving == true) { //not moving
      newEvent.EventType = NotMoving;
      PostTemplateFSM(newEvent);
      isMoving = false;
      return true;
    }
  }
  return false;
}

bool CheckLight(void) {
  static bool bright = true;
  ES_Event newEvent;
  unsigned light = analogRead(lightPin);
  if(light > LIGHT_THRESH && bright == true) { //it is dark
    newEvent.EventType = Dark;
    PostTemplateFSM(newEvent);
    bright = false;
    return true;
  }else if(light <= LIGHT_THRESH && bright == false) { //it is light
    newEvent.EventType = Bright;
    PostTemplateFSM(newEvent);
    bright = true;
    return false;
  }
  return false;
}

  /***************************************************************************
  private functions
  ***************************************************************************/

