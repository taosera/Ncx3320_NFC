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
 * $URL: https://www.collabnet.nxp.com/svn/reference_designs/door_handle/trunk/D%20Software/onhost/DHRD%20V2_0/DHRD-Jcop-Demo/cards.c $
 */

/**
 * Header for hardware configuration: bus interface, reset of attached reader ID, onboard LED handling etc.
 * */
#include <phhwConfig.h>

/* Status code definitions */
#include <ph_Status.h>
#include <phpalI14443p3a.h>
#include <phpalI14443p4.h>
#include <phpalFelica.h>
#include <phpalI14443p3b.h>
#include <phpalI14443p4a.h>
#include <phpalI18092mPI.h>
#include <phacDiscLoop.h>
#include <phbalReg.h>
#include <phOsal.h>
#include <phlnLlcp.h>
#include <phnpSnep.h>
#include <phalT1T.h>
#include <phalTop.h>
#include <phpalMifare.h>
#include <phalMful.h>
#include <phalMfdf.h>
#include <phalFelica.h>
#include <phKeyStore.h>
#include "cards.h"

#pragma CODE_SEG DEFAULT

// Arrays
/* Set the HEX code for the Select App command */
static const uint8_t AppSelection1[20] = {0x00, 0xA4, 0x04, 0x00, 0x0E, 0x31, 0x50, 0x41, 0x59,
		0x2E, 0x53, 0x59, 0x53, 0x2E, 0x44, 0x44, 0x46, 0x30, 0x31, 0x00};
/* Set the HEX code for the Select App command */
static const uint8_t AppSelection2[20] = {0x00, 0xA4, 0x04, 0x00, 0x0E, 0x32, 0x50, 0x41, 0x59,
		0x2E, 0x53, 0x59, 0x53, 0x2E, 0x44, 0x44, 0x46, 0x30, 0x31, 0x00};

#ifdef   DEBUG
#include <Tools.h>
#define  DEBUG_PRINTF(...) Terminal_SendStr(__VA_ARGS__)
#else
#define  DEBUG_PRINTF(...)
#endif


/*******************************************************************************
**   DetectMifare
**   This function recognizes which kind of Mifare card is in the field.
**   Card type will be returned.
**   If 0 returned, card isn't recognized.
*******************************************************************************/
uint32_t DetectMifare(uint8_t *pAtqa, uint8_t *bSak)
{
  uint32_t sak_atqa = 0;
  uint16_t detected_card = 0xFFFF;

  sak_atqa = ((uint32_t)bSak[0] << 24) | ((uint32_t)pAtqa[0] << 8) | (uint32_t)pAtqa[1];
  sak_atqa &= 0xFFFF0FFF;

  /* Detect mini or classic */
  switch (sak_atqa)
  {
    case (((uint32_t)sak_mfc_1k << 24) | (uint32_t)atqa_mfc):
      DEBUG_PRINTF("Product: MIFARE Classic\r\n");
      detected_card &= mifare_classic;
      break;
    case (((uint32_t)sak_mfc_4k << 24) | (uint32_t)atqa_mfc):
      DEBUG_PRINTF("Product: MIFARE Classic\r\n");
      detected_card &= mifare_classic;
      break;
    case (((uint32_t)sak_mfp_2k_sl1 << 24) | (uint32_t)atqa_mfp_s):
      DEBUG_PRINTF("Product: MIFARE Classic\r\n");
      detected_card &= mifare_classic;
      break;
    case (((uint32_t)sak_mini << 24) | (uint32_t)atqa_mini):
      DEBUG_PRINTF("Product: MIFARE Mini\r\n");
      detected_card &= mifare_mini;
      break;
    case (((uint32_t)sak_mfp_4k_sl1 << 24) | (uint32_t)atqa_mfp_s):
      DEBUG_PRINTF("Product: MIFARE Classic\r\n");
      detected_card &= mifare_classic;
      break;
    case (((uint32_t)sak_mfp_2k_sl1 << 24) | (uint32_t)atqa_mfp_x):
      DEBUG_PRINTF("Product: MIFARE Classic\r\n");
      detected_card &= mifare_classic;
      break;
    case (((uint32_t)sak_mfp_4k_sl1 << 24) | (uint32_t)atqa_mfp_x):
      DEBUG_PRINTF("Product: MIFARE Classic\r\n");
      detected_card &= mifare_classic;
      break;
    default:
      detected_card = 0xFFFF;
      break;
  }

  if (detected_card == 0xFFFF)
  {
    sak_atqa = ((uint32_t)bSak[0] << 24) | ((uint32_t)pAtqa[0] << 8) | (uint32_t)pAtqa[1];
    switch (sak_atqa)
    {
      case (((uint32_t)sak_ul << 24) | (uint32_t)atqa_ul):
        DEBUG_PRINTF("Product: MIFARE Ultralight\r\n");
        detected_card &= mifare_ultralight;
        break;
      case (((uint32_t)sak_mfp_2k_sl2 << 24) | (uint32_t)atqa_mfp_s):
        DEBUG_PRINTF("Product: MIFARE Plus\r\n");
        detected_card &= mifare_plus;
        break;
      case (((uint32_t)sak_mfp_2k_sl3 << 24) | (uint32_t)atqa_mfp_s):
        DEBUG_PRINTF("Product: MIFARE Plus\r\n");
        detected_card &= mifare_plus;
        break;
      case (((uint32_t)sak_mfp_4k_sl2 << 24) | (uint32_t)atqa_mfp_s):
        DEBUG_PRINTF("Product: MIFARE Plus\r\n");
        detected_card &= mifare_plus;
        break;
      case (((uint32_t)sak_mfp_2k_sl2 << 24) | (uint32_t)atqa_mfp_x):
        DEBUG_PRINTF("Product: MIFARE Plus\r\n");
        detected_card &= mifare_plus;
        break;
      case (((uint32_t)sak_mfp_2k_sl3 << 24) | (uint32_t)atqa_mfp_x):
        DEBUG_PRINTF("Product: MIFARE Plus\r\n");
        detected_card &= mifare_plus;
        break;
      case (((uint32_t)sak_mfp_4k_sl2 << 24) | (uint32_t)atqa_mfp_x):
        DEBUG_PRINTF("Product: MIFARE Plus\r\n");
        detected_card &= mifare_plus;
        break;
      case (((uint32_t)sak_desfire << 24) | (uint32_t)atqa_desfire):
        DEBUG_PRINTF("Product: MIFARE DESFire\r\n");
        detected_card &= mifare_desfire;
        break;
      case (((uint32_t)sak_jcop << 24) | (uint32_t)atqa_jcop):
        DEBUG_PRINTF("Payment card:\r\n");
        detected_card &= jcop;
        break;
      case (((uint32_t)sak_layer4 << 24) | (uint32_t)atqa_nPA):
		DEBUG_PRINTF("German eID (neuer Personalausweis) detected\r\n");
		detected_card &= nPA;
		break;
      default:
        return 0;
    }
  }

  return detected_card;
}

