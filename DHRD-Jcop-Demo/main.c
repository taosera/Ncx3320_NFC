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
 * $Revision: 344 $
 * $URL: https://www.collabnet.nxp.com/svn/reference_designs/door_handle/trunk/D%20Software/onhost/DHRD%20V2_0/DHRD-Jcop-Demo/main.c $
 */

#include <stdio.h>

/**
 * Header for hardware configuration: bus interface, reset of attached reader ID, onboard LED handling etc.
 * */
#include <phhwConfig.h>
/**
* Reader Library Headers
*/
/* Status code definitions */
#include <ph_Status.h>
/* Generic ISO14443-3A Component of
 * Reader Library Framework */
#include <phpalI14443p3a.h>
/* Generic ISO14443-4 Component of
 * Reader Library Framework */
#include <phpalI14443p4.h>
/* Generic ISO14443-3B Component of
 * Reader Library Framework */
#include <phpalI14443p3b.h>
/* Generic ISO14443-4 Component of
 * Reader Library Framework */
#include <phpalI14443p4a.h>
/* Generic Felica Application
 * Component of Reader Library Framework.*/
#include <phalFelica.h>
/* Generic ISO18092 passive initiator
 * mode Component of Reader Library Framework. */
#include <phpalI18092mPI.h>
/* Generic Discovery Loop Activities
 * Component of Reader Library Framework */
#include <phacDiscLoop.h>
/* Generic BAL Component of
 * Reader Library Framework */
#include <phbalReg.h>
/* Generic OSAL Component of
 * Reader Library Framework */
#include <phOsal.h>
/* Generic MIFARE(R) Component of
 * Reader Library Framework. */
#include <phpalMifare.h>
/* Generic MIFARE(R) Ultralight Application
 * Component of Reader Library Framework. */
#include <phalMful.h>
/* Generic MIFARE DESFire(R) EV1 Application
 * Component of Reader Library Framework.*/
#include <phalMfdf.h>
/* Generic Felica Application
 * Component of Reader Library Framework.*/
#include <phpalFelica.h>
/* Cards Identification List */
#include "cards.h"

#define DISCOVERY_MODE      PHAC_DISCLOOP_SET_POLL_MODE | PHAC_DISCLOOP_SET_PAUSE_MODE     /**< Enable Poll and Pause mode  */

#define POLL_TYPE    PHAC_DISCLOOP_CON_POLL_A   /**< Enable Technology type */

/* prints if error is detected */
#define CHECK_STATUS(x)                                      \
    if ((x) != PH_ERR_SUCCESS)                               \
{                                                            \
        DEBUG_PRINTF_DEC("Line: ", __LINE__);                \
        DEBUG_PRINTF_X16("   Error - (0x", (x));             \
        DEBUG_PRINTF(") has occurred : 0xCCEE CC-Component ID, EE-Error code. Refer-ph_Status.h\r\n");   \
}

/* Returns if error is detected */
#define CHECK_SUCCESS(x)                                     \
    if ((x) != PH_ERR_SUCCESS)                               \
{                                                            \
        DEBUG_PRINTF_DEC("\r\nLine: ", __LINE__);            \
        DEBUG_PRINTF_X16("   Error - (0x", (x));             \
        DEBUG_PRINTF(") has occurred : 0xCCEE CC-Component ID, EE-Error code. Refer-ph_Status.h\r\n");   \
    return x;                         \
}

#ifdef   DEBUG
#include <Tools.h>
#define  DEBUG_PRINTF(...) Terminal_SendStr(__VA_ARGS__)
#define  DEBUG_PRINTF_X16(...) PrintDataValuesX16(__VA_ARGS__)
#define  DEBUG_PRINTF_X8(...) PrintDataValuesX8(__VA_ARGS__)
#define  DEBUG_PRINTF_DEC(...) PrintDataValuesDec(__VA_ARGS__)
#else
#define  DEBUG_PRINTF(...)
#endif

/* LPCD powerdown */
#define NFC_LPCD_POWER_DOWN_TIME_MS (59u)
#define NFC_LPCD_DETECTION_TIME_US  (10u)


