/**
  ******************************************************************************
  * @file    usbpd_pwr_if.h
  * @version 1.0.1
  * @date    2021-11-08
  * @author  MCD Application Team and SRA.SLDU.SC Team
  * @brief   This file contains the headers of usbpd_pw_if.h.
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

#ifndef __USBPD_PW_IF_H_
#define __USBPD_PW_IF_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbpd_def.h"

/** @addtogroup STM32_USBPD_APPLICATION
  * @{
  */

/** @defgroup STM32_USBPD_APPLICATION_POWER_IF Power Interface
  * @{
  */

/* Exported typedef ----------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/** @defgroup USBPD_USER_PWR_IF_Exported_Macros USBPD PWR IF Exported Macros
  * @{
  */

/* Enumeration of the different power status available for VBUS */    
typedef enum
{
  USBPD_PWR_BELOWVSAFE0V,
  USBPD_PWR_VSAFE5V,
  USBPD_PWR_SNKDETACH
} USBPD_VBUSPOWER_STATUS;

/* Enumeration of the different errors detected by power IF */
typedef enum
{
  USBPD_PWR_IF_OTHER        = 0U,
  USBPD_PWR_IF_NMI          = 2U,
  USBPD_PWR_IF_HARD_FAULT   = 3U,
  USBPD_PWR_IF_OVER_CURRENT = 4U,
  USBPD_PWR_IF_ACDC_REMOVED = 5U
} USBPD_PWR_IF_ERROR;

/* Macros used to convert values into PDO representation */
#define PWR_V_20MV(_V_)        ((uint16_t)(( (_V_) * 1000) / 20))   /* From Volt to 20mV multiples      */
#define PWR_V_50MV(_V_)        ((uint16_t)(( (_V_) * 1000) / 50))   /* From Volt to 50mV multiples      */
#define PWR_V_100MV(_V_)       ((uint16_t)(( (_V_) * 1000) / 100))  /* From Volt to 100mV multiples     */
#define PWR_A_10MA(_A_)        ((uint16_t)(( (_A_) * 1000) / 10))   /* From Ampere to 10mA multiples    */
#define PWR_A_50MA(_A_)        ((uint16_t)(( (_A_) * 1000) / 50))   /* From Ampere to 50mA multiples    */
#define PWR_W(_W_)             ((uint16_t)(( (_W_) * 1000) / 250))  /* From Watt to 250mW multiples     */

/* Macros used to get values from PDO representation */
#define PWR_DECODE_50MV(_Value_)           ((uint16_t)(((_Value_) * 50)))     /* From 50mV multiples to mV        */
#define PWR_DECODE_100MV(_Value_)          ((uint16_t)(((_Value_) * 100)))    /* From 100mV multiples to mV       */
#define PWR_DECODE_10MA(_Value_)           ((uint16_t)(((_Value_) * 10)))     /* From 10mA multiples to mA        */
#define PWR_DECODE_50MA(_Value_)           ((uint16_t)(((_Value_) * 50)))     /* From 50mA multiples to mA        */
#define PWR_DECODE_MW(_Value_)             ((uint16_t)(((_Value_) * 250)))    /* From 250mW multiples to mW       */

#define USBPD_PORT_IsValid(__Port__) ((__Port__) < (USBPD_PORT_COUNT))

/**
  * @} USBPD_USER_PWR_IF_Exported_Macros
  */

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/** @defgroup STM32_USBPD_APPLICATION_POWER_IF_Exported_Functions USBPD PWR IF Exported Functions
  * @{
  */

void USBPD_PWR_IF_GetPortPDOs(uint8_t PortNum, USBPD_CORE_DataInfoType_TypeDef DataId, uint8_t *Ptr, uint32_t *Size);
USBPD_StatusTypeDef USBPD_PWR_IF_Init(void);
USBPD_StatusTypeDef USBPD_PWR_IF_StartMonitoring(void);
USBPD_StatusTypeDef USBPD_PWR_IF_SetProfile(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_PWR_IF_SupplyReady(uint8_t PortNum, USBPD_VSAFE_StatusTypeDef Vsafe);
USBPD_StatusTypeDef USBPD_PWR_IF_VBUSEnable(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_PWR_IF_VBUSDisable(uint8_t PortNum);
USBPD_FunctionalState USBPD_PWR_IF_VBUSIsEnabled(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_PWR_IF_ReadVA(uint8_t PortNum, uint16_t *pVoltage, uint16_t *pCurrent);
USBPD_StatusTypeDef USBPD_PWR_IF_SearchRequestedPDO(uint8_t PortNum, uint32_t RdoPosition, uint32_t *Pdo);
void USBPD_PWR_IF_AlarmType(USBPD_PWR_IF_ERROR ErrorType);
void USBPD_PWR_IF_Alarm(void);
uint8_t USBPD_PWR_IF_GetVBUSStatus(uint8_t PortNum, USBPD_VBUSPOWER_STATUS PowerTypeStatus);
void USBPD_PWR_IF_UpdateVbusThreshold(uint8_t PortNum);
void USBPD_PWR_IF_ResetVbusThreshold(uint8_t PortNum);

/**
  * @} STM32_USBPD_APPLICATION_POWER_IF_Exported_Functions
  */

/**
  * @} STM32_USBPD_APPLICATION_POWER_IF
  */

/**
  * @} STM32_USBPD_APPLICATION
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBPD_PW_IF_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

