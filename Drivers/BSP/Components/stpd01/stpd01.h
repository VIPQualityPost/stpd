/**
  ******************************************************************************
  * @file    stpd01.h
  * @author  SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   This file contains all the functions prototypes for the
  *          stpd01.c driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#ifndef STPD01_H
#define STPD01_H

/** @addtogroup BSP_Components
  * @{
  */

/** @defgroup BSP_Components_STPD01 STPD01
  * @brief     This file provides a set of functions needed to drive the
  *            STPD01 USBPD Programmable Buck Converter.
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "stpd01_reg.h"

/* Exported types ------------------------------------------------------------*/
/** @defgroup BSP_Components_STPD01_Exported_Types Exported Types
  * @{
  */
typedef int32_t (*STPD01_Init_Func)(void);                                        /*!< Init type function                 */
typedef int32_t (*STPD01_DeInit_Func)(void);                                      /*!< DeInit type function               */
typedef int32_t (*STPD01_GetTick_Func)(void);                                     /*!< GetTick type function              */
typedef int32_t (*STPD01_Delay_Func)(uint32_t);                                   /*!< Delay type function                */
typedef int32_t (*STPD01_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t); /*!< Write Register type function       */
typedef int32_t (*STPD01_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);  /*!< Read Register type function        */

/**
  * @brief  Register IO Bus to component
  */
typedef struct
{
  STPD01_Init_Func          Init;
  STPD01_DeInit_Func        DeInit;
  uint16_t                  Address;
  STPD01_WriteReg_Func      WriteReg;
  STPD01_ReadReg_Func       ReadReg;
  STPD01_GetTick_Func       GetTick;
  STPD01_Delay_Func         Delay;
} STPD01_IO_t;

/**
  * @brief  Component object
  */
typedef struct
{
  STPD01_IO_t         IO;
  STPD01_ctx_t        Ctx;
  uint8_t             IsInitialized;
  uint8_t             PortNum;
} STPD01_Object_t;

/**
  * @brief  Driver functions
  */
typedef struct
{
  int32_t (*Init)(STPD01_Object_t *);
  int32_t (*DeInit)(STPD01_Object_t *);
  int32_t (*Reset)(STPD01_Object_t *);

  int32_t (*SetVoltage)(STPD01_Object_t *, int32_t);
  int32_t (*GetVoltage)(STPD01_Object_t *);
} STPD01_Drv_t;

/**
  * @} BSP_Components_STPD01_Exported_Types
  */

/** @defgroup BSP_Components_STPD01_Exported_Defines Exported Defines
  * @{
  */
/* I2C Address definition */
#define STPD01_I2C_ADDRESS_VCC          (0x08U) /*!< I2C address 0x04 (ADDR0) */
#define STPD01_I2C_ADDRESS_GND          (0x0AU) /*!< I2C address 0x05 (ADDR1) */

#define STPD01_OK                       (0)     /*!< Driver Response codes OK    */
#define STPD01_ERROR                    (-1)    /*!< Driver Response codes ERROR */
/**
  * @} BSP_Components_STPD01_Exported_Defines
  */

/** @defgroup BSP_Components_STPD01_Exported_Functions Exported Functions
  * @{
  */
int32_t STPD01_RegisterBusIO(STPD01_Object_t *pObj, STPD01_IO_t *pIO);
/**
  * @} BSP_Components_STPD01_Exported_Functions
  */

/** @defgroup BSP_Components_STPD01_Exported_Variables Exported Variables
  * @{
  */
/**
  * @brief  USBPD Programmable Buck Converter driver structure
  */
extern STPD01_Drv_t STPD01_Driver;
/**
  * @} BSP_Components_STPD01_Exported_Variables
  */

/**
  * @} BSP_Components_STPD01
  */

/**
  * @} BSP_Components
  */

#endif /* STPD01_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
