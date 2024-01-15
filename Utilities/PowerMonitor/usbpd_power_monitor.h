/**
  ******************************************************************************
  * @file    usbpd_power_monitor.h
  * @version 1.0.0
  * @date    June, 30th 2021
  * @author  S&C USBPD Team
  * @brief   Header file for usbpd_power_monitor.c file for STEVAL-2STPD01
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBPD_POWER_MONITOR_H
#define __USBPD_POWER_MONITOR_H

#ifdef __cplusplus
extern "C" {
#endif
  
/** @addtogroup UTILITIES
  * @{
  */

/** @defgroup USBPD_POWER_MONITOR Power Monitor Module
  * @brief The power monitor checks the status of the power and forward the data.
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usbpd_def.h"
 
/* Exported define -----------------------------------------------------------*/ 
/** @defgroup USBPD_POWER_MONITOR_Exported_Defines Exported Defines
  * @{
  */
/* Exported constants --------------------------------------------------------*/
#define PM_PORT_NA 99
#define PM_PORT_0 USBPD_PORT_0
#define PM_PORT_1 USBPD_PORT_1
#define PM_PORT_COUNT USBPD_PORT_COUNT

/* Exported macro ------------------------------------------------------------*/
/** @brief Event code macro
  */
#define PM_EVENT_ITEM(eventtype, eventcode) (((eventcode) & 0xFF) | (eventtype & 0x07)<<5) 
/** 
  * @} USBPD_POWER_MONITOR_Exported_Defines
  */

/* Exported typedef ----------------------------------------------------------*/
/** @defgroup USBPD_POWER_MONITOR_Exported_Typedef Exported Typedef
  * @{
  */
/** @brief Type of event notified by external 
  */
typedef enum 
{
  PM_EV_TYPE_NONE    = 0x00, /*!< No type */
  PM_EV_TYPE_SERVICE = 0x01, /*!< Service type event */
  PM_EV_TYPE_NOTIF   = 0x02, /*!< Notification type event */
  PM_EV_TYPE_FAULT   = 0x03, /*!< Fault type event */
} PM_EventType_Typedef;

/** @brief Status condition returned from API
  */
typedef enum
{
  PM_STATUS_ERR = 0,         /*!< Error status condition */
  PM_STATUS_OK  = 1,         /*!< Ok status condition */
  PM_STATUS_NA  = 2,         /*!< Not applicable */
} PM_Status_TypeDef;

/** @brief Data handle
  * @note The data are continously updated by the control task
  */
typedef struct
{
  uint8_t PortNum;            /*!< Port number of the data */
  uint32_t VBus;              /*!< Last read voltage (mV) */
  int32_t IBus;               /*!< Last read current (mA) */
  uint8_t Transition;         /*!< Transition on-going */
} PM_Data_Typedef;            

/** @brief Base/generic parameter structure
  */
typedef struct
{
    uint32_t Value:22;        /*!< Parameter value */
    uint32_t Reserved:10;     /*!< Reserved and not used bits (to pad 32-bit) */
} PM_Param_Base_Typedef;

/** @brief Connection parameter structure
  * @note This structure is used to exchange notification parameters
  */
typedef struct
{
    uint32_t PortNum:4;       /*!< Port number */
    uint32_t Role:4;          /*!< Role (normally is Source) */
    uint32_t Connected:1;     /*!< Port pair is connected to type-C port */
    uint32_t Explicit:1;      /*!< Received a explicit contract with pair port */
    uint32_t CCLine:2;        /*!< cc-line used for communication */
    uint32_t ConnReserved:10; /*!< Reserved bits of the connection structure */
    uint32_t Reserved:10;     /*!< Reserved bits of the parameter strcuture */
} PM_Param_Connection_Typedef;

/** @brief Data parameter structure
  * @note This structure is used to exchange data parameters
  */
typedef struct
{
    uint32_t Voltage:11;      /*!< 10mV step unsigned [0, 20.47] V */
    int32_t Current:11;       /*!< 10mA step signed   ]-10.24, 10.24] A */
    uint32_t Reserved:10;     /*!< Reserved bits of the parameter strcuture */
} PM_Param_Data_Typedef;

