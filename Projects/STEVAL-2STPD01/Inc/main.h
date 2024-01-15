/**
  ******************************************************************************
  * @file    main.h
  * @version 1.0.1
  * @date    2021-11-08
  * @author  SRA.SLDU.SC Team
  * @brief   Header for main.c file. This file contains the common defines of 
  *          the application.
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
#ifndef __MAIN_H
#define __MAIN_H

/**
  * @defgroup STSW-2STPD01 STSW-2STPD01
  * @brief The STSW-2STPD01 software package contains dual port application 
           source code designed to support the operability of the STEVAL-2STPD01
           kit with Power Sharing in case of not enough Power Supply input.
  * @{
  */

/** @defgroup APPLICATION Application
  * @brief User Application source code
  * @{
  */

/** @defgroup STM32_USBPD_APPLICATION USBPD Application
  * @brief USBPD Source code DPM User and Power Interface (according to USBPD Stack)
  * @{
  */

/**
  * @} STM32_USBPD_APPLICATION
  */

/**
  * @} APPLICATION
  */

/**
  * @defgroup BSP BSP
  * @brief Board Specification Package
  * @{
  */

/**
  * @defgroup BSP_Components Components
  * @{
  */
    
/**
  * @} BSP_Components
  */

/**
  * @} BSP
  */

/**
  * @defgroup MIDDLEWARE Middleware
  * @{
  */

/**
  * @defgroup STM32_USBPD_LIBRARY STM32_USBPD_LIBRARY Core
  * @brief Based on the STM32Cube MCU Package for STM32G0 for more information refer to:
  * <ul>
  * <li><b><a href="https://www.st.com/content/ccc/resource/technical/document/user_manual/group1/aa/15/14/5d/f5/b8/4a/fc/DM00598101/files/DM00598101.pdf/jcr:content/translations/en.DM00598101.pdf" target="_about:blank">UM2552: Managing USB power delivery systems with STM32 microcontrollers</a></b></li>
  * <li><b><a href="https://www.st.com" target="_about:blank">ST Official website</a></b></li>
  * <li><b><a href="https://www.st.com/content/st_com/en/stm32-usb-c.html" target="_about:blank">USBPD Type-C online page on st.com</a></b></li>
  * <li><b><a href="https://wiki.st.com/stm32mcu/wiki/USB_Power_Delivery_overview" target="_about:blank">STM32 USBPD wiki online page</a></b></li>
  * </ul>
  * @{
  */
    
/**
  * @} STM32_USBPD_LIBRARY
  */

/**
  * @} MIDDLEWARE
  */

/**
  * @defgroup UTILITIES Utilities
  * @{
  */

/**
  * @} UTILITIES
  */

/**
  * @} STSW-USBPD27S
  */

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup APPLICATION
  * @{
  */

/** @defgroup APPLICATION_MAIN Main
  * @{
  */
  
/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "string.h"

#include "cmsis_os.h"

/* STM32G0 include file */
#include "stm32g0xx.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_nucleo.h"

/* USBPD include files */
#include "usbpd_core.h"
#include "usbpd_dpm_user.h"
#include "usbpd_dpm_core.h"
#include "usbpd_hw_if.h"
#include "usbpd_trace.h"


/** @defgroup APPLICATION_MAIN_Exported_Defines Exported Defines
  * @{
  */
  
/** @defgroup APPLICATION_MAIN_Exported_Defines_FirmwareHeader Firmware Header
  * @{
  */
#define APP_FWHEADER_NAME  "STSW-2STPD01"
#define APP_FWHEADER_VER   "1.0.1"
#define APP_FWHEADER_DATE  "Nov. 8th, 2021" 

#define APP_FWHEADER_DESCR APP_FWHEADER_NAME " v" APP_FWHEADER_VER "" " (Official)"
  

/* IDE selection */
#if defined(__GNUC__) /* STM32CubeIDE */
#define APP_FWHEADER_IDE "STM32CubeIDE (STMicroelectronics)"
#elif defined (__ICCARM__) /* IAR EWARM */
#define APP_FWHEADER_IDE "EWARM (IAR Systems)"
#undef APP_FWHEADER_DATE
#define APP_FWHEADER_DATE  __DATE__

/* the __USED definition is not correct */
#undef __USED
#define __USED __root
#elif defined (__CC_ARM) /* Keil uVision */
#define APP_FWHEADER_IDE "uVision (ARM Keil)"
#else   /* Unknown */
#define APP_FWHEADER_IDE "Unknown"
#endif  /* IDE selection */


#define APP_FWHEADER \
      "\r\n[Header]" \
      "\r\nName:"    APP_FWHEADER_NAME \
      "\r\nVer:"     APP_FWHEADER_VER \
      "\r\nDate:"    APP_FWHEADER_DATE \
      "\r\nDescr:"   APP_FWHEADER_DESCR \
      "\r\nIDE:"     APP_FWHEADER_IDE \
      "\r\n[/Header]\r\n"

/** 
  * @} APPLICATION_MAIN_Exported_Defines_FirmwareHeader
  */
/** 
  * @} APPLICATION_MAIN_Exported_Defines
  */

/* Private includes ----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/** @defgroup APPLICATION_MAIN_Exported_Functions Exported Functions
  * @{
  */
/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(uint32_t Code);
void Feedback_Handler(uint32_t Code, uint32_t Count, uint32_t Delay, uint8_t Mode);
/** 
  * @} APPLICATION_MAIN_Exported_Functions
  */

/** 
  * @} APPLICATION_MAIN
  */

/** 
  * @} APPLICATION
  */

/* Private defines -----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
