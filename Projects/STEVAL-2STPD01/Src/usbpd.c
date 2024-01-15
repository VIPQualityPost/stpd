/**
  ******************************************************************************
  * @file    usbpd.c
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

/** @addtogroup STM32_USBPD_APPLICATION_USBPD_Device
  * @{
  */
/* Includes ------------------------------------------------------------------*/
#include "usbpd.h"
#include "steval_2stpd01.h"

#if defined(_GUI_INTERFACE)
#include "gui_api.h"
#endif /* _GUI_INTERFACE */

/** @defgroup STM32_USBPD_APPLICATION_USBPD_Device_Private_Variables Private Variables
  * @{
  */
/* Private variables ---------------------------------------------------------*/
#if defined(_GUI_INTERFACE)
const uint8_t BSP_HWBoardVersionName[] = "STEVAL-2STPD01";
const uint8_t BSP_PDTypeName[] = "AB0049";
#endif /* _GUI_INTERFACE */
/** 
  * @} STM32_USBPD_APPLICATION_USBPD_Device_Private_Variables
  */

/** @defgroup STM32_USBPD_APPLICATION_USBPD_Device_Private_Functions Private Functions
  * @{
  */
/* Private functions ---------------------------------------------------------*/
#if defined(_GUI_INTERFACE)
static const uint8_t*          GetHWBoardVersionName(void);
static const uint8_t*          GetPDTypeName(void);
#endif /* _GUI_INTERFACE */
/** 
  * @} STM32_USBPD_APPLICATION_USBPD_Device_Private_Functions
  */

/** @addtogroup STM32_USBPD_APPLICATION_USBPD_Device_Exported_Functions
  * @{
  */
/* USBPD init function */
void MX_USBPD_Init(void)
{

  /* Global Init of USBPD HW */
  USBPD_HW_IF_GlobalHwInit();

  /* STEVAL-2STPD01 Init */
//  if (BSP_ERROR_NONE == BSPX_Init()) Error_Handler(0);
  BSPX_Init();

  /* Initialize the Device Policy Manager */
  if (USBPD_OK != USBPD_DPM_InitCore())
  {
    while(1);
  }

#if defined(_GUI_INTERFACE)
  /* Initialize GUI before retrieving PDO from RAM */
  GUI_Init(GetHWBoardVersionName, GetPDTypeName, HW_IF_PWR_GetVoltage, HW_IF_PWR_GetCurrent);
#endif /* _GUI_INTERFACE */

  /* Initialise the DPM application */
  if (USBPD_OK != USBPD_DPM_UserInit())
  {
    while(1);
  }

  if (USBPD_OK != USBPD_DPM_InitOS())
  {
    while(1);
  }

  /* Enable IRQ which has been disabled by FreeRTOS services */
  __enable_irq();

}
/** 
  * @} STM32_USBPD_APPLICATION_USBPD_Device_Exported_Functions
  */

/** @addtogroup STM32_USBPD_APPLICATION_USBPD_Device_Private_Functions
  * @{
  */
#if defined(_GUI_INTERFACE)
/**
  * @brief  This method returns HW board version name
  * @retval HW Board version name
  */
static const uint8_t* GetHWBoardVersionName(void)
{
  return BSP_HWBoardVersionName;
}

/**
  * @brief  This method returns HW PD Type name
  * @retval HW Board version name
  */
static const uint8_t* GetPDTypeName(void)
{
  return BSP_PDTypeName;
}
#endif /* _GUI_INTERFACE */
/**
  * @} STM32_USBPD_APPLICATION_USBPD_Device_Private_Functions
  */

/**
  * @} STM32_USBPD_APPLICATION_USBPD_Device
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
