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
 * $URL: https://www.collabnet.nxp.com/svn/reference_designs/door_handle/trunk/D%20Software/onhost/DHRD%20V2_0/S12ZVL128/Stub/phOsal_S12Zvl_Int.c $
 */


#include "Cpu.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

#include <ph_Status.h>

#ifdef NXPBUILD__PH_OSAL_STUB

#include "phOsal_S12Zvl_Int.h"
#include <phOsal.h>

#pragma CODE_SEG DEFAULT

static phOsal_Stub_DataParams_t* pTimerOsal;

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
void TMR0_OnInterrupt(void)
{

    /* This is the only timer we are going to use for call backs  */
    phOsal_Timer_ExecCallback(pTimerOsal, 0);
    //LED_BIT_NegVal();
}

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
void TMR2_OnInterrupt(void)
{
  /* Write your code here ... */
	phOsal_Timer_ExecCallback(pTimerOsal, 2);
}

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
void TMR1_OnInterrupt(void)
{
  /* Write your code here ... */
	phOsal_Timer_ExecCallback(pTimerOsal, 1);
}

/*
 * Timers handling functions */
void phOsal_s12_Int_Timer_Init(phOsal_Stub_DataParams_t *pDataParams)
{
    uint8_t count = 0;

    /* Initialize the timer structure */
    for (count = 0; count < S12_MAX_TIMERS; count++)
    {
        pDataParams->gTimers[count].dwTimerId   = count;
        pDataParams->gTimers[count].bTimerFree = TIMER_FREE;
    }

    /* Also keep the timers ready with all the default settings.
      This also does the job of registering the IRQ with NVIC */
    
    /* This is done in the PE_init function */

    pTimerOsal = pDataParams;
}

void phOsal_s12_Int_Timer_Create(phOsal_Stub_DataParams_t *pDataParams,
        uint32_t *pTimerId)
{
    uint8_t count = 0;

    *pTimerId = -1;
    /* Look for a free timer that is available for use */
    for (count = 0; count < S12_MAX_TIMERS; count++)
    {
        if (pDataParams->gTimers[count].bTimerFree == TIMER_FREE)
        {
            /* Use this timer */
            pDataParams->gTimers[count].bTimerFree = TIMER_USED;
            pDataParams->gTimers[count].dwTimerId = count;
            *pTimerId = pDataParams->gTimers[count].dwTimerId;

            break;
        }
    }
}

phStatus_t phOsal_s12_Int_Timer_Register(phOsal_Stub_DataParams_t  *pDataParams,
                                             uint32_t                      dwTimerId,
                                             uint32_t                      dwRegTimeCnt,
                                             uint16_t                      wOption,
                                             ppCallBck_t                   pApplicationCallback,
                                             void                         *pContext
                                             )
{
    if (dwTimerId > S12_MAX_TIMERS ||  pDataParams->gTimers[dwTimerId].bTimerFree == TIMER_FREE)
    {
        /* Can't use a non existent timer */
        /* Can't start a free timer, first create the timer */
        
        return PH_ERR_INTERNAL_ERROR;
    }

    /* Remember the call back function as well as argument to be passed */
    pDataParams->gTimers[dwTimerId].pContext = pContext;
    pDataParams->gTimers[dwTimerId].pApplicationCallback = pApplicationCallback;

    phOsal_s12_Int_Init_Timer32(dwTimerId, dwRegTimeCnt);
    if (wOption == 1)
    {
        /* Load the delay value in Milli seconds */
        phOsal_s12_Int_Load_TimerMs(dwTimerId, dwRegTimeCnt);
    }
    else if (wOption == 0)
    {
        /* Load the delay value in Micro seconds */
        phOsal_s12_Int_Load_TimerUs(dwTimerId, dwRegTimeCnt);
    }
    else
    {
        return PH_ERR_INTERNAL_ERROR;
    }

    /* Start the timer */
    phOsal_s12_Int_Enable_Timer32(dwTimerId);

    return PH_ERR_SUCCESS;
}

phStatus_t phOsal_s12_Int_Timer_Stop(uint32_t dwTimerId)
{
    if (dwTimerId > S12_MAX_TIMERS)
    {
        return PH_ERR_INTERNAL_ERROR;
    }
    /* Stop the timer */
    phOsal_s12_Int_Disable_Timer32(dwTimerId);

    return PH_ERR_SUCCESS;
}

phStatus_t phOsal_s12_Int_Timer_Delete(phOsal_Stub_DataParams_t *pDataParams,uint32_t dwTimerId)
{
    if (dwTimerId > S12_MAX_TIMERS)
    {
        return PH_ERR_INTERNAL_ERROR;
    }
    /* Stop the timer and also mark the timer as available */
    phOsal_s12_Int_Disable_Timer32(dwTimerId);

    pDataParams->gTimers[dwTimerId].dwTimerId = -1;
    pDataParams->gTimers[dwTimerId].bTimerFree = TIMER_FREE;
    pDataParams->gTimers[dwTimerId].pApplicationCallback = NULL;

    return PH_ERR_SUCCESS;
}

