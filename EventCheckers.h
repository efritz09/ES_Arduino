/****************************************************************************
 Module
     EventCheckers.h
 Description
     header file for the event checking functions
 Notes

 History
 When           Who     What/Why
 -------------- ---     --------
 08/06/13 14:37 jec      started coding
*****************************************************************************/

#ifndef EventCheckers_H
#define EventCheckers_H

// prototypes for event checkers

#ifdef __cplusplus
extern "C" {
#endif

bool CheckSerial(void);
bool CheckMode(void);

#ifdef __cplusplus
}
#endif




#endif /* EventCheckers_H */
