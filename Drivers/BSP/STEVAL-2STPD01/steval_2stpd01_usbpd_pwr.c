/**
  ******************************************************************************
  * @file    steval_2stpd01_usbpd_pwr.c
  * @author  MCD Application Team and SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   This file provides a set of firmware functions to manage USB-PD Power
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

/** @addtogroup BSP_STEVAL_2STPD01_USBPD_PWR
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "steval_2stpd01_usbpd_pwr.h"
#include "steval_2stpd01_bus.h"
#include "steval_2stpd01_stpd01.h"
#include "steval_2stpd01_tcpp02.h"

#include "steval_2stpd01_tcpp02_conf.h"
#include "steval_2stpd01_stpd01_conf.h"
    
/** @defgroup BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Variables Exported Variables
  * @{
  */
USBPD_PWR_PortHandle_t USBPD_PWR_Port_Handle[USBPD_PWR_INSTANCES_NBR] =
{
  {
    .IsInitialized = 0U, 
    .IsItEnabled = 0U, 
    .PwrSavingMode = USBPD_PWR_MODE_OFF, 
    .VBUSDetectCallback = NULL,
    .Config = {
      .TCPP02_Address = BSP_TCPP02_P0_ADDRESS,
      .STPD01_Address = BSP_STPD01_P0_ADDRESS,
    }
  },
  {
    .IsInitialized = 0U, 
    .IsItEnabled = 0U, 
    .PwrSavingMode = USBPD_PWR_MODE_OFF, 
    .VBUSDetectCallback = NULL,
    .Config = {
      .TCPP02_Address = BSP_TCPP02_P1_ADDRESS,
      .STPD01_Address = BSP_STPD01_P1_ADDRESS,
    }
  },
};
/**
  * @}
  */

/** @addtogroup BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_Common
  * @{
  */
/**
  * @brief  Global initialization of PWR resource used by USB-PD
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_Init(uint32_t PortNum)
{
  BSP_PWR_PORT_CHECK_RET(PortNum);
  
  int32_t ret_TCPP02 = BSP_ERROR_NONE;
  int32_t ret_STPD01 = BSP_ERROR_NONE;
  
  BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_Init --");
  
  if (USBPD_PWR_Port_Handle[PortNum].IsInitialized != 0U)
  {
    return BSP_ERROR_ALREADY_INIT;
  }
  
  /* Initialization */

  /* Reset port callback for VBUS detection event */
  USBPD_PWR_Port_Handle[PortNum].VBUSDetectCallback = NULL;
  
  /* Initialize components */
  ret_TCPP02 = BSP_TCPP02_Init(PortNum, USBPD_PWR_Port_Handle[PortNum].Config.TCPP02_Address);
  ret_STPD01 = BSP_STPD01_Init(PortNum, USBPD_PWR_Port_Handle[PortNum].Config.STPD01_Address);
  
  BSP_COMPONENT_CHECK_RET(ret_TCPP02, ret_STPD01);

  /* Set default Power Mode to Off */
  BSP_USBPD_PWR_SetPowerMode(PortNum, USBPD_PWR_MODE_OFF);
  
  /* Set Initilisation flag */
  USBPD_PWR_Port_Handle[PortNum].IsInitialized = 1U;
  
  return BSP_ERROR_NONE;
}

/**
  * @brief  Global de-initialization of PWR resource used by USB-PD
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_DeInit(uint32_t PortNum)
{
  BSP_PWR_PORT_CHECK_RET(PortNum);
  
  int32_t ret_TCPP02 = BSP_ERROR_NONE;
  int32_t ret_STPD01 = BSP_ERROR_NONE;
  
  BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_DeInit --");
  
  /* Deinitialize Power config for port */

  /* Perform any treatments required for de-initialization for GPIO, BUS communication, Interrupts */
  ret_TCPP02 = BSP_TCPP02_DeInit(PortNum);
  ret_STPD01 = BSP_STPD01_DeInit(PortNum);

  /* Reset Initilisation flag */
  USBPD_PWR_Port_Handle[PortNum].IsInitialized = 0U;
  
  return BSP_COMPONENT_CHECK(ret_TCPP02, ret_STPD01);
}

