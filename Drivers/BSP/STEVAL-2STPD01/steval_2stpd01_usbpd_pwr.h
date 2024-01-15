/**
  ******************************************************************************
  * @file    steval_2stpd01_usbpd_pwr.h
  * @author  MCD Application Team and SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   This file contains definitions USBPD Power resources.
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
#ifndef STEVAL_2STPD01_USBPD_PWR_H
#define STEVAL_2STPD01_USBPD_PWR_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP_STEVAL_2STPD01
  * @{
  */ 
   
/** @defgroup BSP_STEVAL_2STPD01_USBPD_PWR USBPD Power
  * @{
  */
  
/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_system.h"

#include "steval_2stpd01_errno.h"
#include "steval_2stpd01_conf.h"

#if  defined(_TRACE)
#include "usbpd_core.h"
#include "usbpd_trace.h"
#include "string.h"
#ifndef _STDIO
#include "stdio.h"
#endif /* _STDIO */
#endif /* _TRACE */

/** @defgroup BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Types Exported Types
  * @{
  */
/**
  * @brief  Power role
  */
typedef enum
{
  POWER_ROLE_SOURCE = 0,
  POWER_ROLE_SINK,
  POWER_ROLE_DUAL
} USBPD_PWR_PowerRoleTypeDef;

/**
  * @brief  Low Power Mode of TypeC ports
  */
typedef enum
{
  USBPD_PWR_MODE_OFF = 0,
  USBPD_PWR_MODE_HIBERNATE,
  USBPD_PWR_MODE_LOWPOWER,
  USBPD_PWR_MODE_NORMAL
} USBPD_PWR_PowerModeTypeDef;

/**
  * @brief  VBUS connection status
  */
typedef enum
{
  VBUS_CONNECTED = 0,
  VBUS_NOT_CONNECTED
} USBPD_PWR_VBUSConnectionStatusTypeDef;

/**
  * @brief  Port HW configuration description
  */
typedef struct
{
  uint8_t   TCPP02_Address;    /*!< TCPP02 I2C address (if any) associated to Port  */
  uint8_t   STPD01_Address;    /*!< STPD01 I2C address (if any) associated to Port  */
} USBPD_PWR_PortConfig_t;

/**
  * @brief VBUS Detection Callback
  */
typedef void USBPD_PWR_VBUSDetectCallbackFunc(uint32_t PortNum,
                                              USBPD_PWR_VBUSConnectionStatusTypeDef VBUSConnectionStatus);
/**
  * @brief  Port BSP Power status
  */
typedef struct
{
  uint8_t                           IsInitialized;     /*!< Port Context Initialization status       */
  uint8_t                           IsItEnabled;       /*!< Port IT enabled status                   */
  USBPD_PWR_PowerRoleTypeDef        PwrRole;           /*!< Port current power role (SRC or SNK)     */
  USBPD_PWR_PowerModeTypeDef        PwrSavingMode;     /*!< Port current power saving mode           */
  USBPD_PWR_VBUSDetectCallbackFunc *VBUSDetectCallback;/*!< Port callback for VBUS detection event   */
  USBPD_PWR_PortConfig_t            Config;            /*!< Port configuration                       */
} USBPD_PWR_PortHandle_t;

/**
  * @} BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Types
  */

/** @defgroup BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Defines Exported Defines
  * @{
  */

/**
  * @brief  Number of TypeC ports
  */
#define USBPD_PWR_INSTANCES_NBR           (2U)             /*!< Number of USBC instance  */
#define USBPD_PWR_TYPE_C_PORT_1           (0U)             /*!< Type-C port 0 identifier */
#define USBPD_PWR_TYPE_C_PORT_2           (1U)             /*!< Type-C port 1 identifier */
#define USBPD_PWR_TYPE_C_CC1              (1U)             /*!< CC1 pin identifier       */
#define USBPD_PWR_TYPE_C_CC2              (2U)             /*!< CC2 pin identifier       */

/**
  * @brief  
  */
#define USBPD_PWR_HIGH_VBUS_THRESHOLD     (2800U)          /*!< VBUS disconnection high threshold value (mV) */
#define USBPD_PWR_LOW_VBUS_THRESHOLD      (750U)           /*!< VBUS disconnection low  threshold value (mV) */

