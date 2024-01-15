/**
  ******************************************************************************
  * @file    steval_2stpd01_stpd01.h
  * @author  SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   This file contains definitions for STPD01 component.
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
#ifndef STEVAL_2STPD01_STPD01_H
#define STEVAL_2STPD01_STPD01_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup BSP_STEVAL_2STPD01
  * @{
  */ 
   
/** @defgroup BSP_STEVAL_2STPD01_STPD01 STDP01 Support
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "steval_2stpd01.h"

/** @defgroup BSP_STEVAL_2STPD01_STPD01_Exported_Defines Exported Defines
  * @{
  */
#define BSP_STPD01_GPIO_ENABLE    0  /*!< Enable gpio configuration index       */
#define BSP_STPD01_GPIO_ALERT     1  /*!< Alert gpio configuration index        */
#define BSP_STPD01_GPIO_PGOOD     2  /*!< Power Good gpio configuration index   */
#define BSP_STPD01_GPIO_NUM       3  /*!< STPD01 gpio number                    */
/**
  * @} BSP_STEVAL_2STPD01_STPD01_Exported_Defines
  */

/** @defgroup BSP_STEVAL_2STPD01_STPD01_Exported_Functions Exported Functions
  * @{
  */
int32_t BSP_STPD01_Init(uint32_t PortNum, uint16_t Address);
int32_t BSP_STPD01_DeInit(uint32_t PortNum);
int32_t BSP_STPD01_Enable(uint32_t PortNum, uint32_t Enable);
int32_t BSP_STPD01_IsEnable(uint32_t PortNum);

int32_t BSP_STPD01_VBus_Init(uint32_t PortNum);
int32_t BSP_STPD01_VBus_DeInit(uint32_t PortNum);
int32_t BSP_STPD01_VBus(uint32_t PortNum, uint32_t Enable);
int32_t BSP_STPD01_VBus_Value(uint32_t PortNum, int32_t Voltage);
/**
  * @} BSP_STEVAL_2STPD01_STPD01_Exported_Functions
  */

/**
  * @} BSP_STEVAL_2STPD01_STPD01
  */

/**
  * @} BSP_STEVAL_2STPD01
  */

#ifdef __cplusplus
}
#endif

#endif /* STEVAL_2STPD01_STPD01_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