/**
  * @brief  Enable / Disable of the specified port
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  Enable On / Off
  *         @arg @ref BSP_PWR_ON
  *         @arg @ref BSP_PWR_OFF
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_EnablePort(uint32_t PortNum, uint32_t Enable)
{
  BSP_PWR_PORT_CHECK_RET(PortNum);
  if (Enable == BSP_PWR_ON)
  {
    BSP_STPD01_Enable(PortNum, BSP_PWR_ON);
    BSP_TCPP02_Enable(PortNum, BSP_PWR_ON);
    BSP_TCPP02_PowerMode(PortNum, USBPD_PWR_MODE_NORMAL);
  }
  else
  {
    BSP_TCPP02_PowerMode(PortNum, USBPD_PWR_MODE_OFF); 
    BSP_TCPP02_VBus(PortNum, BSP_PWR_OFF);
    BSP_TCPP02_Enable(PortNum, BSP_PWR_OFF);
    BSP_STPD01_VBus_Value(PortNum, BSP_PWR_VSAFE);
    BSP_STPD01_Enable(PortNum, BSP_PWR_OFF);
  }
  return BSP_ERROR_NONE;
}

/**
  * @brief  Assign Power role for current Port (Source or Sink)
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  Role Type-C port role
  *         This parameter can be take one of the following values:
  *         @arg @ref POWER_ROLE_SOURCE
  *         @arg @ref POWER_ROLE_SINK
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_SetRole(uint32_t PortNum, USBPD_PWR_PowerRoleTypeDef Role)
{
  BSP_PWR_PORT_CHECK_RET(PortNum);
  
  int32_t ret = BSP_ERROR_NONE;
  
  switch (Role)
  {
  case POWER_ROLE_SOURCE:
    /* Port Role is now SRC */
    BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_SetRole : SRC --");
    break;
    
  case POWER_ROLE_SINK:
    /* Port Role is now SNK */
    BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_SetRole : SNK -- NOT SUPPORTED");

    ret = BSP_ERROR_WRONG_PARAM;
    break;
    
  default:
    BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_SetRole : Role -- NOT SUPPORTED");
    ret = BSP_ERROR_WRONG_PARAM;
    break;
  }
  
  return ret;
}


/**
  * @brief  Set operating mode of Port regarding power saving constraints
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  PwrMode Type-C port power saving mode
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_MODE_OFF
  *         @arg @ref USBPD_PWR_MODE_HIBERNATE
  *         @arg @ref USBPD_PWR_MODE_LOWPOWER
  *         @arg @ref USBPD_PWR_MODE_NORMAL
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_SetPowerMode(uint32_t PortNum, USBPD_PWR_PowerModeTypeDef PwrMode)
{
  BSP_PWR_PORT_CHECK_RET(PortNum);
  int32_t ret_TCPP02 = BSP_ERROR_NONE;
  int32_t ret_STPD01 = BSP_ERROR_NONE;
  
  BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_SetPowerMode --");
  
  if (PwrMode == USBPD_PWR_MODE_HIBERNATE)
  {
    return BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  
  /* Evaluate requested Power config for port */
  ret_TCPP02 = BSP_TCPP02_PowerMode(PortNum, PwrMode);  
  ret_STPD01 = BSP_ERROR_NONE; /* no power mode for STPD01 */

  /* Set port low power operating mode */
  USBPD_PWR_Port_Handle[PortNum].PwrSavingMode = PwrMode;
  
  return BSP_COMPONENT_CHECK(ret_TCPP02, ret_STPD01);
}

