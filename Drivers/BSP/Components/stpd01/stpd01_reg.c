/**
  ******************************************************************************
  * @file    stpd01_reg.c
  * @author  SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   This file provides unitary register function to control the STPD01
  *          Programmable Buck Converter for USBPD.
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
  
/** @addtogroup BSP_Components_STPD01_RegisterLayer
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "stpd01_reg.h"

/** @addtogroup BSP_Components_STPD01_RegisterLayer_Exported_Functions
  * @{
  */
/**
  * @brief  Read Register Wrapper
  * @param  ctx     Context
  * @param  reg     Register address
  * @param  data    Data buffer
  * @param  len     Length of buffer
  * @retval Status
  */
int32_t STPD01_ReadReg(STPD01_ctx_t *ctx, uint8_t reg, uint8_t * data, uint16_t len)
{
	return ctx->ReadReg(ctx->handle, reg, data, len);
}

/**
  * @brief  Write Register Wrapper
  * @param  ctx     Context
  * @param  reg     Register address
  * @param  data    Data buffer
  * @param  len     Length of buffer
  * @retval Status
  */
int32_t STPD01_WriteReg (STPD01_ctx_t *ctx, uint8_t reg, uint8_t * data, uint16_t len)
{
	return ctx->WriteReg(ctx->handle, reg, data, len);
}


/** @addtogroup BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg0
  * @{
  */
/**
  * @brief  Get the Vout register value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_Vout_Select_Get(STPD01_ctx_t *ctx, STPD01_VOUT_SELECT_t *val)
{
  int32_t _status;
  _status = STPD01_ReadReg(ctx, STPD01_VOUT_SELECT, (uint8_t *)val, 1);
  return _status;
}

/**
  * @brief  Set the Vout register value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_Vout_Select_Set(STPD01_ctx_t *ctx, STPD01_VOUT_SELECT_t *val)
{
	return STPD01_WriteReg(ctx, STPD01_VOUT_SELECT, (uint8_t *)val, 1);
}
/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg0
  */

/** @addtogroup BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg1
  * @{
  */
/**
  * @brief  Get the current limit register value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_Ilim_Select_Get(STPD01_ctx_t *ctx, STPD01_ILIM_SELECT_t *val)
{
	return STPD01_ReadReg(ctx, STPD01_ILIM_SELECT, (uint8_t *)val, 1);
}

/**
  * @brief  Set the current limit register value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_Ilim_Select_Set(STPD01_ctx_t *ctx, STPD01_ILIM_SELECT_t *val)
{
	return STPD01_WriteReg(ctx, STPD01_ILIM_SELECT, (uint8_t *)val, 1);
}
/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg1
  */

/** @addtogroup BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg2
  * @{
  */
/**
  * @brief  Get the interrupt status register value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_STATUS_Get(STPD01_ctx_t *ctx, STPD01_REGISTER_t *val)
{
	return STPD01_ReadReg(ctx, STPD01_INT_STATUS, (uint8_t *)val, 1);
}

/**
  * @brief  Get the OVP (Over-voltage protection) interrupt status value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_STATUS_OVP_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_STATUS_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_STATUS_REG.INT_STATUS_OVP));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the OCP (Over-current protection) interrupt status value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_STATUS_OCP_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_STATUS_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_STATUS_REG.INT_STATUS_OCP));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the short interrupt status value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_STATUS_SHORT_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_STATUS_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_STATUS_REG.INT_STATUS_SHORT));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the power good interrupt status value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_STATUS_PGOOD_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_STATUS_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_STATUS_REG.INT_STATUS_PGOOD));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the WD (Watch-dog) interrupt status value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_STATUS_WD_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_STATUS_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_STATUS_REG.INT_STATUS_WD));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the OTP (Over-temperature protection) interrupt status value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_STATUS_OTP_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_STATUS_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_STATUS_REG.INT_STATUS_OTP));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the OTW interrupt status value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_STATUS_OTW_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_STATUS_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_STATUS_REG.INT_STATUS_OTW));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the IPK interrupt status value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_STATUS_IPK_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_STATUS_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_STATUS_REG.INT_STATUS_IPK));
	return STPD01_STATUS_OK;
}
/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg2
  */