#define USBPD_PWR_VBUS_THRESHOLD_5V       (3900U)          /*!< VBUS disconnection 5V threshold value (mV)   */
#define USBPD_PWR_VBUS_THRESHOLD_9V       (7000U)          /*!< VBUS disconnection 9V threshold value (mV)   */
#define USBPD_PWR_VBUS_THRESHOLD_15V      (12500U)         /*!< VBUS disconnection 15V threshold value (mV)  */
#define USBPD_PWR_VBUS_THRESHOLD_20V      (17000U)         /*!< VBUS disconnection 20V threshold value (mV)  */
#define USBPD_PWR_VBUS_THRESHOLD_APDO     (2150U)          /*!< VBUS disconnection APDO threshold value (mV) */

/**
  * @brief  power timeout
  */
#define USBPD_PWR_TIMEOUT_PDO             250U             /*!< Timeout for PDO to PDO or PDO to APDO at 250ms */
#define USBPD_PWR_TIMEOUT_APDO            25U              /*!< Timeout for APDO to APDO at 25ms               */

#define USBPD_PWR_INVALID_VALUE           0xFFFFFFFFU      /*!< Invalid value set during issue with voltage setting */

#define BSP_PWR_OFF                       0                /*!< Power off value */
#define BSP_PWR_ON                        1                /*!< Power on value  */
#define BSP_PWR_VSAFE                     5000             /*!< VSafe value     */
/**
  * @} BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Defines
  */

/** @defgroup BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Macros Exported Macros
  * @{
  */
/**
  * @brief  Power Trace definition
  */
#ifdef _TRACE
#define BSP_USBPD_PWR_TRACE(_PORT_,_MSG_) USBPD_TRACE_Add(USBPD_TRACE_DEBUG, (uint8_t)_PORT_, 0U , (uint8_t *)_MSG_,\
                                                          strlen(_MSG_));
#else
#define BSP_USBPD_PWR_TRACE(_PORT_,_MSG_)
#endif /* _TRACE */

/**
  * @brief  Check the port validity
  */
#define BSP_PWR_PORT_IS_VALID(PortNum) ((PortNum) < USBPD_PWR_INSTANCES_NBR)
/**
  * @brief  Check the port validity and in case of error return @ref BSP_ERROR_WRONG_PARAM
  */
#define BSP_PWR_PORT_CHECK_RET(PortNum) { if (!BSP_PWR_PORT_IS_VALID(PortNum)) return BSP_ERROR_WRONG_PARAM; }
/**
  * @brief  Assert the port validity
  */
#define BSP_PWR_PORT_ASSERT(PortNum) (assert_param(BSP_PWR_PORT_IS_VALID(PortNum)))

/**
  * @brief  Evaluate the correct component result (ok or error)
  */
#define BSP_COMPONENT_CHECK(ret_tcpp02, ret_stpd01) (ret_tcpp02 >= BSP_ERROR_NONE && ret_stpd01 >= BSP_ERROR_NONE ? BSP_ERROR_NONE : (ret_stpd01 >= BSP_ERROR_NONE ? BSP_ERROR_TCPP02_COMPONENT_FAILURE : BSP_ERROR_STPD01_COMPONENT_FAILURE) )
/**
  * @brief  Return in case of failure of a component
  */
#define BSP_COMPONENT_CHECK_RET(ret_tcpp02, ret_stpd01) { \
                                                            int32_t __local_ret = BSP_COMPONENT_CHECK(ret_tcpp02, ret_stpd01); \
                                                            if (BSP_ERROR_NONE != __local_ret) return __local_ret; \
                                                        }
  
/**
  * @} BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Macros
  */

/** @defgroup BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Variables Exported Variables
  * @{
  */
extern USBPD_PWR_PortHandle_t USBPD_PWR_Port_Handle[];
/**
  * @} BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Variables
  */

/** @defgroup BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions Exported Functions
  * @{
  */
/** @defgroup BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_Common Common
  * @{
  */
int32_t BSP_USBPD_PWR_Init(uint32_t PortNum);
int32_t BSP_USBPD_PWR_DeInit(uint32_t PortNum);
int32_t BSP_USBPD_PWR_EnablePort(uint32_t PortNum, uint32_t Enable);
int32_t BSP_USBPD_PWR_SetRole(uint32_t PortNum, USBPD_PWR_PowerRoleTypeDef Role);
int32_t BSP_USBPD_PWR_SetPowerMode(uint32_t PortNum, USBPD_PWR_PowerModeTypeDef PwrMode);
int32_t BSP_USBPD_PWR_GetPowerMode(uint32_t PortNum, USBPD_PWR_PowerModeTypeDef *PwrMode);
void BSP_USBPD_PWR_EventCallback(uint32_t PortNum);
/**
  * @} BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_Common
  */

/** @defgroup BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_VBUS VBUS Management
  * @{
  */
