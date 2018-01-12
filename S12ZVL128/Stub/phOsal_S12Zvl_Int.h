/*
   (c) NXP B.V. 2009-2015. All rights reserved.

   Disclaimer
   1. The NXP Software/Source Code is provided to Licensee "AS IS" without any
      warranties of any kind. NXP makes no warranties to Licensee and shall not
      indemnify Licensee or hold it harmless for any reason related to the NXP
      Software/Source Code or otherwise be liable to the NXP customer. The NXP
      customer acknowledges and agrees that the NXP Software/Source Code is
      provided AS-IS and accepts all risks of utilizing the NXP Software under
      the conditions set forth according to this disclaimer.

   2. NXP EXPRESSLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING,
      BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
      FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT OF INTELLECTUAL PROPERTY
      RIGHTS. NXP SHALL HAVE NO LIABILITY TO THE NXP CUSTOMER, OR ITS
      SUBSIDIARIES, AFFILIATES, OR ANY OTHER THIRD PARTY FOR ANY DAMAGES,
      INCLUDING WITHOUT LIMITATION, DAMAGES RESULTING OR ALLEGDED TO HAVE
      RESULTED FROM ANY DEFECT, ERROR OR OMMISSION IN THE NXP SOFTWARE/SOURCE
      CODE, THIRD PARTY APPLICATION SOFTWARE AND/OR DOCUMENTATION, OR AS A
      RESULT OF ANY INFRINGEMENT OF ANY INTELLECTUAL PROPERTY RIGHT OF ANY
      THIRD PARTY. IN NO EVENT SHALL NXP BE LIABLE FOR ANY INCIDENTAL,
      INDIRECT, SPECIAL, EXEMPLARY, PUNITIVE, OR CONSEQUENTIAL DAMAGES
      (INCLUDING LOST PROFITS) SUFFERED BY NXP CUSTOMER OR ITS SUBSIDIARIES,
      AFFILIATES, OR ANY OTHER THIRD PARTY ARISING OUT OF OR RELATED TO THE NXP
      SOFTWARE/SOURCE CODE EVEN IF NXP HAS BEEN ADVISED OF THE POSSIBILITY OF
      SUCH DAMAGES.

   3. NXP reserves the right to make changes to the NXP Software/Sourcecode any
      time, also without informing customer.

   4. Licensee agrees to indemnify and hold harmless NXP and its affiliated
      companies from and against any claims, suits, losses, damages,
      liabilities, costs and expenses (including reasonable attorney's fees)
      resulting from Licensee's and/or Licensee customer's/licensee's use of the
      NXP Software/Source Code.

 */


/*
 * $Revision: 315 $
 * $URL: https://www.collabnet.nxp.com/svn/reference_designs/door_handle/trunk/D%20Software/onhost/DHRD%20V2_0/S12ZVL128/Stub/phOsal_S12Zvl_Int.h $
 */      

#ifndef PHOSAL_S12ZVL_INT_H
#define PHOSAL_S12ZVL_INT_H

#include <phOsal.h>
#include "phOsal_Stub.h"
#include "ph_NxpBuild.h"
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "PE_Timer.h"
#include "SPI.h"
#include "PIN_RESET.h"
#include "PIN_SSEL.h"
#include "LED_BIT.h"
#include "TMR0.h"
#include "TMR1.h"
#include "TMR2.h"
#include "Terminal.h"
#include "Inhr1.h"
#include "Utility.h"
#include "LED_R_BIT.h"

#define S12_MAX_TIMERS		 		3U		   /* number of available timers */
#define S12_DEFAULT_TIME_INTERVAL   10000U     /**< Default timer interval that will be loaded */

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
/**
* \ Initializes timers to the application
*/

void phOsal_s12_Int_Timer_Init(phOsal_Stub_DataParams_t *pDataParams);

/**
* Creates a timer component.
* Internally, an array is maintained which stores timers along with
* information as to whether the timer is available or not. This
* function searches a free timer that is available and returns the
* timer id in \a pTimerId. If there are no free timers,
* then appropriate error code is returned.
*
* \return   status code
* \retval   #PH_ERR_SUCCESS on success
* \retval   #PH_OSAL_ERR_NO_FREE_TIMER if no timers are available
*
*/

void phOsal_s12_Int_Timer_Create( phOsal_Stub_DataParams_t *pDataParams, uint32_t *pTimerId );

/**
* Timer Register.
* \retval  #PH_ERR_SUCCESS on success
*
*/

phStatus_t phOsal_s12_Int_Timer_Register( phOsal_Stub_DataParams_t  *pDataParams,
                                             uint32_t                       dwTimerId,
                                             uint32_t                       dwRegTimeCnt,
                                             uint16_t                       wOption,
                                             ppCallBck_t                    pApplicationCallback,
                                             void                          *pContext
                                             );

/**
* Stops the said timer.
* This function does not free the timer. It only disables the timer.
* Use phOsal_Timer_Delete() to free the timer.
*
* \return  status code
* \retval  #PH_OSAL_ERR_INVALID_TIMER if the timer ID supplied was invalid
* \retval  #PH_ERR_SUCCESS            on success
*
*/

phStatus_t phOsal_s12_Int_Timer_Stop( uint32_t dwTimerId );

/**
* Frees the timer.
* When this function is called, the timer with given ID is returned to the
* free timer pool.
*
* \return  status code
* \retval  #PH_OSAL_ERR_INVALID_TIMER if the timer ID supplied was invalid
* \retval  #PH_ERR_SUCCESS            on success
*
*/

phStatus_t phOsal_s12_Int_Timer_Delete( phOsal_Stub_DataParams_t *pDataParams, uint32_t dwTimerId );

/**
* Delays the timer in milli seconds.
* When this function is called, the timer waits until delay time has elapsed.
*
*
*/

void phOsal_s12_Int_Delay32Ms( uint32_t dwTimerId, uint32_t delayInMs );

/**
* Delays the timer in micro seconds.
* When this function is called, the timer waits until delay time has elapsed.
*
*
*/

void phOsal_s12_Int_Delay32Us( uint32_t dwTimerId, uint32_t delayInUs );

/**
* IRQ Handler for Timer0.
*
*/

void phOsal_s12_Int_TIMER0_IRQHandler(void);

/**
* IRQ Handler for Timer1.
*
*/

void phOsal_s12_Int_TIMER1_IRQHandler(void);

/**
* IRQ Handler for Timer2.
*
*/

void phOsal_s12_Int_TIMER2_IRQHandler(void);

/**
* Start the timer
* When this function is called with timer number provided
* as an argument, the corresponding timer is enabled.
*
*
*/

void phOsal_s12_Int_Enable_Timer32( uint32_t dwTimerId );

/**
* Stop the timer.
* When this function is called with timer number provided
* as an argument, the corresponding timer is disabled.
*
*
*/

void phOsal_s12_Int_Disable_Timer32( uint32_t dwTimerId );

/**
* Timer Reset
*
*/

void phOsal_s12_Int_Reset_Timer32( uint32_t dwTimerId );

/**
* \ Initializes the timer.
*/

void phOsal_s12_Int_Init_Timer32( uint32_t dwTimerId, uint32_t TimerInterval );

/**
* \ Load the given timer interval.
*/

void phOsal_s12_Int_Load_TimerMs( uint32_t dwTimerId, uint32_t dwTimerInterval );

/**
* \ Load the given timer interval.
*/

void phOsal_s12_Int_Load_TimerUs( uint32_t dwTimerId, uint32_t dwTimerInterval );


/* END Events */
#endif /* PHOSAL_S12ZVL_INT_H */

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