/** @addtogroup BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg3
  * @{
  */
/**
  * @brief  Get the interrupt latach register value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_LATCH_Get(STPD01_ctx_t *ctx, STPD01_REGISTER_t *val)
{
	return STPD01_ReadReg(ctx, STPD01_INT_LATCH, (uint8_t *)val, 1);
}

/**
  * @brief  Get the OVP (Over-voltage protection) interrupt latch value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_LATCH_OVP_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_LATCH_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_LATCH_REG.INT_LATCH_OVP));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the OCP (Over-current protection) interrupt status value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_LATCH_OCP_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_LATCH_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_LATCH_REG.INT_LATCH_OCP));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the short interrupt latch value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_LATCH_SHORT_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_LATCH_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_LATCH_REG.INT_LATCH_SHORT));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the power good interrupt latch value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_LATCH_PGOOD_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_LATCH_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_LATCH_REG.INT_LATCH_PGOOD));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the WD (Watch-dog) interrupt latch value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_LATCH_WD_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_LATCH_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_LATCH_REG.INT_LATCH_WD));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the OTP (Over-temperature protection) interrupt status value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_LATCH_OTP_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_LATCH_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_LATCH_REG.INT_LATCH_OTP));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the OTW interrupt latch value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_LATCH_OTW_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_LATCH_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_LATCH_REG.INT_LATCH_OTW));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the IPK interrupt latch value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_LATCH_IPK_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_LATCH_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_LATCH_REG.INT_LATCH_IPK));
	return STPD01_STATUS_OK;
}
/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg3
  */

/** @addtogroup BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg4
  * @{
  */
/**
  * @brief  Get the interrupt mask register value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_MASK_Get(STPD01_ctx_t *ctx, STPD01_REGISTER_t *val)
{
	return STPD01_ReadReg(ctx, STPD01_INT_MASK, (uint8_t *)val, 1);
}

/**
  * @brief  Set the interrupt mask register value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_MASK_Set(STPD01_ctx_t *ctx, STPD01_REGISTER_t *val)
{
	return STPD01_WriteReg(ctx, STPD01_INT_MASK, (uint8_t *)val, 1);
}

/**
  * @brief  Get the OVP (Over-voltage protection) interrupt mask value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_MASK_OVP_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_MASK_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_MASK_REG.INT_MASK_OVP));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the OCP (Over-current protection) interrupt mask value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_MASK_OCP_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_MASK_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_MASK_REG.INT_MASK_OCP));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the short interrupt mask value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_MASK_SHORT_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_MASK_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_MASK_REG.INT_MASK_SHORT));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the power good interrupt mask value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_MASK_PGOOD_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_MASK_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_MASK_REG.INT_MASK_PGOOD));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the WD (Watch-dog) interrupt mask value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_MASK_WD_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_MASK_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_MASK_REG.INT_MASK_WD));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the OTP (Over-temperature protection) interrupt mask value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_MASK_OTP_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_MASK_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_MASK_REG.INT_MASK_OTP));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the OTW interrupt mask value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_MASK_OTW_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_MASK_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_MASK_REG.INT_MASK_OTW));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Get the IPK interrupt mask value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_MASK_IPK_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_MASK_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_INT_MASK_REG.INT_MASK_IPK));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Set the OVP (Over-voltage protection) interrupt mask value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_MASK_OVP_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_MASK_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	tempreg.STPD01_INT_MASK_REG.INT_MASK_OVP = *val;

	return STPD01_INT_MASK_Set(ctx,&tempreg);
}

/**
  * @brief  Set the OCP (Over-current protection) interrupt mask value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_MASK_OCP_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_MASK_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	tempreg.STPD01_INT_MASK_REG.INT_MASK_OCP = *val;

	return STPD01_INT_MASK_Set(ctx,&tempreg);
}

/**
  * @brief  Set the short interrupt mask value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_MASK_SHORT_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_MASK_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	tempreg.STPD01_INT_MASK_REG.INT_MASK_SHORT = *val;

	return STPD01_INT_MASK_Set(ctx,&tempreg);
}

/**
  * @brief  Set the power good interrupt mask value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_MASK_PGOOD_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_MASK_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	tempreg.STPD01_INT_MASK_REG.INT_MASK_PGOOD = *val;

	return STPD01_INT_MASK_Set(ctx,&tempreg);
}

/**
  * @brief  Set the WD interrupt mask value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_MASK_WD_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_MASK_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	tempreg.STPD01_INT_MASK_REG.INT_MASK_WD = *val;

	return STPD01_INT_MASK_Set(ctx,&tempreg);
}

/**
  * @brief  Set the OTP (Over-temperature protection) interrupt mask value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_MASK_OTP_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_MASK_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	tempreg.STPD01_INT_MASK_REG.INT_MASK_OTP = *val;

	return STPD01_INT_MASK_Set(ctx,&tempreg);
}

/**
  * @brief  Set the OTW interrupt mask value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_MASK_OTW_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_MASK_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	tempreg.STPD01_INT_MASK_REG.INT_MASK_OTW = *val;

	return STPD01_INT_MASK_Set(ctx,&tempreg);
}

/**
  * @brief  Set the IPK interrupt mask value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_INT_MASK_IPK_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if (STPD01_STATUS_OK != STPD01_INT_MASK_Get(ctx,&tempreg))
		return STPD01_ERROR_GENERIC;
	tempreg.STPD01_INT_MASK_REG.INT_MASK_IPK = *val;

	return STPD01_INT_MASK_Set(ctx,&tempreg);
}
/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg4
  */

