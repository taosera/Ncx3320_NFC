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
* Internal definitions for Pn512 specific HAL-Component of Reader Library Framework.
* $Author: nxp40786 $
* $Revision: 177 $
* $Date: 2013-06-18 11:16:14 +0530 (Tue, 18 Jun 2013) $
*
* History:
*
*/

#ifndef PHHALHW_PN512_CONFIG_H
#define PHHALHW_PN512_CONFIG_H

#include <ph_Status.h>
#include <phhalHw.h>

/** \defgroup phhalHw_Pn512_Config Configuration
* \brief Custom Configuration of hardware related settings.
* @{
*/

/** \name Memory related constants
*/
/*@{*/
#define PHHAL_HW_PN512_PRECACHED_BYTES  0x40U   /**< Maximum number of bytes to precache into FIFO before triggering a command. */
/*@}*/

/** \name Modulation indices
*/
/*@{*/
#define PHHAL_HW_PN512_MODINDEX_I14443B     0x11U   /**< Default modulation index for ISO14443B. */
#define PHHAL_HW_PN512_MODINDEX_FELICA      0x12U   /**< Default modulation index for Felica. */
#define PHHAL_HW_PN512_MODINDEX_TARGET      0xF2U   /**< Default conductance for LoadModulation. */
/*@}*/

/** \name RxThreshold values
*/
/*@{*/
#define PHHAL_HW_PN512_RXTHRESHOLD_I14443A          0x55U   /**< RxThreshold for ISO14443A. */
#define PHHAL_HW_PN512_RXTHRESHOLD_I14443B          0x50U   /**< RxThreshold for ISO14443B. */
#define PHHAL_HW_PN512_RXTHRESHOLD_FELICA           0x55U   /**< RxThreshold for Felica. */
/*@}*/

/** \name Feature selection
*/
/*@{*/
#define PHHAL_HW_PN512_FEATURE_FIFO_UNDERFLOW_CHECK /**< Checks for FIFO underflow during data transmission if defined. */
/*@}*/

/** @}
* end of phhalHw_Pn512_Config group
*/

#endif /* PHHALHW_PN512_CONFIG_H */
