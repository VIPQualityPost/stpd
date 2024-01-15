/**
  ******************************************************************************
  * @file    usbpd_power_sharing_app.h
  * @version 1.0.1
  * @date    2021-11-08
  * @author  SRA.SLDU.SC Team
  * @brief   Header file for usbpd_power_sharing_app.c file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBPD_POWER_SHARING_APP_H
#define __USBPD_POWER_SHARING_APP_H

#ifdef __cplusplus
extern "C" {
#endif
  
/** @addtogroup APPLICATION
  * @{
  */

/** @defgroup USBPD_POWER_SHARING_APP Power Sharing Application
  * @brief The power sharing handlers and customizations
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "usbpd_power_sharing.h" 

/** @defgroup USBPD_POWER_SHARING_APP_Exported_Defines Exported Defines
  * @{
  */

/* Exported constants --------------------------------------------------------*/

#define PS_CONF_MIN_CURRENT             300U        /*!< Minimum current for a valid PDO (mA) */
#define PS_CONF_POWER_NO_LOAD           500U        /*!< Power required from the system without external loads (mW)  */
#define PS_CONF_PCOEFFICIENT            850U        /*!< Power safety coefficient 85% (/1000)      */
#define PS_CONF_VCOEFFICIENT            980U        /*!< Conversion Voltage Coefficient of Vin 98% (/1000) */
#define PS_CONF_PWRSUPPLY_VBUS          24000U      /*!< Power Supply VBus default (mV) */
#define PS_CONF_PWRSUPPLY_IBUS          6000U       /*!< Power Supply IBus default (mA) */
#define PS_CONF_MAX_PORT_VOLTAGE        20000U      /*!< (mV) */
#define PS_CONF_MAX_PORT_CURRENT        3000U       /*!< (mA) */
#define PS_CONF_MAX_PORT_POWER          (((PS_CONF_MAX_PORT_VOLTAGE) * (PS_CONF_MAX_PORT_CURRENT)) / 1000u)  /*!< (mW) */
#define PS_CONF_MIN_PORT_POWER          15000U      /*!< (mW) */

#define PS_POWER_SUPPLY_DATA_ADDRESS    0x0801F000                              /*!< Power Supply parameters flash address */
#define PS_POWER_SUPPLY_VOLTAGE_ADDRESS (PS_POWER_SUPPLY_DATA_ADDRESS)          /*!< Voltage parameter flash address       */
#define PS_POWER_SUPPLY_CURRENT_ADDRESS (PS_POWER_SUPPLY_DATA_ADDRESS + 4)      /*!< Current parameter flash address       */
    
#define PS_APP_MIN_CURRENT             1000         /*!< Power supply input minimum current (mA) */
#define PS_APP_MAX_CURRENT             10000        /*!< Power supply input maximum current (mA) */
#define PS_APP_MIN_VOTAGE              8100         /*!< Power supply input minimum voltage (mV) */
#define PS_APP_MAX_VOTAGE              24000        /*!< Power supply input maximum voltage (mV) */
#define PS_APP_MIN_POWER               18500        /*!< Power supply input minimum power (mW) */

/** 
  * @} USBPD_POWER_SHARING_APP_Exported_Defines
  */

    
/** @defgroup USBPD_POWER_SHARING_APP_Exported_Variables Exported Variables
  * @{
  */
extern PS_Config_Typedef PS_Config;
/** 
  * @} USBPD_POWER_SHARING_APP_Exported_Variables
  */

/** @defgroup USBPD_POWER_SHARING_APP_Exported_Functions Exported Functions
  * @{
  */
PS_Status_TypeDef PS_App_Init(void);
PS_Status_TypeDef PS_App_ParametersFromString(char const * Text, uint8_t Len);
/** 
  * @} USBPD_POWER_SHARING_APP_Exported_Functions
  */

/** 
  * @} USBPD_POWER_SHARING_APP
  */

/** 
  * @} APPLICATION
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBPD_POWER_SHARING_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