phbalReg_Stub_DataParams_t         balReader;                 /**< BAL component holder */
uint8_t                            bHalBufferTx[256];         /* HAL TX buffer. Size 256 - Based on maximum FSL */
uint8_t                            bHalBufferRx[256];         /* HAL RX buffer. Size 256 - Based on maximum FSL */
uint8_t                            bAtr_Res[30];              /* ATR response holder */
uint8_t                            bAtr_ResLength;            /* ATR response length */
#ifdef NXPBUILD__PHHAL_HW_PN512
phhalHw_Pn512_DataParams_t         hal;                       /* HAL component holder */
#endif
#ifdef NXPBUILD__PHHAL_HW_NCx3320
phhalHw_Ncx3320_DataParams_t         hal;                       /* HAL component holder */
#endif
phpalI14443p3a_Sw_DataParams_t     palI14443p3a;              /* PAL I14443-A component */
phpalI14443p4a_Sw_DataParams_t     palI14443p4a;              /* PAL I14443-4A component */
phpalI14443p3b_Sw_DataParams_t     palI14443p3b;              /* PAL I14443-B component */
phpalI14443p4_Sw_DataParams_t      palI14443p4;               /* PAL I14443-4 component */
phpalFelica_Sw_DataParams_t        palFelica;                 /* PAL Felica component */
phpalI18092mPI_Sw_DataParams_t     palI18092mPI;              /* PAL MPI component */
phpalMifare_Sw_DataParams_t        palMifare;                 /* PAL Mifare component */
phalMful_Sw_DataParams_t           alMful;                    /* AL Mifare UltraLite component */
phalMfdf_Sw_DataParams_t           alMfdf;                    /* AL Mifare Desfire component */
phalFelica_Sw_DataParams_t         alFelica;                  /* AL Felica component */
phOsal_Stub_DataParams_t           osal;                      /* OSAL component holder */
phacDiscLoop_Sw_DataParams_t       discLoop;                  /* Discovery loop component */
void                              *pHal;                      /* HAL pointer */

extern void phlnLlcp_Fri_OvrHal_Init(void);


/**
* \brief      Initializes the discover loop component
*/
phStatus_t DiscLoopInit( phacDiscLoop_Sw_DataParams_t   *pDataParams )    /* DataParams representing the discovery loop */
{
    phStatus_t status;

    /* Set for poll and listen mode */
    status = phacDiscLoop_SetConfig( pDataParams,
        PHAC_DISCLOOP_CONFIG_MODE,
        DISCOVERY_MODE );
    CHECK_SUCCESS(status);

    /* Set for detection of TypeA tags */
    status = phacDiscLoop_SetConfig(
        pDataParams,
        PHAC_DISCLOOP_CONFIG_DETECT_TAGS,
        POLL_TYPE
        );

    status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_PAUSE_PERIOD_MS, 500);
    CHECK_SUCCESS(status);

    /* Set number of polling loops to 5 */
    status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_NUM_POLL_LOOPS, 1);
    CHECK_SUCCESS(status);

    /* Configure felica discovery */
    /* Set the system code to 0xffff */
    status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TYPEF_SYSTEM_CODE, 0xffff);
    CHECK_SUCCESS(status);
    /* Set the maximum number of Type F tags to be detected to 3 */
    status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TYPEF_DEVICE_LIMIT, 3);
    CHECK_SUCCESS(status);
    /* Set the polling limit for Type F tags to 5 */
    status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TYPEF_POLL_LIMIT, 5);
    CHECK_SUCCESS(status);
    /* Assign ATR response */
    discLoop.sTypeFTargetInfo.sTypeF_P2P.pAtrRes = bAtr_Res;
    /* Set ATR response length */
    status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TYPEF_P2P_ATR_RES_LEN, bAtr_ResLength);

    /* Configure Type B tag discovery */
    /* Set slot coding number to 0 */
    status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TYPEB_NCODING_SLOT, 0);
    CHECK_SUCCESS(status);
    /* Set AFI to 0, Let all TypeB tags in field respond */
    status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TYPEB_AFI_REQ, 0);
    CHECK_SUCCESS(status);
    /* Disable extended ATQB response */
    status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TYPEB_EXTATQB, 0);
    CHECK_SUCCESS(status);
    /* Set poll limit for Type B tags to 5 */
    status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TYPEB_POLL_LIMIT, 5);
    CHECK_SUCCESS(status);

    /* Set LRI to 3 */
    status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TYPEA_P2P_LRI, 3);
    CHECK_SUCCESS(status);
    /* Set DID to 3 */
    status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TYPEA_P2P_DID, 0);
    CHECK_SUCCESS(status);
    /* Disable NAD */
    status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TYPEA_P2P_NAD_ENABLE, PH_OFF);
    CHECK_SUCCESS(status);
    /* Clear NAD info */
    status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TYPEA_P2P_NAD, 0);
    CHECK_SUCCESS(status);
    /* Assign ATR response */
    discLoop.sTypeATargetInfo.sTypeA_P2P.pAtrRes = bAtr_Res;
    /* Set ATR response length */
    status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TYPEA_P2P_ATR_RES_LEN, bAtr_ResLength);

    /* RATS FSD */
    /* Set FSD response length */
    status = phacDiscLoop_SetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TYPEA_I3P4_FSDI, 8);

    /* Enable bail-out */
    discLoop.bBailOut = 1;
    
    return PH_ERR_SUCCESS;
}


