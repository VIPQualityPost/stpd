/**
  ******************************************************************************
  * @file    steval_2stpd01_tcpp02.c
  * @author  SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   TCPP02 component support for STEVAL-2STPD01.
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

/** @addtogroup BSP_STEVAL_2STPD01_TCPP02
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "steval_2stpd01_tcpp02.h"
#include "steval_2stpd01_tcpp02_conf.h"
#include "steval_2stpd01_usbpd_pwr.h"
#include "steval_2stpd01_bus.h"
#include "usbpd_power_monitor.h"

/** @defgroup BSP_STEVAL_2STPD01_TCPP02_Private_Variables Private Variables
  * @{
  */
/**
  * @brief  TCPP02 Port GPIO Configuration
  */
static const BSP_PortPin_Conf_t BSP_TCPP02_GPIO_Conf[USBPD_PWR_INSTANCES_NBR][BSP_TCPP02_GPIO_NUM] = {
  { 
    BSP_GPIO_Def(TCPP02_P0_ENABLE),
    BSP_GPIO_Def(TCPP02_P0_FLG),
  },
  { 
    BSP_GPIO_Def(TCPP02_P1_ENABLE),
    BSP_GPIO_Def(TCPP02_P1_FLG),
  },
};

/**
  * @brief  TCPP02 Port Component object 
  */
TCPP0203_Object_t  BSP_TCPP02_PortCompObj[USBPD_PWR_INSTANCES_NBR] = {0};
/** 
  * @} BSP_STEVAL_2STPD01_TCPP02_Private_Variables
  */

/** @defgroup BSP_STEVAL_2STPD01_TCPP02_Private_Functions Private Functions
  * @brief TCPP02 functions useful locally
  * @{
  */
static void     BSP_TCPP02_GPIO_Init(uint32_t PortNum);
static void     BSP_TCPP02_IT_Init(uint32_t PortNum);
static int32_t  BSP_TCPP02_BUS_Init(uint32_t PortNum, uint16_t Address);
static int32_t  BSP_TCPP02_VBus_DischargeOnPowerOff(uint32_t PortNum);
/** 
  * @} BSP_STEVAL_2STPD01_TCPP02_Private_Functions
  */

/** @defgroup BSP_STEVAL_2STPD01_TCPP02_Exported_Functions Exported Functions
  * @{
  */
/**
  * @brief  Initialize TCPP02 HW resources associated to Type-C port
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  Address I2C address
  * @retval BSP errorcode
  */
int32_t BSP_TCPP02_Init(uint32_t PortNum, uint16_t Address)
{
  BSP_PWR_PORT_ASSERT(PortNum);
  
  int32_t ret = BSP_ERROR_NONE;

  /* Initialize required GPIOs */
  BSP_TCPP02_GPIO_Init(PortNum);
  
  /* Enabling the component */
  BSP_TCPP02_Enable(PortNum, BSP_PWR_ON);

  /* Initialize EXTI for FLGn signal */
  BSP_TCPP02_IT_Init(PortNum);

  /* Initialize bus and component */
  ret = BSP_TCPP02_BUS_Init(PortNum, Address);

  return ret;
}

/**
  * @brief  De-Initialize TCPP02 HW resources associated to Type-C port
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP Status
  */
int32_t BSP_TCPP02_DeInit(uint32_t PortNum)
{
  BSP_PWR_PORT_ASSERT(PortNum);
  UNUSED(PortNum);
  
  /* It's not possible to de-init this part */
  return (BSP_ERROR_NONE);
}

/**
  * @brief  Reset TCPP02 component
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP errorcode
  */
int32_t  BSP_TCPP02_Reset(uint32_t PortNum)
{
  uint8_t flg_reg = 0x18;
  TCPP0203_WriteCtrlRegister(&BSP_TCPP02_PortCompObj[PortNum], &flg_reg);
  
  return BSP_ERROR_NONE;
}

/**
  * @brief  Enable TCPP02 component
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  Enable On / Off
  *         @arg @ref BSP_PWR_ON
  *         @arg @ref BSP_PWR_OFF
  * @retval BSP errorcode
  */
