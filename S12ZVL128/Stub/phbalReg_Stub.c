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
 * $URL: https://www.collabnet.nxp.com/svn/reference_designs/door_handle/trunk/D%20Software/onhost/DHRD%20V2_0/S12ZVL128/Stub/phbalReg_Stub.c $
 */

#include <ph_Status.h>
#include <phbalReg.h>
#include <ph_RefDefs.h>
#include "phhwConfig.h"

#ifdef NXPBUILD__PHBAL_REG_STUB

#include "phbalReg_Stub.h"
#include "Cpu.h"
#include "SPI.h"
#include "PIN_SSEL.h"

phStatus_t phbalReg_Stub_Init(
                              phbalReg_Stub_DataParams_t * pDataParams,
                              uint16_t wSizeOfDataParams
                              )
{
    if (sizeof(phbalReg_Stub_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_BAL);
    }
	PH_ASSERT_NULL (pDataParams);

    pDataParams->wId = PH_COMP_BAL | PHBAL_REG_STUB_ID;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_GetPortList(
    phbalReg_Stub_DataParams_t * pDataParams,
    uint16_t wPortBufSize,
    uint8_t * pPortNames,
    uint16_t * pNumOfPorts
    )
{
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_SetPort(
                            phbalReg_Stub_DataParams_t * pDataParams,
                            uint8_t * pPortName
                            )
{
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_OpenPort(
                                  phbalReg_Stub_DataParams_t * pDataParams
                                  )
{
	PIN_SSEL_PutVal(TRUE);
	SPI_Enable();
	
	/* wait Startup time */
	Cpu_Delay100US(10);
	
	return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

phStatus_t phbalReg_Stub_ClosePort(
                                   phbalReg_Stub_DataParams_t * pDataParams
                                   )
{
    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

//phStatus_t I_swear_stub()
//{
//	Terminal_SendStr("I swear, the field is activated!!!\r\n");
//	
//	return(0);
//}

phStatus_t phbalReg_Stub_Exchange(
                                  phbalReg_Stub_DataParams_t * pDataParams,
                                  uint16_t wOption,
                                  uint8_t * pTxBuffer,
                                  uint16_t wTxLength,
                                  uint16_t wRxBufSize,
                                  uint8_t * pRxBuffer,
                                  uint16_t * pRxLength
                                  )
{
#ifdef SPI_USED
	uint16_t xferLen;
	SSP_DATA_SETUP_Type xferConfig;

	xferConfig.length = wTxLength;
	xferConfig.rx_data = pRxBuffer;
	xferConfig.tx_data = pTxBuffer;

	/* chip select reader IC */
	PIN_SSEL_PutVal(FALSE);
	/* data exchange */
	xferLen = SPI_ReadWrite (&xferConfig, SSP_TRANSFER_POLLING);
	//xferLen = SPI_ReadWrite (&xferConfig, SSP_TRANSFER_INTERRUPT);
	
	/* chip deselect reader IC */
	PIN_SSEL_PutVal(TRUE);

	if (xferLen != wTxLength)
	{
		return PH_ADD_COMPCODE(PH_ERR_INTERFACE_ERROR, PH_COMP_BAL);
	}

	*pRxLength = xferLen;

	return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
#endif /* SPI_USED */
#ifdef I2C_USED
	I2C_M_SETUP_Type transferMCfg;

	/* Only 7Bits of slave address should be initialized as Last bit(R/W) will be appended automatically
	 * based on the operation done. */
	transferMCfg.sl_addr7bit         = 0x28;
	transferMCfg.tx_data              = pTxBuffer;
	transferMCfg.tx_length              = (uint32_t)wTxLength;
	transferMCfg.rx_data              = pRxBuffer;
	transferMCfg.rx_length              = (uint32_t)wRxBufSize;
	transferMCfg.retransmissions_max = 3;
	if (SUCCESS != I2C_MasterTransferData(LPC_I2C2, &transferMCfg, I2C_TRANSFER_POLLING))
	{
		return PH_ADD_COMPCODE(PH_ERR_INTERFACE_ERROR, PH_COMP_BAL);
	}

	if(pRxLength != NULL)
	{
		*pRxLength = (uint16_t)(transferMCfg.rx_count & 0x0000FFFF);
	}

	return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
#endif /* I2C_USED */
}

phStatus_t phbalReg_Stub_SetConfig(
                                   phbalReg_Stub_DataParams_t * pDataParams,
                                   uint16_t wConfig,
                                   uint16_t wValue
                                   )
{
	switch (wConfig)
	{
	case PHBAL_REG_CONFIG_HAL_HW_TYPE:
		switch(wValue)
		{
		case PHBAL_REG_HAL_HW_PN512:
			pDataParams->wHalType = wValue;
			break;
		case PHBAL_REG_HAL_HW_NCx3320:
			pDataParams->wHalType = wValue;
			break;
		default:
			return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_BAL);
		}
	case PHBAL_REG_STUBI2C_CONFIG_IC:
		switch(wValue)
		{
		case PHBAL_REG_STUBI2C_CONFIG_IC_NCx3320:
			pDataParams->ic = PHBAL_REG_STUBI2C_CONFIG_IC_NCx3320;
			break;
		case PHBAL_REG_STUBI2C_CONFIG_IC_PN512:
			pDataParams->ic = PHBAL_REG_STUBI2C_CONFIG_IC_PN512;
			break;
		default:
			return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_BAL);
		}
	default:
		return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_BAL);
	}
	return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);

}

phStatus_t phbalReg_Stub_GetConfig(
                                   phbalReg_Stub_DataParams_t * pDataParams,
                                   uint16_t wConfig,
                                   uint16_t * pValue
                                   )
{
	return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_BAL);
}

#endif /* NXPBUILD__PHBAL_REG_STUB */
