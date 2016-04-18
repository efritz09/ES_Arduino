/****************************************************************************
 
  Header file for template Flat Sate Machine 
  based on the Gen2 Events and Services Framework

 ****************************************************************************/

#ifndef FSMTemplate_H
#define FSMTemplate_H

// Event Definitions
#include "ES_Configure.h" /* gets us event definitions */
#include "ES_Types.h"     /* gets bool type for returns */

// typedefs for the states
// State definitions for use with the query function
typedef enum { Start, Blinking, Solid, 
               LEDOff } TemplateState_t ;


// Public Function Prototypes
#ifdef __cplusplus
extern "C" {
#endif
bool InitTemplateFSM ( uint8_t Priority );
bool PostTemplateFSM( ES_Event ThisEvent );
ES_Event RunTemplateFSM( ES_Event ThisEvent );
TemplateState_t QueryTemplateSM ( void );
#ifdef __cplusplus
}
#endif

#endif /* FSMTemplate_H */