/*******************************************************************************
**   PaymentCard
**   This function checks whether the card has a PSE or not.
**   If it does, the AID of the card will be read and the
**   card type will be returned.
*******************************************************************************/
void JcopExchange(phpalMifare_Sw_DataParams_t *ppalI14443p4, uint8_t *pWait)
{
    phStatus_t    status=0;
    
    /* Mifare Desfire Crypto variables */
    uint8_t bBufferReader[0x60] = {0};
    uint8_t **ppRxBuffer = (void *)&bBufferReader[0];
    uint16_t bRxLength;
    uint16_t *pRxLength = &bRxLength;
    
    /* Initialize the wait variable */
    *pWait = 0x00;

    /* Send and receive the following data: select card
     *
     */
    bBufferReader[0] = 0x02;           /* PCB will be attached automatically */
    bBufferReader[1] = 0x00;           /* CID */
    bBufferReader[2] = 0xA4;           /* Command:  */
    bBufferReader[3] = 0x04;           /* P1 */
    bBufferReader[4] = 0x00;           /* P2 */
    bBufferReader[5] = 0x07;           /* Lc: 0x10 --> 16 bytes will follow */
    bBufferReader[6] = 0xF0;           /* AID[3]  (in this case AID[2]): */
    bBufferReader[7] = 0x01;           /* Select main application                                          */
    bBufferReader[8] = 0x02;
    bBufferReader[9] = 0x03;
    bBufferReader[10] = 0x04;
    bBufferReader[11] = 0x05;
    bBufferReader[12] = 0x06;
    bBufferReader[13] = 0x00;         /* Le: 0x00 --> zero bytes to be read */
    /*CRC done automatically: 0xAA 0xE5 */

    DEBUG_PRINTF("\r\nISO14443 Command >>");
    PRINT_BUFF( &bBufferReader[1], 13);
    DEBUG_PRINTF ("\r\n");
    
    /* Send the command */
    status = phpalMifare_Sw_ExchangeL4(ppalI14443p4, PH_EXCHANGE_DEFAULT, &bBufferReader[1], 13, ppRxBuffer, pRxLength);
    //CHECK_SUCCESS(status);

    DEBUG_PRINTF("Card response    << ");
    PRINT_BUFF( *ppRxBuffer, *pRxLength);
    DEBUG_PRINTF ("\r\n");
    DEBUG_PRINTF ("\r\n");
    
    if ((*((*ppRxBuffer)) == 0x90) && (*((*ppRxBuffer) + 1) == 0x00))
    {
        DEBUG_PRINTF("\r\nSelect app successful!\r\n");
        DEBUG_PRINTF ("\r\n");
    }
    else if ((*((*ppRxBuffer)) == 0x6A) && (*((*ppRxBuffer) + 1) == 0x82))
    {
        DEBUG_PRINTF("\r\nSelect app not successful!\r\n");
        DEBUG_PRINTF ("\r\n");
    }
    
    /* Send and receive the following data: start authentication with correct key
     *
     */
    bBufferReader[0] = 0x02;           /* PCB will be attached automatically */
    bBufferReader[1] = 0x00;           /* CID */
    bBufferReader[2] = 0xD0;           /* Command:  */
    bBufferReader[3] = 0x04;           /* P1 */
    bBufferReader[4] = 0x00;           /* P2 */
    bBufferReader[5] = 0x07;           /* Lc: 0x10 --> 16 bytes will follow */
    bBufferReader[6] = 0xF0;   
    bBufferReader[7] = 0x00; 
    bBufferReader[8] = 0x02;
    bBufferReader[9] = 0x03;
    bBufferReader[10] = 0x04;
    bBufferReader[11] = 0x05;
    bBufferReader[12] = 0x06;
    bBufferReader[13] = 0x00;       
    /*CRC done automatically: 0x?? 0x?? */
    
    DEBUG_PRINTF("\r\nISO14443 Command >>");
    PRINT_BUFF( &bBufferReader[1], 13);
    DEBUG_PRINTF ("\r\n");
    
    /* Send the command */
    status = phpalMifare_Sw_ExchangeL4(ppalI14443p4, PH_EXCHANGE_DEFAULT , &bBufferReader[1], 13, ppRxBuffer, pRxLength);
    //CHECK_SUCCESS(status);
    
    DEBUG_PRINTF("Card response    << ");
    PRINT_BUFF( *ppRxBuffer, *pRxLength);
    DEBUG_PRINTF ("\r\n");
    DEBUG_PRINTF ("\r\n");
    
    if ((*((*ppRxBuffer)) == 0x90) && (*((*ppRxBuffer) + 1) == 0x00))
    {
        DEBUG_PRINTF("\r\nAuthentication successful!\r\n");
        DEBUG_PRINTF ("\r\n");
    }
    else if ((*((*ppRxBuffer)) == 0x90) && (*((*ppRxBuffer) + 1) == 0x01))
    {
        DEBUG_PRINTF("\r\nUnlock successful!\r\n");
        DEBUG_PRINTF ("\r\n");
        
        LED_R_BIT_SetVal();
        
        /* Ignore phone taps for the next 1.5 seconds */
        *pWait = 0x5A;
    }
    else if ((*((*ppRxBuffer)) == 0x90) && (*((*ppRxBuffer) + 1) == 0x02))
    {
        DEBUG_PRINTF("\r\nLock successful!\r\n");
        DEBUG_PRINTF ("\r\n");
        
        LED_R_BIT_ClrVal();
        
        /* Ignore phone taps for the next 1.5 seconds */
        *pWait = 0x5A;
    }
    else if ((*((*ppRxBuffer)) == 0x90) && (*((*ppRxBuffer) + 1) == 0x03))
    {
        DEBUG_PRINTF("\r\nEngine Start successful!\r\n");
        DEBUG_PRINTF ("\r\n");
        
        LED_BIT_SetVal();
        
        /* Ignore phone taps for the next 1.5 seconds */
        *pWait = 0x5A;
    }
    else if ((*((*ppRxBuffer)) == 0x90) && (*((*ppRxBuffer) + 1) == 0x04))
    {
        DEBUG_PRINTF("\r\nEngine stop successful successful!\r\n");
        DEBUG_PRINTF ("\r\n");
        
        LED_BIT_ClrVal();
        
        /* Ignore phone taps for the next 1.5 seconds */
        *pWait = 0x5A;
    }
    else if ((*((*ppRxBuffer)) == 0x6A) && (*((*ppRxBuffer) + 1) == 0x82))
    {
        DEBUG_PRINTF("\r\nAuthentication not successful!\r\n");
        DEBUG_PRINTF ("\r\n");
        
        LED_R_BIT_ClrVal();
        LED_BIT_ClrVal();
    }
}

