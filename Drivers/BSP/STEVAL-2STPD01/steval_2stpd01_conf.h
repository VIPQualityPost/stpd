/**
  ******************************************************************************
  * @file    steval_2stpd01_conf.h
  * @author  MCD Application Team and SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   STEVAL-2STPD01 board configuration file.
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
#ifndef STEVAL_2STPD01_CONF_H__
#define STEVAL_2STPD01_CONF_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP_STEVAL_2STPD01
  * @{
  */

/** @defgroup BSP_STEVAL_2STPD01_CONFIG Configurations
  * @{
  */

/* Includes ------------------------------------------------------------------*/
/* Replace the header file names with the ones of the target platform */
#include "stm32g0xx_nucleo.h"
#include "stm32g0xx_ll_adc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_exti.h"

/** @defgroup BSP_STEVAL_2STPD01_CONFIG_Exported_Defines Exported Defines
  * @{
  */
#define BSP_GPIOS_CLK_ENABLE() {  \
                                      __HAL_RCC_GPIOA_CLK_ENABLE(); \
                                      __HAL_RCC_GPIOB_CLK_ENABLE(); \
                                      __HAL_RCC_GPIOC_CLK_ENABLE(); \
                                      __HAL_RCC_GPIOD_CLK_ENABLE(); \
                                      __HAL_RCC_GPIOF_CLK_ENABLE(); \
                                   }

/** @defgroup BSP_STEVAL_2STPD01_CONFIG_Exported_Defines_I2C I2C
  * @{
  */
/* I2C BUS definitions */
#define BUS_I2C_INSTANCE                 I2C1                                   /*!< I2C Instance                       */
#define BUS_I2C_FREQUENCY                100000U                                /*!< I2C Frequency = 100 kHz            */
#define BUS_I2C_CLK_ENABLE()             __HAL_RCC_I2C1_CLK_ENABLE()            /*!< I2C Peripheral Clock enable        */
#define BUS_I2C_CLK_DISABLE()            __HAL_RCC_I2C1_CLK_DISABLE()           /*!< I2C Peripheral Clock disable       */
#define BUS_I2C_FORCE_RESET()            __HAL_RCC_I2C1_FORCE_RESET()           /*!< I2C Peripheral Reset               */
#define BUS_I2C_RELEASE_RESET()          __HAL_RCC_I2C1_RELEASE_RESET()         /*!< I2C Peripheral Release             */

#define BUS_I2C_SCL_GPIO_PORT            GPIOA                                  /*!< I2C SCL GPIO Port                  */
#define BUS_I2C_SCL_GPIO_PIN             GPIO_PIN_9                             /*!< I2C SCL GPIO Pin                   */
#define BUS_I2C_SCL_GPIO_AF              GPIO_AF6_I2C1                          /*!< I2C SCL GPIO Alternate Function    */
#define BUS_I2C_SCL_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()           /*!< I2C SCL GPIO Clock enable          */
#define BUS_I2C_SCL_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOB_CLK_DISABLE()          /*!< I2C SCL GPIO Clock disable         */
  
#define BUS_I2C_SDA_GPIO_PORT            GPIOA                                  /*!< I2C SDA GPIO Port                  */
#define BUS_I2C_SDA_GPIO_PIN             GPIO_PIN_10                            /*!< I2C SDA GPIO Pin                   */
#define BUS_I2C_SDA_GPIO_AF              GPIO_AF6_I2C1                          /*!< I2C SDA GPIO Alternate Function    */
#define BUS_I2C_SDA_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()           /*!< I2C SDA GPIO Clock enable          */
#define BUS_I2C_SDA_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOB_CLK_DISABLE()          /*!< I2C SDA GPIO Clock disable         */

#define BUS_I2C_POLL_TIMEOUT             0x1000U                                /*!< I2C Poll timeout                   */
/**
  * @} BSP_STEVAL_2STPD01_CONFIG_Exported_Defines_I2C
  */

/** @defgroup BSP_STEVAL_2STPD01_CONFIG_Exported_Defines_ADC ADC
  * @{
  */
#define BSP_ADC_INSTANCE                     ADC1                               /*!< ADC Instance                       */
#define BSP_ADC_CH_NUM                       7                                  /*!< ADC Channel number                 */
#define BSP_ADC_CLK_ENABLE()                 __HAL_RCC_ADC_CLK_ENABLE()         /*!< ADC Peripheral Clock enable        */
#define BSP_ADC_CLK_DISABLE()                __HAL_RCC_ADC_CLK_DISABLE()        /*!< ADC Peripheral Clock disable       */

/**
  * @brief  ADC GPIOs Clock enable
  */
#define BSP_ADC_GPIOS_CLK_ENABLE() {  \
                                   }
                                    /* already enabled in the generic gpio clock */
                                      
