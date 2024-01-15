/**
  ******************************************************************************
  * @file    stm32g0xx_it.h
  * @version 1.0.1
  * @date    2021-11-08
  * @author  SRA.SLDU.SC Team
  * @brief   This file contains the headers of the interrupt handlers.
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
#ifndef __STM32G0xx_IT_H
#define __STM32G0xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup APPLICATION
  * @{
  */

/** @defgroup APPLICATION_IT_Handlers Interrupt handlers
  * @{
  */

/* Private includes ----------------------------------------------------------*/
#if defined(_GUI_INTERFACE) || defined(_TRACE)
#include "tracer_emb_conf.h"
#endif /*_TRACE || _GUI_INTERFACE */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions prototypes ---------------------------------------------*/
/** @defgroup APPLICATION_IT_Handlers_Handlers Handlers
  * @{
  */
void NMI_Handler(void);
void HardFault_Handler(void);
void SysTick_Handler(void);
#if defined (STM32G0C1xx)
void USB_UCPD1_2_IRQHandler(void);
#else
void UCPD1_2_IRQHandler(void);
#endif
#if defined(_GUI_INTERFACE) || defined(_TRACE)
void TRACER_EMB_USART_IRQHANDLER(void);
#if TRACER_EMB_DMA_MODE == 1UL
void DMA1_Ch4_7_DMAMUX1_OVR_IRQHandler(void);
void DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX1_OVR_IRQHandler(void);
#endif
#endif /*_TRACE || _GUI_INTERFACE */

void EXTI0_1_IRQHandler(void);
void EXTI2_3_IRQHandler(void);
void EXTI4_15_IRQHandler(void);

void RTC_TAMP_IRQHandler( void );
void I2C1_IRQHandler(void);

/**
  * @} APPLICATION_IT_Handlers_Handlers
  */

/**
  * @} APPLICATION_IT_Handlers
  */

/**
  * @} APPLICATION
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32G0xx_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