int32_t BSP_TCPP02_Enable(uint32_t PortNum, uint32_t Enable)
{
  /* Check the port */
  BSP_PWR_PORT_ASSERT(PortNum);

  /* Set/REset the corresponding gpio */
  HAL_GPIO_WritePin(BSP_TCPP02_GPIO_Conf[PortNum][BSP_TCPP02_GPIO_ENABLE].Port,
                    BSP_TCPP02_GPIO_Conf[PortNum][BSP_TCPP02_GPIO_ENABLE].Pin,
                    Enable == BSP_PWR_ON ? GPIO_PIN_SET : GPIO_PIN_RESET);

  return BSP_ERROR_NONE;
}

/**
  * @brief  Get the enabling status of the TCPP02 component
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP_PWR_ON | BSP_PWR_OFF or a BSP errorcode
  */
int32_t BSP_TCPP02_IsEnable(uint32_t PortNum)
{
  return HAL_GPIO_ReadPin(BSP_TCPP02_GPIO_Conf[PortNum][BSP_TCPP02_GPIO_ENABLE].Port,
                          BSP_TCPP02_GPIO_Conf[PortNum][BSP_TCPP02_GPIO_ENABLE].Pin) == GPIO_PIN_SET ? BSP_PWR_ON : BSP_PWR_OFF;
}

/**
  * @brief  Set the power mode of TCPP02 component
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  PwrMode @ref USBPD_PWR_PowerModeTypeDef
  *         @arg @ref USBPD_PWR_MODE_OFF
  *         @arg @ref USBPD_PWR_MODE_HIBERNATE
  *         @arg @ref USBPD_PWR_MODE_LOWPOWER
  *         @arg @ref USBPD_PWR_MODE_NORMAL
  * @retval BSP errorcode
  */
