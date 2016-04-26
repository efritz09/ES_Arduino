#ifndef BLUETOOTH_H
#define BLUETOOTH_H

// Event Definitions
#include "ES_Configure.h" /* gets us event definitions */
#include "ES_Types.h"     /* gets bool type for returns */

#ifdef __cplusplus
extern "C" {
#endif

void CheckBluetooth(void);
bool CheckBluetoothConnection(void);
void WriteBluetooth(void);

#ifdef __cplusplus
}
#endif

#endif /* BLUETOOTH_H */

