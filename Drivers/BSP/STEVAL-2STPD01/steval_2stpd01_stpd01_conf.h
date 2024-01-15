/**
  ******************************************************************************
  * @file    steval_2stpd01_stpd01_conf.h
  * @author  SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   Configuration file for STPD01 component.
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
#ifndef STEVAL_2STPD01_STPD01_CONF_H
#define STEVAL_2STPD01_STPD01_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup BSP_STEVAL_2STPD01_STPD01
  * @{
  */

/** @defgroup BSP_STEVAL_2STPD01_STPD01_Config Config
  * @{
  */
 /* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stpd01.h"

/* Port 0 Configuration */
/** @defgroup BSP_STEVAL_2STPD01_STPD01_Config_P0 Port 0
  * @{
  */
#define BSP_STPD01_P0_ADDRESS         STPD01_I2C_ADDRESS_VCC     /*!< STPD01 I2C Addr Port 0  */

/** @defgroup BSP_STEVAL_2STPD01_STPD01_Config_P0_GPIOs GPIOs
  * @{
  */

/** @defgroup BSP_STEVAL_2STPD01_STPD01_Config_P0_GPIOs_Enable Enable Pin
  * @{
  */
#define BSP_STPD01_P0_ENABLE_PORT     GPIOB
#define BSP_STPD01_P0_ENABLE_PIN      GPIO_PIN_3
#define BSP_STPD01_P0_ENABLE_MODE     GPIO_MODE_OUTPUT_PP
#define BSP_STPD01_P0_ENABLE_PUPD     GPIO_NOPULL
/**
  * @} BSP_STEVAL_2STPD01_STPD01_Config_P0_GPIOs_Enable
  */
   
/** @defgroup BSP_STEVAL_2STPD01_STPD01_Config_P0_GPIOs_Alert Alert Pin
  * @{
  */
#define BSP_STPD01_P0_ALERT_PORT      GPIOC
#define BSP_STPD01_P0_ALERT_PIN       GPIO_PIN_0
#define BSP_STPD01_P0_ALERT_MODE      GPIO_MODE_IT_FALLING
#define BSP_STPD01_P0_ALERT_PUPD      GPIO_NOPULL
/**
  * @} BSP_STEVAL_2STPD01_STPD01_Config_P0_GPIOs_Alert
  */
   
/** @defgroup BSP_STEVAL_2STPD01_STPD01_Config_P0_GPIOs_PowerGood Power Good Pin
  * @{
  */
#define BSP_STPD01_P0_PGOOD_PORT      GPIOC
#define BSP_STPD01_P0_PGOOD_PIN       GPIO_PIN_2
#define BSP_STPD01_P0_PGOOD_MODE      GPIO_MODE_IT_FALLING
#define BSP_STPD01_P0_PGOOD_PUPD      GPIO_NOPULL
/**
  * @} BSP_STEVAL_2STPD01_STPD01_Config_P0_GPIOs_PowerGood
  */
/**
  * @} BSP_STEVAL_2STPD01_STPD01_Config_P0_GPIOs
  */
/**
  * @} BSP_STEVAL_2STPD01_STPD01_Config_P0
  */
  
/* Configuration Port 1 */
/** @defgroup BSP_STEVAL_2STPD01_STPD01_Config_P1 Port 1
  * @{
  */
#define BSP_STPD01_P1_ADDRESS         STPD01_I2C_ADDRESS_GND     /*!< STPD01 I2C Addr Port 1  */

/** @defgroup BSP_STEVAL_2STPD01_STPD01_Config_P1_GPIOs_Enable Enable Pin
  * @{
  */
#define BSP_STPD01_P1_ENABLE_PORT     GPIOB
#define BSP_STPD01_P1_ENABLE_PIN      GPIO_PIN_2
#define BSP_STPD01_P1_ENABLE_MODE     GPIO_MODE_OUTPUT_PP
#define BSP_STPD01_P1_ENABLE_PUPD     GPIO_NOPULL
/**
  * @} BSP_STEVAL_2STPD01_STPD01_Config_P1_GPIOs_Enable
  */
   
/** @defgroup BSP_STEVAL_2STPD01_STPD01_Config_P1_GPIOs_Alert Alert Pin
  * @{
  */
#define BSP_STPD01_P1_ALERT_PORT      GPIOC
#define BSP_STPD01_P1_ALERT_PIN       GPIO_PIN_1
#define BSP_STPD01_P1_ALERT_MODE      GPIO_MODE_IT_FALLING
#define BSP_STPD01_P1_ALERT_PUPD      GPIO_NOPULL
/**
  * @} BSP_STEVAL_2STPD01_STPD01_Config_P1_GPIOs_Alert
  */
   
/** @defgroup BSP_STEVAL_2STPD01_STPD01_Config_P1_GPIOs_PowerGood Power Good Pin
  * @{
  */
#define BSP_STPD01_P1_PGOOD_PORT      GPIOC
#define BSP_STPD01_P1_PGOOD_PIN       GPIO_PIN_3
#define BSP_STPD01_P1_PGOOD_MODE      GPIO_MODE_IT_FALLING
#define BSP_STPD01_P1_PGOOD_PUPD      GPIO_NOPULL
/**
  * @} BSP_STEVAL_2STPD01_STPD01_Config_P1_GPIOs_PowerGood
  */
/**
  * @} BSP_STEVAL_2STPD01_STPD01_Config_P1_GPIOs
  */  
/**
  * @} BSP_STEVAL_2STPD01_STPD01_Config_P1
  */  

/** @defgroup BSP_STEVAL_2STPD01_STPD01_Config_IRQ IRQ
  * @{
  */   
#define BSP_STPD01_IRQ_NUM       1               /*!< EXTI IRQ number on the STPD01 (1 or 2) */

#if BSP_STPD01_IRQ_NUM==1
#define BSP_STPD01_IRQ           {EXTI0_1_IRQn}  /*!< EXTI IRQ enabling: only Alert pin for both ports */
#elif BSP_STPD01_IRQ_NUM==2
#define BSP_STPD01_IRQ           {EXTI0_1_IRQn, EXTI2_3_IRQn} /*!< EXTI IRQ enabling: Alert and PowerGood pins for both ports */
#else
#error "Wrong number of IRQ for STPD01 component"
#endif
/**
  * @} BSP_STEVAL_2STPD01_STPD01_Config_IRQ
  */
   
/**
  * @} BSP_STEVAL_2STPD01_STPD01_Config
  */   
   
#ifdef __cplusplus
}
#endif

#endif /* STEVAL_2STPD01_STPD01_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
