/**
  ******************************************************************************
  * @file    steval_2stpd01_tcpp02_conf.h
  * @author  SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   Configuration file for TCPP02 component.
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
#ifndef STEVAL_2STPD01_TCPP02_CONF_H
#define STEVAL_2STPD01_TCPP02_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/** @addtogroup BSP_STEVAL_2STPD01_TCPP02
  * @{
  */

/** @defgroup BSP_STEVAL_2STPD01_TCPP02_Config Config
  * @{
  */
   
 /* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tcpp0203.h"

/* Port 0 Configuration */
/** @defgroup BSP_STEVAL_2STPD01_TCPP02_Config_P0 Port 0
  * @{
  */
#define BSP_TCPP02_P0_ADDRESS         TCPP0203_I2C_ADDRESS_X6A     /*!< TCPP02 I2C Addr Port 0  */

/** @defgroup BSP_STEVAL_2STPD01_TCPP02_Config_P0_GPIOs GPIOs
  * @{
  */

/** @defgroup BSP_STEVAL_2STPD01_TCPP02_Config_P0_GPIOs_Enable Enable Pin
  * @{
  */
#define BSP_TCPP02_P0_ENABLE_PORT             GPIOB
#define BSP_TCPP02_P0_ENABLE_PIN              GPIO_PIN_10
#define BSP_TCPP02_P0_ENABLE_MODE             GPIO_MODE_OUTPUT_PP
#define BSP_TCPP02_P0_ENABLE_PUPD             GPIO_NOPULL
/**
  * @} BSP_STEVAL_2STPD01_TCPP02_Config_P0_GPIOs_Enable
  */
   
/** @defgroup BSP_STEVAL_2STPD01_TCPP02_Config_P0_GPIOs_Flag Flag Pin
  * @{
  */
#define BSP_TCPP02_P0_FLG_PORT                GPIOB
#define BSP_TCPP02_P0_FLG_PIN                 GPIO_PIN_13
#define BSP_TCPP02_P0_FLG_MODE                GPIO_MODE_IT_FALLING
#define BSP_TCPP02_P0_FLG_PUPD                GPIO_PULLUP
/**
  * @} BSP_STEVAL_2STPD01_TCPP02_Config_P0_GPIOs_Flag
  */
/**
  * @} BSP_STEVAL_2STPD01_TCPP02_Config_P0_GPIOs
  */
/**
  * @} BSP_STEVAL_2STPD01_TCPP02_Config_P0
  */

/* Configuration Port 1 */
/** @defgroup BSP_STEVAL_2STPD01_TCPP02_Config_P1 Port 1
  * @{
  */
#define BSP_TCPP02_P1_ADDRESS         TCPP0203_I2C_ADDRESS_X68     /*!< TCPP02 I2C Addr Port 1  */


/** @defgroup BSP_STEVAL_2STPD01_TCPP02_Config_P1_GPIOs_Enable Enable Pin
  * @{
  */
#define BSP_TCPP02_P1_ENABLE_PORT             GPIOC
#define BSP_TCPP02_P1_ENABLE_PIN              GPIO_PIN_6
#define BSP_TCPP02_P1_ENABLE_MODE             GPIO_MODE_OUTPUT_PP
#define BSP_TCPP02_P1_ENABLE_PUPD             GPIO_NOPULL
/**
  * @} BSP_STEVAL_2STPD01_TCPP02_Config_P1_GPIOs_Enable
  */
   
/** @defgroup BSP_STEVAL_2STPD01_TCPP02_Config_P1_GPIOs_Flag Flag Pin
  * @{
  */
#define BSP_TCPP02_P1_FLG_PORT                GPIOC
#define BSP_TCPP02_P1_FLG_PIN                 GPIO_PIN_8
#define BSP_TCPP02_P1_FLG_MODE                GPIO_MODE_IT_FALLING
#define BSP_TCPP02_P1_FLG_PUPD                GPIO_PULLUP
/**
  * @} BSP_STEVAL_2STPD01_TCPP02_Config_P1_GPIOs_Flag
  */
/**
  * @} BSP_STEVAL_2STPD01_TCPP02_Config_P1_GPIOs
  */
/**
  * @} BSP_STEVAL_2STPD01_TCPP02_Config_P1
  */
   
/** @defgroup BSP_STEVAL_2STPD01_TCPP02_Config_IRQ IRQ
  * @{
  */   
#define BSP_TCPP02_IRQ_NUM      1               /*!< EXTI IRQ number on the TCPP02 */
#define BSP_TCPP02_IRQ          {EXTI4_15_IRQn} /*!< EXTI IRQ enabling: Flag pin for both ports */
/**
  * @} BSP_STEVAL_2STPD01_TCPP02_Config_IRQ
  */
   
/**
  * @} BSP_STEVAL_2STPD01_TCPP02_Config
  */ 

#ifdef __cplusplus
}
#endif

#endif /* STEVAL_2STPD01_TCPP02_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
