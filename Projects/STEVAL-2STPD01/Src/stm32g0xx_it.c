/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @version 1.0.1
  * @date    2021-11-08
  * @author  SRA.SLDU.SC Team
  * @brief   Interrupt Service Routines.
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

/** @addtogroup APPLICATION_IT_Handlers
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx.h"
#include "stm32g0xx_it.h"
/* Private includes ----------------------------------------------------------*/
#if defined(_TRACE) || defined(_GUI_INTERFACE)
#include "tracer_emb.h"
#include "usbpd_trace.h"
#ifdef _GUI_INTERFACE
#include "gui_api.h"
#endif /* _GUI_INTERFACE */
#endif

#include "usbpd_pwr_if.h"

#include "steval_2stpd01_usbpd_pwr.h"
#include "steval_2stpd01_stpd01_conf.h"
#include "steval_2stpd01_tcpp02_conf.h"


/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/** @addtogroup APPLICATION_IT_Handlers_External_Variables External Variables
  * @{
  */
/* External variables --------------------------------------------------------*/
extern I2C_HandleTypeDef    hbus_i2c;
/**
  * @} APPLICATION_IT_Handlers_External_Variables
  */

/** @addtogroup APPLICATION_IT_Handlers_Handlers
  * @{
  */
/* Exported function ---------------------------------------------------------*/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  USBPD_PWR_IF_AlarmType(USBPD_PWR_IF_NMI);
  while (1)
  {
  }
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  USBPD_PWR_IF_AlarmType(USBPD_PWR_IF_HARD_FAULT);
  while (1)
  {
  }
}

/**
  * @brief  This function handles SysTick Handler.
  */
void SysTick_Handler(void)
{
  /* Update DPM Timer */
  USBPD_DPM_TimerCounter();
  
#if defined(_GUI_INTERFACE)
  /* Update GUI Interface Counter */
  GUI_TimerCounter();
#endif /* _GUI_INTERFACE */
  
  HAL_IncTick();
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/
#if defined(_TRACE) || defined(_GUI_INTERFACE)
#if TRACER_EMB_DMA_MODE == 1UL
/**
  * @brief This function handles DMA1 channel 4, channel 5, channel 6, channel 7 and DMAMUX1 interrupts.
  */
void TRACER_EMB_TX_DMA_IRQHANDLER(void)
{
   TRACER_EMB_IRQHandlerDMA();
}
#endif
#endif /*_TRACE || _GUI_INTERFACE */

/**
  * @brief This function handles UCPD1 and UCPD2 interrupts / UCPD1 and UCPD2 wake-up interrupts through EXTI lines 32 and 33.
  */
#if defined (STM32G0C1xx)
void USB_UCPD1_2_IRQHandler()
#else
void UCPD1_2_IRQHandler(void)
#endif
{
  USBPD_PORT0_IRQHandler();

#if USBPD_PORT_COUNT == 2
  USBPD_PORT1_IRQHandler();
#endif
}

#if defined(_GUI_INTERFACE) || defined(_TRACE)
/**
  * @brief  This function handles USART 3 and 4 interrupts.
  */
void TRACER_EMB_USART_IRQHANDLER(void)
{
  TRACER_EMB_IRQHandlerUSART();
}
#endif /*_GUI_INTERFACE || _TRACE */

/**
  * @brief This function handles I2C1 event global interrupt
  */
void I2C1_IRQHandler(void)
{
  if (hbus_i2c.Instance->ISR & (I2C_FLAG_BERR | I2C_FLAG_ARLO | I2C_FLAG_OVR))
  {
    HAL_I2C_ER_IRQHandler(&hbus_i2c);
  }
  else
  {
    HAL_I2C_EV_IRQHandler(&hbus_i2c);
  }
}

/**
  * @brief This function handles EXTI line 0 and line 1 interrupts.
  */
void EXTI0_1_IRQHandler(void)
{
  /* Manage the Alert pin of both STPD01 (P0 and P1) */
  HAL_GPIO_EXTI_IRQHandler(BSP_STPD01_P0_ALERT_PIN); /* IT Handler for P0 STPD01 Alert (PC0) */
  HAL_GPIO_EXTI_IRQHandler(BSP_STPD01_P1_ALERT_PIN); /* IT Handler for P1 STPD01 Alert (PC1) */
}

/**
  * @brief This function handles EXTI line 2 and line 3 interrupts.
  */
void EXTI2_3_IRQHandler(void)
{
  /* Manage (if enabled) the Power Good pin of both STPD01 (P0 and P1) */
  HAL_GPIO_EXTI_IRQHandler(BSP_STPD01_P0_PGOOD_PIN); /* IT Handler for P0 PGood (PB2) */
  HAL_GPIO_EXTI_IRQHandler(BSP_STPD01_P1_PGOOD_PIN); /* IT Handler for P1 PGood (PC3) */
}

/**
  * @brief This function handles EXTI line 4 to 15 interrupts.
  */
void EXTI4_15_IRQHandler(void)
{
#if defined(NUCLEO_MB1360A)
  //HAL_GPIO_EXTI_IRQHandler(USER_BUTTON_PIN); /* IT Handler for Button (PC13) */
#endif /* NUCLEO_MB1360A */
  
  /* Manage (if enabled) the Flag pin of both TCPP02 (P0 and P1) */
  HAL_GPIO_EXTI_IRQHandler(BSP_TCPP02_P0_FLG_PIN); /* IT Handler for P0 TCPP02 Flag (PB13) */
  HAL_GPIO_EXTI_IRQHandler(BSP_TCPP02_P1_FLG_PIN); /* IT Handler for P1 TCPP02 Flag (PC8)  */
}

/**
  * @} APPLICATION_IT_Handlers_Handlers
  */

/**
  * @} APPLICATION_IT_Handlers
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