/** @brief Union to same structure parameters
  * @note The parameter length is 22-bits, other 10-bits are reserved (not used)
  */
typedef union
{
  PM_Param_Base_Typedef Base;              /*!< to manage a base/generic parameter structure */
  PM_Param_Connection_Typedef Connection;  /*!< to manage a connection parameter structure */
  PM_Param_Data_Typedef Data;              /*!< to manage a data parameter structure */
} PM_Param_Typedef;

/** @brief Ids for the used events. First event has the highest priority
  */
typedef enum
{
  PM_EV_NONE                     = PM_EVENT_ITEM(PM_EV_TYPE_NONE,    0x00), /*!< no event */ 
  PM_EV_SERVICE_STANDBY          = PM_EVENT_ITEM(PM_EV_TYPE_SERVICE, 0x01), /*!< stanby service */ 
  PM_EV_NOTIF_PD_CONN_STATUS     = PM_EVENT_ITEM(PM_EV_TYPE_NOTIF,   0x02), /*!< connection data  (PM_ParamConnection_Typedef) */ 
  PM_EV_NOTIF_PD_CONN_EXPLICIT   = PM_EVENT_ITEM(PM_EV_TYPE_NOTIF,   0x03), /*!< connection data  (PM_ParamConnection_Typedef) */ 
  PM_EV_NOTIF_PD_CONN_ROLE       = PM_EVENT_ITEM(PM_EV_TYPE_NOTIF,   0x04), /*!< connection data  (PM_ParamConnection_Typedef) */ 
  PM_EV_NOTIF_PD_REQ_RX          = PM_EVENT_ITEM(PM_EV_TYPE_NOTIF,   0x05), /*!< connection data  (PM_ParamConnection_Typedef) */ 
  PM_EV_NOTIF_PD_FIXED_ACCEPTED  = PM_EVENT_ITEM(PM_EV_TYPE_NOTIF,   0x06), /*!< pdo item         (PM_Param_Data_Typedef) */ 
  PM_EV_NOTIF_PD_APDO_ACCEPTED   = PM_EVENT_ITEM(PM_EV_TYPE_NOTIF,   0x07), /*!< pdo item         (PM_Param_Data_Typedef) */ 
  PM_EV_NOTIF_PD_VOLTAGE_CHANGE  = PM_EVENT_ITEM(PM_EV_TYPE_NOTIF,   0x08), /*!< pdo item         (PM_Param_Data_Typedef) */ 
  PM_EV_NOTIF_PD_CURRENT_CHANGE  = PM_EVENT_ITEM(PM_EV_TYPE_NOTIF,   0x09), /*!< pdo item         (PM_Param_Data_Typedef) */ 
  PM_EV_NOTIF_PD_PSREADY         = PM_EVENT_ITEM(PM_EV_TYPE_NOTIF,   0x0A), /*!< n/a              */ 
  PM_EV_NOTIF_PD_HARDRESET       = PM_EVENT_ITEM(PM_EV_TYPE_NOTIF,   0x0B), /*!< n/a              */ 
  PM_EV_FAULT_BUS_OVER_VOLTAGE   = PM_EVENT_ITEM(PM_EV_TYPE_FAULT,   0x0C), /*!< read voltage     (PM_Param_Data_Typedef) */ 
  PM_EV_FAULT_BUS_UNDER_VOLTAGE  = PM_EVENT_ITEM(PM_EV_TYPE_FAULT,   0x0D), /*!< read voltage     (PM_Param_Data_Typedef) */ 
  PM_EV_FAULT_BUS_OVER_CURRENT   = PM_EVENT_ITEM(PM_EV_TYPE_FAULT,   0x0E), /*!< read current     (PM_Param_Data_Typedef) */ 
  PM_EV_FAULT_VCONN_OVER_VOLTAGE = PM_EVENT_ITEM(PM_EV_TYPE_FAULT,   0x0F), /*!< read voltage     (PM_Param_Data_Typedef) */ 
  PM_EV_FAULT_VCONN_UNDER_VOLTAGE= PM_EVENT_ITEM(PM_EV_TYPE_FAULT,   0x10), /*!< read voltage     (PM_Param_Data_Typedef) */ 
  PM_EV_FAULT_VCONN_OVER_CURRENT = PM_EVENT_ITEM(PM_EV_TYPE_FAULT,   0x11), /*!< read current     (PM_Param_Data_Typedef) */ 
  PM_EV_FAULT_THERMAL            = PM_EVENT_ITEM(PM_EV_TYPE_FAULT,   0x12), /*!< read temperature (PM_Param_Base_Typedef) */ 
  PM_EV_FAULT_CC_LINES           = PM_EVENT_ITEM(PM_EV_TYPE_FAULT,   0x13), /*!< tbd              (PM_Param_Base_Typedef) */
  PM_EV_FAULT_STATUS             = PM_EVENT_ITEM(PM_EV_TYPE_FAULT,   0x14), /*!< n/a              */
} PM_Event_Typedef;

