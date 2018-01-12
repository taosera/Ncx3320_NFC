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
 * $URL: https://www.collabnet.nxp.com/svn/reference_designs/door_handle/trunk/D%20Software/onhost/DHRD%20V2_0/S12ZVL128/Stub/phOsal_Stub.c $
 */

#include <ph_Status.h>
#include <ph_NxpBuild.h>

#ifdef NXPBUILD__PH_OSAL_STUB

#include <phOsal.h>
#include <phOsal_Stub.h>
#include "phOsal_S12Zvl_Int.h"
#include "cpu.h"
#include "TMR0.h"
#include "TMR1.h"
#include "TMR2.h"

phStatus_t phOsal_Stub_Init( phOsal_Stub_DataParams_t  *pDataParams )
{
    pDataParams->wId = PH_COMP_OSAL | PH_OSAL_STUB_ID;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
}

phStatus_t phOsal_Stub_Timer_Init( phOsal_Stub_DataParams_t  *pDataParams )
{
    /* Lets be ready with the timer */
    phOsal_s12_Int_Timer_Init(pDataParams);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
}

phStatus_t phOsal_Stub_Timer_Create( phOsal_Stub_DataParams_t   *pDataParams,
                                       uint32_t                       *pTimerId
                                       )
{
    phOsal_s12_Int_Timer_Create(pDataParams, pTimerId);
    
    if (*pTimerId == -1)
    {
        /* No timer found, need to return error */
        return PH_ADD_COMPCODE(PH_OSAL_ERR_NO_FREE_TIMER,  PH_COMP_OSAL);
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
    }
}

phStatus_t phOsal_Stub_Timer_Reset( phOsal_Stub_DataParams_t   *pDataParams,
                                       uint32_t                       dwTimerId
                                     )
{
    phOsal_s12_Int_Reset_Timer32(dwTimerId);

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
}

phStatus_t phOsal_Stub_Timer_Wait( phOsal_Stub_DataParams_t  *pDataParams,
                                     uint8_t                       bTimerDelayUnit,
                                     uint16_t                      wDelay
                                     )
{
    uint32_t    dwTimerId;
    phStatus_t  status;
    
    status = phOsal_Stub_Timer_Create(pDataParams, &dwTimerId);
    if(status == PH_ERR_SUCCESS)
    {
        phOsal_s12_Int_Enable_Timer32( dwTimerId );
        if (bTimerDelayUnit == PH_OSAL_TIMER_UNIT_MS)
        {
            /* Delay in terms of Milli seconds */
            //phOsal_s12_Int_Delay32Ms( dwTimerId, wDelay);
        	wDelay = (uint16_t)(wDelay * 10);
            Cpu_Delay100US(wDelay);
        }
        else
        {
            /* Delay in terms of Micro seconds */
            //phOsal_s12_Int_Delay32Us( dwTimerId, wDelay);
        	wDelay = (uint16_t)(wDelay / 100);
            Cpu_Delay100US(wDelay);
        }
        status = phOsal_Stub_Timer_Delete(pDataParams, dwTimerId);
        return status;
    }
    else
    {
        /* Return error status */
        return status;
    }
}

phStatus_t phOsal_Stub_Timer_Start( phOsal_Stub_DataParams_t  *pDataParams,
                                      uint32_t                      dwTimerId,
                                      uint32_t                      dwRegTimeCnt,
                                      uint16_t                      wOption,
                                      ppCallBck_t                   pApplicationCallback,
                                      void                         *pContext
                                      )
{
    phStatus_t status;
    status = phOsal_s12_Int_Timer_Register( pDataParams, dwTimerId, dwRegTimeCnt, wOption, pApplicationCallback, pContext);

    if (status == PH_ERR_SUCCESS)
    {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
    }
    else
    {
        return PH_ADD_COMPCODE(PH_OSAL_ERR_INVALID_TIMER, PH_COMP_OSAL);
    }
}

phStatus_t phOsal_Stub_Timer_Stop( phOsal_Stub_DataParams_t  *pDataParams,
                                     uint32_t                       dwTimerId
                                     )
{
    phStatus_t status;
    status = phOsal_s12_Int_Timer_Stop(dwTimerId);
    
    if (status == PH_ERR_SUCCESS)
    {
       return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
    }
    else
    {
        return PH_ADD_COMPCODE(PH_OSAL_ERR_INVALID_TIMER, PH_COMP_OSAL);
    }
}

phStatus_t phOsal_Stub_Timer_Delete( phOsal_Stub_DataParams_t *pDataParams,
                                        uint32_t                     dwTimerId
                                      )
{
    phStatus_t status;

    status = phOsal_s12_Int_Timer_Delete(pDataParams, dwTimerId);
    if (status == PH_ERR_INTERNAL_ERROR)
    {
        return PH_ADD_COMPCODE(PH_OSAL_ERR_INVALID_TIMER, PH_COMP_OSAL);
    }
    else
    {
        return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);    
    }
}

phStatus_t phOsal_Stub_Timer_ExecCallback( phOsal_Stub_DataParams_t   *pDataParams,
                                             uint32_t                        dwTimerId
                                             )
{
    phStatus_t status = PH_ERR_SUCCESS;
    if (dwTimerId < S12_MAX_TIMERS)
    {
        /* Call the call back function */
        if (pDataParams->gTimers[dwTimerId].pApplicationCallback != NULL)
        {
            (pDataParams->gTimers[dwTimerId].pApplicationCallback)(dwTimerId, pDataParams->gTimers[dwTimerId].pContext);
        }
    }

     status = PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_OSAL);
     return status;
}

phStatus_t phOsal_Stub_GetMemory( phOsal_Stub_DataParams_t   *pDataParams,
                                    uint32_t                       dwLength,
                                    void                         **pMem
                                    )
{
    phStatus_t status = PH_ERR_SUCCESS;

    PHOSAL_UNUSED_VARIABLE(pDataParams);
    *pMem = (void *) malloc(dwLength);

    if (*pMem == NULL)
        status = PH_ERR_RESOURCE_ERROR;

    return status;
}

phStatus_t phOsal_Stub_FreeMemory( phOsal_Stub_DataParams_t   *pDataParams,
                                     void                           *ptr
                                     )
{
    phStatus_t status = PH_ERR_SUCCESS;

    PHOSAL_UNUSED_VARIABLE(pDataParams);
    //if(NULL !=  ptr)
       // free(ptr);
    return status;
}


#endif /* NXPBUILD__PH_OSAL_STUB */
/******************************************************************************
**                            End Of File
******************************************************************************/

