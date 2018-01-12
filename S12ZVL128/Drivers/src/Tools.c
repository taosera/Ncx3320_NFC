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
 * Tools.c
 *
 *  Created on: Sep 3, 2015
 *      Author: nxp84325
 */

#include "Tools.h"

/*==============================================================================================
 * Function:	PrintDataValuesX16
 *
 * brief:
 *
 * -------------------------------------------------------------------------------------------*/
void PrintDataValuesX16(char *str, uint16_t val1) 
{
	unsigned char buf[128];
 
	Utility_strcpy(buf, sizeof(buf), (unsigned char*)str);
	Utility_strcatNum16Hex(buf, sizeof(buf), val1);
	Terminal_SendStr(buf);
}

/*==============================================================================================
 * Function:	PrintDataValuesX8
 *
 * brief:
 *
 * -------------------------------------------------------------------------------------------*/
void PrintDataValuesX8(char *str, uint8_t val1) 
{
	unsigned char buf[128];
 
	Utility_strcpy(buf, sizeof(buf), (unsigned char*)str);
	Utility_strcatNum8Hex(buf, sizeof(buf), val1);
	Terminal_SendStr(buf);
}

/*==============================================================================================
 * Function:	PrintDataValuesDec
 *
 * brief:
 *
 * -------------------------------------------------------------------------------------------*/
void PrintDataValuesDec(char *str, uint16_t val1) 
{
	unsigned char buf[128];
 
	Utility_strcpy(buf, sizeof(buf), (unsigned char*)str);
	Utility_strcatNum16u(buf, sizeof(buf), val1);
	Terminal_SendStr(buf);
}

/*==============================================================================================
 * Function:	PRINT_BUFF
 *
 * brief:
 *
 * -------------------------------------------------------------------------------------------*/
void PRINT_BUFF(uint8_t *hex, uint8_t num)
{
	unsigned char buf[128];
    uint32_t	i;

    for(i = 0; i < num; i++)
	{
        Utility_strcpy(buf, sizeof(buf), (unsigned char*)" ");
        Utility_strcatNum8Hex(buf, sizeof(buf), hex[i]);
        Terminal_SendStr(buf);
	}
}
