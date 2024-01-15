/**
  ******************************************************************************
  * @file    usbpd_power_monitor_app.c
  * @version 1.0.1
  * @date    2021-11-08
  * @author  SRA.SLDU.SC Team
  * @brief   File containing Low Power Mode APIs implementation.
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

/** @addtogroup USBPD_POWER_MONITOR_APP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "usbpd_power_monitor_app.h"

/** @defgroup USBPD_POWER_MONITOR_APP_Private_Functions_Callbacks_Handlers Callback Handlers
  * @brief Power Monitor callback handlers implementation
  * @{
  */
static PM_Status_TypeDef PM_ReadData_Handler(PM_Data_Typedef * pData);
static PM_Status_TypeDef PM_NotifyData_Handler(PM_Data_Typedef * pData);
static PM_Status_TypeDef PM_CheckStatus_Handler(PM_Data_Typedef * pData);
static PM_Status_TypeDef PM_FaultCondition_Handler(uint8_t PortNum, uint32_t Param);
static PM_Status_TypeDef PM_CriticalCondition_Handler(uint8_t PortNum, uint32_t Param);
__weak void BSP_PWR_VoltageCtrl(uint32_t VBus, int32_t IBus);

/** 
 * @} USBPD_POWER_MONITOR_APP_Private_Functions_Callbacks_Handlers
 */

/** @defgroup USBPD_POWER_MONITOR_APP_Private_Variables Private Variables
  * @brief Power Monitor callback handlers implementation
  * @{
  */
/** @brief Callback vars
  */
PM_Callback_Typedef PM_Callbacks = {
  .PM_ReadData                = PM_ReadData_Handler,
  .PM_NotifyData              = PM_NotifyData_Handler,
  .PM_CheckStatus             = PM_CheckStatus_Handler,
  .PM_FaultCondition          = PM_FaultCondition_Handler,
  .PM_CriticalCondition       = PM_CriticalCondition_Handler,
};

/** @brief Fault counter
  */
__USED uint8_t PM_FaultCount[PM_PORT_COUNT] = {PM_MAX_FAULT,PM_MAX_FAULT};

/** 
 * @} USBPD_POWER_MONITOR_APP_Private_Variables
 */

/** @addtogroup USBPD_POWER_MONITOR_APP_Exported_Functions
  * @{
  */
/** @brief Init the module
  */
PM_Status_TypeDef PM_App_Init(void)
{
  return PM_Monitor_Init(&PM_Callbacks);
}

/** 
  * @} USBPD_POWER_MONITOR_APP_Exported_Functions
  */

/** @addtogroup USBPD_POWER_MONITOR_APP_Private_Functions_Callbacks_Handlers
  * @{
  */

/** @brief Read data from the device
  * @param pData structure pointer (input)
  */
static PM_Status_TypeDef PM_ReadData_Handler(PM_Data_Typedef * pData)
{
  assert_param(pData);
  
  BSP_USBPD_PWR_VBUSGetVoltage(pData->PortNum,&pData->VBus);
  BSP_USBPD_PWR_VBUSGetCurrent(pData->PortNum,&pData->IBus);

  return PM_STATUS_OK;
}

/** @brief Externally data notification
  * @param pData structure pointer (output)
  */

static PM_Status_TypeDef PM_NotifyData_Handler(PM_Data_Typedef * pData)
{
  assert_param(pData);

  
  return PM_STATUS_OK;
}

/** @brief Externally check of the system status 
  * @param pData structure pointer (output)
  */
static PM_Status_TypeDef PM_CheckStatus_Handler(PM_Data_Typedef * pData)
{
  assert_param(pData);
  
  
  if (DPM_Ports[pData->PortNum].DPM_IsConnected == 0)
  {
    if (PM_FaultCount[pData->PortNum] > 0)
    {
      PM_FaultCount[pData->PortNum] = PM_MAX_FAULT;
    }
  }
  
  return PM_STATUS_OK;
}

/** @brief Fault condition handler
  * @param PortNum port number
  * @param Param parameter structure
  */
static PM_Status_TypeDef PM_FaultCondition_Handler(uint8_t PortNum, uint32_t Param)
{
#if defined(_TRACE)
  char text[50] = {0};
  sprintf(text, "FaultCondition detect Param:%d", Param);
  USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PortNum, 0, (uint8_t *)text, strlen(text));
#endif  

  /* Disabling only the STPD01 and leave the TCPP02 to get the detach event */
  BSP_STPD01_Enable(PortNum, BSP_PWR_OFF);
  
  return PM_STATUS_OK;
}

/** @brief Critical condition handler 
  * @param PortNum port number, 0xFF for all
  * @param Param parameter structure
  */
static PM_Status_TypeDef PM_CriticalCondition_Handler(uint8_t PortNum, uint32_t Param)
{
  /* In case of critical condition, remove the vbus */
  /* disable/suspend all task */
  /* de-init USBC periph */
  /* turn off the load switch */
  /* block the board */

#if defined(_TRACE)
  char text[50] = {0};
  sprintf(text, "CriticalCondition detect Param:%d", Param);
  USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PortNum, 0, (uint8_t *)text, strlen(text));
#endif  

  /* only the switch power is admitted to enable again the port */
  BSP_USBPD_PWR_EnablePort(PortNum, BSP_PWR_OFF);
  
  return PM_STATUS_ERR;
}

/** @brief Callback to receive bus values
  * @param VBus value
  * @param IBus value
  */
void BSP_PWR_VoltageCtrl(uint32_t VBus, int32_t IBus)
{
  (void)VBus;
  (void)IBus;
}

/**
  * @} USBPD_POWER_MONITOR_APP_Private_Functions_Callbacks_Handlers
  */

/** 
  * @} USBPD_POWER_MONITOR_APP
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
