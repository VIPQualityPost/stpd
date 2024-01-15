/**
  ******************************************************************************
  * @file    tcpp0203_reg.h
  * @author  MCD Application Team and SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   Header of tcpp0203_reg.c
  *
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
#ifndef TCPP0203_REG_H
#define TCPP0203_REG_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP_Components_TCPP0203
  * @{
  */

/** @defgroup BSP_Components_TCPP0203_RegisterLayer Register Layer
  * @{
  */
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @defgroup BSP_Components_TCPP0203_RegisterLayer_Exported_Defines Exported Defines
  * @{
  */
#define TCPP0203_WRITE_REG                   0x00U                 /*!< Write register       */
#define TCPP0203_PROG_CTRL                   TCPP0203_WRITE_REG    /*!< Programming register */
#define TCPP0203_READ_REG1                   0x01U                 /*!< Read 1 register      */
#define TCPP0203_ACK_REG                     TCPP0203_READ_REG1    /*!< ACK register         */
#define TCPP0203_READ_REG2                   0x02U                 /*!< Read 2 register      */
#define TCPP0203_FLAG_REG                    TCPP0203_READ_REG2    /*!< Flag register        */
/**
  * @} BSP_Components_TCPP0203_RegisterLayer_Exported_Defines
  */

/** @defgroup BSP_Components_TCPP0203_RegisterLayer_Exported_Types Types
  * @{
  */
/************** Generic Function  *******************/
/**
  * @brief  Write function type
  */
typedef int32_t (*TCPP0203_Write_Func)(void *, uint8_t, uint8_t *, uint8_t);
/**
  * @brief  Read function type
  */
typedef int32_t (*TCPP0203_Read_Func)(void *, uint8_t, uint8_t *, uint8_t);

/**
  * @brief  Context structure
  */
typedef struct
{
  TCPP0203_Write_Func   WriteReg;
  TCPP0203_Read_Func    ReadReg;
  void                  *handle;
} TCPP0203_ctx_t;
/**
  * @} BSP_Components_TCPP0203_RegisterLayer_Exported_Types
  */

/** @defgroup BSP_Components_TCPP0203_RegisterLayer_Exported_Functions Exported Functions
  * @{
  */
int32_t tcpp0203_write_reg(TCPP0203_ctx_t *ctx, uint8_t reg, uint8_t *data, uint8_t length);
int32_t tcpp0203_read_reg(TCPP0203_ctx_t *ctx, uint8_t reg, uint8_t *data, uint8_t length);
/**
  * @} BSP_Components_TCPP0203_RegisterLayer_Exported_Functions
  */

/**
  * @} BSP_Components_TCPP0203_RegisterLayer
  */

/**
  * @} BSP_Components_TCPP0203
  */

#ifdef __cplusplus
}
#endif

#endif /* TCPP0203_REG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