/* DMA ADC */
#define BSP_ADC_DMA_INSTANCE                 DMA1_Channel1                      /*!< DMA ADC Instance                   */
#define BSP_ADC_DMA_REQUEST                  DMA_REQUEST_ADC1                   /*!< DMA ADC Request                    */
#define BSP_ADC_DMA_CLK_ENABLE()             __HAL_RCC_DMA1_CLK_ENABLE()        /*!< DMA ADC Peripheral Clock enable    */
#define BSP_ADC_DMA_CLK_DISABLE()            __HAL_RCC_DMA1_CLK_DISABLE()       /*!< DMA ADC Peripheral Clock disable   */

#define BSP_ADC_CONV_VIN                     6447U                              /*!< ADC Vin Power Supply Convertion parameter (/1000)          */
#define BSP_ADC_CONV_P0_VBUS                 5389U                              /*!< ADC Port 0 VBus Convertion parameter (/1000)               */
#define BSP_ADC_CONV_P0_IBUS_TCPP02          1119U                              /*!< ADC Port 0 IBus from TCPP02 Convertion parameter (/1000)   */
#define BSP_ADC_CONV_P0_IBUS_STPD01           895U                              /*!< ADC Port 0 IBus from STPD01 Convertion parameter (/1000)   */
#define BSP_ADC_CONV_P1_VBUS                 5389U                              /*!< ADC Port 1 VBus Convertion parameter (/1000)               */
#define BSP_ADC_CONV_P1_IBUS_TCPP02          1119U                              /*!< ADC Port 1 IBus from TCPP02 Convertion parameter (/1000)   */
#define BSP_ADC_CONV_P1_IBUS_STPD01           895U                              /*!< ADC Port 1 IBus from STPD01 Convertion parameter (/1000)   */

#define BSP_ADC_GPIO_VIN_PORT                GPIOB                              /*!< ADC GPIO Vin Port                               */
#define BSP_ADC_GPIO_VIN_PIN                 GPIO_PIN_0                         /*!< ADC GPIO Vin Pin                                */
#define BSP_ADC_GPIO_VIN_MODE                GPIO_MODE_INPUT                    /*!< ADC GPIO Vin Mode                               */
#define BSP_ADC_GPIO_VIN_PUPD                GPIO_NOPULL                        /*!< ADC GPIO Vin Pull-up/down                       */
#define BSP_ADC_GPIO_VIN_CH                  ADC_CHANNEL_8                      /*!< ADC GPIO Vin Channel                            */
#define BSP_ADC_GPIO_VIN_RANK                ADC_REGULAR_RANK_1                 /*!< ADC GPIO Vin Rank                               */

#define BSP_ADC_GPIO_P0_VBUS_PORT            GPIOA                              /*!< ADC GPIO Port 0 VBus Port                       */
#define BSP_ADC_GPIO_P0_VBUS_PIN             GPIO_PIN_0                         /*!< ADC GPIO Port 0 VBus Pin                        */
#define BSP_ADC_GPIO_P0_VBUS_MODE            GPIO_MODE_INPUT                    /*!< ADC GPIO Port 0 VBus Mode                       */
#define BSP_ADC_GPIO_P0_VBUS_PUPD            GPIO_NOPULL                        /*!< ADC GPIO Port 0 VBus Pull-up/down               */
#define BSP_ADC_GPIO_P0_VBUS_CH              ADC_CHANNEL_0                      /*!< ADC GPIO Port 0 VBus Channel                    */
#define BSP_ADC_GPIO_P0_VBUS_RANK            ADC_REGULAR_RANK_2                 /*!< ADC GPIO Port 0 VBus Rank                       */

#define BSP_ADC_GPIO_P0_IBUS_TCPP02_PORT     GPIOA                              /*!< ADC GPIO Port 0 IBus from TCPP02 Port           */
#define BSP_ADC_GPIO_P0_IBUS_TCPP02_PIN      GPIO_PIN_1                         /*!< ADC GPIO Port 0 IBus from TCPP02 Pin            */
#define BSP_ADC_GPIO_P0_IBUS_TCPP02_MODE     GPIO_MODE_INPUT                    /*!< ADC GPIO Port 0 IBus from TCPP02 Mode           */
#define BSP_ADC_GPIO_P0_IBUS_TCPP02_PUPD     GPIO_NOPULL                        /*!< ADC GPIO Port 0 IBus from TCPP02 Pull-up/down   */
#define BSP_ADC_GPIO_P0_IBUS_TCPP02_CH       ADC_CHANNEL_1                      /*!< ADC GPIO Port 0 IBus from TCPP02 Channel        */
#define BSP_ADC_GPIO_P0_IBUS_TCPP02_RANK     ADC_REGULAR_RANK_3                 /*!< ADC GPIO Port 0 IBus from TCPP02 Rank           */
                                                                                              