/**
* This function demonstrates the usage of discovery loop
* It detects and displays the type of card and UID
* In case of P2P device, it enables LLCP and transmits the message
*
* \param   pDataParams      The discovery loop data parameters
*
* \note   This function will never return
*
*/
phStatus_t DiscLoopDemo( phacDiscLoop_Sw_DataParams_t  *pDataParams )
{
    phStatus_t    status=0;
    uint16_t      wTagsDetected=0;
    uint32_t      loop = 0;
    uint16_t      wNumberOfTags=0;
    uint8_t       bGtLen=0;    /* Gt length */
    uint8_t       bWait;
    PH_UNUSED_VARIABLE(bGtLen);


    while(1)
    {
        DEBUG_PRINTF("Ready to detect.");
        DEBUG_PRINTF("\r\n");

        status = phhalHw_FieldOff(pHal);
        Cpu_Delay100US(5000);
        
        status = phacDiscLoop_Start(pDataParams);
        
        if ((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
        {
            /* Get the tag types detected info */
            status = phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TAGS_DETECTED, &wTagsDetected);
        
            if (((status & PH_ERR_MASK) != PH_ERR_SUCCESS) || wTagsDetected == PHAC_DISCLOOP_NO_TAGS_FOUND)
            {
                //phhalHw_FieldReset(&hal);
            	status = phhalHw_FieldOff(&hal);
                continue;
            }
            DEBUG_PRINTF ("\r\n");
            
            if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_TYPEA_DETECTED))
            {
                if (wTagsDetected & PHAC_DISCLOOP_TYPEA_DETECTED_TAG_P2P)
                {
                    /* If Type A tag with P2P capability was discovered, do and P2P exchange */
                    DEBUG_PRINTF("\r\nType A with P2P has been detected");
               
                    status = phhalHw_FieldOff(&hal);
                    CHECK_SUCCESS(status);
        
                    /* Start the next polling cycle */
                    continue;
                }
                /* Get the number of Type A tags detected */
                status = phacDiscLoop_GetConfig(pDataParams, PHAC_DISCLOOP_CONFIG_TYPEA_NR_TAGS_FOUND, &wNumberOfTags);
                CHECK_SUCCESS(status);
        
        
                /* Loop through all the Type A tags detected and print their UIDs */
                for (loop = 0; loop < wNumberOfTags; loop++)
                {
                    DEBUG_PRINTF_DEC ("\r\nCard ",loop + 1);
                    DEBUG_PRINTF ("\r\nUID: ");
                    PRINT_BUFF(
                        pDataParams->sTypeATargetInfo.aTypeA_I3P3[loop].aUid,
                        pDataParams->sTypeATargetInfo.aTypeA_I3P3[loop].bUidSize
                        );
                    if(loop == 0)
                    {
                        DEBUG_PRINTF("\r\nAtqa:");
                        PRINT_BUFF(
                            pDataParams->sTypeATargetInfo.aTypeA_I3P3[loop].aAtqa,
                            2
                            );
                        DEBUG_PRINTF("\r\nSak:");
                        PRINT_BUFF(
                            pDataParams->sTypeATargetInfo.aTypeA_I3P3[loop].aSak,
                            1
                            );
                        DEBUG_PRINTF ("\r\n");
                        /* check which Mifare card is in field */
                        status = DetectMifare(pDataParams->sTypeATargetInfo.aTypeA_I3P3[loop].aAtqa,pDataParams->sTypeATargetInfo.aTypeA_I3P3[loop].aSak);
        
                        /* Perform exchange with JCOP app */
                        JcopExchange(&palMifare, &bWait);                     
        
                        if(status == 0xffff)
                        {
                            if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_TYPEA_DETECTED_TAG_TYPE1))
                            {
                                DEBUG_PRINTF ("\r\nType A T1 tag detected");
                            }
                            else if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_TYPEA_DETECTED_TAG_TYPE2))
                            {
                                DEBUG_PRINTF ("\r\nType A T2 tag detected");
                            }
                            else if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_TYPEA_DETECTED_TAG_TYPE4A))
                            {
                                DEBUG_PRINTF ("\r\nType 4A tag detected");
                            }
                            else if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_TYPEA_DETECTED_TAG_P2P))
                            {
                                DEBUG_PRINTF ("\r\nType A P2P tag detected");
                            }
                        }
                    }
                    else
                    {
                        DEBUG_PRINTF ("\r\n");
                    }
                }
            }
        }
        status = phpalI14443p4_ResetProtocol(&palI14443p4);
        CHECK_SUCCESS(status);
        status = phhalHw_FieldOff(pHal);
        CHECK_SUCCESS(status);
        /* Ignore phone taps for the next 1.5 seconds */
        if(bWait != 0)
        {
            Cpu_Delay100US(15000);
        }
    }
}


