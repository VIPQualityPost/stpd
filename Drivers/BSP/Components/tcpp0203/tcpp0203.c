/**
  ******************************************************************************
  * @file    tcpp0203.c
  * @author  MCD Application Team and SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   This file provides the TCPP02/03 Type-C port protection driver.
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

/** @addtogroup BSP_Components_TCPP0203
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "tcpp0203.h"

#if  defined(_TRACE)
#include "usbpd_core.h"
#include "usbpd_trace.h"
#include "string.h"
#include "stdio.h"
#endif /* _TRACE */

/** @defgroup BSP_Components_TCPP0203_Private_Defines Private Defines
  * @{
  */
/**
  * @brief  Compilation option in order to enable/disable a concistency check 
  * performed after each I2C access into TCPP0203 registers : 
  * goal is to check that written value in Reg0
  * is properly reflected into reg1 register content.
  * Set to 1 to enable register consistency check
  * Reset to 0 to disable it
  */
#define TCPP0203_REGISTER_CONSISTENCY_CHECK 0
/**
  * @} BSP_Components_TCPP0203_Private_Defines
  */

/** @defgroup BSP_Components_TCPP0203_Private_Types Private Types
  * @{
  */
/**
  * @brief  Type-C port protection driver structure initialization
  */
TCPP0203_Drv_t TCPP0203_Driver =
{
  TCPP0203_Init,
  TCPP0203_DeInit,
  TCPP0203_Reset,
  TCPP0203_SetVConnSwitch,
  TCPP0203_SetGateDriverProvider,
  TCPP0203_SetGateDriverConsumer,
  TCPP0203_SetPowerMode,
  TCPP0203_SetVBusDischarge,
  TCPP0203_SetVConnDischarge,
  TCPP0203_GetVConnSwitchAck,
  TCPP0203_GetGateDriverProviderAck,
  TCPP0203_GetGateDriverConsumerAck,
  TCPP0203_GetPowerModeAck,
  TCPP0203_GetVBusDischargeAck,
  TCPP0203_GetVConnDischargeAck,
  TCPP0203_GetOCPVConnFlag,
  TCPP0203_GetOCPVBusFlag,
  TCPP0203_GetOVPVBusFlag,
  TCPP0203_GetOVPCCFlag,
  TCPP0203_GetOTPFlag,
  TCPP0203_GetVBusOkFlag,
  TCPP0203_ReadTCPPType,
  TCPP0203_ReadVCONNPower,
  TCPP0203_WriteCtrlRegister,
  TCPP0203_ReadAckRegister,
  TCPP0203_ReadFlagRegister,
};
/**
  * @} BSP_Components_TCPP0203_Private_Types
  */

/** @defgroup BSP_Components_TCPP0203_Private_Variables Private Variables
  * @{
  */


/**
  * @} BSP_Components_TCPP0203_Private_Variables
  */

/* Private function prototypes -----------------------------------------------*/

/** @defgroup BSP_Components_TCPP0203_Private_Functions Private Functions
  * @{
  */
static int32_t TCPP0203_ReadRegWrap(void *handle, uint8_t Reg, uint8_t *Data, uint8_t Length);
static int32_t TCPP0203_WriteRegWrap(void *handle, uint8_t Reg, uint8_t *Data, uint8_t Length);

static int32_t TCPP0203_ModifyReg0(TCPP0203_Object_t *pObj, uint8_t Value, uint8_t Mask);

/**
  * @} BSP_Components_TCPP0203_Private_Functions
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief  Register Bus Io to component
  * @param  pObj Pointer to component object
  * @param  pIO  Pointer to IO object
  * @retval Status of execution
  */