int32_t BSP_USBPD_PWR_VBUSInit(uint32_t PortNum);
int32_t BSP_USBPD_PWR_VBUSDeInit(uint32_t PortNum);

int32_t BSP_USBPD_PWR_VBUS(uint32_t PortNum, uint32_t Enable);
int32_t BSP_USBPD_PWR_VBUSOn(uint32_t PortNum);
int32_t BSP_USBPD_PWR_VBUSOff(uint32_t PortNum);
int32_t BSP_USBPD_PWR_VBUSIsOn(uint32_t PortNum, uint8_t *pState);
int32_t BSP_USBPD_PWR_VBUSWaitValue(uint32_t PortNum, 
                                    uint32_t VBusTarget, 
                                    uint32_t Delta, 
                                    uint16_t Timeout, 
                                    uint16_t Delay, 
                                    void (*pWaitForTime)(uint32_t));

int32_t BSP_USBPD_PWR_VBUSSetVoltage_Fixed(uint32_t PortNum,
                                           uint32_t VbusTargetInmv,
                                           uint32_t OperatingCurrent,
                                           uint32_t MaxOperatingCurrent);

int32_t BSP_USBPD_PWR_VBUSSetVoltage_Variable(uint32_t PortNum,
                                              uint32_t VbusTargetMaxInmv,
                                              uint32_t VbusTargetMinInmv,
                                              uint32_t OperatingCurrent,
                                              uint32_t MaxOperatingCurrent);

int32_t BSP_USBPD_PWR_VBUSSetVoltage_Battery(uint32_t PortNum,
                                             uint32_t VbusTargetMin,
                                             uint32_t VbusTargetMax,
                                             uint32_t OperatingPower,
                                             uint32_t MaxOperatingPower);

int32_t BSP_USBPD_PWR_VBUSSetVoltage_APDO(uint32_t PortNum,
                                          uint32_t VbusTargetInmv,
                                          uint32_t OperatingCurrent,
                                          int32_t Delta);

int32_t BSP_USBPD_PWR_SetVBUSDisconnectionThreshold(uint32_t PortNum,
                                                    uint32_t VoltageThreshold);

int32_t BSP_USBPD_PWR_RegisterVBUSDetectCallback(uint32_t PortNum,
                                                 USBPD_PWR_VBUSDetectCallbackFunc *pfnVBUSDetectCallback);

int32_t BSP_USBPD_PWR_VBUSGetVoltage(uint32_t PortNum, uint32_t *pVoltage);
int32_t BSP_USBPD_PWR_VBUSGetCurrent(uint32_t PortNum, int32_t *pCurrent);

/** @defgroup BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_VBUS_Discharge Discharge
  * @{
  */
int32_t BSP_USBPD_PWR_VBUSDischarge(uint32_t PortNum, uint32_t Enable);
int32_t BSP_USBPD_PWR_VBUSDischargeOn(uint32_t PortNum);
int32_t BSP_USBPD_PWR_VBUSDischargeOff(uint32_t PortNum);
/**
  * @} BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_VBUS_Discharge
  */
/**
  * @} BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_VBUS
  */

/** @defgroup BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_VCONN VConn Management
  * @{
  */
int32_t BSP_USBPD_PWR_VCONNInit(uint32_t PortNum, uint32_t CCPinId);
int32_t BSP_USBPD_PWR_VCONNDeInit(uint32_t PortNum, uint32_t CCPinId);
int32_t BSP_USBPD_PWR_VCONNOn(uint32_t PortNum, uint32_t CCPinId);
int32_t BSP_USBPD_PWR_VCONNOff(uint32_t PortNum, uint32_t CCPinId);

int32_t BSP_USBPD_PWR_VCONNIsOn(uint32_t PortNum, uint32_t CCPinId, uint8_t *pState);

/** @defgroup BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_VCONN_Discharge Discharge
  * @{
  */
int32_t BSP_USBPD_PWR_VCONNDischargeOn(uint32_t PortNum);
int32_t BSP_USBPD_PWR_VCONNDischargeOff(uint32_t PortNum);
/**
  * @} BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_VCONN_Discharge
  */
/**
  * @} BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions_VCONN
  */


/**
  * @} BSP_STEVAL_2STPD01_USBPD_PWR_Exported_Functions
  */

/**
  * @} BSP_STEVAL_2STPD01_USBPD_PWR
  */

/**
  * @} BSP_STEVAL_2STPD01
  */

#ifdef __cplusplus
}
#endif

#endif /* STEVAL_2STPD01_USBPD_PWR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