/*******************************************************************************
**   Main Function
*******************************************************************************/
int main (void)
{
    phStatus_t  status;

    /* Set the interface link for the
     * internal chip communication */
    Set_Interface_Link();

    DEBUG_PRINTF("\r\nStart Example: Polling");

#ifdef SPI_USED
    DEBUG_PRINTF("\r\nSPI link selected");
#endif /* SPI_USED */
#ifdef I2C_USED
    DEBUG_PRINTF("\r\nI2C link selected");
#endif /* I2C_USED */

#ifndef TUSA
    /* Set LED port pin to output */
    //Set_Port(); //TBD already done in CPU files by processor expert
#endif
    /* Ensure, that the LED is off */
    LED_R_BIT_ClrVal();
    LED_BIT_ClrVal();

    /* Perform a hardware reset */
    Reset_reader_device();

    /* Initialize the Reader BAL (Bus Abstraction Layer) component */
    phbalReg_Stub_Init(&balReader, sizeof(phbalReg_Stub_DataParams_t));

    /* Initialize the Stub timers component */
    status = phOsal_Stub_Init(&osal);
    CHECK_SUCCESS(status);

    /* Initialize the Reader HAL (Hardware Abstraction Layer) component */
#ifdef NXPBUILD__PHHAL_HW_NCx3320
    status = phhalHw_Ncx3320_Init(
        &hal,
        sizeof(phhalHw_Ncx3320_DataParams_t),
        &balReader,
        0,
        bHalBufferTx,
        sizeof(bHalBufferTx),
        bHalBufferRx,
        sizeof(bHalBufferRx)
        );
#endif
    /* Set the parameter to use the SPI interface */
    hal.bBalConnectionType = PHHAL_HW_BUS;

    /* Set the generic pointer */
    pHal = &hal;

    status = phbalReg_OpenPort(&balReader);
    CHECK_SUCCESS(status);

#if defined NXPBUILD__PHHAL_HW_NCx3320
    phhalHw_ReadRegister(pHal, PHHAL_HW_NCx3320_REG_VERSION, bHalBufferTx);
    DEBUG_PRINTF_X8("\r\nReader chip NCx3320: 0x", bHalBufferTx[0]);
    DEBUG_PRINTF("\r\n");
#endif

    /* Initialize the I14443-A PAL layer */
    status = phpalI14443p3a_Sw_Init(&palI14443p3a, sizeof(phpalI14443p3a_Sw_DataParams_t), &hal);
    CHECK_SUCCESS(status);

    /* Initialize the I14443-A PAL component */
    status = phpalI14443p4a_Sw_Init(&palI14443p4a, sizeof(phpalI14443p4a_Sw_DataParams_t), &hal);
    CHECK_SUCCESS(status);

    /* Initialize the I14443-4 PAL component */
    status = phpalI14443p4_Sw_Init(&palI14443p4, sizeof(phpalI14443p4_Sw_DataParams_t), &hal);
    CHECK_SUCCESS(status);

    /* Initialize the I14443-B PAL  component */
    status = phpalI14443p3b_Sw_Init(&palI14443p3b, sizeof(palI14443p3b), &hal);
    CHECK_SUCCESS(status);

    /* Initialize PAL Felica PAL component */
    status = phpalFelica_Sw_Init(&palFelica, sizeof(phpalFelica_Sw_DataParams_t), &hal);
    CHECK_SUCCESS(status);

    /* Init 18092 PAL component */
    status = phpalI18092mPI_Sw_Init(&palI18092mPI, sizeof(phpalI18092mPI_Sw_DataParams_t), pHal);
    CHECK_SUCCESS(status);

    /* Initialize the OvrHal component */
    phlnLlcp_Fri_OvrHal_Init();

    /* Initialize the Mifare PAL component */
    status = phpalMifare_Sw_Init(&palMifare, sizeof(phpalMifare_Sw_DataParams_t), &hal, &palI14443p4);
    CHECK_SUCCESS(status);

    /* Initialize the Felica AL component */
    status = phalFelica_Sw_Init(&alFelica, sizeof(phalFelica_Sw_DataParams_t), &palFelica);
    CHECK_SUCCESS(status);

    /* Initialize the Mful AL component */
    status = phalMful_Sw_Init(&alMful, sizeof(phalMful_Sw_DataParams_t), &palMifare, NULL, NULL, NULL);
    CHECK_SUCCESS(status);

    /* Initialize the MF DesFire EV1 component */
    status = phalMfdf_Sw_Init(&alMfdf, sizeof(phalMfdf_Sw_DataParams_t), &palMifare, NULL, NULL, NULL, &hal);
    CHECK_SUCCESS(status);

    /* Initialize the timer component */
    status = phOsal_Timer_Init(&osal);
    CHECK_SUCCESS(status);

    /* Initialize the discover component */
    status = phacDiscLoop_Sw_Init(&discLoop, sizeof(phacDiscLoop_Sw_DataParams_t), &hal, &osal);
    /*&palI14443p3a, &palI14443p4a, &palI14443p3b, &palFelica, &palI18092mPI, */
    CHECK_SUCCESS(status);

    /* Initialize the discover structure */
    discLoop.pPal1443p3aDataParams  = &palI14443p3a;
    discLoop.pPal1443p3bDataParams  = &palI14443p3b;
    discLoop.pPal1443p4aDataParams  = &palI14443p4a;
    discLoop.pPal18092mPIDataParams = &palI18092mPI;
    discLoop.pPalFelicaDataParams   = &palFelica;
    discLoop.pHalDataParams         = &hal;
    discLoop.pOsal                  = &osal;

    /* reset the IC  */
    status = phhalHw_FieldReset(pHal);
    CHECK_SUCCESS(status);

    DiscLoopInit(&discLoop);
    DiscLoopDemo(&discLoop);

    return 0;
}
/******************************************************************************
**                            End Of File
******************************************************************************/