/** @addtogroup BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg5
  * @{
  */
/**
  * @brief  Get auto discharge value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_Service_Auto_Dis_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if(STPD01_STATUS_OK != STPD01_ReadReg(ctx, STPD01_SERVICE, (uint8_t *)&tempreg, 1))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_SERVICE_REG.SERVICE_AUTO_DIS));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Set auto discharge value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_Service_Auto_Dis_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if(STPD01_STATUS_OK != STPD01_ReadReg(ctx, STPD01_SERVICE, (uint8_t *)&tempreg, 1))
		return STPD01_ERROR_GENERIC;
	tempreg.STPD01_SERVICE_REG.SERVICE_AUTO_DIS = *val;

	return STPD01_WriteReg(ctx, STPD01_SERVICE, (uint8_t *)&tempreg, 1);
}

/**
  * @brief  Get dith value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_Service_Dith_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if(STPD01_STATUS_OK != STPD01_ReadReg(ctx, STPD01_SERVICE, (uint8_t *)&tempreg, 1))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_SERVICE_REG.SERVICE_DITH));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Set dith value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_Service_Dith_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if(STPD01_STATUS_OK != STPD01_ReadReg(ctx, STPD01_SERVICE, (uint8_t *)&tempreg, 1))
		return STPD01_ERROR_GENERIC;
	tempreg.STPD01_SERVICE_REG.SERVICE_DITH = *val;

	return STPD01_WriteReg(ctx, STPD01_SERVICE, (uint8_t *)&tempreg, 1);
}

/**
  * @brief  Get sw freq value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_Service_SW_Frq_Get(STPD01_ctx_t *ctx, STPD01_SERVICE_SW_FRQ_t *val)
{
	STPD01_REGISTER_t tempreg;
	if(STPD01_STATUS_OK != STPD01_ReadReg(ctx, STPD01_SERVICE, (uint8_t *)&tempreg, 1))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_SERVICE_SW_FRQ_t) (tempreg.STPD01_SERVICE_REG.SERVICE_SW_FRQ));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Set sw freq value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_Service_SW_Frq_Set(STPD01_ctx_t *ctx, STPD01_SERVICE_SW_FRQ_t *val)
{
	STPD01_REGISTER_t tempreg;
	if(STPD01_STATUS_OK != STPD01_ReadReg(ctx, STPD01_SERVICE, (uint8_t *)&tempreg, 1))
		return STPD01_ERROR_GENERIC;
	tempreg.STPD01_SERVICE_REG.SERVICE_SW_FRQ = *val;

	return STPD01_WriteReg(ctx, STPD01_SERVICE, (uint8_t *)&tempreg, 1);
}

/**
  * @brief  Get CDC value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_Service_CDC_Get(STPD01_ctx_t *ctx, STPD01_SERVICE_CDC_t *val)
{
	STPD01_REGISTER_t tempreg;
	if(STPD01_STATUS_OK != STPD01_ReadReg(ctx, STPD01_SERVICE, (uint8_t *)&tempreg, 1))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_SERVICE_CDC_t) (tempreg.STPD01_SERVICE_REG.SERVICE_CDC));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Set CDC value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_Service_CDC_Set(STPD01_ctx_t *ctx, STPD01_SERVICE_CDC_t *val)
{
	STPD01_REGISTER_t tempreg;
	if(STPD01_STATUS_OK != STPD01_ReadReg(ctx, STPD01_SERVICE, (uint8_t *)&tempreg, 1))
		return STPD01_ERROR_GENERIC;
	tempreg.STPD01_SERVICE_REG.SERVICE_CDC = *val;

	return STPD01_WriteReg(ctx, STPD01_SERVICE, (uint8_t *)&tempreg, 1);
}

/**
  * @brief  Get watch-dog tim value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_Service_Wtchg_Tim_Get(STPD01_ctx_t *ctx, STPD01_SERVICE_WTCHDG_TIM_t *val)
{
	STPD01_REGISTER_t tempreg;
	if(STPD01_STATUS_OK != STPD01_ReadReg(ctx, STPD01_SERVICE, (uint8_t *)&tempreg, 1))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_SERVICE_WTCHDG_TIM_t) (tempreg.STPD01_SERVICE_REG.SERVICE_WTCHDG_TIM));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Set watch-dog tim value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_Service_Wtchg_Tim_Set(STPD01_ctx_t *ctx, STPD01_SERVICE_WTCHDG_TIM_t *val)
{
	STPD01_REGISTER_t tempreg;
	if(STPD01_STATUS_OK != STPD01_ReadReg(ctx, STPD01_SERVICE, (uint8_t *)&tempreg, 1))
		return STPD01_ERROR_GENERIC;
	tempreg.STPD01_SERVICE_REG.SERVICE_WTCHDG_TIM = *val;

	return STPD01_WriteReg(ctx, STPD01_SERVICE, (uint8_t *)&tempreg, 1);
}
/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg5
  */