void phOsal_s12_Int_Delay32Ms(uint32_t dwTimerId, uint32_t delayInMs)
{
    if (dwTimerId == 0)
    {
        /* setup timer #0 for delay */
    	TMR0_Disable();
    	TMR0_Init();
    	TMR0_SetPeriodMS(delayInMs);
    	TMR0_Enable();

        /* wait until delay time has elapsed */
    	while(TIM0TFLG1_C0F == 0U)
    	{
    		/* Do nothing */
    	}

    }
    else if (dwTimerId == 1)
    {
        /* setup timer #1 for delay */
    	TMR1_Disable();
    	TMR1_Init();
    	TMR1_SetPeriodMS(delayInMs);
    	TMR1_Enable();

        /* wait until delay time has elapsed */
    	while(TIM0TFLG1_C1F == 0U)
    	{
    		/* Do nothing */
    	}
    }
    else if (dwTimerId == 2)
    {
        /* setup timer #2 for delay */
    	TMR2_Disable();
    	TMR2_Init();
    	TMR2_SetPeriodMS(delayInMs);
    	TMR2_Enable();

        /* wait until delay time has elapsed */
    	while(TIM0TFLG1_C2F == 0U)
    	{
    		/* Do nothing */
    	}
    }
    else if (dwTimerId == 3)
    {
        /* setup timer #3 for delay */

        /* wait until delay time has elapsed */

    }

    return;
}

void phOsal_s12_Int_Delay32Us(uint32_t dwTimerId, uint32_t delayInUs)
{
    if (dwTimerId == 0)
    {
        /* setup timer #0 for delay */
    	TMR0_Init();
    	TMR0_SetPeriodMS(delayInUs);
    	TMR0_Enable();

        /* wait until delay time has elapsed */
    	while(TIM0TFLG1_C0F == 0U)
    	{
    		/* Do nothing */
    	}
    }
    else if (dwTimerId == 1)
    {
        /* setup timer #1 for delay */
    	TMR1_Init();
    	TMR1_SetPeriodMS(delayInUs);
    	TMR1_Enable();

        /* wait until delay time has elapsed */
    	while(TIM0TFLG1_C1F == 0U)
    	{
    		/* Do nothing */
    	}
    }
    else if (dwTimerId == 2)
    {
        /* setup timer #1 for delay */
    	TMR2_Init();
    	TMR2_SetPeriodMS(delayInUs);
    	TMR2_Enable();

        /* wait until delay time has elapsed */
    	while(TIM0TFLG1_C2F == 0U)
    	{
    		/* Do nothing */
    	}
    }
    else if (dwTimerId == 3)
    {
        /* setup timer #1 for delay */

        /* wait until delay time has elapsed */

    }

    return;
}

void phOsal_s12_Int_Enable_Timer32(uint32_t dwTimerId)
{
    if ( dwTimerId == 0 )
    {
    	TMR0_Enable();
    }
    else if ( dwTimerId == 1 )
    {
    	TMR1_Enable();
    }
    else if  ( dwTimerId == 2 )
    {
    	TMR2_Enable();
    }
    else
    {

    }

    return;
}

void phOsal_s12_Int_Disable_Timer32(uint32_t dwTimerId)
{
    if ( dwTimerId == 0 )
    {
    	TMR0_Disable();
    }
    else if  ( dwTimerId == 1 )
    {
    	TMR1_Disable();
    }
    else if  ( dwTimerId == 2 )
    {
    	TMR2_Disable();
    }
    else
    {

    }

    return;
}

void phOsal_s12_Int_Reset_Timer32(uint32_t dwTimerId)
{
    uint32_t regVal;

    if ( dwTimerId == 0 )
    {

    }
    else if (dwTimerId == 1)
    {

    }
    else if (dwTimerId == 2)
    {

    }
    else if (dwTimerId == 3)
    {

    }

    return;
}

void phOsal_s12_Int_Init_Timer32(uint32_t dwTimerId, uint32_t TimerInterval)
{

    if ( dwTimerId == 0 )
    {
    	//TMR0_SetPeriodUS(TimerInterval);
    	TMR0_EnableEvent();

    }
    else if ( dwTimerId == 1 )
    {
    	//TMR1_SetPeriodUS(TimerInterval);
    	TMR1_EnableEvent();
    }
    else if ( dwTimerId == 2 )
    {
    	//TMR2_SetPeriodUS(TimerInterval);
    	TMR2_EnableEvent();
    }
    else if ( dwTimerId == 3 )
    {

    }

    return;
}

void phOsal_s12_Int_Load_TimerMs(uint32_t dwTimerId, uint32_t dwTimerInterval)
{
    /* Load the given timer interval onto match register 0 */
    if (dwTimerId == 0)
    {
    	TMR0_SetPeriodMS(dwTimerInterval);
    }
    else if (dwTimerId == 1)
    {
    	TMR1_SetPeriodMS(dwTimerInterval);
    }
    else if (dwTimerId == 2)
    {
    	TMR2_SetPeriodMS(dwTimerInterval);
    }
    else if (dwTimerId == 3)
    {

    }

    return;
}

void phOsal_s12_Int_Load_TimerUs(uint32_t dwTimerId, uint32_t dwTimerInterval)
{
    /* Load the given timer interval onto match register 0 */
    if (dwTimerId == 0)
    {
    	TMR0_SetPeriodUS(dwTimerInterval);
    }
    else if (dwTimerId == 1)
    {
    	TMR1_SetPeriodUS(dwTimerInterval);
    }
    else if (dwTimerId == 2)
    {
    	TMR2_SetPeriodUS(dwTimerInterval);
    }
    else if (dwTimerId == 3)
    {

    }

    return;
}

#endif /* NXPBUILD__PH_OSAL_STUB */
/* END Events */

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


