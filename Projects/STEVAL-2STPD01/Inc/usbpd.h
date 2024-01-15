/**
  ******************************************************************************
  * @file    usbpd.h
  * @version 1.0.1
  * @date    2021-11-08
  * @author  MCD Application Team and SRA.SLDU.SC Team
  * @brief   This file contains the device define.
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
#ifndef __usbpd_H
#define __usbpd_H
#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup STM32_USBPD_APPLICATION
  * @{
  */

/** @defgroup STM32_USBPD_APPLICATION_USBPD_Device USBPD Device
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "usbpd_core.h"
#include "usbpd_dpm_core.h"
#include "usbpd_dpm_conf.h"
#include "usbpd_hw_if.h"

/** @defgroup STM32_USBPD_APPLICATION_USBPD_Device_Exported_Functions Exported Functions
  * @{
  */
void MX_USBPD_Init(void);
/** 
  * @} STM32_USBPD_APPLICATION_USBPD_Device_Exported_Functions
  */

/** 
  * @} STM32_USBPD_APPLICATION_USBPD_Device
  */

/** 
  * @} STM32_USBPD_APPLICATION
  */

#ifdef __cplusplus
}
#endif
#endif /*__usbpd_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