/**
  * @brief  Get operating mode of Port regarding power saving constraints
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  PwrMode Pointer on current Type-C port power saving mode value
  *         Following values are available :
  *         @arg @ref USBPD_PWR_MODE_OFF
  *         @arg @ref USBPD_PWR_MODE_HIBERNATE
  *         @arg @ref USBPD_PWR_MODE_LOWPOWER
  *         @arg @ref USBPD_PWR_MODE_NORMAL
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_GetPowerMode(uint32_t PortNum, USBPD_PWR_PowerModeTypeDef *PwrMode)
{
  BSP_PWR_PORT_CHECK_RET(PortNum);

  assert_param(PwrMode);
  if (!PwrMode)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_GetPowerMode --");
  /* Get port low power operating mode */
  *PwrMode = USBPD_PWR_Port_Handle[PortNum].PwrSavingMode;
  
  return BSP_ERROR_NONE;
}
/**
  * @} BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_Common
  */

/** @addtogroup BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_VBUS VBUS Management
  * @{
  */
/**
  * @brief  Initialize the hardware resources used by the Type-C power delivery (PD)
  *         controller.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VBUSInit(uint32_t PortNum)
{
  BSP_PWR_PORT_CHECK_RET(PortNum);
  
  int32_t ret_TCPP02 = BSP_ERROR_NONE;
  int32_t ret_STPD01 = BSP_ERROR_NONE;
  
  BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_VBUSInit --");
  
  /* Switch to Normal mode */
  BSP_USBPD_PWR_SetPowerMode(PortNum, USBPD_PWR_MODE_NORMAL);
  
  /* Initialize required HW for VBUS management */
  ret_TCPP02 = BSP_TCPP02_VBus_Init(PortNum);
  ret_STPD01 = BSP_STPD01_VBus_Init(PortNum);

  return BSP_COMPONENT_CHECK(ret_TCPP02, ret_STPD01);
}

/**
  * @brief  Release the hardware resources used by the Type-C power delivery (PD)
  *         controller.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VBUSDeInit(uint32_t PortNum)
{
  BSP_PWR_PORT_CHECK_RET(PortNum);
  
  int32_t ret_TCPP02 = BSP_ERROR_NONE;
  int32_t ret_STPD01 = BSP_ERROR_NONE;
  
  BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_VBUSDeInit --");

  /* Switch to Normal mode */
  BSP_USBPD_PWR_SetPowerMode(PortNum, USBPD_PWR_MODE_NORMAL);
  
  BSP_STPD01_Enable(PortNum, BSP_PWR_ON);
  BSP_TCPP02_Enable(PortNum, BSP_PWR_ON);

  /* Initialize required HW for VBUS management */
  ret_TCPP02 = BSP_TCPP02_VBus_Init(PortNum);
  ret_STPD01 = BSP_STPD01_VBus_Init(PortNum);
  
  BSP_USBPD_PWR_SetPowerMode(PortNum, USBPD_PWR_MODE_LOWPOWER);
  
  return BSP_COMPONENT_CHECK(ret_TCPP02, ret_STPD01);
}

