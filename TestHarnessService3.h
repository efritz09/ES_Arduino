/****************************************************************************
 
  Header file for Test Harness Service 3 
  based on the Gen 2 Events and Services Framework

 ****************************************************************************/

#ifndef Bluetooth_H
#define Bluetooth_H

#include "ES_Types.h"
#include "ES_Events.h"
// Public Function Prototypes

bool InitBluetooth ( uint8_t Priority );
bool PostBluetooth( ES_Event ThisEvent );
ES_Event RunBluetooth( ES_Event ThisEvent );


#endif /* Bluetooth_H */

