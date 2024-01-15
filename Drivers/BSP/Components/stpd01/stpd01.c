/**
  ******************************************************************************
  * @file    stpd01.c
  * @author  SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   This file provides the driver for STPD01 Programmable Buck 
  *          Converter for USBPD
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

/** @addtogroup BSP_Components_STPD01
  * @{
  */
    
/* Includes ------------------------------------------------------------------*/
#include "stpd01.h"

#if  defined(_TRACE)
#include "usbpd_core.h"
#include "usbpd_trace.h"
#include "string.h"
#include "stdio.h"
#endif /* _TRACE */

/** @defgroup BSP_Components_STPD01_Private_Defines Private Defines
  * @{
  */
/** @defgroup BSP_Components_STPD01_Private_Defines_20MV 20mV step configuration
  * @{
  */
#define VOLTAGE_L0_MIN_VALUE  3000      /* mV */
#define VOLTAGE_L0_MAX_VALUE  5900      /* mV */
#define VOLTAGE_L0_STEP       20        /* mV */
#define VOLTAGE_L0_OFFSET     VOUT_SELECT_3000_MV
/**
  * @} BSP_Components_STPD01_Private_Defines_20MV
  */

/** @defgroup BSP_Components_STPD01_Private_Defines_100MV 100mV step configuration
  * @{
  */
#define VOLTAGE_L1_MIN_VALUE  VOLTAGE_L0_MAX_VALUE
#define VOLTAGE_L1_MAX_VALUE  11000     /* mV */
#define VOLTAGE_L1_STEP       100       /* mV */
#define VOLTAGE_L1_OFFSET     VOUT_SELECT_5900_MV
/**
  * @} BSP_Components_STPD01_Private_Defines_100MV
  */

/** @defgroup BSP_Components_STPD01_Private_Defines_200MV 200mV step configuration
  * @{
  */
#define VOLTAGE_L2_MIN_VALUE  VOLTAGE_L1_MAX_VALUE
#define VOLTAGE_L2_MAX_VALUE  20000     /* mV */
#define VOLTAGE_L2_STEP       200       /* mV */
#define VOLTAGE_L2_OFFSET     VOUT_SELECT_11000_MV
/**
  * @} BSP_Components_STPD01_Private_Defines_200MV
  */
/**
  * @} BSP_Components_STPD01_Private_Defines
  */
 
/** @defgroup BSP_Components_STPD01_Private_Macros Private Macros
  * @{
  */
/**
  * @brief  Check if the voltage is in the range (min, max)
  */
#define VOLTAGE_CHECK(Value, Min, Max) ((Value) >= (Min) && (Value) < ((Max) + 1)) 
/**
  * @brief  Convert a voltage value in the corresponding register value
  */    
#define VOLTAGE2REG(Value, Min, Step, Offset)  (STPD01_VOUT_SELECT_t)((((Value) - (Min))/(Step)) + Offset)
/**
  * @} BSP_Components_STPD01_Private_Macros
  */
/* Private function prototypes -----------------------------------------------*/
/** @defgroup BSP_Components_STPD01_Private_Functions Private Functions
  * @{
  */
static int32_t STPD01_Init(STPD01_Object_t *pObj);
static int32_t STPD01_DeInit(STPD01_Object_t *pObj);
static int32_t STPD01_Reset(STPD01_Object_t *pObj);
static int32_t STPD01_SetVoltage(STPD01_Object_t *pObj, int32_t Voltage);

static int32_t STPD01_ReadRegWrap(void *handle, uint16_t Reg, uint8_t *Data, uint16_t Length);
static int32_t STPD01_WriteRegWrap(void *handle, uint16_t Reg, uint8_t *Data, uint16_t Length);
/**
  * @} BSP_Components_STPD01_Private_Functions
  */

/** @defgroup BSP_Components_STPD01_Private_Variables Private Variables
  * @{
  */
/**
  * @brief  USBPD Programmable Buck Converter driver structure initialization
  */
STPD01_Drv_t STPD01_Driver =
{
  .Init         = STPD01_Init,
  .DeInit       = STPD01_DeInit,
  .Reset        = STPD01_Reset,
  .SetVoltage   = STPD01_SetVoltage,
  .GetVoltage   = NULL,
};
/**
  * @} BSP_Components_STPD01_Private_Variables
  */

/** @addtogroup BSP_Components_STPD01_Exported_Functions
  * @{
  */

/**
  * @brief  Register Bus Io to component
  * @param  pObj Pointer to component object
  * @param  pIO  Pointer to IO object
  * @retval Status of execution
  */
