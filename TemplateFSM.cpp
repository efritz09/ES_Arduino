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

#include <Arduino.h>

/*----------------------------- Module Defines ----------------------------*/

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
static bool Connection = false;
static bool Mode = false;
static bool State = false;
static bool LightSensorState = false;
static bool AccelState = false;

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
  CurrentState = Start;
  // post the initial transition event
  Serial.println("main code initialized");
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
  if (NewEvent == ES_CONNECTED) Connection = true;
  else if (NewEvent == ES_DISCONNECTED) Connection = false;
  else if (NewEvent == ES_BLINK) Mode = true; //Blink mode = 1
  else if (NewEvent == ES_SOLID) Mode = false;
  else if (NewEvent == ES_AUTO) State = true;
  else if (NewEvent == ES_ON) State = false;
  else if (NewEvent == ES_DARK) LightSensorState = true;
  else if (NewEvent == ES_BRIGHT) LightSensorState = false;
  else if (NewEvent == ES_MOVING) AccelState = true;
  else if (NewEvent == ES_NOTMOVING) AccelState = false;
  Serial.print("Connection: ");
  Serial.println(Connection);
  Serial.print("Mode: ");
  Serial.println(Mode);
  Serial.print("State: ");
  Serial.println(State);
  Serial.print("Light Sensor: ");
  Serial.println(LightSensorState);
  Serial.print("Accelerometer: ");
  Serial.println(AccelState);
//  else if (NewEvent == ES_WRONG) printf("Something is wrong\n");
// (Connection && !Mode && (!State || LightSensorState))
  switch ( CurrentState )
  {
      case Start :       // If current state is initial Psedudo State
           if (Connection && Mode && (!State || LightSensorState) || (!Connection && AccelState)) {
            CurrentState = Blinking; 
            Serial.println("Blinking State");
           }

           if (Connection && !Mode && (!State || LightSensorState)){
            CurrentState = Solid;
            Serial.println("Solid State");
           }

           if ((!Connection && !AccelState) || (Connection && !LightSensorState)) {
            CurrentState = LEDOff;
            Serial.println("LEDOff state");
           }
           break;

      case Blinking :       // If current state is state one
           if (Connection && !Mode && (!State || LightSensorState)){
            CurrentState = Solid;
            Serial.println("Solid State");
           }

           if ((!Connection && !AccelState) || (Connection && !LightSensorState)) {
            CurrentState = LEDOff;
            Serial.println("LED Off State");
           }
          break;
      // repeat state pattern as required for other states
      
      case Solid :     
           if (Connection && Mode && (!State || LightSensorState) || (!Connection && AccelState)) {
            CurrentState = Blinking; 
            Serial.println("Blinking State");
           }  

           if ((!Connection && !AccelState) || (Connection && !LightSensorState)) {
            CurrentState = LEDOff;
            Serial.println("LED Off State");
           }
      
          break;
          
      case LEDOff :       // If current state is state one
           if (Connection && Mode && (!State || LightSensorState) || (!Connection && AccelState)) {
            CurrentState = Blinking; 
            Serial.println("Blinking State");
           }

           if (Connection && !Mode && (!State || LightSensorState)){
            CurrentState = Solid;
            Serial.println("Solid State");
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

/***************************************************************************
 private functions
 ***************************************************************************/