/** @addtogroup BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg6
  * @{
  */
/**
  * @brief  Get enable dig value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_Enable_Dig_En_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if(STPD01_STATUS_OK != STPD01_ReadReg(ctx, STPD01_DIG_ENABLE, (uint8_t *)&tempreg, 1))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_DIG_ENABLE_REG.ENABLE_DIG_EN));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Set enable dig value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_Enable_Dig_En_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if(STPD01_STATUS_OK != STPD01_ReadReg(ctx, STPD01_DIG_ENABLE, (uint8_t *)&tempreg, 1))
		return STPD01_ERROR_GENERIC;
	tempreg.STPD01_DIG_ENABLE_REG.ENABLE_DIG_EN = *val;

	return STPD01_WriteReg(ctx, STPD01_DIG_ENABLE, (uint8_t *)&tempreg, 1);
}

/**
  * @brief  Get WD value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_Enable_WD_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if(STPD01_STATUS_OK != STPD01_ReadReg(ctx, STPD01_DIG_ENABLE, (uint8_t *)&tempreg, 1))
		return STPD01_ERROR_GENERIC;
	*val = ((STPD01_FUNCTIONALSTATE_t) (tempreg.STPD01_DIG_ENABLE_REG.ENABLE_WD));
	return STPD01_STATUS_OK;
}

/**
  * @brief  Set WD value
  * @param  ctx     Context
  * @param  val     pointer to the value
  * @retval Status
  */
int32_t STPD01_Enable_WD_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val)
{
	STPD01_REGISTER_t tempreg;
	if(STPD01_STATUS_OK != STPD01_ReadReg(ctx, STPD01_DIG_ENABLE, (uint8_t *)&tempreg, 1))
		return STPD01_ERROR_GENERIC;
	tempreg.STPD01_DIG_ENABLE_REG.ENABLE_WD = *val;

	return STPD01_WriteReg(ctx, STPD01_DIG_ENABLE, (uint8_t *)&tempreg, 1);
}
/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg6
  */

/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Functions
  */

/**
  * @} BSP_Components_STPD01_RegisterLayer
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
