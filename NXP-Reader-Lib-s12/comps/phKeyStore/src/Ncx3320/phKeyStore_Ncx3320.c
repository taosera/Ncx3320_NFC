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

/** \file
* Ncx3320 Keystore Component of Reader Library Framework.
* $Author: nxp40786 $
* $Revision: 161 $
* $Date: 2013-06-05 14:04:36 +0530 (Wed, 05 Jun 2013) $
*
* History:
*  RSn: Generated 5. May 2010
*
*/

#include <ph_Status.h>
#include <ph_RefDefs.h>
#include <phhalHw.h>
#include <phKeyStore.h>

#ifdef NXPBUILD__PH_KEYSTORE_NCx3320

#include "phKeyStore_Ncx3320.h"
#include "phKeyStore_Ncx3320_Int.h"
#include <phhalHw_Ncx3320_Reg.h>

phStatus_t phKeyStore_Ncx3320_Init(
                                 phKeyStore_Ncx3320_DataParams_t * pDataParams,
                                 uint16_t wSizeOfDataParams, 
                                 void * pHalDataParams
                                 )
{
    /* Dataparam structure check */
    if (sizeof(phKeyStore_Ncx3320_DataParams_t) != wSizeOfDataParams)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_DATA_PARAMS, PH_COMP_KEYSTORE);
    }
    PH_ASSERT_NULL (pDataParams);
    PH_ASSERT_NULL (pHalDataParams);

    /* Init private data */
    pDataParams->wId = PH_COMP_KEYSTORE| PH_KEYSTORE_NCx3320_ID;
    pDataParams->pHalDataParams = pHalDataParams;

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_Ncx3320_FormatKeyEntry(
    phKeyStore_Ncx3320_DataParams_t * pDataParams,
    uint16_t wKeyNo,
    uint16_t wNewKeyType 
    )
{
    uint8_t PH_MEMLOC_BUF bKey[PH_KEYSTORE_KEY_TYPE_MIFARE_SIZE];

    /* clear key buffer */
    memset(bKey, 0x00, PH_KEYSTORE_KEY_TYPE_MIFARE_SIZE); /* PRQA S 3200 */

    return phKeyStore_Ncx3320_SetKeyAtPos(pDataParams, wKeyNo, 0, wNewKeyType, bKey, 0);
}

phStatus_t phKeyStore_Ncx3320_SetKey(
                                   phKeyStore_Ncx3320_DataParams_t * pDataParams,
                                   uint16_t wKeyNo,
                                   uint16_t wKeyVersion,
                                   uint16_t wKeyType,
                                   uint8_t * pNewKey,
                                   uint16_t wNewKeyVersion
                                   )
{
    return (phKeyStore_Ncx3320_SetKeyAtPos(pDataParams, wKeyNo, 0, wKeyType, pNewKey, wNewKeyVersion));
}