int32_t BSP_TCPP02_PowerMode(uint32_t PortNum, USBPD_PWR_PowerModeTypeDef PwrMode)
{
  BSP_PWR_PORT_ASSERT(PortNum);
 
  int32_t ret = BSP_ERROR_NONE;
  
  switch (PwrMode)
  {
  case USBPD_PWR_MODE_OFF:
    BSP_USBPD_PWR_TRACE(PortNum, "-- Mode Off --");
    /* Port should enter OFF mode : Reset TCPP0203 Reg0 */
    if (TCPP0203_Reset(&BSP_TCPP02_PortCompObj[PortNum]) != TCPP0203_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    break;
    
  case USBPD_PWR_MODE_LOWPOWER:
    /* Port Mode is Low Power : suitable while toggling SNK/SRC is case of DRP
    - Dead Batteries Disabled,
    - Gate Driver Consumer or Provider controlled by I2C
    - waiting for VBUS_OK in FLGn (interrupt)
    */
    BSP_USBPD_PWR_TRACE(PortNum, "-- Mode Low Power --");
    if (TCPP0203_SetPowerMode(&BSP_TCPP02_PortCompObj[PortNum], TCPP0203_POWER_MODE_LOWPOWER) != TCPP0203_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    break;
    
  case USBPD_PWR_MODE_NORMAL:
    /* Port Mode is Normal : No power saving measure (Explicit contract in place) */
    BSP_USBPD_PWR_TRACE(PortNum, "-- Mode Normal --");
    if (TCPP0203_SetPowerMode(&BSP_TCPP02_PortCompObj[PortNum], TCPP0203_POWER_MODE_NORMAL) != TCPP0203_OK)
    {
      ret = BSP_ERROR_COMPONENT_FAILURE;
    }
    break;
    
  default:
    ret = BSP_ERROR_WRONG_PARAM;
    break;
  }
  
  return ret;
}

/**
  * @brief  Handle Event callback
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  */
    
void BSP_TCPP02_EventCallback(uint32_t PortNum)
{
  BSP_USBPD_PWR_TRACE(PortNum, "Event");
  uint8_t flg_reg;

  /* In case of TCPP02, this callback could be called when an event is detected
     on FLGn line */
  if (USBPD_PWR_Port_Handle[PortNum].IsInitialized == 1U)
  {
    /* Read Flags register (FLGn) */
    if (TCPP0203_ReadFlagRegister(&BSP_TCPP02_PortCompObj[PortNum], &flg_reg) == TCPP0203_OK)
    {
#ifdef _TRACE
      char _str[30];
      (void)sprintf(_str, "TCPP02 Reg2 0x%02x", flg_reg);
      BSP_USBPD_PWR_TRACE(PortNum, _str);
#endif /* _TRACE */
      
      /* If FLGn has been set to 0 in LOW POWER or HIBERNATE mode,
         it is expected that a VBUS_OK condition is detected.
         In this case, if any, vBus Detection callback could be called */
      switch (USBPD_PWR_Port_Handle[PortNum].PwrSavingMode)
      {
        case USBPD_PWR_MODE_HIBERNATE:
        {
          if ((flg_reg & TCPP0203_FLAG_VBUS_OK_SET) == TCPP0203_FLAG_VBUS_OK_SET)
          {
            if (USBPD_PWR_Port_Handle[PortNum].VBUSDetectCallback != NULL)
            {
              /* In case of TCPP03 in Hibernate Mode (SNK config),
                 Mode is set to Low power to enable TCPP03 behavior on CC lines */
              /* Switch to Low Power mode */
              (void)BSP_USBPD_PWR_SetPowerMode(PortNum, USBPD_PWR_MODE_LOWPOWER);

              USBPD_PWR_Port_Handle[PortNum].VBUSDetectCallback(PortNum, VBUS_CONNECTED);
            }
          }
          else
          {
            /* In all other cases (NORMAL mode), FLGn signals fault detection events. */
            BSP_USBPD_PWR_TRACE(PortNum, "-- Wrong detection --");
          }
          break;
        }

        case USBPD_PWR_MODE_LOWPOWER:
        {
          if ((flg_reg & TCPP0203_FLAG_VBUS_OK_SET) == TCPP0203_FLAG_VBUS_OK_SET)
          {
            if (USBPD_PWR_Port_Handle[PortNum].VBUSDetectCallback != NULL)
            {
              USBPD_PWR_Port_Handle[PortNum].VBUSDetectCallback(PortNum, VBUS_CONNECTED);
            }
          }
          else
          {
            /* In all other cases (NORMAL mode), FLGn signals fault detection events. */
            BSP_USBPD_PWR_TRACE(PortNum, "-- Wrong detection --");
          }
          break;
        }

        case USBPD_PWR_MODE_NORMAL:
        {
          /* In all other cases (NORMAL mode), FLGn signals fault detection events. */
          /* Check type of error raised in FLGn */
          if ((flg_reg & TCPP0203_FLAG_OCP_VCONN_SET) == TCPP0203_FLAG_OCP_VCONN_SET)
          {
            BSP_USBPD_PWR_TRACE(PortNum, "-- OCP VCONN detected --");
            PM_Notify(PortNum, PM_EV_FAULT_VCONN_OVER_CURRENT, 0, 100);
          }
          if ((flg_reg & TCPP0203_FLAG_OCP_VBUS_SET) == TCPP0203_FLAG_OCP_VBUS_SET)
          {
            BSP_USBPD_PWR_TRACE(PortNum, "-- OCP VBUS detected --");
            PM_Notify(PortNum, PM_EV_FAULT_BUS_OVER_CURRENT, 0, 100);

          }
          if ((flg_reg & TCPP0203_FLAG_OVP_VBUS_SET) == TCPP0203_FLAG_OVP_VBUS_SET)
          {
            BSP_USBPD_PWR_TRACE(PortNum, "-- OVP VBUS detected --");
            PM_Notify(PortNum, PM_EV_FAULT_BUS_OVER_VOLTAGE, 0, 100);
          }
          if ((flg_reg & TCPP0203_FLAG_OTP_SET) == TCPP0203_FLAG_OTP_SET)
          {
            BSP_USBPD_PWR_TRACE(PortNum, "-- Over Temp detected --");
            PM_Notify(PortNum, PM_EV_FAULT_THERMAL, 0, 100);
          }
          if ((flg_reg & TCPP0203_FLAG_OVP_CC_SET) == TCPP0203_FLAG_OVP_CC_SET)
          {
            BSP_USBPD_PWR_TRACE(PortNum, "-- OVP CC detected --");
            PM_Notify(PortNum, PM_EV_FAULT_CC_LINES, 0, 100);
          }
          break;
        }

        default:
        {
          /* Unexpected port state */
          BSP_USBPD_PWR_TRACE(PortNum, "-- Unexpected State for FLGn event --");
          break;
        }
      }
    }
  }
}

/**
  * @brief  Initialize VBUS for TCPP02
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP errorcode
  */
int32_t BSP_TCPP02_VBus_Init(uint32_t PortNum)
{
  /* Check the port */
  BSP_PWR_PORT_ASSERT(PortNum);

  /* nothing */
  return BSP_ERROR_NONE;
}

/**
  * @brief  De-Initialize VBUS for TCPP02
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP errorcode
  */
int32_t BSP_TCPP02_VBus_DeInit(uint32_t PortNum)
{
  /* Check the port */
  BSP_PWR_PORT_ASSERT(PortNum);

  if (TCPP0203_SetGateDriverConsumer(&BSP_TCPP02_PortCompObj[PortNum], TCPP0203_GD_CONSUMER_SWITCH_CLOSED) != TCPP0203_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }
  
  return BSP_ERROR_NONE;
}

/**
  * @brief  Enable / Disable VBUS for TCPP02
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  Enable On / Off
  *         @arg @ref BSP_PWR_ON
  *         @arg @ref BSP_PWR_OFF
  * @retval BSP errorcode
  */
int32_t BSP_TCPP02_VBus(uint32_t PortNum, uint32_t Enable)
{
  /* Check the port */
  BSP_PWR_PORT_ASSERT(PortNum);

  uint32_t switch_status = Enable == BSP_PWR_ON ? TCPP0203_GD_PROVIDER_SWITCH_CLOSED : TCPP0203_GD_PROVIDER_SWITCH_OPEN;

  BSP_USBPD_PWR_TRACE(PortNum, (switch_status ? "-- GDP/GDC setting : SRC Closed --" : "-- GDP/GDC setting : SRC Open --"));

  if (TCPP0203_SetGateDriverProvider(&BSP_TCPP02_PortCompObj[PortNum], switch_status) != TCPP0203_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  /* in case of disable perfom a discharge */
  if (Enable == BSP_PWR_OFF)
  {
    BSP_TCPP02_VBus_DischargeOnPowerOff(PortNum);
  }
  
  return BSP_ERROR_NONE;
}

/**
  * @brief  Get the VBUS status for TCPP02
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval VBus status or BSP errorcode
  */
int32_t  BSP_TCPP02_VBus_Status(uint32_t PortNum)
{
  BSP_PWR_PORT_ASSERT(PortNum);
  
  /* read the status of the load switch into the component */
  uint8_t GateDriverProviderAck;
  if (TCPP0203_GetGateDriverProviderAck(&BSP_TCPP02_PortCompObj[PortNum], &GateDriverProviderAck) != TCPP0203_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }
  
  return GateDriverProviderAck == TCPP0203_GD_PROVIDER_SWITCH_ACK_CLOSED ? BSP_PWR_ON : BSP_PWR_OFF;
}

/**
  * @brief  Enable / Disable VBUS Discharge for TCPP02
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  Discharge On / Off
  *         @arg @ref BSP_PWR_ON
  *         @arg @ref BSP_PWR_OFF
  * @retval BSP errorcode
  */
int32_t BSP_TCPP02_VBus_Discharge(uint32_t PortNum, uint32_t Discharge)
{
  BSP_PWR_PORT_ASSERT(PortNum);
  
  uint8_t discharge = Discharge == BSP_PWR_ON ? TCPP0203_VBUS_DISCHARGE_ON : TCPP0203_VBUS_DISCHARGE_OFF;

  if (Discharge)
  {
    /* Open switch in case of off */
    if (TCPP0203_SetGateDriverProvider(&BSP_TCPP02_PortCompObj[PortNum],
                                       TCPP0203_GD_PROVIDER_SWITCH_OPEN) != TCPP0203_OK)
    {
      return BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  
  /* Drive the discharge */
  if (TCPP0203_SetVBusDischarge(&BSP_TCPP02_PortCompObj[PortNum], discharge) != TCPP0203_OK)
  {
    return  BSP_ERROR_COMPONENT_FAILURE;
  }
  
  return BSP_ERROR_NONE;
}

/**
  * @brief  Initialize VConn for TCPP02
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP errorcode
  */
int32_t BSP_TCPP02_VConn_Init(uint32_t PortNum)
{
  BSP_PWR_PORT_ASSERT(PortNum);
  /* No specific treatment required for initialising VCONN management from TCPP03 */
  /* Initialise VCONN management from TCPP03 :
  Ensure VCONN switch to Open/Open
  Ensure VCONN Discharge OFF
  */
  
  if (TCPP0203_SetVConnSwitch(&BSP_TCPP02_PortCompObj[PortNum], TCPP0203_VCONN_SWITCH_OPEN) != TCPP0203_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }
  if (TCPP0203_SetVConnDischarge(&BSP_TCPP02_PortCompObj[PortNum], TCPP0203_VCONN_DISCHARGE_OFF) != TCPP0203_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }
  return BSP_ERROR_NONE;
}


/**
  * @brief  De-Initialize VConn for TCPP02
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP errorcode
  */
int32_t BSP_TCPP02_VConn_DeInit(uint32_t PortNum)
{
  BSP_PWR_PORT_ASSERT(PortNum);
  /* De-initialise VCONN management from TCPP02 :
  Force VCONN switch to Open/Open
  Force VCONN Discharge OFF
  */
  
  if (TCPP0203_SetVConnSwitch(&BSP_TCPP02_PortCompObj[PortNum], TCPP0203_VCONN_SWITCH_OPEN) != TCPP0203_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }
  if (TCPP0203_SetVConnDischarge(&BSP_TCPP02_PortCompObj[PortNum], TCPP0203_VCONN_DISCHARGE_OFF) != TCPP0203_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }
  return BSP_ERROR_NONE;
}

/**
  * @brief  Switch the VConn to the specified CC line for TCPP02
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  CCPinId CC1 or CC2
  *         @arg @ref USBPD_PWR_TYPE_C_CC1
  *         @arg @ref USBPD_PWR_TYPE_C_CC2
  * @retval BSP errorcode
  */
int32_t BSP_TCPP02_VConn_Switch(uint32_t PortNum, uint32_t CCPinId)
{
  /* Check the port */
  BSP_PWR_PORT_ASSERT(PortNum);
  
  /* check the CC lines */
  if ((CCPinId != USBPD_PWR_TYPE_C_CC1) && (CCPinId != USBPD_PWR_TYPE_C_CC2))
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  
  /* Set TCPP02 VCONN Switch according to CC pin used for VCONN */
  uint8_t vconnswitch = CCPinId == USBPD_PWR_TYPE_C_CC2 ? TCPP0203_VCONN_SWITCH_CC2 : TCPP0203_VCONN_SWITCH_CC1;
  if (TCPP0203_SetVConnSwitch(&BSP_TCPP02_PortCompObj[PortNum], vconnswitch) != TCPP0203_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }
  
  return BSP_ERROR_NONE;
}

/**
  * @brief  Enable / Disable VConn for TCPP02
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  CCPinId CC1 or CC2
  *         @arg @ref USBPD_PWR_TYPE_C_CC1
  *         @arg @ref USBPD_PWR_TYPE_C_CC2
  * @param  Enable On / Off
  *         @arg @ref BSP_PWR_ON
  *         @arg @ref BSP_PWR_OFF
  * @retval BSP errorcode
  */
int32_t BSP_TCPP02_VConn_Enable(uint32_t PortNum, uint32_t CCPinId, uint32_t Enable)
{
  BSP_PWR_PORT_ASSERT(PortNum);
  
  if (USBPD_PWR_TYPE_C_CC1 != CCPinId && USBPD_PWR_TYPE_C_CC2 != CCPinId)
  {
    return BSP_ERROR_WRONG_PARAM;
  }
  
  if (Enable == BSP_PWR_ON)
  {
    uint32_t cc_vconn_param = USBPD_PWR_TYPE_C_CC2 == CCPinId ? TCPP0203_VCONN_SWITCH_CC2 : TCPP0203_VCONN_SWITCH_CC1;
    
    if (TCPP0203_SetVConnSwitch(&BSP_TCPP02_PortCompObj[PortNum], cc_vconn_param) != TCPP0203_OK)
    {
      return BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  else
  {
    /* Switch off VCONN : restore VCONN switch to Open/open position */
    if (TCPP0203_SetVConnSwitch(&BSP_TCPP02_PortCompObj[PortNum], TCPP0203_VCONN_SWITCH_OPEN) != TCPP0203_OK)
    {
      return BSP_ERROR_COMPONENT_FAILURE;
    }
    
    /* Discharge VCONN path */
    if (TCPP0203_SetVConnDischarge(&BSP_TCPP02_PortCompObj[PortNum], TCPP0203_VCONN_DISCHARGE_ON) != TCPP0203_OK)
    {
      return BSP_ERROR_COMPONENT_FAILURE;
    }
    
    /* Wait DISCHARGE is completed then disable VCONN Discharge */
    HAL_Delay(BSP_TCPP02_DIS_FINAL_DELAY); 
    if (TCPP0203_SetVConnDischarge(&BSP_TCPP02_PortCompObj[PortNum], TCPP0203_VCONN_DISCHARGE_OFF) != TCPP0203_OK)
    {
      return BSP_ERROR_COMPONENT_FAILURE;
    }
  }
    
  return BSP_ERROR_NONE;
}

/**
  * @brief  Get the VConn status for TCPP02
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval VConn status or BSP errorcode
  */
int32_t BSP_TCPP02_VConn_Status(uint32_t PortNum)
{
  BSP_PWR_PORT_ASSERT(PortNum);
  uint8_t vconnSwitch;
  int32_t ret;
  
  if (TCPP0203_GetVConnSwitchAck(&BSP_TCPP02_PortCompObj[PortNum], &vconnSwitch) != TCPP0203_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }
  
  if (vconnSwitch == TCPP0203_VCONN_SWITCH_CC1) ret = USBPD_PWR_TYPE_C_CC1;
  if (vconnSwitch == TCPP0203_VCONN_SWITCH_CC2) ret = USBPD_PWR_TYPE_C_CC2;
  
  return ret;
}

/**
  * @brief  Enable / Disable VConn Discharge for TCPP02
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  Discharge On / Off
  *         @arg @ref BSP_PWR_ON
  *         @arg @ref BSP_PWR_OFF
  * @retval BSP errorcode
  */
int32_t BSP_TCPP02_VConn_Discharge(uint32_t PortNum, uint32_t Discharge)
{
  BSP_PWR_PORT_ASSERT(PortNum);
  
  uint8_t discharge = Discharge == BSP_PWR_ON ? TCPP0203_VCONN_DISCHARGE_ON : TCPP0203_VCONN_DISCHARGE_OFF;

  if (Discharge)
  {
    /* Open switch in case of on */
    if (TCPP0203_SetVConnSwitch(&BSP_TCPP02_PortCompObj[PortNum], TCPP0203_VCONN_SWITCH_OPEN) != TCPP0203_OK)
    {
      return BSP_ERROR_COMPONENT_FAILURE;
    }
    
  }
  
  /* Drive the discharge */
  if (TCPP0203_SetVConnDischarge(&BSP_TCPP02_PortCompObj[PortNum], discharge) != TCPP0203_OK)
  {
    return  BSP_ERROR_COMPONENT_FAILURE;
  }
  
  return BSP_ERROR_NONE;
}

/** 
  * @} BSP_STEVAL_2STPD01_TCPP02_Exported_Functions
  */

/** @addtogroup BSP_STEVAL_2STPD01_TCPP02_Private_Functions Private Functions
  * @{
  */

/**
  * @brief  Configure TCPP02 used GPIO.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval None
  */
static void BSP_TCPP02_GPIO_Init(uint32_t PortNum)
{
  BSP_PWR_PORT_ASSERT(PortNum);

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  
  /*Configure GPIO pins */
  for(uint8_t idx = 0; idx < BSP_TCPP02_GPIO_NUM; idx++)
  {
    if (USBPD_PWR_INVALID_VALUE == BSP_TCPP02_GPIO_Conf[PortNum][idx].Mode)
    {
      continue;
    }

    GPIO_InitStruct.Pin = BSP_TCPP02_GPIO_Conf[PortNum][idx].Pin;
    GPIO_InitStruct.Mode = BSP_TCPP02_GPIO_Conf[PortNum][idx].Mode;
    GPIO_InitStruct.Pull = BSP_TCPP02_GPIO_Conf[PortNum][idx].PuPd;
    HAL_GPIO_Init(BSP_TCPP02_GPIO_Conf[PortNum][idx].Port, &GPIO_InitStruct);
  }
}

/**
  * @brief  Configure TCPP02 low level interrupt.
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval None
  */
static void BSP_TCPP02_IT_Init(uint32_t PortNum)
{
  BSP_PWR_PORT_ASSERT(PortNum);
  
  IRQn_Type irq_array[BSP_TCPP02_IRQ_NUM] = BSP_TCPP02_IRQ;
  
  for(uint8_t i = 0; i < BSP_TCPP02_IRQ_NUM; i++)
  {
    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(irq_array[i], 0, 0);
    HAL_NVIC_EnableIRQ(irq_array[i]);
  }
}

/**
  * @brief  I2C BUS regsitration for TCPP02 communication
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  Address I2C address
  * @retval BSP errorcode
  */
static int32_t BSP_TCPP02_BUS_Init(uint32_t PortNum, uint16_t Address)
{
  TCPP0203_IO_t            TCPP02_IO_Ctx;
  uint8_t                  tcpp_type;

  /* Configure the TCPP0203 I2C driver */
  TCPP02_IO_Ctx.Address     = Address;
  TCPP02_IO_Ctx.Init        = BSP_BUS_I2C_Init;
  TCPP02_IO_Ctx.DeInit      = BSP_BUS_I2C_DeInit;
  TCPP02_IO_Ctx.ReadReg     = BSP_BUS_I2C_ReadReg;
  TCPP02_IO_Ctx.WriteReg    = BSP_BUS_I2C_WriteReg;
  TCPP02_IO_Ctx.GetTick     = BSP_BUS_GetTick;

  /* Initialization */
  BSP_TCPP02_PortCompObj[PortNum].PortNum       = PortNum;
  BSP_TCPP02_PortCompObj[PortNum].DeviceType    = TCPP0203_DEVICE_TYPE_NA;
  BSP_TCPP02_PortCompObj[PortNum].IsInitialized = 0;

  /* Register the component on BUS IO */
  if (TCPP0203_RegisterBusIO(&BSP_TCPP02_PortCompObj[PortNum], &TCPP02_IO_Ctx) != TCPP0203_OK)
  {
    return BSP_ERROR_BUS_FAILURE;
  }

  /* Initialisation step */
  if (TCPP0203_Init(&BSP_TCPP02_PortCompObj[PortNum]) != TCPP0203_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }
  
  /* Read the component type */
  if (TCPP0203_ReadTCPPType(&BSP_TCPP02_PortCompObj[PortNum], &tcpp_type) != TCPP0203_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }
  
  /* Check the component type */
  if (tcpp_type != TCPP0203_DEVICE_TYPE_02)
  {
    return BSP_ERROR_WRONG_COMPONENT;
  }

  /* Reset TCPP02 Reg0 register */
  if (TCPP0203_Reset(&BSP_TCPP02_PortCompObj[PortNum]) != TCPP0203_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }
  
  return BSP_ERROR_NONE;
}

/**
  * @brief  Perform a VBUS discharge
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP errorcode
  */
int32_t BSP_TCPP02_VBus_DischargeOnPowerOff(uint32_t PortNum)
{
  uint32_t vbus;
  uint32_t vbus_target = USBPD_PWR_LOW_VBUS_THRESHOLD - 100u;
  uint16_t dis_counter;
  
  /* first VBus reading */
  BSP_USBPD_PWR_VBUSGetVoltage(PortNum, &vbus);
  
  if (vbus > vbus_target)
  {
#if defined (_TRACE)
    uint8_t str[40];
#endif /* _TRACE */

    /* Set Discharge On */
    TCPP0203_SetVBusDischarge(&BSP_TCPP02_PortCompObj[PortNum], TCPP0203_VBUS_DISCHARGE_ON);
    
#ifdef _TRACE
    sprintf((char *)str, "Discharge VBUS - Start:%05d", vbus);
    USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PortNum, 0U, (uint8_t *)str, 8);
#endif /* _TRACE */
    
    while (1)
    {
      BSP_USBPD_PWR_VBUSGetVoltage(PortNum, &vbus);
      
      if (vbus <= vbus_target) break;
      if (dis_counter >= BSP_TCPP02_DIS_NUM) break;
      
      HAL_Delay(BSP_TCPP02_DIS_DELAY);
      dis_counter++;
    }
    
    HAL_Delay(BSP_TCPP02_DIS_FINAL_DELAY);
    
    /* Set Discharge Off */
    if (TCPP0203_SetVBusDischarge(&BSP_TCPP02_PortCompObj[PortNum], TCPP0203_VBUS_DISCHARGE_OFF) != TCPP0203_OK)
    {
      return BSP_ERROR_COMPONENT_FAILURE;
    }
  }
  
  return BSP_ERROR_NONE;
}
/**
  * @} BSP_STEVAL_2STPD01_TCPP02_Private_Functions
  */

/**
  * @} BSP_STEVAL_2STPD01_TCPP02
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
