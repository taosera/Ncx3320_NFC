/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : SPI.c
**     Project     : Reader-Lib-HAL-PE
**     Processor   : MC9S12ZVL32MLC
**     Component   : SynchroMaster
**     Version     : Component 02.347, Driver 01.20, CPU db: 3.00.000
**     Compiler    : CodeWarrior HCS12Z C Compiler
**     Date/Time   : 2016-02-16, 09:13, # CodeGen: 61
**     Abstract    :
**         This component "SynchroMaster" implements MASTER part of synchronous
**         serial master-slave communication.
**     Settings    :
**         Synchro type                : MASTER
**
**         Serial channel              : SPI0
**
**         Protocol
**             Init baud rate          : 1_041667MHz
**             Clock edge              : falling
**             Width                   : 8 bits
**             Empty character         : 0
**             Empty char. on input    : RECEIVED
**
**         Registers
**             Input buffer            : SPI0DR    [0x0784]
**             Output buffer           : SPI0DR    [0x0784]
**             Control register        : SPI0CR1   [0x0780]
**             Mode register           : SPI0CR2   [0x0781]
**             Baud setting reg.       : SPI0BR    [0x0782]
**
**             Priority                : 
**
**             Priority                : 
**
**         Used pins                   :
**         ----------------------------------------------------------
**              Function    | On package |    Name
**         ----------------------------------------------------------
**               Input      |     20     |  PS0_KWS0_MISO0_PWM4_RXD0_IOC0_2
**               Output     |     21     |  PS1_KWS1_MOSI0_PWM6_TXD0_LPDC0_IOC0_3
**               Clock      |     22     |  PS2_KWS2_SCK0_IOC0_4_DBGEEV
**         ----------------------------------------------------------
**
**     Contents    :
**         Enable          - byte SPI_Enable(void);
**         Disable         - byte SPI_Disable(void);
**         RecvChar        - byte SPI_RecvChar(SPI_TComData *Chr);
**         SendChar        - byte SPI_SendChar(SPI_TComData Chr);
**         GetCharsInRxBuf - word SPI_GetCharsInRxBuf(void);
**         GetCharsInTxBuf - word SPI_GetCharsInTxBuf(void);
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file SPI.c
** @version 01.20
** @brief
**         This component "SynchroMaster" implements MASTER part of synchronous
**         serial master-slave communication.
*/         
/*!
**  @addtogroup SPI_module SPI module documentation
**  @{
*/         


/* MODULE SPI. */

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

#pragma DATA_SEG SPI_DATA              /* Select data segment "SPI_DATA" */
#pragma CODE_SEG SPI_CODE


#define OVERRUN_ERR      0x01U         /* Overrun error flag bit   */
#define CHAR_IN_RX       0x08U         /* Char is in RX buffer     */
#define FULL_TX          0x10U         /* Full transmit buffer     */
#define RUNINT_FROM_TX   0x20U         /* Interrupt is in progress */
#define FULL_RX          0x40U         /* Full receive buffer      */

static bool EnUser;                    /* Enable/Disable SPI */
static byte SerFlag;                   /* Flags for serial communication */
                                       /* Bits: 0 - OverRun error */
                                       /*       1 - Unused */
                                       /*       2 - Unused */
                                       /*       3 - Char in RX buffer */
                                       /*       4 - Full TX buffer */
                                       /*       5 - Running int from TX */
                                       /*       6 - Full RX buffer */
                                       /*       7 - Unused */
static SPI_TComData BufferWrite;       /* Output char SPI commmunication */