/*******************************************************************************
**   Compare
**   This function checks the PSE of the card.
*******************************************************************************/
int Compare(uint8_t *input, uint8_t length)
{
	uint8_t i = 0;
	for(i = 0; i < length; i++)
	{
		if(input[i] == PSE1[i] || input[i] == PSE2[i])
		{
			//nothing to be done. just check the next PSE value.
		}
		else
		{
			return 0;
		}
	}
	DEBUG_PRINTF("\r\nPSE okay.");
	return 1;
}

/*******************************************************************************
**   Card_Scheme
**   This function checks the AID of the card.
*******************************************************************************/
int Card_Scheme(uint8_t *input, uint8_t length)
{
	uint8_t i = 0;
	uint8_t j = length;
	uint8_t comp;
	uint8_t card = VISA;
	unsigned long long ref = AID_VISA;

	for(i = 0; i < length; i++)
	{
		comp = (ref  >> (j - (i+1))*8);
		if(input[i] == comp)
		{
			//nothing to be done.
		}
		else
		{
			/* Get next AID and change card via card-number */
			switch(ref)
			{
			case AID_VISA:
				ref = AID_VISA_ELECTRON;
				card = VISA_ELECTRON;
				break;
			case AID_VISA_ELECTRON:
				ref = AID_VISA_VPAY;
				card = VISA_VPAY;
				break;
			case AID_VISA_VPAY:
				ref = AID_VISA_PLUS;
				card = VISA_PLUS;
				break;
			case AID_VISA_PLUS:
				ref = AID_MASTERCARD;
				card = MASTERCARD;
				break;
			case AID_MASTERCARD:
				ref = AID_MASTERCARD_WORLDWIDE;
				card = MASTERCARD_WORLDWIDE;
				break;
			case AID_MASTERCARD_WORLDWIDE:
				ref = AID_MASTERCARD_MAESTRO;
				card = MASTERCARD_MAESTRO;
				break;
			case AID_MASTERCARD_MAESTRO:
				ref = AID_MASTERCARD_CIRRUS;
				card = MASTERCARD_CIRRUS;
				break;
			case AID_MASTERCARD_CIRRUS:
				ref = AID_MASTERCARD_MAESTRO_UK;
				card = MASTERCARD_MAESTRO_UK;
				break;
			case AID_MASTERCARD_MAESTRO_UK:
				ref = AID_AMERICAN_EXPRESS;
				card = AMERICAN_EXPRESS;
				break;
			case AID_AMERICAN_EXPRESS:
				ref = AID_LINK;
				card = LINK;
				break;
			case AID_LINK:
				ref = AID_CB;
				card = CB;
				break;
			case AID_CB:
				ref = AID_CB_DEBIT;
				card = CB_DEBIT;
				break;
			case AID_CB_DEBIT:
				ref = AID_JCB;
				card = JCB;
				break;
			case AID_JCB:
				ref = AID_DANKORT;
				card = DANKORT;
				break;
			case AID_DANKORT:
				ref = AID_COGEBAN;
				card = COGEBAN;
				break;
			case AID_COGEBAN:
				ref = AID_DINERSCLUB_DISCOVER;
				card = DINERSCLUB_DISCOVER;
				break;
			case AID_DINERSCLUB_DISCOVER:
				ref = AID_BANRISUL;
				card = BANRISUL;
				break;
			case AID_BANRISUL:
				ref = AID_SPAN2;
				card = SPAN2;
				break;
			case AID_SPAN2:
				ref = AID_INTERAC;
				card = INTERAC;
				break;
			case AID_INTERAC:
				ref = AID_DISCOVER;
				card = DISCOVER;
				break;
			case AID_DISCOVER:
				ref = AID_CHINA_UNIONPAY_DEBIT;
				card = CHINA_UNIONPAY_DEBIT;
				break;
			case AID_CHINA_UNIONPAY_DEBIT:
				ref = AID_CHINA_UNIONPAY_CREDIT;
				card = CHINA_UNIONPAY_CREDIT;
				break;
			case AID_CHINA_UNIONPAY_CREDIT:
				ref = AID_CHINA_UNIONPAY_QUASI_CREDIT;
				card = CHINA_UNIONPAY_QUASI_CREDIT;
				break;
			case AID_CHINA_UNIONPAY_QUASI_CREDIT:
				ref = AID_VERVE;
				card = AID_VERVE;//CHINA_UNIONPAY_ELECTRONIC_CASH;
				break;
/*			case AID_CHINA_UNIONPAY_ELECTRONIC_CASH:
				ref = AID_ZKA;
				card = ZKA;
				break;
			case AID_ZKA:
				ref = AID_EAPS;
				card = EAPS;
				break;
			case AID_EAPS:
				ref = AID_VERVE;
				card = VERVE;
				break;*/
			case AID_VERVE:
				ref = AID_RUPAY;
				card = RUPAY;
				break;
			case AID_RUPAY:
				ref = AID_JCOP;
				card = JCOP;
				break;
			case AID_JCOP:
				card = 0;
				break;
			default:
				card = 0;
				DEBUG_PRINTF("\r\nOther Card or Wrong Card.");
				break;
			}
			/* Reset the counter to start from 0 for the new AID */
			if(card != 0)
			{
				i = 0;
				continue;
			}
			else
			{
				break;
			}
		}
	}
	return card;
}
/******************************************************************************
**                            End Of File
******************************************************************************/
