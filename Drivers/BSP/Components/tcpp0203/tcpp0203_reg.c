/**
  ******************************************************************************
  * @file    tcpp0203_reg.c
  * @author  MCD Application Team and SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   This file provides unitary register function to control the TCPP02-03
  *          Type-C port protection driver.
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

/** @addtogroup BSP_Components_TCPP0203_RegisterLayer
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "tcpp0203_reg.h"

/** @addtogroup BSP_Components_TCPP0203_RegisterLayer_Exported_Functions
  * @{
  */
/**
  * @brief  Read Register Wrapper
  * @param  ctx     Context
  * @param  reg     Register address
  * @param  data    Data buffer
  * @param  length  Length of buffer
  * @retval Status
  */
int32_t tcpp0203_read_reg(TCPP0203_ctx_t *ctx, uint8_t reg, uint8_t *data, uint8_t length)
{
  return ctx->ReadReg(ctx->handle, reg, data, length);
}

/**
  * @brief  Write Register Wrapper
  * @param  ctx     Context
  * @param  reg     Register address
  * @param  data    Data buffer
  * @param  length  Length of buffer
  * @retval Status
  */
int32_t tcpp0203_write_reg(TCPP0203_ctx_t *ctx, uint8_t reg, uint8_t *data, uint8_t length)
{
  return ctx->WriteReg(ctx->handle, reg, data, length);
}
/**
  * @} BSP_Components_TCPP0203_RegisterLayer_Exported_Functions
  */

/**
  * @} BSP_Components_TCPP0203_RegisterLayer
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


