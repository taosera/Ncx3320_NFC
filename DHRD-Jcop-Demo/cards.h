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
 * $URL: https://www.collabnet.nxp.com/svn/reference_designs/door_handle/trunk/D%20Software/onhost/DHRD%20V2_0/DHRD-Jcop-Demo/cards.h $
 */

#ifndef CARDS_H
#define	CARDS_H

#pragma CODE_SEG DEFAULT

/*
 * SAK codes
 */
#define sak_ul         0x00
#define sak_ulc	       0x00
#define sak_mini       0x09
#define sak_mfc_1k     0x08
#define sak_mfc_4k     0x18
#define sak_mfp_2k_sl1 0x08
#define sak_mfp_4k_sl1 0x18
#define sak_mfp_2k_sl2 0x10
#define sak_mfp_4k_sl2 0x11
#define sak_mfp_2k_sl3 0x20
#define sak_mfp_4k_sl3 0x20
#define sak_desfire    0x20
#define sak_jcop       0x28
#define sak_layer4     0x20

/*
 * ATQ codes
 */
#define atqa_ul        0x4400
#define atqa_ulc       0x4400
#define atqa_mfc       0x0200
#define atqa_mfp_s     0x0400
#define atqa_mfp_x     0x4200
#define atqa_desfire   0x4403
#define atqa_jcop      0x0400
#define atqa_mini      0x0400
#define atqa_nPA       0x0800

#define mifare_ultralight    0x01
#define mifare_ultralight_c  0x02
#define mifare_classic       0x03
#define mifare_classic_1k    0x04
#define mifare_classic_4k    0x05
#define mifare_plus          0x06
#define mifare_plus_2k_sl1   0x07
#define mifare_plus_4k_sl1   0x08
#define mifare_plus_2k_sl2   0x09
#define mifare_plus_4k_sl2   0x0A
#define mifare_plus_2k_sl3   0x0B
#define mifare_plus_4k_sl3   0x0C
#define mifare_desfire       0x0D
#define jcop                 0x0F
#define mifare_mini          0x10
#define nPA                  0x11

#define RET_ANSWER				0x9000
#define FILE_NAME				0x840E
#define PSE1					"1PAY.SYS.DDF01"
#define PSE2					"2PAY.SYS.DDF01"

/*
 * Paycards identification
 */
#define AID_JCOP							0xF0505053451010
#define AID_VISA							0xA0000000031010
#define AID_VISA_ELECTRON					0xA0000000032010
#define AID_VISA_VPAY						0xA0000000032020
#define AID_VISA_PLUS						0xA0000000038010
#define AID_MASTERCARD						0xA0000000041010
#define AID_MASTERCARD_WORLDWIDE			0xA0000000049999
#define AID_MASTERCARD_MAESTRO				0xA0000000043060
#define AID_MASTERCARD_CIRRUS				0xA0000000046000
#define AID_MASTERCARD_MAESTRO_UK			0xA0000000050001
#define AID_AMERICAN_EXPRESS				0xA00000002501
#define AID_LINK							0xA0000000291010
#define AID_CB								0xA0000000421010
#define AID_CB_DEBIT						0xA0000000422010
#define AID_JCB								0xA0000000651010
#define AID_DANKORT							0xA0000001211010
#define AID_COGEBAN							0xA0000001410001
#define AID_DINERSCLUB_DISCOVER				0xA0000001523010
#define AID_BANRISUL						0xA0000001544442
#define AID_SPAN2							0xA0000002281010 //??? Attention! There are two different descriptions at wikipedia EMV.
#define AID_INTERAC							0xA0000002771010
#define AID_DISCOVER						0xA0000003241010
#define AID_CHINA_UNIONPAY_DEBIT			0xA000000333010101
#define AID_CHINA_UNIONPAY_CREDIT			0xA000000333010102
#define AID_CHINA_UNIONPAY_QUASI_CREDIT		0xA000000333010103
//#define AID_ZKA							0xA0000003591010028001  //Too long value
//#define AID_EAPS							0xA00000035910100380  //Too long value
#define AID_VERVE							0xA0000003710001
#define AID_RUPAY							0xA0000005241010
#define AID_CHINA_UNIONPAY_ELECTRONIC_CASH	0xA000000333010106

#define JCOP							1
#define VISA							2
#define VISA_ELECTRON					3
#define VISA_VPAY						4
#define VISA_PLUS						5
#define MASTERCARD						6
#define MASTERCARD_WORLDWIDE			7
#define MASTERCARD_MAESTRO				8
#define MASTERCARD_CIRRUS				9
#define MASTERCARD_MAESTRO_UK			10
#define AMERICAN_EXPRESS				11
#define LINK							12
#define CB								13
#define CB_DEBIT						14
#define JCB								15
#define DANKORT							16
#define COGEBAN							17
#define DINERSCLUB_DISCOVER				18
#define BANRISUL						19
#define SPAN2							20
#define INTERAC							21
#define DISCOVER						22
#define CHINA_UNIONPAY_DEBIT			23
#define CHINA_UNIONPAY_CREDIT			24
#define CHINA_UNIONPAY_QUASI_CREDIT		25
//#define ZKA								26
//#define EAPS							27
#define VERVE							28
#define RUPAY							29
#define CHINA_UNIONPAY_ELECTRONIC_CASH	30

/*
 * Exported functions
 */
uint32_t DetectMifare(uint8_t *pAtqa, uint8_t *bSak);
void JcopExchange(phpalMifare_Sw_DataParams_t *ppalI14443p4, uint8_t *pWait);

/*
 * Internal functions
 */
int Compare(uint8_t *input, uint8_t length);
int Card_Scheme(uint8_t *input, uint8_t length);

#endif /* CARDS_H */
