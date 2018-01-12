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
* NCx3320 functions of Reader Library Framework.
* $Author: nxp40786 $
* $Revision: 161 $
* $Date: 2013-06-05 14:04:36 +0530 (Wed, 05 Jun 2013) $
*
* History:
*  RSn: Generated 5. March 2010
*
*/

#ifndef PHKEYSTORE_NCx3320_H
#define PHKEYSTORE_NCx3320_H

#include <ph_Status.h>
#include <phhalHw.h>
#include <phKeyStore.h>

phStatus_t phKeyStore_Ncx3320_FormatKeyEntry(
    phKeyStore_Ncx3320_DataParams_t * pDataParams,
    uint16_t wKeyNo,
    uint16_t wNewKeyType
    );

phStatus_t phKeyStore_Ncx3320_SetKey(
                                   phKeyStore_Ncx3320_DataParams_t * pDataParams,
                                   uint16_t wKeyNo,
                                   uint16_t wKeyVersion,
                                   uint16_t wKeyType,
                                   uint8_t * pNewKey,
                                   uint16_t wNewKeyVersion
                                   );

phStatus_t phKeyStore_Ncx3320_SetKeyAtPos(
                                        phKeyStore_Ncx3320_DataParams_t * pDataParams,
                                        uint16_t wKeyNo,
                                        uint16_t wPos,
                                        uint16_t wKeyType,
                                        uint8_t * pNewKey,
                                        uint16_t wNewKeyVersion
                                        );       

phStatus_t phKeyStore_Ncx3320_SetKUC(
                                   phKeyStore_Ncx3320_DataParams_t * pDataParams,
                                   uint16_t wKeyNo,
                                   uint16_t wRefNoKUC
                                   );

phStatus_t phKeyStore_Ncx3320_SetFullKeyEntry(
    phKeyStore_Ncx3320_DataParams_t * pDataParams,
    uint16_t wNoOfKeys,
    uint16_t wKeyNo,
    uint16_t wNewRefNoKUC,
    uint16_t wNewKeyType,
    uint8_t * pNewKeys,
    uint16_t * pNewKeyVersionList
    );

phStatus_t phKeyStore_Ncx3320_GetKeyEntry(
                                        phKeyStore_Ncx3320_DataParams_t * pDataParams,    
                                        uint16_t wKeyNo,
                                        uint16_t wKeyVersionBufSize,
                                        uint16_t * wKeyVersion,
                                        uint16_t * wKeyVersionLength,
                                        uint16_t * pKeyType
                                        );

phStatus_t phKeyStore_Ncx3320_GetKey(
                                   phKeyStore_Ncx3320_DataParams_t * pDataParams,    
                                   uint16_t wKeyNo,
                                   uint16_t wKeyVersion,
                                   uint8_t bKeyBufSize,
                                   uint8_t * pKey,
                                   uint16_t * pKeyType
                                   );


phStatus_t phKeyStore_Ncx3320_SetConfig(
                                      phKeyStore_Ncx3320_DataParams_t * pDataParams,
                                      uint16_t wConfig,
                                      uint16_t wValue
                                      );

phStatus_t phKeyStore_Ncx3320_SetConfigStr(
                                         phKeyStore_Ncx3320_DataParams_t * pDataParams,
                                         uint16_t wConfig,
                                         uint8_t *pBuffer,
                                         uint16_t wBufferLength
                                         );

phStatus_t phKeyStore_Ncx3320_GetConfig(
                                      phKeyStore_Ncx3320_DataParams_t * pDataParams,
                                      uint16_t wConfig,
                                      uint16_t * pValue
                                      );

phStatus_t phKeyStore_Ncx3320_GetConfigStr(
                                         phKeyStore_Ncx3320_DataParams_t * pDataParams,
                                         uint16_t wConfig,
                                         uint8_t ** ppBuffer,
                                         uint16_t * pBufferLength
                                         );

phStatus_t phKeyStore_Ncx3320_ChangeKUC(
                                      phKeyStore_Ncx3320_DataParams_t * pDataParams,
                                      uint16_t wRefNoKUC,
                                      uint32_t dwLimit
                                      );

phStatus_t phKeyStore_Ncx3320_GetKUC(
                                   phKeyStore_Ncx3320_DataParams_t * pDataParams,
                                   uint16_t wRefNoKUC,
                                   uint32_t * pdwLimit,
                                   uint32_t * pdwCurVal
                                   );

#endif /* PHKEYSTORE_NCx3320_H */
