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
 * $URL: https://www.collabnet.nxp.com/svn/reference_designs/door_handle/trunk/D%20Software/onhost/DHRD%20V2_0/S12ZVL128/phhwConfig.h $
 */


/*----------------------------------------------------------------------------------------------
 * Includes
 ---------------------------------------------------------------------------------------------*/
#ifndef __PHHW_CONFIG_H__
#define __PHHW_CONFIG_H__

/* Including needed modules to compile this module/procedure */
//#include "SPI.h"
//#include "PIN_RESET.h"
//#include "LED_BIT.h"
//#include "PIN_SSEL.h"
//#include "TMR0.h"
//#include "Terminal.h"
//#include "Inhr1.h"
//#include "Utility.h"
//#include "LED_R_BIT.h"
/* Include shared modules, which are used for whole project */
//#include "PE_Types.h"
//#include "PE_Error.h"
//#include "PE_Const.h"
//#include "IO_Map.h"

#include "Cpu.h"
#include <phhalHw.h>
#include <ph_NxpBuild.h>
#include <phOsal.h>

/***********************************************************************************************
 **	Global macros and definitions
 ***********************************************************************************************/

//#define TUSA

#if defined(TUSA) & !defined(NXPBUILD__PHHAL_HW_NCx3320)
#error ** Mismatch in hardware configuration - TUSA board includes NCx3320 reader IC **
#endif

#define SPI_USED
//#define I2C_USED

#if defined (SPI_USED) && defined (I2C_USED)
#error ** Mismatch interface definition - I2C and SPI not alloved together **
#endif

#ifdef SPI_USED
#define PHHAL_HW_BUS	PHHAL_HW_BAL_CONNECTION_SPI
#endif /* SPI_USED */
#ifdef I2C_USED
#define PHHAL_HW_BUS	PHHAL_HW_BAL_CONNECTION_I2C
#define READER_CHIP_ADDR 0x50
#endif /* I2C_USED */

/***********************************************************************************************
 **	Global variables
 ***********************************************************************************************/

/***********************************************************************************************
 **	Global function prototypes
 ***********************************************************************************************/
extern void Set_Interface_Link(void);
extern void GPIO_Init(void);
extern void Reset_reader_device(void);
extern void LedOn(void);
extern void LedOff(void);
/******************************************************************************************
 * Reader IC specific soft reset function - command
 ******************************************************************************************/
#ifdef NXPBUILD__PHHAL_HW_NCx3320
	#define SoftReset_reader_device(pHal)	phhalHw_Ncx3320_Cmd_SoftReset(pHal)
#endif

#endif	// __PHHW_CONFIG_H__

/***********************************************************************************************
 **                            End Of File
 ***********************************************************************************************/





