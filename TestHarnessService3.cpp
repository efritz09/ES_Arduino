/****************************************************************************
 Module
   TestHarnessService3.c

 Revision
   1.0.1

 Description
   This is the first service for the Test Harness under the
   Gen2 Events and Services Framework.

 Notes

 History
 When           Who     What/Why
 -------------- ---     --------
 10/21/13 19:38 jec      created to test 16 possible serves, we need a bunch
                         of service test harnesses
 08/05/13 20:33 jec      converted to test harness service
 01/16/12 09:58 jec      began conversion from TemplateFSM.c
****************************************************************************/
/*----------------------------- Include Files -----------------------------*/
/* include header files for the framework and this service
*/

#include <stdio.h>

extern "C" {
	
	#include "ES_Configure.h"
	#include "ES_Framework.h"
	#include "templateFSM.h"

}
#include "Arduino.h"
/*----------------------------- Module Defines ----------------------------*/
// these times assume a 1.024mS/tick timing
#define ONE_SEC 1000
#define HALF_SEC (ONE_SEC/2)
#define TWO_SEC (ONE_SEC*2)
#define FIVE_SEC (ONE_SEC*5)

/*---------------------------- Module Functions ---------------------------*/
/* prototypes for private functions for this service.They should be functions
   relevant to the behavior of this service
*/

/*---------------------------- Module Variables ---------------------------*/
// with the introduction of Gen2, we need a module level Priority variable
static uint8_t MyPriority;

static int i = 0;
/*------------------------------ Module Code ------------------------------*/
/****************************************************************************
 Function
     InitTestHarnessService3

 Parameters
     uint8_t : the priorty of this service

 Returns
     bool, false if error in initialization, true otherwise

 Description
     Saves away the priority, and does any
     other required initialization for this service
 Notes

 Author
     J. Edward Carryer, 01/16/12, 10:00
****************************************************************************/
bool InitBluetooth ( uint8_t Priority )
{
  ES_Event ThisEvent;

  MyPriority = Priority;
  /********************************************
   in here you write your initialization code
   *******************************************/
  // post the initial transition event
  ThisEvent.EventType = ES_INIT;
	ES_Timer_InitTimer(TEST_TIMER, FIVE_SEC);
	i=0;
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
     PostTestHarnessService3

 Parameters
     EF_Event ThisEvent ,the event to post to the queue

 Returns
     bool false if the Enqueue operation failed, true otherwise

 Description
     Posts an event to this state machine's queue
 Notes

 Author
     J. Edward Carryer, 10/23/11, 19:25
****************************************************************************/
bool PostBluetooth( ES_Event ThisEvent )
{
  return ES_PostToService( MyPriority, ThisEvent);
}

/****************************************************************************
 Function
    RunTestHarnessService3

 Parameters
   ES_Event : the event to process

 Returns
   ES_Event, ES_NO_EVENT if no error ES_ERROR otherwise

 Description
   add your description here
 Notes

 Author
   J. Edward Carryer, 01/15/12, 15:23
****************************************************************************/
ES_Event RunBluetooth( ES_Event ThisEvent )
{
  ES_Event ReturnEvent;
  ReturnEvent.EventType = ES_NO_EVENT; // assume no errors
	int j = i;
  switch (ThisEvent.EventType){
    case ES_INIT :
     // ES_Timer_InitTimer(SERVICE3_TIMER, HALF_SEC);
      //puts("Service 03:");
//      printf("ES_INIT received in Service %d\r\n", MyPriority);
    break;

    case ES_TIMEOUT :  // re-start timer & announce
      ES_Timer_InitTimer(TEST_TIMER, ONE_SEC);
      //printf("ES_TIMEOUT received from Timer %d in Service %d\r\n",
      //        ThisEvent.EventParam, MyPriority);
			i++;
    if (PORTB&(1<<7))
    {
      PORTB &= ~(1 << 7);
    }
    else
    {
      PORTB |= (1 << 7);
    }
    break;

    case EV_SERIAL_RECEIVE :  // announce
      //printf("ES_NEW_KEY: received ->%c<- in Service %d\r\n",
     //        ThisEvent.EventParam, MyPriority);
    break;
}
  return ReturnEvent;
}

/***************************************************************************
 private functions
 ***************************************************************************/

/*------------------------------- Footnotes -------------------------------*/
/*------------------------------ End of file ------------------------------*/