/**
  * @brief  Enable power supply over VBUS.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  Enable On / Off
  *         @arg @ref BSP_PWR_ON
  *         @arg @ref BSP_PWR_OFF
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VBUS(uint32_t PortNum, uint32_t Enable)
{
  BSP_PWR_PORT_CHECK_RET(PortNum);
  
  int32_t ret_TCPP02 = BSP_ERROR_NONE;
  int32_t ret_STPD01 = BSP_ERROR_NONE;
  
  BSP_USBPD_PWR_TRACE(PortNum, (char const *)(Enable ? "-- BSP_USBPD_PWR_VBUSOn --" : "-- BSP_USBPD_PWR_VBUSOff --"));
  
//  BSP_TCPP02_Enable(PortNum, BSP_PWR_ON);
//  BSP_STPD01_Enable(PortNum, BSP_PWR_ON);
  
  ret_TCPP02 = BSP_TCPP02_VBus(PortNum, Enable);
  ret_STPD01 = BSP_STPD01_VBus(PortNum, Enable);

//  if (Enable == PWR_OFF)
//  {
//    BSP_STPD01_VBus_Value(PortNum, PWR_VSAFE);
//  }

  return BSP_COMPONENT_CHECK(ret_TCPP02, ret_STPD01);
}

/**
  * @brief  Enable power supply over VBUS.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VBUSOn(uint32_t PortNum)
{
  return BSP_USBPD_PWR_VBUS(PortNum, BSP_PWR_ON);
}

/**
  * @brief  Disable power supply over VBUS.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VBUSOff(uint32_t PortNum)
{
  return BSP_USBPD_PWR_VBUS(PortNum, BSP_PWR_OFF);
}

/**
  * @brief  Get actual VBUS status.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  pState VBUS status (1: On, 0: Off)
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VBUSIsOn(uint32_t PortNum, uint8_t *pState)
{
  UNUSED(pState);

  BSP_PWR_PORT_CHECK_RET(PortNum);

  int32_t ret_TCPP02 = BSP_ERROR_NONE;
  int32_t ret_STPD01 = BSP_ERROR_NONE;
  
  BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_VBUSIsOn --");

  if (!pState)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  
  /* get the status from TCPP02 */
  ret_TCPP02 = BSP_TCPP02_VBus_Status(PortNum);
  UNUSED(ret_STPD01);
  
  BSP_COMPONENT_CHECK_RET(ret_TCPP02, ret_STPD01);
  *pState = ret_TCPP02;
  
  return BSP_ERROR_NONE;
}

/**
  * @brief  Wait for time.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  VBusTarget target VBus value
  * @param  Delta  +/- acceptable VBus value
  * @param  Timeout global time to wait for reach the VBus target
  * @param  Delay time from 2 check
  * @param  pWaitForTime pointer to the a Delay function (set NULL to use HAL_Delay)
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VBUSWaitValue(uint32_t PortNum, 
                                    uint32_t VBusTarget, 
                                    uint32_t Delta, 
                                    uint16_t Timeout, 
                                    uint16_t Delay, 
                                    void (*pWaitForTime)(uint32_t))
{
  BSP_PWR_PORT_CHECK_RET(PortNum);
  
  uint32_t vbus;
  uint16_t counter = Timeout / Delay;
  int32_t ret = BSP_ERROR_NONE;
  
  while(1)
  {
    /* get the current value */
    BSP_USBPD_PWR_VBUSGetVoltage(PortNum, &vbus);
    
    if (vbus >= (VBusTarget - Delta)) break;
    if (counter == 0) 
    {
      ret = BSP_ERROR_VBUS_NOT_REACHED;
      break;
    }
    
    if (pWaitForTime)
    {
      pWaitForTime(Delay);
    }
    else
    {
      HAL_Delay(Delay);
    }
    counter++;
    
  }
  
  return ret;
}

