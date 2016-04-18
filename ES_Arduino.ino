#include <stdio.h>
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "Arduino.h"


void setup() {
  // put your setup code here, to run once:
  
  
   Serial.begin(115200);

   DDRB  = 0xFF; // All PORTB pins are outputs (user LED)
   PORTB &= ~(1<<7); // drive all pins low
   
}

void loop() {
  
  ES_Return_t ErrorType = ES_Initialize(ES_Timer_RATE_1MS);
  // put your main code here, to run repeatedly:
  if ( ErrorType == Success ) {

    ErrorType = ES_Run();

  }
  //if we got to here, there was an error
  switch (ErrorType){
    case FailedPost:
      Serial.println("Failed on attempt to Post");
      break;
    case FailedPointer:
      Serial.println("Failed on NULL pointer");
      break;
    case FailedInit:
      Serial.println("Failed Initialization");
      break;
   default:
      Serial.println("Other Failure");
      break;
}
for(;;)
  ;

}