int32_t TCPP0203_RegisterBusIO(TCPP0203_Object_t *pObj, TCPP0203_IO_t *pIO)
{
  int32_t ret;

  if (pObj == NULL)
  {
    ret = TCPP0203_ERROR;
  }
  else
  {
    pObj->IO.Init      = pIO->Init;
    pObj->IO.DeInit    = pIO->DeInit;
    pObj->IO.Address   = pIO->Address;
    pObj->IO.WriteReg  = pIO->WriteReg;
    pObj->IO.ReadReg   = pIO->ReadReg;
    pObj->IO.GetTick   = pIO->GetTick;

    pObj->Ctx.ReadReg  = TCPP0203_ReadRegWrap;
    pObj->Ctx.WriteReg = TCPP0203_WriteRegWrap;
    pObj->Ctx.handle   = pObj;

    if (pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = TCPP0203_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Initializes the TCPP0203 interface
  * @param  pObj Pointer to component object
  * @retval Component status (TCPP0203_OK / TCPP0203_ERROR)
  */
int32_t TCPP0203_Init(TCPP0203_Object_t *pObj)
{
  int32_t ret = TCPP0203_OK;
  uint8_t tmp;

  if (pObj->IsInitialized == 0U)
  {
    /* Read TCPP Device type */
    ret = tcpp0203_read_reg(&pObj->Ctx, TCPP0203_READ_REG2, &tmp, 1);

    if (ret == TCPP0203_OK)
    {
      pObj->DeviceType= (tmp & TCPP0203_DEVICE_TYPE_MSK);
      pObj->IsInitialized = 1U;
    }
      
  }

  if (ret != TCPP0203_OK)
  {
    ret = TCPP0203_ERROR;
  }

  return ret;
}

/**
  * @brief  Deinitializes the TCPP0203 interface
  * @param  pObj Pointer to component object
  * @retval Component status (TCPP0203_OK / TCPP0203_ERROR)
  */
int32_t TCPP0203_DeInit(TCPP0203_Object_t *pObj)
{
  if (pObj->IsInitialized == 1U)
  {
    /* De-Initialize IO BUS layer */
    pObj->IO.DeInit();

    pObj->IsInitialized = 0U;
  }

  return TCPP0203_OK;
}

/**
  * @brief  Resets TCPP0203 register (Reg0)
  * @param  pObj Pointer to component object
  * @retval Component status (TCPP0203_OK / TCPP0203_ERROR)
  */
int32_t TCPP0203_Reset(TCPP0203_Object_t *pObj)
{
  int32_t ret = TCPP0203_OK;
  uint8_t tmp = TCPP0203_REG0_RST_VALUE;

  /* Write reset values in Reg0 register */
  if (tcpp0203_write_reg(&pObj->Ctx, TCPP0203_PROG_CTRL, &tmp, 1) != TCPP0203_OK)
  {
    ret = TCPP0203_ERROR;
  }


  return ret;
}

/**
  * @brief  Configure TCPP0203 VConn Switch
  * @param  pObj Pointer to component object
  * @param  VConnSwitch VConn Switch requested setting
  *         This parameter can be one of the following values:
  *          @arg TCPP0203_VCONN_SWITCH_OPEN VConn switch open
  *          @arg TCPP0203_VCONN_SWITCH_CC1  VConn closed on CC1
  *          @arg TCPP0203_VCONN_SWITCH_CC2  VConn closed on CC2
  * @retval Component status
  */
int32_t TCPP0203_SetVConnSwitch(TCPP0203_Object_t *pObj, uint8_t VConnSwitch)
{
  int32_t ret = TCPP0203_OK;

  if ((VConnSwitch != TCPP0203_VCONN_SWITCH_OPEN)
      && (VConnSwitch != TCPP0203_VCONN_SWITCH_CC1)
      && (VConnSwitch != TCPP0203_VCONN_SWITCH_CC2))
  {
    ret = TCPP0203_ERROR;
  }
  else
  {
    /* Update VConn switch setting in Writing register Reg0 */
    ret += TCPP0203_ModifyReg0(pObj, VConnSwitch, TCPP0203_VCONN_SWITCH_MSK);
  }

  return ret;
}

/**
  * @brief  Configure TCPP0203 Gate Driver for Provider path
  * @param  pObj Pointer to component object
  * @param  GateDriverProvider GDP switch load requested setting
  *         This parameter can be one of the following values:
  *          @arg TCPP0203_GD_PROVIDER_SWITCH_OPEN    GDP Switch Load Open
  *          @arg TCPP0203_GD_PROVIDER_SWITCH_CLOSED  GDP Switch Load closed
  * @retval Component status
  */
int32_t TCPP0203_SetGateDriverProvider(TCPP0203_Object_t *pObj, uint8_t GateDriverProvider)
{
  int32_t ret = TCPP0203_OK;

  if ((GateDriverProvider != TCPP0203_GD_PROVIDER_SWITCH_OPEN)
      && (GateDriverProvider != TCPP0203_GD_PROVIDER_SWITCH_CLOSED))
  {
    ret = TCPP0203_ERROR;
  }
  else
  {
    /* Update GDP Switch Load setting in Writing register Reg0 */
    if (GateDriverProvider == TCPP0203_GD_PROVIDER_SWITCH_CLOSED)
    {
      /* If Gate Driver Provider is to be closed, Gate Driver Consumer should be open */
      ret += TCPP0203_ModifyReg0(pObj, (GateDriverProvider | TCPP0203_GD_CONSUMER_SWITCH_OPEN),
                                 (TCPP0203_GD_PROVIDER_SWITCH_MSK | TCPP0203_GD_CONSUMER_SWITCH_MSK));
    }
    else
    {
      ret += TCPP0203_ModifyReg0(pObj, GateDriverProvider, TCPP0203_GD_PROVIDER_SWITCH_MSK);
    }
  }

  return ret;
}

/**
  * @brief  Configure TCPP0203 Gate Driver for Consumer path
  * @param  pObj Pointer to component object
  * @param  GateDriverConsumer GDC switch load requested setting
  *         This parameter can be one of the following values:
  *          @arg TCPP0203_GD_CONSUMER_SWITCH_OPEN    GDC Switch Load Open
  *          @arg TCPP0203_GD_CONSUMER_SWITCH_CLOSED  GDC Switch Load closed
  * @retval Component status
  */
int32_t TCPP0203_SetGateDriverConsumer(TCPP0203_Object_t *pObj, uint8_t GateDriverConsumer)
{
  int32_t ret = TCPP0203_OK;

  /* Check if TCPP type is TCPP03. Otherwise, return error */
  if (pObj->DeviceType != TCPP0203_DEVICE_TYPE_03)
  {
    return (TCPP0203_ERROR);
  }

  if ((GateDriverConsumer != TCPP0203_GD_CONSUMER_SWITCH_OPEN)
      && (GateDriverConsumer != TCPP0203_GD_CONSUMER_SWITCH_CLOSED))
  {
    ret = TCPP0203_ERROR;
  }
  else
  {
    /* Update GDC Switch Load setting in Writing register Reg0 */
    if (GateDriverConsumer == TCPP0203_GD_CONSUMER_SWITCH_CLOSED)
    {
      /* If Gate Driver Consumer is to be closed, Gate Driver Provider should be open */
      ret += TCPP0203_ModifyReg0(pObj, (GateDriverConsumer | TCPP0203_GD_PROVIDER_SWITCH_OPEN),
                                 (TCPP0203_GD_PROVIDER_SWITCH_MSK | TCPP0203_GD_CONSUMER_SWITCH_MSK));
    }
    else
    {
      ret += TCPP0203_ModifyReg0(pObj, GateDriverConsumer, TCPP0203_GD_CONSUMER_SWITCH_MSK);
    }
  }

  return ret;
}

/**
  * @brief  Configure TCPP0203 Power Mode
  * @param  pObj Pointer to component object
  * @param  PowerMode Power mode requested setting
  *         This parameter can be one of the following values:
  *          @arg TCPP0203_POWER_MODE_HIBERNATE    Hibernate
  *          @arg TCPP0203_POWER_MODE_LOWPOWER     Low Power
  *          @arg TCPP0203_POWER_MODE_NORMAL       Normal
  * @retval Component status
  */
int32_t TCPP0203_SetPowerMode(TCPP0203_Object_t *pObj, uint8_t PowerMode)
{
  int32_t ret = TCPP0203_OK;

  if ((PowerMode != TCPP0203_POWER_MODE_HIBERNATE)
      && (PowerMode != TCPP0203_POWER_MODE_LOWPOWER)
      && (PowerMode != TCPP0203_POWER_MODE_NORMAL))
  {
    ret = TCPP0203_ERROR;
  }
  else
  {
    /* Update Power Mode setting in Writing register Reg0 */
    ret += TCPP0203_ModifyReg0(pObj, PowerMode, TCPP0203_POWER_MODE_MSK);
  }

  return ret;
}

/**
  * @brief  Configure TCPP0203 Gate Driver for Provider path
  * @param  pObj Pointer to component object
  * @param  VBusDischarge VBUS Discharge requested setting
  *         This parameter can be one of the following values:
  *          @arg TCPP0203_VBUS_DISCHARGE_OFF    VBUS Discharge Off
  *          @arg TCPP0203_VBUS_DISCHARGE_ON     VBUS Discharge On
  * @retval Component status
  */
int32_t TCPP0203_SetVBusDischarge(TCPP0203_Object_t *pObj, uint8_t VBusDischarge)
{
  int32_t ret = TCPP0203_OK;

  if ((VBusDischarge != TCPP0203_VBUS_DISCHARGE_OFF)
      && (VBusDischarge != TCPP0203_VBUS_DISCHARGE_ON))
  {
    ret = TCPP0203_ERROR;
  }
  else
  {
    /* Update VBUS Discharge setting in Writing register Reg0 */
    ret += TCPP0203_ModifyReg0(pObj, VBusDischarge, TCPP0203_VBUS_DISCHARGE_MSK);
  }

  return ret;
}

/**
  * @brief  Configure TCPP0203 Gate Driver for Provider path
  * @param  pObj Pointer to component object
  * @param  VConnDischarge GDP switch load requested setting
  *         This parameter can be one of the following values:
  *          @arg TCPP0203_VCONN_DISCHARGE_OFF    VConn Discharge Off
  *          @arg TCPP0203_VCONN_DISCHARGE_ON     VConn Discharge On
  * @retval Component status
  */
int32_t TCPP0203_SetVConnDischarge(TCPP0203_Object_t *pObj, uint8_t VConnDischarge)
{
  int32_t ret = TCPP0203_OK;

  if ((VConnDischarge != TCPP0203_VCONN_DISCHARGE_OFF)
      && (VConnDischarge != TCPP0203_VCONN_DISCHARGE_ON))
  {
    ret = TCPP0203_ERROR;
  }
  else
  {
    /* Update VConn Discharge setting in Writing register Reg0 */
    ret += TCPP0203_ModifyReg0(pObj, VConnDischarge, TCPP0203_VCONN_DISCHARGE_MSK);
  }

  return ret;
}

/**
  * @brief  Get VConn switch Ack value
  * @param  pObj Pointer to component object
  * @param  pVConnSwitchAck Pointer on VConn switch Ack value
  *         This output parameter can be one of the following values:
  *          @arg TCPP0203_VCONN_SWITCH_OPEN VConn switch open Ack
  *          @arg TCPP0203_VCONN_SWITCH_CC1  VConn closed on CC1 Ack
  *          @arg TCPP0203_VCONN_SWITCH_CC2  VConn closed on CC2 Ack
  * @retval Component status
  */
int32_t TCPP0203_GetVConnSwitchAck(TCPP0203_Object_t *pObj, uint8_t *pVConnSwitchAck)
{
  int32_t ret;
  uint8_t tmp;

  ret = tcpp0203_read_reg(&pObj->Ctx, TCPP0203_ACK_REG, &tmp, 1);
  *pVConnSwitchAck = (tmp & TCPP0203_VCONN_SWITCH_ACK_MSK);

  return ret;
}

/**
  * @brief  Get Gate Driver Provider Ack value
  * @param  pObj Pointer to component object
  * @param  pGateDriverProviderAck Pointer on Gate Driver Provider Ack value
  *         This output parameter can be one of the following values:
  *          @arg TCPP0203_GD_PROVIDER_SWITCH_ACK_OPEN    Gate Driver Provider Open Ack
  *          @arg TCPP0203_GD_PROVIDER_SWITCH_ACK_CLOSED  Gate Driver Provider Closed Ack
  * @retval Component status
  */
int32_t TCPP0203_GetGateDriverProviderAck(TCPP0203_Object_t *pObj, uint8_t *pGateDriverProviderAck)
{
  int32_t ret;
  uint8_t tmp;

  ret = tcpp0203_read_reg(&pObj->Ctx, TCPP0203_ACK_REG, &tmp, 1);
  *pGateDriverProviderAck = (tmp & TCPP0203_GD_PROVIDER_SWITCH_ACK_MSK);

  return ret;
}

/**
  * @brief  Get Gate Driver Consumer Ack value
  * @param  pObj Pointer to component object
  * @param  pGateDriverConsumerAck Pointer on Gate Driver Consumer Ack value
  *         This output parameter can be one of the following values:
  *          @arg TCPP0203_GD_CONSUMER_SWITCH_ACK_OPEN    Gate Driver Consumer Open Ack
  *          @arg TCPP0203_GD_CONSUMER_SWITCH_ACK_CLOSED  Gate Driver Consumer Closed Ack
  * @retval Component status
  */
int32_t TCPP0203_GetGateDriverConsumerAck(TCPP0203_Object_t *pObj, uint8_t *pGateDriverConsumerAck)
{
  int32_t ret;
  uint8_t tmp;

  /* Check if TCPP type is TCPP03. Otherwise, return error */
  if (pObj->DeviceType != TCPP0203_DEVICE_TYPE_03)
  {
    return (TCPP0203_ERROR);
  }

  ret = tcpp0203_read_reg(&pObj->Ctx, TCPP0203_ACK_REG, &tmp, 1);
  *pGateDriverConsumerAck = (tmp & TCPP0203_GD_CONSUMER_SWITCH_ACK_MSK);

  return ret;
}

/**
  * @brief  Get Power Mode Ack value
  * @param  pObj Pointer to component object
  * @param  pPowerModeAck Pointer on Power Mode Ack value
  *         This output parameter can be one of the following values:
  *          @arg TCPP0203_POWER_MODE_ACK_HIBERNATE  Power Mode Hibernate Ack
  *          @arg TCPP0203_POWER_MODE_ACK_LOWPOWER   Power Mode Low Power Ack
  *          @arg TCPP0203_POWER_MODE_ACK_NORMAL     Power Mode Normal Ack
  * @retval Component status
  */
int32_t TCPP0203_GetPowerModeAck(TCPP0203_Object_t *pObj, uint8_t *pPowerModeAck)
{
  int32_t ret;
  uint8_t tmp;

  ret = tcpp0203_read_reg(&pObj->Ctx, TCPP0203_ACK_REG, &tmp, 1);
  *pPowerModeAck = (tmp & TCPP0203_POWER_MODE_ACK_MSK);

  return ret;
}

/**
  * @brief  Get VBUS Discharge Ack value
  * @param  pObj Pointer to component object
  * @param  pVBusDischargeAck Pointer on VBUS Discharge Ack value
  *         This output parameter can be one of the following values:
  *          @arg TCPP0203_VBUS_DISCHARGE_ACK_OFF   VBUS Discharge Off Ack
  *          @arg TCPP0203_VBUS_DISCHARGE_ACK_ON    VBUS Discharge On Ack
  * @retval Component status
  */
int32_t TCPP0203_GetVBusDischargeAck(TCPP0203_Object_t *pObj, uint8_t *pVBusDischargeAck)
{
  int32_t ret;
  uint8_t tmp;

  ret = tcpp0203_read_reg(&pObj->Ctx, TCPP0203_ACK_REG, &tmp, 1);
  *pVBusDischargeAck = (tmp & TCPP0203_VBUS_DISCHARGE_ACK_MSK);

  return ret;
}

/**
  * @brief  Get VConn Discharge Ack value
  * @param  pObj Pointer to component object
  * @param  pVConnDischargeAck Pointer on VConn Discharge Ack value
  *         This output parameter can be one of the following values:
  *          @arg TCPP0203_VCONN_DISCHARGE_ACK_OFF   VConn Discharge Off Ack
  *          @arg TCPP0203_VCONN_DISCHARGE_ACK_ON    VConn Discharge On Ack
  * @retval Component status
  */
int32_t TCPP0203_GetVConnDischargeAck(TCPP0203_Object_t *pObj, uint8_t *pVConnDischargeAck)
{
  int32_t ret;
  uint8_t tmp;

  ret = tcpp0203_read_reg(&pObj->Ctx, TCPP0203_ACK_REG, &tmp, 1);
  *pVConnDischargeAck = (tmp & TCPP0203_VCONN_DISCHARGE_ACK_MSK);

  return ret;
}

/**
  * @brief  Get OCP VConn Flag value
  * @param  pObj Pointer to component object
  * @param  pOCPVConnFlag Pointer on OCP VConn Flag value
  *         This output parameter can be one of the following values:
  *          @arg TCPP0203_FLAG_OCP_VCONN_RESET   OCP VConn flag not set
  *          @arg TCPP0203_FLAG_OCP_VCONN_SET     OCP VConn flag set
  * @retval Component status
  */
int32_t TCPP0203_GetOCPVConnFlag(TCPP0203_Object_t *pObj, uint8_t *pOCPVConnFlag)
{
  int32_t ret;
  uint8_t tmp;

  ret = tcpp0203_read_reg(&pObj->Ctx, TCPP0203_FLAG_REG, &tmp, 1);
  *pOCPVConnFlag = (tmp & TCPP0203_FLAG_OCP_VCONN_MSK);

  return ret;
}

/**
  * @brief  Get OCP VBUS Flag value
  * @param  pObj Pointer to component object
  * @param  GetOCPVBusFlag Pointer on OCP VBUS Flag value
  *         This output parameter can be one of the following values:
  *          @arg TCPP0203_FLAG_OCP_VBUS_RESET   OCP VBUS flag not set
  *          @arg TCPP0203_FLAG_OCP_VBUS_SET     OCP VBUS flag set
  * @retval Component status
  */
int32_t TCPP0203_GetOCPVBusFlag(TCPP0203_Object_t *pObj, uint8_t *GetOCPVBusFlag)
{
  int32_t ret;
  uint8_t tmp;

  ret = tcpp0203_read_reg(&pObj->Ctx, TCPP0203_FLAG_REG, &tmp, 1);
  *GetOCPVBusFlag = (tmp & TCPP0203_FLAG_OCP_VBUS_MSK);

  return ret;
}

/**
  * @brief  Get OVP VBUS Flag value
  * @param  pObj Pointer to component object
  * @param  OVPVBusFlag Pointer on OVP VBUS Flag value
  *         This output parameter can be one of the following values:
  *          @arg TCPP0203_FLAG_OVP_VBUS_RESET   OVP VBUS flag not set
  *          @arg TCPP0203_FLAG_OVP_VBUS_SET     OVP VBUS flag set
  * @retval Component status
  */
int32_t TCPP0203_GetOVPVBusFlag(TCPP0203_Object_t *pObj, uint8_t *OVPVBusFlag)
{
  int32_t ret;
  uint8_t tmp;

  ret = tcpp0203_read_reg(&pObj->Ctx, TCPP0203_FLAG_REG, &tmp, 1);
  *OVPVBusFlag = (tmp & TCPP0203_FLAG_OVP_VBUS_MSK);

  return ret;
}

/**
  * @brief  Get OVP CC Flag value
  * @param  pObj Pointer to component object
  * @param  OVPCCFlag Pointer on OVP CC Flag value
  *         This output parameter can be one of the following values:
  *          @arg TCPP0203_FLAG_OVP_CC_RESET   OVP CC flag not set
  *          @arg TCPP0203_FLAG_OVP_CC_SET     OVP CC flag set
  * @retval Component status
  */
int32_t TCPP0203_GetOVPCCFlag(TCPP0203_Object_t *pObj, uint8_t *OVPCCFlag)
{
  int32_t ret;
  uint8_t tmp;

  ret = tcpp0203_read_reg(&pObj->Ctx, TCPP0203_FLAG_REG, &tmp, 1);
  *OVPCCFlag = (tmp & TCPP0203_FLAG_OVP_CC_MSK);

  return ret;
}

/**
  * @brief  Get Over Temperature Flag value
  * @param  pObj Pointer to component object
  * @param  OTPFlag Pointer on Over Temperature Flag value
  *         This output parameter can be one of the following values:
  *          @arg TCPP0203_FLAG_OTP_RESET   Over Temperature flag not set
  *          @arg TCPP0203_FLAG_OTP_SET     Over Temperature flag set
  * @retval Component status
  */
int32_t TCPP0203_GetOTPFlag(TCPP0203_Object_t *pObj, uint8_t *OTPFlag)
{
  int32_t ret;
  uint8_t tmp;

  ret = tcpp0203_read_reg(&pObj->Ctx, TCPP0203_FLAG_REG, &tmp, 1);
  *OTPFlag = (tmp & TCPP0203_FLAG_OTP_MSK);

  return ret;
}

/**
  * @brief  Get VBUS OK Flag value
  * @param  pObj Pointer to component object
  * @param  VBusOkFlag Pointer on VBUS OK Flag value
  *         This output parameter can be one of the following values:
  *          @arg TCPP0203_FLAG_VBUS_OK_RESET   VBUS OK flag not set
  *          @arg TCPP0203_FLAG_VBUS_OK_SET     VBUS OK flag set
  * @retval Component status
  */
int32_t TCPP0203_GetVBusOkFlag(TCPP0203_Object_t *pObj, uint8_t *VBusOkFlag)
{
  int32_t ret;
  uint8_t tmp;

  ret = tcpp0203_read_reg(&pObj->Ctx, TCPP0203_FLAG_REG, &tmp, 1);
  *VBusOkFlag = (tmp & TCPP0203_FLAG_VBUS_OK_MSK);

  return ret;
}

/**
  * @brief  Get TCPP0203 Device Type value
  * @param  pObj Pointer to component object
  * @param  pTCPPType Pointer on TCPP0203 Device Type value
  *         This output parameter can be one of the following values:
  *          @arg TCPP0203_DEVICE_TYPE_02   TCPP02 Type
  *          @arg TCPP0203_DEVICE_TYPE_03   TCPP03 Type
  * @retval Component status
  */
int32_t TCPP0203_ReadTCPPType(TCPP0203_Object_t *pObj, uint8_t *pTCPPType)
{
  int32_t ret;
  uint8_t tmp;

  ret = tcpp0203_read_reg(&pObj->Ctx, TCPP0203_FLAG_REG, &tmp, 1);
  *pTCPPType = (tmp & TCPP0203_DEVICE_TYPE_MSK);

  return ret;
}

/**
  * @brief  Get VConn Power value
  * @param  pObj Pointer to component object
  * @param  pVCONNPower Pointer on VConn Power value
  *         This output parameter can be one of the following values:
  *          @arg TCPP0203_FLAG_VCONN_PWR_1W       OCP VConn flag not set
  *          @arg TCPP0203_FLAG_VCONN_PWR_0_1W     OCP VConn flag set
  * @retval Component status
  */
int32_t TCPP0203_ReadVCONNPower(TCPP0203_Object_t *pObj, uint8_t *pVCONNPower)
{
  int32_t ret;
  uint8_t tmp;

  ret = tcpp0203_read_reg(&pObj->Ctx, TCPP0203_FLAG_REG, &tmp, 1);
  *pVCONNPower = (tmp & TCPP0203_FLAG_VCONN_PWR_MSK);

  return ret;
}

/**
  * @brief  Set complete Ctrl register value (Reg 0)
  * @param  pObj Pointer to component object
  * @param  pCtrlRegister Pointer on Ctrl register value
  * @retval Component status
  */
int32_t TCPP0203_WriteCtrlRegister(TCPP0203_Object_t *pObj, uint8_t *pCtrlRegister)
{
  int32_t ret;

  /* Update value in writing register (reg0) */
  ret = tcpp0203_write_reg(&pObj->Ctx, TCPP0203_PROG_CTRL, pCtrlRegister, 1);


  return ret;
}

/**
  * @brief  Get complete Ack register value
  * @param  pObj Pointer to component object
  * @param  pAckRegister Pointer on Ack register value
  * @retval Component status
  */
int32_t TCPP0203_ReadAckRegister(TCPP0203_Object_t *pObj, uint8_t *pAckRegister)
{
  int32_t ret;

  ret = tcpp0203_read_reg(&pObj->Ctx, TCPP0203_ACK_REG, pAckRegister, 1);

  return ret;
}

/**
  * @brief  Get complete Flag register value
  * @param  pObj Pointer to component object
  * @param  pFlagRegister Pointer on Flag register value
  * @retval Component status
  */
int32_t TCPP0203_ReadFlagRegister(TCPP0203_Object_t *pObj, uint8_t *pFlagRegister)
{
  int32_t ret;

  ret = tcpp0203_read_reg(&pObj->Ctx, TCPP0203_FLAG_REG, pFlagRegister, 1);

  return ret;
}
/**
  * @} BSP_Components_TCPP0203_Exported_Functions
  */

/******************** Static functions ****************************************/
/** @addtogroup BSP_Components_TCPP0203_Private_Functions
  * @{
  */
/**
  * @brief  Wrap TCPP0203 read function to Bus IO function
  * @param  handle  Component object handle
  * @param  Reg     Target register address to read
  * @param  pData   Buffer where Target register value should be stored
  * @param  Length  buffer size to be read
  * @retval error status
  */
static int32_t TCPP0203_ReadRegWrap(void *handle, uint8_t Reg, uint8_t *pData, uint8_t Length)
{
  TCPP0203_Object_t *pObj = (TCPP0203_Object_t *)handle;

  return pObj->IO.ReadReg(pObj->IO.Address, Reg, pData, Length);
}

/**
  * @brief  Wrap TCPP0203 write function to Bus IO function
  * @param  handle Component object handle
  * @param  Reg    Target register address to write
  * @param  pData  Target register value to be written
  * @param  Length Buffer size to be written
  * @retval error status
  */
static int32_t TCPP0203_WriteRegWrap(void *handle, uint8_t Reg, uint8_t *pData, uint8_t Length)
{
  TCPP0203_Object_t *pObj = (TCPP0203_Object_t *)handle;


  return pObj->IO.WriteReg(pObj->IO.Address, Reg, pData, Length);
}

/**
  * @brief  TCPP0203 register update function to Bus IO function
  * @param  pObj   Pointer to component object
  * @param  Value  Value to be write
  * @param  Mask   Mask to apply
  * @retval error status
  */
static int32_t TCPP0203_ModifyReg0(TCPP0203_Object_t *pObj, uint8_t Value, uint8_t Mask)
{
  int32_t ret;
  uint8_t tmp;

  /* Read current content of ACK register (reflects content of bits set to 1 in Writing register Reg0) */
  ret = tcpp0203_read_reg(&pObj->Ctx, TCPP0203_ACK_REG, &tmp, 1);

  /* Update only the area dedicated to Mask */
  tmp &= ~(Mask);
  tmp |= (Value & Mask);


  /* Update value in writing register (reg0) */
  ret += tcpp0203_write_reg(&pObj->Ctx, TCPP0203_PROG_CTRL, &tmp, 1);


  return ret;
}

/**
  * @} BSP_Components_TCPP0203_Private_Functions
  */

/**
  * @} BSP_Components_TCPP0203
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