/** @brief Standby mode
  */
typedef enum 
{
  PM_STANDBY_MODE_WAKEUP       = 0x00, /*!< Wakeup */
  PM_STANDBY_MODE_LOW_RATE     = 0x01, /*!< Long time sleep */
  PM_STANDBY_MODE_WAIT_FOR_MSG = 0x02, /*!< Indefinitly sleep, waiting for a message */
  PM_STANDBY_MODE_SUSPEND      = 0x03, /*!< Suspend in safe mode the task, wake-up resume the task */
  PM_STANDBY_MODE_KILL         = 0x04, /*!< Kill in safe mode the task, wake-up create again the task */
} PM_StandBy_Mode_Typedef;

/** @brief Callback to read and to notify data, event, fault
  * <ul>
  *  <li><b>@ref PM_ReadData          </b>Read data in input</li>
  *  <li><b>@ref PM_NotifyData        </b>Notify data to output (PPS module, SR module)</li>
  *  <li><b>@ref PM_CheckStatus       </b>Check the system status</li>
  *  <li><b>@ref PM_FaultCondition    </b>Move the system in a safe condition after a fault</li>
  *  <li><b>@ref PM_CriticalCondition </b>Move the system in a very safe condition to avoid damage</li>
  * </ul>
  */
typedef struct 
{
  PM_Status_TypeDef (*PM_ReadData)(PM_Data_Typedef * pData);                    /*!< Callback to read data function */ 
  PM_Status_TypeDef (*PM_NotifyData)(PM_Data_Typedef * pData);                  /*!< Callback to notify data function*/ 
  PM_Status_TypeDef (*PM_CheckStatus)(PM_Data_Typedef * pData);                 /*!< Callback to check the status function */ 
  PM_Status_TypeDef (*PM_FaultCondition)(uint8_t PortNum, uint32_t Param);      /*!< Callback to fault condition function */ 
  PM_Status_TypeDef (*PM_CriticalCondition)(uint8_t PortNum, uint32_t Param);   /*!< Callback to critical condition function */ 
} PM_Callback_Typedef;

/** 
  * @} USBPD_POWER_MONITOR_Exported_Typedef
  */

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @defgroup USBPD_POWER_MONITOR_Exported_Functions Exported Functions
  * @brief Current, vbus and/or power monitor modules exported functions
  * @{
  */
PM_Status_TypeDef PM_Monitor_Init(PM_Callback_Typedef * pCallbacks);
PM_Status_TypeDef PM_RegisterCallbacks(PM_Callback_Typedef * pCallbacks);
void PM_Control_Enable(uint8_t Enable, int32_t Delay);

PM_Status_TypeDef PM_Notify(uint8_t PortNum, PM_Event_Typedef Event, uint32_t Parameter, uint32_t Timeout_ms);
PM_Status_TypeDef PM_StandBy(uint8_t PortNum, uint8_t Mode);
PM_Status_TypeDef PM_Wakeup(uint8_t PortNum);
/** 
  * @} USBPD_POWER_MONITOR_Exported_Functions
  */

/** 
  * @} USBPD_POWER_MONITOR
  */

/** 
  * @} UTILITIES
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBPD_POWER_MONITOR_H */
