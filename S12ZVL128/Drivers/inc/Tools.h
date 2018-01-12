/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is 
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/*
 * Tools.h
 *
 *  Created on: Sep 3, 2015
 *      Author: nxp84325
 */

#ifndef TOOLS_H_
#define TOOLS_H_

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "SPI.h"
#include "PIN_RESET.h"
#include "LED_BIT.h"
#include "TMR0.h"
#include "Terminal.h"
#include "Inhr1.h"
#include "Utility.h"
#include "LED_R_BIT.h"

void PrintDataValuesX16(char *str, uint16_t val1);
/*
** ===================================================================
**     Method      :  PrintDataValuesX16 (component Utility)
**     Description :
**         Same as normal strcpy, but safe as it does not write beyond
**         the buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * dst             - Pointer to destination string
**         dstSize         - Size of the destination buffer (in
**                           bytes).
**       * src             - Pointer to source string.
**     Returns     : Nothing
** ===================================================================
*/

void PrintDataValuesX8(char *str, uint8_t val1) ;
/*
** ===================================================================
**     Method      :  PrintDataValuesX8 (component Utility)
**     Description :
**         Same as normal strcpy, but safe as it does not write beyond
**         the buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * dst             - Pointer to destination string
**         dstSize         - Size of the destination buffer (in
**                           bytes).
**       * src             - Pointer to source string.
**     Returns     : Nothing
** ===================================================================
*/

void PrintDataValuesDec(char *str, uint16_t val1) ;
/*
** ===================================================================
**     Method      :  PrintDataValuesDec (component Utility)
**     Description :
**         Same as normal strcpy, but safe as it does not write beyond
**         the buffer.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * dst             - Pointer to destination string
**         dstSize         - Size of the destination buffer (in
**                           bytes).
**       * src             - Pointer to source string.
**     Returns     : Nothing
** ===================================================================
*/

void PRINT_BUFF(uint8_t *hex, uint8_t num);
/*
 ** ===================================================================
 **     Method      :  PRINT_BUFF (component Utility)
 **     Description :
 **         Same as normal strcpy, but safe as it does not write beyond
 **         the buffer.
 **     Parameters  :
 **         NAME            - DESCRIPTION
 **       * dst             - Pointer to destination string
 **         dstSize         - Size of the destination buffer (in
 **                           bytes).
 **       * src             - Pointer to source string.
 **     Returns     : Nothing
 ** ===================================================================
 */


#endif /* TOOLS_H_ */