/*
** ===================================================================
**     Method      :  SPI_Enable (component SynchroMaster)
**     Description :
**         Enable the component - it starts send and receive functions.
**         Events may be generated ("DisableEvent"/"EnableEvent"). This
**         method cannot be disabled if the Fault mode is enabled. It's
**         intended for re-enabling the module if fault occurred. For
**         more information please see the <"Fault mode" >property .
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte SPI_Enable(void)
{
  if (!EnUser) {                       /* Is the device disabled by user? */
    EnUser = TRUE;                     /* If yes then set the flag "device enabled" */
    SPI0CR1_SPE = 1U;                  /* Enable device */
    if (SerFlag & FULL_TX) {           /* Is any char in transmit buffer? */
      (void)SPI0SR;                    /* Read the status register */
      SPI0DRL = BufferWrite;           /* Store char to transmitter register */
      SerFlag &= (byte)(~(byte)FULL_TX); /* Zeroize FULL_TX flag */
    }
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  SPI_Disable (component SynchroMaster)
**     Description :
**         Disable the component - it stops the send and receive
**         functions. No events will be generated. Note: When this
**         method is called while a transmission is in progress, the
**         data being transmitted/received may be lost.
**     Parameters  : None
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
** ===================================================================
*/
byte SPI_Disable(void)
{
  if (EnUser) {                        /* Is the device enabled by user? */
    EnUser = FALSE;                    /* If yes then set the flag "device disabled" */
    SPI0CR1_SPE = 0U;                  /* Disable device */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  SPI_RecvChar (component SynchroMaster)
**     Description :
**         If any data is received, this method returns one character,
**         otherwise it returns an error code (it does not wait for
**         data). 
**         For information about SW overrun behavior please see
**         <General info page>.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Chr             - A pointer to the received character
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK - The valid data is received.
**                           ERR_SPEED - This device does not work in
**                           the active speed mode.
**                           ERR_RXEMPTY - No data in receiver.
**                           ERR_OVERRUN - Overrun error was detected
**                           from the last char or block received. In
**                           polling mode, this error code is returned
**                           only when the hardware supports detection
**                           of the overrun error. If interrupt service
**                           is enabled, and input buffer allocated by
**                           the component is full, the component
**                           behaviour depends on <Input buffer size>
**                           property : if property is 0, last received
**                           data-word is preserved (and previous is
**                           overwritten), if property is greater than 0,
**                           new received data-word are ignored.
**                           ERR_FAULT - Fault error was detected from
**                           the last char or block received. In the
**                           polling mode the ERR_FAULT is return until
**                           the user clear the fault flag bit, but in
**                           the interrupt mode ERR_FAULT is returned
**                           only once after the fault error occured.
**                           This error is supported only on the CPUs
**                           supports the faul mode function - where
**                           <Fault mode> property is available.
** ===================================================================
*/
byte SPI_RecvChar(SPI_TComData *Chr)
{
  if ((SPI0SR & SPI0SR_SPIF_MASK) == 0U) { /* Is receive buffer empty? */
    return ERR_RXEMPTY;                /* If yes then error is returned */
  }
  *Chr = SPI0DRL;                      /* Read data from receiver */
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  SPI_SendChar (component SynchroMaster)
**     Description :
**         Sends one character to the channel.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Chr             - Character to send
**     Returns     :
**         ---             - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active speed mode
**                           ERR_DISABLED - Device is disabled (only if
**                           output DMA is supported and enabled)
**                           ERR_TXFULL - Transmitter is full
** ===================================================================
*/
byte SPI_SendChar(SPI_TComData Chr)
{
  if ((SPI0SR_SPTEF == 0U) || (SerFlag & FULL_TX)) { /* Is last character send? */
    return ERR_TXFULL;                 /* If no then return error */
  }
  if(EnUser) {                         /* Is device enabled? */
    SPI0DRL = Chr;                     /* If yes, send character */
  } else {
    BufferWrite = Chr;                 /* If no, save character */
    SerFlag |= FULL_TX;                /* ...and set flag */
  }
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  SPI_GetCharsInRxBuf (component SynchroMaster)
**     Description :
**         Returns the number of characters in the input buffer.
**         Note: If the Interrupt service is disabled, and the Ignore
**         empty character is set to yes, and a character has been
**         received, then this method returns 1 although it was an
**         empty character.
**     Parameters  : None
**     Returns     :
**         ---             - Number of characters in the input buffer.
** ===================================================================
*/
/*
word SPI_GetCharsInRxBuf(void)

**      This method is implemented as a macro. See header module. **
*/

/*
** ===================================================================
**     Method      :  SPI_GetCharsInTxBuf (component SynchroMaster)
**     Description :
**         Returns the number of characters in the output buffer.
**     Parameters  : None
**     Returns     :
**         ---             - Number of characters in the output buffer.
** ===================================================================
*/
word SPI_GetCharsInTxBuf(void)
{
  return ((EnUser) ? (SPI0SR_SPTEF ? (word)0U : (word)1U) : ((SerFlag & FULL_TX) ? (word)1U : (word)0U)); /* Return number of chars in the transmit buffer */
}
/*
** ===================================================================
**     Method      :  SPI_Init (component SynchroMaster)
**
**     Description :
**         Initializes the associated peripheral(s) and the component 
**         internal variables. The method is called automatically as a 
**         part of the application initialization code.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void SPI_Init(void)
{
  /* SPI0CR1: SPIE=0,SPE=0,SPTIE=0,MSTR=0,CPOL=0,CPHA=1,SSOE=0,LSBFE=0 */
  SPI0CR1 = 0x04U;                     /* Reset the device register */
  (void)SPI0SR;                        /* Read the status register */
  (void)SPI0DRL;                       /* Read the device register */
  /* SPI0BR: ??=0,SPPR2=0,SPPR1=1,SPPR0=0,??=0,SPR2=0,SPR1=0,SPR0=0 */
  SPI0BR = 0x20U;                      /* Set the baud rate register */
  /* SPI0CR2: ??=0,XFRW=0,??=0,MODFEN=0,BIDIROE=0,??=0,SPISWAI=0,SPC0=0 */
  SPI0CR2 = 0x00U;                     /* Set control register 2 */
  /* SPI0CR1: SPIE=0,SPE=0,SPTIE=0,MSTR=1,CPOL=0,CPHA=0,SSOE=0,LSBFE=0 */
  SPI0CR1 = 0x10U;                     /* Set control register 1 */
  SerFlag = 0U;                        /* Reset all flags */
  EnUser = FALSE;                      /* Disable device */
}

/*********************************************************************//**
 * @brief 		SSP Read write data function
 * @param[in]	dataCfg	Pointer to a SSP_DATA_SETUP_Type structure that
 * 						contains specified information about transmit
 * 						data configuration.
 * @param[in]	xfType	Transfer type, should be:
 * 						- SSP_TRANSFER_POLLING: Polling mode
 * 						- SSP_TRANSFER_INTERRUPT: Interrupt mode
 * @return 		Actual Data length has been transferred in polling mode.
 * 				In interrupt mode, always return (0)
 * 				Return (-1) if error.
 * Note: This function can be used in both master and slave mode.
 ***********************************************************************/
int32_t SPI_ReadWrite (SSP_DATA_SETUP_Type *dataCfg, SSP_TRANSFER_Type xfType)
{
	uint8_t *rdata8 = NULL;
	uint8_t *wdata8 = NULL;
	uint16_t *rdata16 = NULL;
	uint16_t *wdata16 = NULL;
	SPI_TError stat;
	uint32_t tmp;
	SPI_TComData Chr_data = 0x00;
	SPI_TComData *Chr = &Chr_data;
	int32_t dataword;
	byte tx_state = ERR_OK;
	byte rx_state = ERR_OK;
	//byte store[2] = {0x00, 0x00};

    dataCfg->rx_cnt = 0;
    dataCfg->tx_cnt = 0;
    dataCfg->status = 0;
      
	/* Clear all remaining data in RX FIFO */
	while (rx_state == ERR_OK){
		rx_state = SPI_RecvChar(Chr);
	}
    
    /* Check value of XFRW bit */
	if((SPI0CR2 & 0x40) != 0)
		dataword = 1;
	else dataword = 0;

	// Polling mode ----------------------------------------------------------------------
	if (xfType == SSP_TRANSFER_POLLING){
		if (dataword == 0){
			rdata8 = (uint8_t *)dataCfg->rx_data;
			wdata8 = (uint8_t *)dataCfg->tx_data;
		} else {
			rdata16 = (uint16_t *)dataCfg->rx_data;
			wdata16 = (uint16_t *)dataCfg->tx_data;
		}
		while ((dataCfg->tx_cnt != dataCfg->length) || (dataCfg->rx_cnt != dataCfg->length)){
			tx_state = ERR_OK;
			rx_state = ERR_OK;
			if (dataCfg->tx_cnt != dataCfg->length){
				// Write data to buffer
				if(dataCfg->tx_data == NULL){
					if (dataword == 0){
						tx_state = SPI_SendChar(0xFF);
						if (tx_state == ERR_OK)
							dataCfg->tx_cnt++;
					} else {
						tx_state = SPI_SendChar(0xFFFF);
						if (tx_state == ERR_OK)
							dataCfg->tx_cnt += 2;
					}
				} else {
					if (dataword == 0){
						tx_state = SPI_SendChar(*wdata8);
						if (tx_state == ERR_OK)
						{
							wdata8++;
							dataCfg->tx_cnt++;
						}
					} else {
						tx_state = SPI_SendChar(*wdata16);
						if (tx_state == ERR_OK)
						{
							wdata16++;
							dataCfg->tx_cnt += 2;
						}
					}
				}
			}

						
			// Check overrun error
			//SPI_GetError(&stat);
			//if ((stat.err & 0x01) != 0){
			if (tx_state != 0){
				// save status and return
				//dataCfg->status = stat.err | SPI_STAT_ERROR;
				dataCfg->status = SPI_STAT_ERROR;
				return (-1);
			}
			
			// Check for any data available in RX FIFO
			while ((rx_state == ERR_OK) && (dataCfg->rx_cnt != dataCfg->length)){
				// Read data from SSP data
				rx_state = SPI_RecvChar(Chr);
				tmp = (uint32_t)(*Chr);

				if (rx_state == ERR_OK)
				{
					// Store data to destination
					if (dataCfg->rx_data != NULL)
					{
						if (dataword == 0){
							*(rdata8) = (uint8_t) tmp;
							//store[dataCfg->rx_cnt] =  (uint8_t) tmp;
							rdata8++;
						} else {
							*(rdata16) = (uint16_t) tmp;
							rdata16++;
						}
					}
					// Increase counter
					if (dataword == 0){
					dataCfg->rx_cnt++;
					} else {
						dataCfg->rx_cnt += 2;
					}
				}
			}
		}

		// save status
		dataCfg->status = SPI_STAT_DONE;

		if (dataCfg->tx_data != NULL){
			return dataCfg->tx_cnt;
		} else if (dataCfg->rx_data != NULL){
			return dataCfg->rx_cnt;
		} else {
			return (0);
		}
	}

	return (-1);
}

/* END SPI. */

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