/**
  * @brief  Set a fixed/variable PDO and manage the power control.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  VbusTargetInmv the vbus Target (in mV)
  * @param  OperatingCurrent the Operating Current (in mA)
  * @param  MaxOperatingCurrent the Max Operating Current (in mA)
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VBUSSetVoltage_Fixed(uint32_t PortNum,
                                           uint32_t VbusTargetInmv,
                                           uint32_t OperatingCurrent,
                                           uint32_t MaxOperatingCurrent)
{
  UNUSED(MaxOperatingCurrent);
  UNUSED(OperatingCurrent);
  UNUSED(VbusTargetInmv);

  BSP_PWR_PORT_CHECK_RET(PortNum);

  int32_t ret_TCPP02 = BSP_ERROR_NONE;
  int32_t ret_STPD01 = BSP_ERROR_NONE;

  UNUSED(ret_TCPP02);
  ret_STPD01 = BSP_STPD01_VBus_Value(PortNum, VbusTargetInmv);

  return BSP_COMPONENT_CHECK(ret_TCPP02, ret_STPD01);
}

/**
  * @brief  Set a fixed/variable PDO and manage the power control.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  VbusTargetMinInmv the vbus Target min (in mV)
  * @param  VbusTargetMaxInmv the vbus Target max (in mV)
  * @param  OperatingCurrent the Operating Current (in mA)
  * @param  MaxOperatingCurrent the Max Operating Current (in mA)
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VBUSSetVoltage_Variable(uint32_t PortNum,
                                              uint32_t VbusTargetMinInmv,
                                              uint32_t VbusTargetMaxInmv,
                                              uint32_t OperatingCurrent,
                                              uint32_t MaxOperatingCurrent)
{
  UNUSED(MaxOperatingCurrent);
  UNUSED(OperatingCurrent);

  BSP_PWR_PORT_CHECK_RET(PortNum);

  if (VbusTargetMinInmv > VbusTargetMaxInmv)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  
  /* set the average volatge value*/
  int32_t ret_TCPP02 = BSP_ERROR_NONE;
  int32_t ret_STPD01 = BSP_ERROR_NONE;
  
  int32_t Voltage = (VbusTargetMaxInmv - VbusTargetMinInmv) / 2;
  UNUSED(ret_TCPP02);
  ret_STPD01 = BSP_STPD01_VBus_Value(PortNum, Voltage);

  return BSP_COMPONENT_CHECK(ret_TCPP02, ret_STPD01);
}

/**
  * @brief  Set a Battery PDO and manage the power control.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  VbusTargetMin the vbus Target min (in mV)
  * @param  VbusTargetMax the vbus Target max (in mV)
  * @param  OperatingPower the Operating Power (in mW)
  * @param  MaxOperatingPower the Max Operating Power (in mW)
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VBUSSetVoltage_Battery(uint32_t PortNum,
                                             uint32_t VbusTargetMin,
                                             uint32_t VbusTargetMax,
                                             uint32_t OperatingPower,
                                             uint32_t MaxOperatingPower)
{
  UNUSED(MaxOperatingPower);
  UNUSED(OperatingPower);

  BSP_PWR_PORT_CHECK_RET(PortNum);

  if (VbusTargetMin > VbusTargetMax)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  
  /* set the average volatge value*/
  int32_t ret_TCPP02 = BSP_ERROR_NONE;
  int32_t ret_STPD01 = BSP_ERROR_NONE;
  
  int32_t Voltage = (VbusTargetMax - VbusTargetMin) / 2;
  UNUSED(ret_TCPP02);
  ret_STPD01 = BSP_STPD01_VBus_Value(PortNum, Voltage);

  return BSP_COMPONENT_CHECK(ret_TCPP02, ret_STPD01);
}

/**
  * @brief  Set a APDO and manage the power control.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  VbusTargetInmv the vbus Target (in mV)
  * @param  OperatingCurrent the Operating current (in mA)
  * @param  Delta Delta between with previous APDO (in mV), 0 means APDO start
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VBUSSetVoltage_APDO(uint32_t PortNum,
                                          uint32_t VbusTargetInmv,
                                          uint32_t OperatingCurrent,
                                          int32_t Delta)
{
  UNUSED(Delta);
  UNUSED(OperatingCurrent);
  UNUSED(VbusTargetInmv);

  BSP_PWR_PORT_CHECK_RET(PortNum);

  int32_t ret_TCPP02 = BSP_ERROR_NONE;
  int32_t ret_STPD01 = BSP_ERROR_NONE;

  UNUSED(ret_TCPP02);
  ret_STPD01 = BSP_STPD01_VBus_Value(PortNum, VbusTargetInmv);
  
  return BSP_COMPONENT_CHECK(ret_TCPP02, ret_STPD01);
}

/**
  * @brief  Set the VBUS disconnection voltage threshold.
  * @note   Callback funtion invoked when VBUS falls below programmed threshold.
  * @note   By default VBUS disconnection threshold is set to 3.3V
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  VoltageThreshold VBUS disconnection voltage threshold (in mV)
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_SetVBUSDisconnectionThreshold(uint32_t PortNum,
                                                    uint32_t VoltageThreshold)
{
  UNUSED(VoltageThreshold);

  BSP_PWR_PORT_CHECK_RET(PortNum);

  return BSP_ERROR_FEATURE_NOT_SUPPORTED;
}

/**
  * @brief  Register USB Type-C Current callback function.
  * @note   Callback function invoked when VBUS rises above 4V (VBUS present) or
  *         when VBUS falls below programmed threshold (VBUS absent).
  * @note   Callback funtion is un-registered when callback function pointer
  *         argument is NULL.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  pfnVBUSDetectCallback callback function pointer
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_RegisterVBUSDetectCallback(uint32_t PortNum,
                                                 USBPD_PWR_VBUSDetectCallbackFunc *pfnVBUSDetectCallback)
{
  BSP_PWR_PORT_CHECK_RET(PortNum);

  /* Set port callback for VBUS detection event */
  USBPD_PWR_Port_Handle[PortNum].VBUSDetectCallback = pfnVBUSDetectCallback;

  return BSP_ERROR_NONE;
}