phStatus_t phKeyStore_Ncx3320_SetKeyAtPos(
                                        phKeyStore_Ncx3320_DataParams_t * pDataParams,
                                        uint16_t wKeyNo,
                                        uint16_t wPos,
                                        uint16_t wKeyType,
                                        uint8_t * pNewKey,
                                        uint16_t wNewKeyVersion
                                        )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_BUF bKeyNoInt;

    /* Check if wKeyNo is out of range */
    if (wKeyNo >= PH_KEYSTORE_NCx3320_NUM_KEYS)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
    }

    /* Retrieve Key type */
    switch(wKeyType)
    {
        /* MIFARE keys are the only supported keys */
    case PH_KEYSTORE_KEY_TYPE_MIFARE:

        /* prepare the command to be sent */
        bKeyNoInt = (uint8_t)(wKeyNo << 1);

        PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_Ncx3320_StoreKeyE2_Int(
            pDataParams,
            bKeyNoInt,
            pNewKey,
            2));

        break;

    case PH_KEYSTORE_KEY_TYPE_AES128:
    case PH_KEYSTORE_KEY_TYPE_AES192:
    case PH_KEYSTORE_KEY_TYPE_AES256:
    case PH_KEYSTORE_KEY_TYPE_DES:
    case PH_KEYSTORE_KEY_TYPE_2K3DES:
    case PH_KEYSTORE_KEY_TYPE_3K3DES:

        return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_KEYSTORE);

    default:

        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_Ncx3320_SetKUC(
                                   phKeyStore_Ncx3320_DataParams_t * pDataParams,
                                   uint16_t wKeyNo,
                                   uint16_t wRefNoKUC
                                   )
{
    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_Ncx3320_SetFullKeyEntry(
    phKeyStore_Ncx3320_DataParams_t * pDataParams,
    uint16_t wNoOfKeys,
    uint16_t wKeyNo,
    uint16_t wNewRefNoKUC,
    uint16_t wNewKeyType,
    uint8_t * pNewKeys,
    uint16_t * pNewKeyVersionList
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint16_t    PH_MEMLOC_REM wKeyVersion = 0;
    uint16_t    PH_MEMLOC_COUNT i;

    /* check wNoOfKeys */
    if (wNoOfKeys > PH_KEYSTORE_NCx3320_NUM_VERSIONS)
    {
        return PH_ADD_COMPCODE(PH_ERR_INVALID_PARAMETER, PH_COMP_KEYSTORE);
    }

    /* set keys */
    for (i = 0; i < wNoOfKeys; ++i)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phKeyStore_Ncx3320_SetKeyAtPos(pDataParams, wKeyNo,  i, wNewKeyType, &pNewKeys[i*PH_KEYSTORE_KEY_TYPE_MIFARE_SIZE], wKeyVersion));
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_Ncx3320_GetKeyEntry(
                                        phKeyStore_Ncx3320_DataParams_t * pDataParams,    
                                        uint16_t wKeyNo,
                                        uint16_t wKeyVersionBufSize,
                                        uint16_t * wKeyVersion,
                                        uint16_t * wKeyVersionLength,
                                        uint16_t * pKeyType
                                        )
{
    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_Ncx3320_GetKey(
                                   phKeyStore_Ncx3320_DataParams_t * pDataParams,    
                                   uint16_t wKeyNo,
                                   uint16_t wKeyVersion,
                                   uint8_t bKeyBufSize,
                                   uint8_t * pKey,
                                   uint16_t * pKeyType
                                   )
{
    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_Ncx3320_SetConfig(
                                      phKeyStore_Ncx3320_DataParams_t * pDataParams,
                                      uint16_t wConfig,
                                      uint16_t wValue
                                      )
{
    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_Ncx3320_SetConfigStr(
    phKeyStore_Ncx3320_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint8_t *pBuffer,
    uint16_t wBufferLength
    )
{
    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_Ncx3320_GetConfig(
                                      phKeyStore_Ncx3320_DataParams_t * pDataParams,
                                      uint16_t wConfig,
                                      uint16_t * pValue
                                      )
{
    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_Ncx3320_GetConfigStr(
    phKeyStore_Ncx3320_DataParams_t * pDataParams,
    uint16_t wConfig,
    uint8_t ** ppBuffer,
    uint16_t * pBufferLength
    )
{
    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_PARAMETER, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_Ncx3320_ChangeKUC(
                                      phKeyStore_Ncx3320_DataParams_t * pDataParams,                    
                                      uint16_t wRefNoKUC,
                                      uint32_t dwLimit
                                      )
{
    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_Ncx3320_GetKUC(
                                   phKeyStore_Ncx3320_DataParams_t * pDataParams,
                                   uint16_t wRefNoKUC,
                                   uint32_t * pdwLimit,
                                   uint32_t * pdwCurVal
                                   )
{
    return PH_ADD_COMPCODE(PH_ERR_UNSUPPORTED_COMMAND, PH_COMP_KEYSTORE);
}

phStatus_t phKeyStore_Ncx3320_StoreKeyE2_Int(
    phKeyStore_Ncx3320_DataParams_t * pDataParams,
    uint8_t bKeyNumber,
    uint8_t * pKey,
    uint8_t bNumKeys
    )
{
    phStatus_t  PH_MEMLOC_REM statusTmp;
    uint8_t     PH_MEMLOC_REM bRegValue;
    uint16_t    PH_MEMLOC_COUNT wIndex;

    /* clear all IRQ0 flags */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(
        pDataParams->pHalDataParams,
        PH_KEYSTORE_NCx3320_REG_IRQ0,
        (uint8_t)~(uint8_t)PH_KEYSTORE_NCx3320_BIT_IRQ_SET));

    /* clear all IRQ1 flags */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(
        pDataParams->pHalDataParams,
        PH_KEYSTORE_NCx3320_REG_IRQ1,
        (uint8_t)~(uint8_t)PH_KEYSTORE_NCx3320_BIT_IRQ_SET));

    /* Stop an eventual previous Command (enter Idle state) */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(    
        pDataParams->pHalDataParams,
        PH_KEYSTORE_NCx3320_REG_COMMAND,
        PH_KEYSTORE_NCx3320_CMD_IDLE
        ));

    /* Flush Ncx3320 fifo */
    /* Read FIFO Control register*/
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams->pHalDataParams, PH_KEYSTORE_NCx3320_REG_FIFOCONTROL, &bRegValue));

    /* Set FlushFifo bit */
    bRegValue |= PH_KEYSTORE_NCx3320_BIT_FLUSHFIFO;

    /* Write to FIFO Control register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams->pHalDataParams, PH_KEYSTORE_NCx3320_REG_FIFOCONTROL, bRegValue));

    /* Write the Key number */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams->pHalDataParams, PH_KEYSTORE_NCx3320_REG_FIFODATA, bKeyNumber));

    /* Write each single key byte */
    for (wIndex = 0; wIndex < (bNumKeys * PH_KEYSTORE_KEY_TYPE_MIFARE_SIZE); ++wIndex)
    {
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams->pHalDataParams, PH_KEYSTORE_NCx3320_REG_FIFODATA, pKey[wIndex]));
    }

    /* start the command */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(pDataParams->pHalDataParams, PH_KEYSTORE_NCx3320_REG_COMMAND, PH_KEYSTORE_NCx3320_CMD_STOREKEYE2));

    /* wait for the EE programming to be finished */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_Wait(pDataParams->pHalDataParams, PHHAL_HW_TIME_MILLISECONDS, PH_KEYSTORE_NCx3320_EEP_WR_TO_MS * bNumKeys));

    /* take a look  at the IDLE IRQ */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams->pHalDataParams, PH_KEYSTORE_NCx3320_REG_IRQ0, &bRegValue));

    if ( !(PHHAL_HW_NCx3320_BIT_IDLEIRQ & bRegValue))
    {
        /* stop the command */
        PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_WriteRegister(
            pDataParams->pHalDataParams,
            PH_KEYSTORE_NCx3320_REG_COMMAND,
            PH_KEYSTORE_NCx3320_CMD_IDLE));

        return PH_ADD_COMPCODE(PH_ERR_READ_WRITE_ERROR, PH_COMP_KEYSTORE);
    }

    /* read the error register */
    PH_CHECK_SUCCESS_FCT(statusTmp, phhalHw_ReadRegister(pDataParams->pHalDataParams, PHHAL_HW_NCx3320_REG_ERROR, &bRegValue));

    /* check for EEPROM error */
    if (bRegValue & PHHAL_HW_NCx3320_BIT_EE_ERR)
    {
        return PH_ADD_COMPCODE(PH_ERR_READ_WRITE_ERROR, PH_COMP_KEYSTORE);
    }

    return PH_ADD_COMPCODE(PH_ERR_SUCCESS, PH_COMP_KEYSTORE);
}

#endif /* NXPBUILD__PH_KEYSTORE_NCx3320 */
