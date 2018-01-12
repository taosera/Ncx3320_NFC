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
* PN512 specific HAL-Component of Reader Library Framework.
* $Author: nxp40786 $
* $Revision: 161 $
* $Date: 2013-06-05 14:04:36 +0530 (Wed, 05 Jun 2013) $
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHHALHW_NCx3320_H
#define PHHALHW_NCx3320_H

#include <ph_Status.h>
#include <phhalHw.h>

phStatus_t phhalHw_Ncx3320_Exchange(
                                  phhalHw_Ncx3320_DataParams_t * pDataParams,
                                  uint16_t wOption,
                                  uint8_t * pTxBuffer,
                                  uint16_t wTxLength,
                                  uint8_t ** ppRxBuffer,
                                  uint16_t * pRxLength
                                  );

phStatus_t phhalHw_Ncx3320_WriteRegister(
                                       phhalHw_Ncx3320_DataParams_t * pDataParams,
                                       uint8_t bAddress,
                                       uint8_t bValue
                                       );

phStatus_t phhalHw_Ncx3320_ReadRegister(
                                      phhalHw_Ncx3320_DataParams_t * pDataParams,
                                      uint8_t bAddress,
                                      uint8_t * pValue
                                      );

phStatus_t phhalHw_Ncx3320_ApplyProtocolSettings(
    phhalHw_Ncx3320_DataParams_t * pDataParams,
    uint8_t bCardType
    );

phStatus_t phhalHw_Ncx3320_MfcAuthenticateKeyNo(
    phhalHw_Ncx3320_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint16_t wKeyNo,
    uint16_t wKeyVersion,
    uint8_t * pUid
    );

phStatus_t phhalHw_Ncx3320_MfcAuthenticate(
    phhalHw_Ncx3320_DataParams_t * pDataParams,
    uint8_t bBlockNo,
    uint8_t bKeyType,
    uint8_t * pKey,
    uint8_t * pUid
    );

phStatus_t phhalHw_Ncx3320_SetConfig(
                                   phhalHw_Ncx3320_DataParams_t * pDataParams,
                                   uint16_t wConfig,
                                   uint16_t wValue
                                   );

phStatus_t phhalHw_Ncx3320_GetConfig(
                                   phhalHw_Ncx3320_DataParams_t * pDataParams,
                                   uint16_t wConfig,
                                   uint16_t * pValue
                                   );

phStatus_t phhalHw_Ncx3320_FieldOn(
                                 phhalHw_Ncx3320_DataParams_t * pDataParams
                                 );

phStatus_t phhalHw_Ncx3320_FieldOff(
                                  phhalHw_Ncx3320_DataParams_t * pDataParams
                                  );

phStatus_t phhalHw_Ncx3320_FieldReset(
                                    phhalHw_Ncx3320_DataParams_t * pDataParams
                                    );

phStatus_t phhalHw_Ncx3320_Wait(
                              phhalHw_Ncx3320_DataParams_t * pDataParams,
                              uint8_t bUnit,
                              uint16_t wTimeout
                              );

phStatus_t phhalHw_Ncx3320_SetMinFDT(
                             phhalHw_Ncx3320_DataParams_t * pDataParams,
                             uint16_t wValue
                             );

phStatus_t phhalHw_Ncx3320_ExecuteCmd(
                                    phhalHw_Ncx3320_DataParams_t * pDataParams,
                                    uint8_t bCmd,  
                                    uint16_t wOption,
                                    uint8_t bIrq0WaitFor,
                                    uint8_t bIrq1WaitFor,
                                    uint8_t * pTxBuffer,
                                    uint16_t wTxLength,
                                    uint16_t wRxBufferSize,
                                    uint8_t * pRxBuffer,
                                    uint16_t * pRxLength
                                    );

#endif /* PHHALHW_NCx3320_H */