/**
  * @brief  Get actual voltage level measured on the VBUS line.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  pVoltage Pointer on measured voltage level (in mV)
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VBUSGetVoltage(uint32_t PortNum, uint32_t *pVoltage)
{
  BSP_PWR_PORT_CHECK_RET(PortNum);
  
  if (!pVoltage)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  
  *pVoltage = BSPX_ADC_GetConvertedValue(PortNum == USBPD_PWR_TYPE_C_PORT_1 ? 
                                         BSP_ADC_VBUS0 : 
                                         BSP_ADC_VBUS1);
  
  return BSP_ERROR_NONE;
}

/**
  * @brief  Get actual current level measured on the VBUS line.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  pCurrent Pointer on measured current level (in mA)
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VBUSGetCurrent(uint32_t PortNum, int32_t *pCurrent)
{
  BSP_PWR_PORT_CHECK_RET(PortNum);

  if (!pCurrent)
  {
    return BSP_ERROR_WRONG_PARAM;
  }

  *pCurrent = (int32_t)BSPX_ADC_GetConvertedValue(PortNum == USBPD_PWR_TYPE_C_PORT_1 ? 
                                                  BSP_ADC_IBUS0_TCPP02 : 
                                                  BSP_ADC_IBUS1_TCPP02);
  return BSP_ERROR_NONE;
}

/** @addtogroup BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_VBUS_Discharge
  * @{
  */
/**
  * @brief  Activate/Deactivate discharge on VBUS.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  Enable On / Off
  *         @arg @ref BSP_PWR_ON
  *         @arg @ref BSP_PWR_OFF
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VBUSDischarge(uint32_t PortNum, uint32_t Enable)
{
  BSP_PWR_PORT_CHECK_RET(PortNum);

  int32_t ret_TCPP02 = BSP_ERROR_NONE;
  int32_t ret_STPD01 = BSP_ERROR_NONE;

  BSP_USBPD_PWR_TRACE(PortNum, (char const *)(Enable ? "-- BSP_USBPD_PWR_VBUSDischargeOn --" : "-- BSP_USBPD_PWR_VBUSDischargeOff --"));

  ret_TCPP02 = BSP_TCPP02_VBus_Discharge(PortNum, Enable);
  UNUSED(ret_STPD01);
  
  return BSP_COMPONENT_CHECK(ret_TCPP02, ret_STPD01);
}
/**
  * @brief  Activate discharge on VBUS.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VBUSDischargeOn(uint32_t PortNum)
{
  return BSP_USBPD_PWR_VBUSDischarge(PortNum, BSP_PWR_ON);
}

/**
  * @brief  Deactivate discharge on VBUS.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VBUSDischargeOff(uint32_t PortNum)
{
  return BSP_USBPD_PWR_VBUSDischarge(PortNum, BSP_PWR_OFF);
}
/**
  * @} BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_VBUS_Discharge
  */
