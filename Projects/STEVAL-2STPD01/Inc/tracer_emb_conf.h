/**
  ******************************************************************************
  * @file    tracer_emb_conf.h
  * @version 1.0.1
  * @date    2021-11-08
  * @author  MCD Application Team and SRA.SLDU.SC Team
  * @brief   This file contains the Trace HW related defines.
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

#ifndef TRACER_EMB_CONF_H
#define TRACER_EMB_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_usart.h"
#if defined(LPUART1)
#include "stm32g0xx_ll_lpuart.h"
#endif /* LPUART1 */
/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* -----------------------------------------------------------------------------
      Definitions for TRACE feature
-------------------------------------------------------------------------------*/
#define TRACER_EMB_BAUDRATE                          921600UL

#define TRACER_EMB_DMA_MODE                          1UL
#define TRACER_EMB_IT_MODE                           0UL

#define TRACER_EMB_BUFFER_SIZE                       1024UL

/* -----------------------------------------------------------------------------
      Definitions for TRACE Hw information
-------------------------------------------------------------------------------*/

#if defined(NUCLEO_MB1360A)

#define TRACER_EMB_IS_INSTANCE_LPUART_TYPE           1UL /* set to 1UL if LPUART is used instead of USART */
#define TRACER_EMB_USART_INSTANCE                    LPUART1

#define TRACER_EMB_TX_GPIO                           GPIOA
#define TRACER_EMB_TX_PIN                            LL_GPIO_PIN_2
#define TRACER_EMB_TX_AF                             LL_GPIO_AF_6
#define TRACER_EMB_TX_GPIO_ENABLE_CLOCK()            LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA)
#define TRACER_EMB_TX_GPIO_DISABLE_CLOCK()           LL_IOP_GRP1_DisableClock(LL_IOP_GRP1_PERIPH_GPIOA)
#define TRACER_EMB_RX_GPIO                           GPIOA
#define TRACER_EMB_RX_PIN                            LL_GPIO_PIN_3
#define TRACER_EMB_RX_AF                             LL_GPIO_AF_6
#define TRACER_EMB_RX_GPIO_ENABLE_CLOCK()            LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA)
#define TRACER_EMB_RX_GPIO_DISABLE_CLOCK()           LL_IOP_GRP1_DisableClock(LL_IOP_GRP1_PERIPH_GPIOA)

#define TRACER_EMB_ENABLE_CLK_USART()                LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_LPUART1)
#define TRACER_EMB_DISABLE_CLK_USART()               LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_LPUART1)
#define TRACER_EMB_SET_CLK_SOURCE_USART()            /* No need for clock source selection in case of USART3 */
#define TRACER_EMB_USART_IRQ                         USART3_4_LPUART1_IRQn
#define TRACER_EMB_USART_IRQHANDLER                  USART3_4_LPUART1_IRQHandler
#define TRACER_EMB_TX_AF_FUNCTION                    LL_GPIO_SetAFPin_0_7
#define TRACER_EMB_RX_AF_FUNCTION                    LL_GPIO_SetAFPin_0_7
#if TRACER_EMB_DMA_MODE == 1UL
#define TRACER_EMB_DMA_INSTANCE                      DMA1
#define TRACER_EMB_ENABLE_CLK_DMA()                  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1)
#define TRACER_EMB_TX_DMA_REQUEST                    LL_DMAMUX_REQ_LPUART1_TX
#define TRACER_EMB_TX_DMA_CHANNEL                    LL_DMA_CHANNEL_7
#define TRACER_EMB_ENABLECHANNEL                     LL_DMA_EnableChannel
#define TRACER_EMB_DISABLECHANNEL                    LL_DMA_DisableChannel
#define TRACER_EMB_TX_DMA_IRQ                        DMA1_Ch4_7_DMAMUX1_OVR_IRQn
#define TRACER_EMB_TX_DMA_IRQHANDLER                 DMA1_Ch4_7_DMAMUX1_OVR_IRQHandler
#define TRACER_EMB_TX_DMA_ACTIVE_FLAG                LL_DMA_IsActiveFlag_TC7
#define TRACER_EMB_TX_DMA_CLEAR_FLAG                 LL_DMA_ClearFlag_GI7
#endif /* TRACER_EMB_DMA_MODE */

#elif  defined(_DISCO_TRACE_CN11)     /* to use the CN11 pins to trace instead of STLink and the VCP */
#define TRACER_EMB_IS_INSTANCE_LPUART_TYPE           0UL /* set to 1UL if LPUART is used instead of USART */
#define TRACER_EMB_USART_INSTANCE                    USART3

#define TRACER_EMB_TX_GPIO                           GPIOB
#define TRACER_EMB_TX_PIN                            LL_GPIO_PIN_8
#define TRACER_EMB_TX_AF                             LL_GPIO_AF_4
#define TRACER_EMB_TX_GPIO_ENABLE_CLOCK()            LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB)
#define TRACER_EMB_TX_GPIO_DISABLE_CLOCK()           LL_IOP_GRP1_DisableClock(LL_IOP_GRP1_PERIPH_GPIOB)
#define TRACER_EMB_RX_GPIO                           GPIOB
#define TRACER_EMB_RX_PIN                            LL_GPIO_PIN_9
#define TRACER_EMB_RX_AF                             LL_GPIO_AF_4
#define TRACER_EMB_RX_GPIO_ENABLE_CLOCK()            LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB)
#define TRACER_EMB_RX_GPIO_DISABLE_CLOCK()           LL_IOP_GRP1_DisableClock(LL_IOP_GRP1_PERIPH_GPIOB)