#define BSP_ADC_GPIO_P0_IBUS_STPD01_PORT     GPIOA                              /*!< ADC GPIO Port 0 IBus from STPD01 Port           */
#define BSP_ADC_GPIO_P0_IBUS_STPD01_PIN      GPIO_PIN_6                         /*!< ADC GPIO Port 0 IBus from STPD01 Pin            */
#define BSP_ADC_GPIO_P0_IBUS_STPD01_MODE     GPIO_MODE_INPUT                    /*!< ADC GPIO Port 0 IBus from STPD01 Mode           */
#define BSP_ADC_GPIO_P0_IBUS_STPD01_PUPD     GPIO_NOPULL                        /*!< ADC GPIO Port 0 IBus from STPD01 Pull-up/down   */
#define BSP_ADC_GPIO_P0_IBUS_STPD01_CH       ADC_CHANNEL_6                      /*!< ADC GPIO Port 0 IBus from STPD01 Channel        */
#define BSP_ADC_GPIO_P0_IBUS_STPD01_RANK     ADC_REGULAR_RANK_4                 /*!< ADC GPIO Port 0 IBus from STPD01 Rank           */
                                                                                
#define BSP_ADC_GPIO_P1_VBUS_PORT            GPIOA                              /*!< ADC GPIO Port 1 VBus Port                       */
#define BSP_ADC_GPIO_P1_VBUS_PIN             GPIO_PIN_4                         /*!< ADC GPIO Port 1 VBus Pin                        */
#define BSP_ADC_GPIO_P1_VBUS_MODE            GPIO_MODE_INPUT                    /*!< ADC GPIO Port 1 VBus Mode                       */
#define BSP_ADC_GPIO_P1_VBUS_PUPD            GPIO_NOPULL                        /*!< ADC GPIO Port 1 VBus Pull-up/down               */
#define BSP_ADC_GPIO_P1_VBUS_CH              ADC_CHANNEL_4                      /*!< ADC GPIO Port 1 VBus Channel                    */
#define BSP_ADC_GPIO_P1_VBUS_RANK            ADC_REGULAR_RANK_5                 /*!< ADC GPIO Port 1 VBus Rank                       */
                                                                                
#define BSP_ADC_GPIO_P1_IBUS_TCPP02_PORT     GPIOB                              /*!< ADC GPIO Port 1 IBus from TCPP02 Port           */
#define BSP_ADC_GPIO_P1_IBUS_TCPP02_PIN      GPIO_PIN_1                         /*!< ADC GPIO Port 1 IBus from TCPP02 Pin            */
#define BSP_ADC_GPIO_P1_IBUS_TCPP02_MODE     GPIO_MODE_INPUT                    /*!< ADC GPIO Port 1 IBus from TCPP02 Mode           */
#define BSP_ADC_GPIO_P1_IBUS_TCPP02_PUPD     GPIO_NOPULL                        /*!< ADC GPIO Port 0 IBus from STPD01 Pull-up/down   */
#define BSP_ADC_GPIO_P1_IBUS_TCPP02_CH       ADC_CHANNEL_9                      /*!< ADC GPIO Port 1 IBus from TCPP02 Channel        */
#define BSP_ADC_GPIO_P1_IBUS_TCPP02_RANK     ADC_REGULAR_RANK_6                 /*!< ADC GPIO Port 1 IBus from TCPP02 Rank           */
                                                                                
#define BSP_ADC_GPIO_P1_IBUS_STPD01_PORT     GPIOA                              /*!< ADC GPIO Port 1 IBus from STPD01 Port           */
#define BSP_ADC_GPIO_P1_IBUS_STPD01_PIN      GPIO_PIN_7                         /*!< ADC GPIO Port 1 IBus from STPD01 Pin            */
#define BSP_ADC_GPIO_P1_IBUS_STPD01_MODE     GPIO_MODE_INPUT                    /*!< ADC GPIO Port 1 IBus from STPD01 Mode           */
#define BSP_ADC_GPIO_P1_IBUS_STPD01_PUPD     GPIO_NOPULL                        /*!< ADC GPIO Port 0 IBus from STPD01 Pull-up/down   */
#define BSP_ADC_GPIO_P1_IBUS_STPD01_CH       ADC_CHANNEL_7                      /*!< ADC GPIO Port 1 IBus from STPD01 Channel        */
#define BSP_ADC_GPIO_P1_IBUS_STPD01_RANK     ADC_REGULAR_RANK_7                 /*!< ADC GPIO Port 1 IBus from STPD01 Rank           */
/**
  * @} BSP_STEVAL_2STPD01_CONFIG_Exported_Defines_ADC
  */

/**
  * @} BSP_STEVAL_2STPD01_CONFIG_Exported_Defines
  */

/**
  * @} BSP_STEVAL_2STPD01_CONFIG
  */

/**
  * @} BSP_STEVAL_2STPD01
  */

#ifdef __cplusplus
}
#endif

#endif /* STEVAL_2STPD01_CONF_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