/**
  * @} BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_VBUS
  */

/** @addtogroup BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_VCONN
  * @{
  */
/**
  * @brief  Initialize VCONN sourcing.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_CC1
  *         @arg @ref USBPD_PWR_TYPE_C_CC2
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VCONNInit(uint32_t PortNum, uint32_t CCPinId)
{
  UNUSED(CCPinId);
  BSP_PWR_PORT_CHECK_RET(PortNum);
  
  int32_t ret = BSP_ERROR_NONE;
  
  BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_VCONNInit --");

  ret = BSP_TCPP02_VConn_Init(PortNum);
  
  return ret;
}

/**
  * @brief  Un-Initialize VCONN sourcing.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_CC1
  *         @arg @ref USBPD_PWR_TYPE_C_CC2
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VCONNDeInit(uint32_t PortNum, uint32_t CCPinId)
{
  UNUSED(CCPinId);
  BSP_PWR_PORT_CHECK_RET(PortNum);

  BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_VCONNDeInit --");
  return BSP_TCPP02_VConn_DeInit(PortNum);
}

/**
  * @brief  Enable VCONN sourcing.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_CC1
  *         @arg @ref USBPD_PWR_TYPE_C_CC2
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VCONNOn(uint32_t PortNum, uint32_t CCPinId)
{
  BSP_PWR_PORT_CHECK_RET(PortNum);
  return BSP_TCPP02_VConn_Enable(PortNum, CCPinId, BSP_PWR_ON);
}

/**
  * @brief  Disable VCONN sourcing.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  CCPinId CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_CC1
  *         @arg @ref USBPD_PWR_TYPE_C_CC2
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VCONNOff(uint32_t PortNum, uint32_t CCPinId)
{
  BSP_PWR_PORT_CHECK_RET(PortNum);
  return BSP_TCPP02_VConn_Enable(PortNum, CCPinId, BSP_PWR_OFF);
}

/**
  * @brief  Get actual VCONN status.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @param  CCPinId Type-C CC pin identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_CC1
  *         @arg @ref USBPD_PWR_TYPE_C_CC2
  * @param  pState VCONN status (1: On, 0: Off)
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VCONNIsOn(uint32_t PortNum, uint32_t CCPinId, uint8_t *pState)
{
  BSP_PWR_PORT_CHECK_RET(PortNum);

  if (NULL == pState)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  
  if ((CCPinId != USBPD_PWR_TYPE_C_CC1) && (CCPinId != USBPD_PWR_TYPE_C_CC2))
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  
  *pState = 0U;
  
  int32_t ret = BSP_TCPP02_VConn_Status(PortNum);
  if (ret < 0) return ret;
  
  *pState = (ret == CCPinId) ? 1U : 0U;
  
  return BSP_ERROR_NONE;
}

/** @addtogroup BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_VCONN_Discharge
  * @{
  */
/**
  * @brief  Activate discharge on VCONN.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VCONNDischargeOn(uint32_t PortNum)
{
  BSP_PWR_PORT_CHECK_RET(PortNum);
  
  BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_VCONNDischargeOn --");
  return BSP_TCPP02_VConn_Discharge(PortNum, BSP_PWR_ON);
}

/**
  * @brief  Deactivate discharge on VCONN.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  * @retval BSP errorcode
  */
int32_t BSP_USBPD_PWR_VCONNDischargeOff(uint32_t PortNum)
{
  BSP_PWR_PORT_CHECK_RET(PortNum);
  
  BSP_USBPD_PWR_TRACE(PortNum, "-- BSP_USBPD_PWR_VCONNDischargeOff --");
  return BSP_TCPP02_VConn_Discharge(PortNum, BSP_PWR_OFF);
}
/**
  * @} BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_VCONN_Discharge
  */
/**
  * @} BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_VCONN
  */

/**
  * @} BSP_STEVAL_2STPD01_USBPD_PWR
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

