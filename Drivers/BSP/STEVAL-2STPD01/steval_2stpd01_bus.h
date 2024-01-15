/**
  ******************************************************************************
  * @file    steval_2stpd01_bus.h
  * @author  MCD Application Team and SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   This file contains definitions for BUS hardware resources.
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
#ifndef STEVAL_2STPD01_BUS_H
#define STEVAL_2STPD01_BUS_H

#ifdef __cplusplus
 extern "C" {
#endif


 /* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_nucleo.h"
#include "steval_2stpd01_errno.h"
#include "steval_2stpd01_conf.h"

/** @addtogroup BSP_STEVAL_2STPD01
  * @{
  */ 

/** @defgroup BSP_STEVAL_2STPD01_BUS Bus
  * @brief      This file includes the BUS and Low-level driver.
  * @{
  */ 

/** @defgroup BSP_STEVAL_2STPD01_BUS_Private_Types Private Types
  * @{
  */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
typedef struct
{
  pI2C_CallbackTypeDef  pMspInitCb;
  pI2C_CallbackTypeDef  pMspDeInitCb;
}BSP_I2C_Cb_t;
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 1) */

/**
  * @}
  */

/** @addtogroup BSP_STEVAL_2STPD01_BUS_Exported_Variables
  * @{
  */
extern I2C_HandleTypeDef hbus_i2c;
/**
  * @} BSP_STEVAL_2STPD01_BUS_Exported_Variables
  */

/** @defgroup BSP_STEVAL_2STPD01_BUS_Exported_Functions Exported Functions
  * @{
  */
int32_t BSP_BUS_I2C_Init(void);
int32_t BSP_BUS_I2C_DeInit(void);
int32_t BSP_BUS_I2C_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_BUS_I2C_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_BUS_I2C_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_BUS_I2C_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int32_t BSP_BUS_I2C_IsReady(uint16_t DevAddr, uint32_t Trials);
int32_t BSP_BUS_GetTick(void);
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
int32_t BSP_BUS_I2C_RegisterDefaultMspCallbacks (void);
int32_t BSP_BUS_I2C_RegisterMspCallbacks (BSP_I2C_Cb_t *Callback);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */
HAL_StatusTypeDef MX_I2C_Init(I2C_HandleTypeDef *phi2c, uint32_t timing);

/**
  * @} BSP_STEVAL_2STPD01_BUS_Exported_Functions
  */

/**
  * @} BSP_STEVAL_2STPD01_BUS
  */

/**
  * @} BSP_STEVAL_2STPD01
  */

#ifdef __cplusplus
}
#endif

#endif /* STEVAL_2STPD01_BUS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
