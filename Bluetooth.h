/****************************************************************************
 
  Header file for template Flat Sate Machine 
  based on the Gen2 Events and Services Framework

 ****************************************************************************/

#ifndef BLUETOOTH_H
#define BLUETOOTH_H

// Event Definitions
#include "ES_Configure.h" /* gets us event definitions */
#include "ES_Types.h"     /* gets bool type for returns */


// Public Function Prototypes
#ifdef __cplusplus
extern "C" {
#endif

void CheckBluetooth(void);
bool CheckBluetoothConnection(void);


#ifdef __cplusplus
}
#endif

#endif /* BLUETOOTH_H */