#define TRACER_EMB_ENABLE_CLK_USART()                LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3)
#define TRACER_EMB_DISABLE_CLK_USART()               LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_USART3)
#define TRACER_EMB_SET_CLK_SOURCE_USART()            /* No need for clock source selection in case of USART3 */
#define TRACER_EMB_USART_IRQ                         USART3_4_LPUART1_IRQn
#define TRACER_EMB_USART_IRQHANDLER                  USART3_4_LPUART1_IRQHandler
#define TRACER_EMB_TX_AF_FUNCTION                    LL_GPIO_SetAFPin_8_15
#define TRACER_EMB_RX_AF_FUNCTION                    LL_GPIO_SetAFPin_8_15
#if TRACER_EMB_DMA_MODE == 1UL
#define TRACER_EMB_DMA_INSTANCE                      DMA1
#define TRACER_EMB_ENABLE_CLK_DMA()                  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1)
#define TRACER_EMB_TX_DMA_REQUEST                    LL_DMAMUX_REQ_USART3_TX
#define TRACER_EMB_TX_DMA_CHANNEL                    LL_DMA_CHANNEL_7
#define TRACER_EMB_ENABLECHANNEL                     LL_DMA_EnableChannel
#define TRACER_EMB_DISABLECHANNEL                    LL_DMA_DisableChannel
#define TRACER_EMB_TX_DMA_IRQ                        DMA1_Ch4_7_DMAMUX1_OVR_IRQn
#define TRACER_EMB_TX_DMA_IRQHANDLER                 DMA1_Ch4_7_DMAMUX1_OVR_IRQHandler
#define TRACER_EMB_TX_DMA_ACTIVE_FLAG                LL_DMA_IsActiveFlag_TC7
#define TRACER_EMB_TX_DMA_CLEAR_FLAG                 LL_DMA_ClearFlag_GI7
#endif /* TRACER_EMB_DMA_MODE */

#else /* if not _DISCO_TRACE_CN11 and NUCLEO_MB1360A */
#define TRACER_EMB_IS_INSTANCE_LPUART_TYPE           0UL /* set to 1UL if LPUART is used instead of USART */
#define TRACER_EMB_USART_INSTANCE                    USART3

#define TRACER_EMB_TX_GPIO                           GPIOC
#define TRACER_EMB_TX_PIN                            LL_GPIO_PIN_10
#define TRACER_EMB_TX_AF                             LL_GPIO_AF_0
#define TRACER_EMB_TX_GPIO_ENABLE_CLOCK()            LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC)
#define TRACER_EMB_TX_GPIO_DISABLE_CLOCK()           LL_IOP_GRP1_DisableClock(LL_IOP_GRP1_PERIPH_GPIOC)
#define TRACER_EMB_RX_GPIO                           GPIOC
#define TRACER_EMB_RX_PIN                            LL_GPIO_PIN_11
#define TRACER_EMB_RX_AF                             LL_GPIO_AF_0
#define TRACER_EMB_RX_GPIO_ENABLE_CLOCK()            LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC)
#define TRACER_EMB_RX_GPIO_DISABLE_CLOCK()           LL_IOP_GRP1_DisableClock(LL_IOP_GRP1_PERIPH_GPIOC)

#define TRACER_EMB_ENABLE_CLK_USART()                LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3)
#define TRACER_EMB_DISABLE_CLK_USART()               LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_USART3)
#define TRACER_EMB_SET_CLK_SOURCE_USART()            /* No need for clock source selection in case of USART3 // LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK2) */
#if defined (STM32G0C1xx)
#define TRACER_EMB_USART_IRQ                         USART3_4_5_6_LPUART1_IRQn
#define TRACER_EMB_USART_IRQHANDLER                  USART3_4_5_6_LPUART1_IRQHandler
#else
#define TRACER_EMB_USART_IRQ                         USART3_4_LPUART1_IRQn
#define TRACER_EMB_USART_IRQHANDLER                  USART3_4_LPUART1_IRQHandler
#endif /* STM32G0C1xx */
#define TRACER_EMB_TX_AF_FUNCTION                    LL_GPIO_SetAFPin_8_15
#define TRACER_EMB_RX_AF_FUNCTION                    LL_GPIO_SetAFPin_8_15
#if TRACER_EMB_DMA_MODE == 1UL
#define TRACER_EMB_DMA_INSTANCE                      DMA1
#define TRACER_EMB_ENABLE_CLK_DMA()                  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1)
#define TRACER_EMB_TX_DMA_REQUEST                    LL_DMAMUX_REQ_USART3_TX
#define TRACER_EMB_TX_DMA_CHANNEL                    LL_DMA_CHANNEL_7
#define TRACER_EMB_ENABLECHANNEL                     LL_DMA_EnableChannel
#define TRACER_EMB_DISABLECHANNEL                    LL_DMA_DisableChannel
#if defined (STM32G0C1xx)
#define TRACER_EMB_TX_DMA_IRQ                        DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX1_OVR_IRQn
#define TRACER_EMB_TX_DMA_IRQHANDLER                 DMA1_Ch4_7_DMA2_Ch1_5_DMAMUX1_OVR_IRQHandler
#else
#define TRACER_EMB_TX_DMA_IRQ                        DMA1_Ch4_7_DMAMUX1_OVR_IRQn
#define TRACER_EMB_TX_DMA_IRQHANDLER                 DMA1_Ch4_7_DMAMUX1_OVR_IRQHandler
#endif /* STM32G0C1xx */
#define TRACER_EMB_TX_DMA_ACTIVE_FLAG                LL_DMA_IsActiveFlag_TC7
#define TRACER_EMB_TX_DMA_CLEAR_FLAG                 LL_DMA_ClearFlag_GI7
#endif /* TRACER_EMB_DMA_MODE == 1UL */
#endif

#ifdef __cplusplus
}
#endif

#endif /* TRACER_EMB_CONF_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

