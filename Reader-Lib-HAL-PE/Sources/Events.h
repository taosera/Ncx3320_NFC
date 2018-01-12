/* ###################################################################
**     Filename    : Events.h
**     Project     : Reader-Lib-HAL-PE
**     Processor   : MC9S12ZVL32MLC
**     Component   : Events
**     Version     : Driver 01.04
**     Compiler    : CodeWarrior HCS12Z C Compiler
**     Date/Time   : 2015-08-26, 09:51, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.04
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Timer.h"
#include "SPI.h"
#include "PIN_RESET.h"
#include "LED_BIT.h"
#include "TMR0.h"
#include "TMR1.h"
#include "TMR2.h"
#include "Terminal.h"
#include "Inhr1.h"
#include "Utility.h"
#include "LED_R_BIT.h"
#include "EN_PWR.h"
#include "PIN_SSEL.h"

#pragma CODE_SEG DEFAULT


void TMR0_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  TMR0_OnInterrupt (module Events)
**
**     Component   :  TMR0 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void TMR2_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  TMR2_OnInterrupt (module Events)
**
**     Component   :  TMR2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void TMR1_OnInterrupt(void);
/*
** ===================================================================
**     Event       :  TMR1_OnInterrupt (module Events)
**
**     Component   :  TMR1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END Events */
#endif /* __Events_H*/

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale HCS12Z series of microcontrollers.
**
** ###################################################################
*/
