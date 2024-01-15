/**
  ******************************************************************************
  * @file    steval_2stpd01_tcpp02.h
  * @author  SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   This file contains definitions for TCPP02 component.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STEVAL_2STPD01_TCPP02_H
#define STEVAL_2STPD01_TCPP02_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup BSP_STEVAL_2STPD01
  * @{
  */ 
   
/** @defgroup BSP_STEVAL_2STPD01_TCPP02 TCPP02 Support
  * @{
  */
   
/* Includes ------------------------------------------------------------------*/
#include "steval_2stpd01.h"

/** @defgroup BSP_STEVAL_2STPD01_TCPP02_Exported_Defines Exported Defines
  * @{
  */
#define BSP_TCPP02_GPIO_ENABLE       0                  /*!< Enable gpio configuration index            */
#define BSP_TCPP02_GPIO_FLAG         1                  /*!< Flag gpio configuration index              */
#define BSP_TCPP02_GPIO_NUM          2                  /*!< TCPP02 gpio number                         */

#define BSP_TCPP02_DIS_DELAY         10U                /*!< timing from 2 read on discharge (ms)       */
#define BSP_TCPP02_DIS_TIME          200U               /*!< max duration of the discharge (ms)         */
#define BSP_TCPP02_DIS_FINAL_DELAY   5U                 /*!< delay when reached the vbus low threshold (ms) */
#define BSP_TCPP02_DIS_NUM           ((uint16_t)(BSP_TCPP02_DIS_TIME/BSP_TCPP02_DIS_DELAY))
/**
  * @} BSP_STEVAL_2STPD01_TCPP02_Exported_Defines
  */

/** @defgroup BSP_STEVAL_2STPD01_TCPP02_Exported_Functions Exported Functions
  * @{
  */
int32_t  BSP_TCPP02_Init(uint32_t PortNum, uint16_t Address);
int32_t  BSP_TCPP02_DeInit(uint32_t PortNum);
int32_t  BSP_TCPP02_Reset(uint32_t PortNum);
int32_t  BSP_TCPP02_Enable(uint32_t PortNum, uint32_t Enable);
int32_t BSP_TCPP02_IsEnable(uint32_t PortNum);
int32_t  BSP_TCPP02_PowerMode(uint32_t PortNum, USBPD_PWR_PowerModeTypeDef PwrMode);

void     BSP_TCPP02_EventCallback(uint32_t PortNum);

int32_t  BSP_TCPP02_VBus_Init(uint32_t PortNum);
int32_t  BSP_TCPP02_VBus_DeInit(uint32_t PortNum);
int32_t  BSP_TCPP02_VBus(uint32_t PortNum, uint32_t Enable);
int32_t  BSP_TCPP02_VBus_Status(uint32_t PortNum);
int32_t  BSP_TCPP02_VBus_Discharge(uint32_t PortNum, uint32_t Discharge);

int32_t  BSP_TCPP02_VConn_Init(uint32_t PortNum);
int32_t  BSP_TCPP02_VConn_DeInit(uint32_t PortNum);

int32_t  BSP_TCPP02_VConn_Switch(uint32_t PortNum, uint32_t CCPinId);
int32_t  BSP_TCPP02_VConn_Enable(uint32_t PortNum, uint32_t CCPinId, uint32_t Enable);
int32_t  BSP_TCPP02_VConn_Status(uint32_t PortNum);
int32_t  BSP_TCPP02_VConn_Discharge(uint32_t PortNum, uint32_t Discharge);
/**
  * @} BSP_STEVAL_2STPD01_TCPP02_Exported_Functions
  */

/**
  * @} BSP_STEVAL_2STPD01_TCPP02
  */

/**
  * @} BSP_STEVAL_2STPD01
  */

#ifdef __cplusplus
}
#endif

#endif /* STEVAL_2STPD01_TCPP02_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
