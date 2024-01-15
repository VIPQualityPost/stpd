/**
  ******************************************************************************
  * @file    usbpd_power_monitor_app.h
  * @version 1.0.1
  * @date    2021-11-08
  * @author  MCD Application Team and SRA.SLDU.SC Team
  * @brief   Header file for usbpd_power_monitor.c file
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
#ifndef __USBPD_POWER_MONITOR_APP_H
#define __USBPD_POWER_MONITOR_APP_H

#ifdef __cplusplus
extern "C" {
#endif
  
/** @addtogroup APPLICATION
  * @{
  */

/** @defgroup USBPD_POWER_MONITOR_APP Power Monitor Application
  * @brief The power monitor handlers and customizations
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "usbpd_power_monitor.h" 

/** @defgroup USBPD_POWER_MONITOR_APP_Exported_Constants Exported Constants
  * @{
  */
/* Exported constants --------------------------------------------------------*/
#define PM_CTRL_TIME_DONTCARE     (-1)                  /*!< No change in the control timing */
#define PM_CTRL_TIME_MS           (200)                 /*!< Default time of execution of @ref PM_Control_Task task */
#define PM_CTRL_TIME_LP_MS        (200)                 /*!< Low power mode time execution of @ref PM_Control_Task task */
#define PM_TASK_TIME_MS           (666)                 /*!< Default time of execution of @ref PM_Monitor_Task task */
#define PM_TASK_TIME_LOWRATE_MS   (888)                 /*!< Lowrate time of execution of @ref PM_Monitor_Task task */
#define PM_FAULT_TIME_MS          (4 * PM_TASK_TIME_MS) /*!< Time of execution of @ref PM_Monitor_Task task in case of fault */
#define PM_MAX_FAULT              (3)                   /*!< Max number di fault condition, after fire a critical condition */
/** 
  * @} USBPD_POWER_MONITOR_APP_Exported_Constants
  */

/** @defgroup USBPD_POWER_MONITOR_APP_Exported_Macros Exported Macros
  * @{
  */
#define PM_LED_Toggle()     BSP_LED_Toggle(LED_GREEN)
#define PM_LED_On()         BSP_LED_On(LED_GREEN)
#define PM_LED_Off()        BSP_LED_Off(LED_GREEN)  
/** 
  * @} USBPD_POWER_MONITOR_APP_Exported_Macros
  */

/** @defgroup USBPD_POWER_MONITOR_APP_Exported_Functions Exported Functions
  * @{
  */
PM_Status_TypeDef PM_App_Init(void);
/** 
  * @} USBPD_POWER_MONITOR_APP_Exported_Functions
  */

/** 
  * @} USBPD_POWER_MONITOR_APP
  */

/** 
  * @} APPLICATION
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBPD_POWER_MONITOR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
