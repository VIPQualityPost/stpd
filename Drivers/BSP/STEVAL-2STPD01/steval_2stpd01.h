/**
  ******************************************************************************
  * @file    steval_2stpd01.h
  * @author  SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   This file contains definitions for STEVAL-2STPD01 BSP.
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
#ifndef STEVAL_2STPD01_H
#define STEVAL_2STPD01_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/** @addtogroup BSP
  * @{
  */
   
/** @defgroup BSP_STEVAL_2STPD01 STEVAL-2STPD01
  * @{
  */ 
   
/**
  * @} BSP_STEVAL_2STPD01
  */
   
/** @addtogroup BSP_STEVAL_2STPD01
  * @{
  */    

/** @defgroup BSP_STEVAL_2STPD01_COMMON Base and ADC
  * @{
  */
   
#include "main.h"

 /* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_nucleo.h"
#include "steval_2stpd01_errno.h"
#include "steval_2stpd01_conf.h"
#include "steval_2stpd01_usbpd_pwr.h"
#include "steval_2stpd01_stpd01.h"
#include "steval_2stpd01_tcpp02.h"



/** @defgroup BSP_STEVAL_2STPD01_COMMON_Exported_Macros Exported Macros
  * @{
  */

/**
  * @brief  Fill Port and Pin configuration structure based on defined naming
  */
#define BSP_GPIO_Def(VAR) { \
   .Port = BSP_##VAR##_PORT, \
   .Pin = BSP_##VAR##_PIN, \
   .Mode = BSP_##VAR##_MODE, \
   .PuPd = BSP_##VAR##_PUPD, \
 }


/**
  * @brief  Fill ADC Port, Pin, channel and rank configuration structure based on defined naming
  */
#define BSP_ADC_GPIO_Def(NAME) { \
  .PortPin = BSP_GPIO_Def(ADC_GPIO_##NAME), \
  .Ch   = BSP_ADC_GPIO_##NAME##_CH,   \
  .Rank = BSP_ADC_GPIO_##NAME##_RANK  \
}
/** 
  * @} BSP_STEVAL_2STPD01_COMMON_Exported_Macros
  */

/** @defgroup BSP_STEVAL_2STPD01_COMMON_Exported_TypeDef Exported Typedef: enum, union and struct
  * @{
  */
/**
  * @brief  Port, pin and configuration of the GPIOs
  */
typedef struct
{
  GPIO_TypeDef * Port;
  uint32_t Pin;
  uint32_t Mode;
  uint32_t PuPd;
} BSP_PortPin_Conf_t;

/**
  * @brief  PortPin structure, channel and rank configuration of the ADC GPIOs
  */
typedef struct
{
  BSP_PortPin_Conf_t PortPin;
  uint32_t Ch;
  uint32_t Rank;
} BSP_ADC_PortPin_Conf_t;

/**
  * @brief  ADC variables
  */
typedef enum
{
  BSP_ADC_VIN          = 0,
  BSP_ADC_VBUS0        = 1,
  BSP_ADC_IBUS0_TCPP02 = 2,
  BSP_ADC_IBUS0_STPD01 = 3,
  BSP_ADC_VBUS1        = 4,
  BSP_ADC_IBUS1_TCPP02 = 5,
  BSP_ADC_IBUS1_STPD01 = 6,
} BSP_ADC_Type_t;
/** 
  * @} BSP_STEVAL_2STPD01_COMMON_Exported_TypeDef
  */


/** @defgroup BSP_STEVAL_2STPD01_COMMON_Exported_Functions Exported Functions
  * @{
  */
int32_t BSPX_Init(void);
int32_t BSPX_GetVersion(void);
uint32_t BSPX_ADC_GetValue(BSP_ADC_Type_t Type);
uint32_t BSPX_ADC_GetConvertedValue(BSP_ADC_Type_t Type);
/**
  * @} BSP_STEVAL_2STPD01_COMMON_Exported_Functions
  */

/** 
  * @} BSP_STEVAL_2STPD01_COMMON
  */

/**
  * @} BSP_STEVAL_2STPD01
  */

/**
  * @} BSP
  */

#ifdef __cplusplus
}
#endif

#endif /* STEVAL_2STPD01_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