int32_t STPD01_RegisterBusIO(STPD01_Object_t *pObj, STPD01_IO_t *pIO)
{
  int32_t ret;
  
  if (pObj == NULL)
  {
    ret = STPD01_ERROR;
  }
  else
  {
    pObj->IO.Init      = pIO->Init;
    pObj->IO.DeInit    = pIO->DeInit;
    pObj->IO.Address   = pIO->Address;
    pObj->IO.WriteReg  = pIO->WriteReg;
    pObj->IO.ReadReg   = pIO->ReadReg;
    pObj->IO.GetTick   = pIO->GetTick;
    
    pObj->Ctx.ReadReg  = STPD01_ReadRegWrap;
    pObj->Ctx.WriteReg = STPD01_WriteRegWrap;
    pObj->Ctx.handle   = pObj;
    
    if (pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = STPD01_ERROR;
    }
  }
  
  return ret;
}
/**
  * @} BSP_Components_STPD01_Exported_Functions
  */

/** @addtogroup BSP_Components_STPD01_Private_Functions
  * @{
  */
/**
  * @brief  Initializes the STPD01 interface
  * @param  pObj Pointer to component object
  * @retval Component status (STPD01_OK / STPD01_ERROR)
  */
static int32_t STPD01_Init(STPD01_Object_t *pObj)
{
  int32_t ret = 0;
//  uint8_t tmp;

  if (pObj->IsInitialized == 0U)
  {
    pObj->IsInitialized = 1U;
  }

  if (ret != STPD01_OK)
  {
    ret = STPD01_ERROR;
  }

  return ret;
}

/**
  * @brief  Deinitializes the STPD01 interface
  * @param  pObj Pointer to component object
  * @retval Component status (STPD01_OK / STPD01_ERROR)
  */
static int32_t STPD01_DeInit(STPD01_Object_t *pObj)
{
  if (pObj->IsInitialized == 1U)
  {
    /* De-Initialize IO BUS layer */
    pObj->IO.DeInit();

    pObj->IsInitialized = 0U;
  }

  return STPD01_OK;
}

/**
  * @brief  Resets STPD01 register (Reg0)
  * @param  pObj Pointer to component object
  * @retval Component status (STPD01_OK / STPD01_ERROR)
  */
static int32_t STPD01_Reset(STPD01_Object_t *pObj)
{
  int32_t ret = STPD01_OK;
  
  /* Perform an de-init and init cycle */
  STPD01_DeInit(pObj);
  
  STPD01_Init(pObj);

  return ret;
}

static int32_t STPD01_SetVoltage(STPD01_Object_t *pObj, int32_t Voltage)
{
//  param_assert(Voltage >= VOLTAGE_L0_MIN_VALUE && Voltage <= VOLTAGE_L2_MAX_VALUE);
  
  STPD01_ILIM_SELECT_t i_reg = ILIM_SELECT_NOLIMIT;
  
  STPD01_VOUT_SELECT_t v_reg = VOUT_Invalid;

  if (VOLTAGE_CHECK(Voltage, VOLTAGE_L0_MIN_VALUE, VOLTAGE_L0_MAX_VALUE))
  {
    v_reg = VOLTAGE2REG(Voltage, VOLTAGE_L0_MIN_VALUE, VOLTAGE_L0_STEP, VOLTAGE_L0_OFFSET);
  }
  else if (VOLTAGE_CHECK(Voltage, VOLTAGE_L1_MIN_VALUE, VOLTAGE_L1_MAX_VALUE))
  {
    v_reg = VOLTAGE2REG(Voltage, VOLTAGE_L1_MIN_VALUE, VOLTAGE_L1_STEP, VOLTAGE_L1_OFFSET);
  }
  else if (VOLTAGE_CHECK(Voltage, VOLTAGE_L2_MIN_VALUE, VOLTAGE_L2_MAX_VALUE))
  {
    v_reg = VOLTAGE2REG(Voltage, VOLTAGE_L2_MIN_VALUE, VOLTAGE_L2_STEP, VOLTAGE_L2_OFFSET);
  }

  STPD01_Ilim_Select_Set(&pObj->Ctx, &i_reg);
  STPD01_Vout_Select_Set(&pObj->Ctx, &v_reg);

  return STPD01_OK;
}

/**
  * @brief  Wrap STPD01 read function to Bus IO function
  * @param  handle  Component object handle
  * @param  Reg     Target register address to read
  * @param  pData   Buffer where Target register value should be stored
  * @param  Length  buffer size to be read
  * @retval error status
  */
static int32_t STPD01_ReadRegWrap(void *handle, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  STPD01_Object_t *pObj = (STPD01_Object_t *)handle;
  return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
}

/**
  * @brief  Wrap STPD01 write function to Bus IO function
  * @param  handle Component object handle
  * @param  Reg    Target register address to write
  * @param  pData  Target register value to be written
  * @param  Length Buffer size to be written
  * @retval error status
  */
static int32_t STPD01_WriteRegWrap(void *handle, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  STPD01_Object_t *pObj = (STPD01_Object_t *)handle;
  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}

/**
  * @} BSP_Components_STPD01_Private_Functions
  */

/**
  * @} BSP_Components_STPD01
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
