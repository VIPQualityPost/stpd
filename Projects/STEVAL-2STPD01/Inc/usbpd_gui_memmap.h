/**
  ******************************************************************************
  * @file    usbpd_gui_memmap.h
  * @version 1.0.1
  * @date    2021-11-08
  * @author  MCD Application Team and SRA.SLDU.SC Team
  * @brief   This file contains memory mapping configuration to be able to run
  *          Cube-Monitor-UCPD on embedded side.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifndef __USBPD_GUI_MEMMAP_H_
#define __USBPD_GUI_MEMMAP_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx.h"
#include "usbpd_def.h"

/** @addtogroup STM32_USBPD_APPLICATION
  * @{
  */

/** @defgroup STM32_USBPD_APPLICATION_GUI GUI Configuration
  * @{
  */

/* Exported typedef ----------------------------------------------------------*/

/* Exported define -----------------------------------------------------------*/

/* Following definitions should be adapted to used Flash configuration :
   INDEX_PAGE : represents the number of the page used for storing USBPD settings (usually the last page)
   ADDR_FLASH_LAST_PAGE : Flash address value of beginning of USBPD settings page
   ADDR_FLASH_PAGE_END : Flash address value of end of USBPD settings page
*/
#if defined (FLASH_DBANK_SUPPORT)
#define INDEX_PAGE              ((FLASH_PAGE_NB * 2u) - 1u)          /* Index of latest page    */
#else
#define INDEX_PAGE              (FLASH_PAGE_NB - 1u)                 /* Index of latest page    */
#endif /* FLASH_DBANK_SUPPORT */
#define ADDR_FLASH_LAST_PAGE    (FLASH_BASE + INDEX_PAGE * FLASH_PAGE_SIZE)  /* Base @ of latest page   */
#define ADDR_FLASH_PAGE_END     (ADDR_FLASH_LAST_PAGE + FLASH_PAGE_SIZE - 1) 

#define GUI_FLASH_MAGIC_NUMBER         ADDR_FLASH_LAST_PAGE
/* should be aligned on double word */
#define GUI_FLASH_ADDR_NB_PDO_SNK_P0   (GUI_FLASH_MAGIC_NUMBER + 8u)
#define GUI_FLASH_ADDR_NB_PDO_SRC_P0   (GUI_FLASH_ADDR_NB_PDO_SNK_P0 + 1u)
#define GUI_FLASH_ADDR_NB_PDO_SNK_P1   (GUI_FLASH_ADDR_NB_PDO_SRC_P0 + 1u)
#define GUI_FLASH_ADDR_NB_PDO_SRC_P1   (GUI_FLASH_ADDR_NB_PDO_SNK_P1 + 1u)

/* should be aligned on double word */
#define GUI_FLASH_ADDR_PDO_SRC_P0      (GUI_FLASH_MAGIC_NUMBER + 16u)
/* New Address should be modulo 8 */
/* New Address = (Previous Address + (size / 8 * 8) + ((size % 8) + 7) / 8 * 8) */
#define GUI_FLASH_ADDR_PDO_SNK_P0      (GUI_FLASH_ADDR_PDO_SRC_P0 + ((USBPD_MAX_NB_PDO * 4) / 8 * 8) + ((((USBPD_MAX_NB_PDO * 4) & 0x07) + 7) / 8 * 8 ))
#define GUI_FLASH_ADDR_PDO_SRC_P1      (GUI_FLASH_ADDR_PDO_SNK_P0 + ((USBPD_MAX_NB_PDO * 4) / 8 * 8) + ((((USBPD_MAX_NB_PDO * 4) & 0x07) + 7) / 8 * 8 ))
#define GUI_FLASH_ADDR_PDO_SNK_P1      (GUI_FLASH_ADDR_PDO_SRC_P1 + ((USBPD_MAX_NB_PDO * 4) / 8 * 8) + ((((USBPD_MAX_NB_PDO * 4) & 0x07) + 7) / 8 * 8 ))

#define GUI_FLASH_ADDR_DPM_SETTINGS    (GUI_FLASH_ADDR_PDO_SNK_P1 + ((USBPD_MAX_NB_PDO * 4) / 8 * 8) + ((((USBPD_MAX_NB_PDO * 4) & 0x07) + 7) / 8 * 8 ))

/* Define by default USBPD_PORT_COUNT to 2 */
#define GUI_FLASH_ADDR_DPM_USER_SETTINGS  (GUI_FLASH_ADDR_DPM_SETTINGS + (sizeof(USBPD_SettingsTypeDef) * 2 /*USBPD_PORT_COUNT*/))

#define GUI_FLASH_ADDR_DPM_VDM_SETTINGS   (GUI_FLASH_ADDR_DPM_USER_SETTINGS + (sizeof(USBPD_USER_SettingsTypeDef) * 2 /*USBPD_PORT_COUNT*/))

#define GUI_FLASH_ADDR_DPM_ID_SETTINGS    (GUI_FLASH_ADDR_DPM_VDM_SETTINGS + (sizeof(USBPD_VDM_SettingsTypeDef) * 2 /*USBPD_PORT_COUNT*/))

#define GUI_FLASH_ADDR_RESERVED           (GUI_FLASH_ADDR_DPM_ID_SETTINGS + (sizeof(USBPD_IdSettingsTypeDef) * 2 /*USBPD_PORT_COUNT*/))

#define GUI_FLASH_SIZE_RESERVED           (ADDR_FLASH_PAGE_END - GUI_FLASH_ADDR_RESERVED)

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBPD_GUI_MEMMAP_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
