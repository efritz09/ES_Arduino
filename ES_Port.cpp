/****************************************************************************
 Module
   ES_Port.c

 Revision
   1.0.1

 Description
   This is the sample file to demonstrate adding the hardware specific
   functions to the Events & Services Framework. This sample is also used
   as the file for the port to the Freescale MC9S12C32 processor.

 Notes

 History
 When           Who     What/Why
 -------------- ---     --------
 08/13/13 12:42 jec     moved the hardware specific aspects of the timer here
 08/06/13 13:17 jec     Began moving the stuff from the V2 framework files
****************************************************************************/


extern "C" {
#include "ES_Port.h"
#include "ES_Types.h"
#include "ES_Timers.h"

}
#include "Arduino.h"






// TickCount is used to track the number of timer ints that have occurred
// since the last check. It should really never be more than 1, but just to
// be sure, we increment it in the interrupt response rather than simply
// setting a flag. Using this variable and checking approach we remove the
// need to post events from the interrupt response routine. This is necesary
// for compilers like HTC for the midrange PICs which do not produce re-entrant
// code so cannot post directly to the queues from within the interrupt resp.
//void SysTick_Handler(void);

static uint8_t TickCount;



/****************************************************************************
 Function
     _Timer_HW_Init
 Parameters
     unsigned char Rate set to one of the TMR_RATE_XX values to set the
     Tick rate
 Returns
     None.
 Description
     Initializes the timer module by setting up the RTI with the requested
    rate
 Notes
     modify as required to port to other timer hardware
 Author
     J. Edward Carryer, 02/24/97 14:23
****************************************************************************/
void _HW_Timer_Init(TimerRate_t Rate)
{
    // set Timer2 in CTC mode, 1/1024 prescaler, start the timer ticking
    TCCR2A = (1 << WGM21) | (0 << WGM20);
    TCCR2B = (1 << CS22 ) | (1 << CS21) | (1 << CS20); // 1/2^10
    ASSR  &= ~(1 << AS2);
    TIMSK2 = (1 << OCIE2A); // Enable TIMER2 compare Interrupt
    TCNT2  = 0;
    OCR2A  = (F_CPU / Rate / 1024) - 1;



}

/****************************************************************************
 Function
     _Timer_INT_Resp
 Parameters
     none
 Returns
     None.
 Description
     interrupt response routine for the tick interupt that will allow the
     framework timers to run.
 Notes
     As currently (8/13/13) implemented this does not actually post events
     but simply sets a flag to indicate that the interrupt has occurred.
     the framework resonse is handled below in _HW_Process_Pending_Ints
 Author
     J. Edward Carryer, 08/13/13 13:29
****************************************************************************/

ISR(TIMER2_COMPA_vect) {
    ++TickCount;



}

/****************************************************************************
 Function
     _HW_Process_Pending_Ints
 Parameters
     none
 Returns
     always true.
 Description
     processes any pending interrupts (actually the hardware interrupt already
     occurred and simply set a flag to tell this routine to execute the non-
     hardware response)
 Notes
     While this routine technically does not need a return value, we always
     return true so that it can be used in the conditional while() loop in
     ES_Run. This way the test for pending interrupts get processed after every
     run function is called and even when there are no queues with events.
     This routine could be expanded to process any onther interrupt sources
     that you would like to use to post events to the framework services.
 Author
     J. Edward Carryer, 08/13/13 13:27
****************************************************************************/
bool _HW_Process_Pending_Ints( void )
{
   while (TickCount > 0)
   {
      /* call the framework tick response to actually run the timers */
      ES_Timer_Tick_Resp();
      TickCount--;
   }
   return true; // always return true to allow loop test in ES_Run to proceed
}
