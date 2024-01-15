/**
  ******************************************************************************
  * @file    usbpd_dpm_user.h
  * @version 1.0.1
  * @date    2021-11-08
  * @author  MCD Application Team and SRA.SLDU.SC Team
  * @brief   Header file for usbpd_dpm_user.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifndef __USBPD_DPM_USER_H_
#define __USBPD_DPM_USER_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"
   
/** @addtogroup STM32_USBPD_APPLICATION
  * @{
  */

/** @defgroup STM32_USBPD_APPLICATION_DPM_USER DPM User
  * @{
  */

/** @defgroup STM32_USBPD_APPLICATION_DPM_USER_EXPORTED_TYPES Exported Types
  * @{
  */
/* Exported typedef ----------------------------------------------------------*/
#if !defined(_GUI_INTERFACE)
/**
  * @brief  USBPD User Setting
  */
typedef struct
{
  uint32_t PE_DataSwap                                    : 1;   /*!< support data swap                                     */
  uint32_t PE_VconnSwap                                   : 1;   /*!< support VCONN swap                                    */
  uint32_t PE_DR_Swap_To_DFP                              : 1U;  /*!< If supported, DR Swap to DFP can be accepted or not by the user else directly rejected */
  uint32_t PE_DR_Swap_To_UFP                              : 1U;  /*!< If supported, DR Swap to UFP can be accepted or not by the user else directly rejected */
  uint32_t Reserved1                                      : 28U; /*!< Reserved bits */
  uint32_t            Reserved_ReqPower[6];                      /*!< Reserved bits to match with Resquested power information            */
#if _MANU_INFO
  USBPD_MIDB_TypeDef  DPM_ManuInfoPort;                         /*!< Manufacturer information used for the port            */
  uint16_t            ReservedManu;                             /*!< Reserved bits to match with Manufacturer information            */
#else
  uint32_t            ReservedManu[7];                          /*!< Reserved bits to match with Manufacturer information            */
#endif /* _MANU_INFO */
#if _SRC_CAPA_EXT
  USBPD_SCEDB_TypeDef DPM_SRCExtendedCapa;                      /*!< SRC Extended Capability                               */
#else
  uint32_t            ReservedSrcCapa[6];                       /*!< Reserved bits to match with SrcCapa information            */
#endif /* _SRC_CAPA_EXT && (_SRC || _DRP) */
  uint32_t            ReservedSnkCapa[6];                       /*!< Reserved bits to match with SnkCapaExt information     */
  uint32_t ReservedByte;                                        /*!< Reserved bits */
} USBPD_USER_SettingsTypeDef;

/**
  * @brief  USBPD Id Setting
  */
typedef struct
{
  uint32_t XID;               /*!< Value provided by the USB-IF assigned to the product   */
  uint16_t VID;               /*!< Vendor ID (assigned by the USB-IF)                     */
  uint16_t PID;               /*!< Product ID (assigned by the manufacturer)              */
} USBPD_IdSettingsTypeDef;
#endif /* !_GUI_INTERFACE */

/**
  * @brief  USBPD Event definition
  */
typedef enum
{
  DPM_USER_EVENT_TIMER,         /* TIMER EVENT */
  DPM_USER_EVENT_GUI,           /* GUI EVENT */
  DPM_USER_EVENT_NONE,          /* NO EVENT */
} DPM_USER_EVENT;

/**
  * @brief  USBPD Data Structure definition
  */
typedef struct
{
  uint32_t                      Voltage;                                 /*!< Value of voltage                                                     */
  int32_t                       Current;                                 /*!< Value of current                                                     */  
} USBPD_DataTypeDef;

/**
  * @brief  USBPD Requested data definition
  */
typedef struct
{
  uint32_t                      RDOPosition;                             /*!< RDO Position of requested DO in Source list of capabilities          */
  uint32_t                      RequestedVoltage;                        /*!< Value of requested voltage                                           */
  uint32_t                      RequestedCurrent;                        /*!< Value of requested current                                           */
  uint32_t                      RequestedMaxCurrent;                     /*!< Value of requested max current                                           */
  uint32_t                      RDOPositionPrevious;                     /*!< RDO Position of previous requested DO in Source list of capabilities */
  uint32_t                      RequestDOMsg;                            /*!< Request Power Data Object message to be sent                         */
  uint32_t                      RequestDOMsgPrevious;                    /*!< Previous Request Power Data Object message to be sent                */
  uint32_t                      RcvRequestDOMsg;                         /*!< Received request Power Data Object message from the port Partner     */
} USBPD_RequestDataTypeDef;

/**
  * @brief  USBPD DPM handle Structure definition
  */
typedef struct
{
  uint32_t                      DPM_ListOfRcvSRCPDO[USBPD_MAX_NB_PDO];   /*!< The list of received Source Power Data Objects from Port partner
                                                                              (when Port partner is a Source or a DRP port).                       */
  uint32_t                      DPM_NumberOfRcvSRCPDO;                   /*!< The number of received Source Power Data Objects from port Partner
                                                                              (when Port partner is a Source or a DRP port).
                                                                              This parameter must be set to a value lower than USBPD_MAX_NB_PDO    */
  uint32_t                      DPM_ListOfRcvSNKPDO[USBPD_MAX_NB_PDO];   /*!< The list of received Sink Power Data Objects from Port partner
                                                                              (when Port partner is a Sink or a DRP port).                         */
  uint32_t                      DPM_NumberOfRcvSNKPDO;                   /*!< The number of received Sink Power Data Objects from port Partner
                                                                              (when Port partner is a Sink or a DRP port).
                                                                              This parameter must be set to a value lower than USBPD_MAX_NB_PDO    */
//  uint32_t                      DPM_MeasuredVoltage;                     /*!< Value of measured voltage have to be respect the struct              */
//  int32_t                       DPM_MeasuredCurrent;                     /*!< Value of measured current                                            */
  USBPD_DataTypeDef             DPM_MeasuredData;                        /*!< Value of measured voltage and current                                */
  
  uint32_t                      DPM_RDOPosition;                         /*!< RDO Position of requested DO in Source list of capabilities          */
  uint32_t                      DPM_RequestedVoltage;                    /*!< Value of requested voltage                                           */
  uint32_t                      DPM_RequestedCurrent;                    /*!< Value of requested current                                           */
  uint32_t                      DPM_RequestedMaxCurrent;                 /*!< Value of requested max current                                           */
  uint32_t                      DPM_RDOPositionPrevious;                 /*!< RDO Position of previous requested DO in Source list of capabilities */
  uint32_t                      DPM_RequestDOMsg;                        /*!< Request Power Data Object message to be sent                         */
  uint32_t                      DPM_RequestDOMsgPrevious;                /*!< Previous Request Power Data Object message to be sent                */
  uint32_t                      DPM_RcvRequestDOMsg;                     /*!< Received request Power Data Object message from the port Partner     */
  uint32_t                      DPM_RequestRejected;                     /*!< Received request Power Data Object message from the port Partner     */

  volatile uint32_t             DPM_ErrorCode;                           /*!< USB PD Error code                                                    */
  volatile uint8_t              DPM_IsConnected;                         /*!< USB PD connection state                                              */
  uint16_t                      DPM_Reserved:16;                         /*!< Reserved bytes                                                       */
#ifdef _SRC_CAPA_EXT
  uint8_t                       FlagSendGetSrcCapaExtended;
  volatile uint16_t             DPM_TimerSRCExtendedCapa;                /*!< timer to request the extended capa                                   */
#endif /* _SRC_CAPA_EXT */
#if _STATUS
  USBPD_SDB_TypeDef             DPM_RcvStatus;                           /*!< Status received by port partner                                      */
#endif /* _STATUS */
#if _SRC_CAPA_EXT
  USBPD_SCEDB_TypeDef           DPM_RcvSRCExtendedCapa;                  /*!< SRC Extended Capability received by port partner                     */
#endif /* _SRC_CAPA_EXT */
#if _MANU_INFO
  USBPD_GMIDB_TypeDef           DPM_GetManufacturerInfo;                 /*!< Get Manufacturer Info                                                */
#endif /* _MANU_INFO */
#if _ALERT
  volatile uint16_t             DPM_TimerAlert;                          /*!< Timer used to monitor current and trig an ALERT                      */
  USBPD_ADO_TypeDef             DPM_SendAlert;                           /*!< Save the Alert sent to port partner                                  */
  USBPD_ADO_TypeDef             DPM_RcvAlert;                            /*!< Save the Alert received by port partner                              */
#endif /* _ALERT */
#if defined(_GUI_INTERFACE)
  volatile uint16_t             DPM_TimerMeasReport;                     /*!< Timer used to send measurement report                                */
#endif /* _GUI_INTERFACE */
} USBPD_HandleTypeDef;

typedef void     (*GUI_NOTIFICATION_POST)(uint8_t PortNum, uint16_t EventVal);
typedef uint32_t (*GUI_NOTIFICATION_FORMAT_SEND)(uint32_t PortNum, uint32_t TypeNotification, uint32_t Value);
typedef void     (*GUI_SAVE_INFO)(uint8_t PortNum, uint8_t DataId, uint8_t *Ptr, uint32_t Size);

/**
  * @} STM32_USBPD_APPLICATION_DPM_USER_PUBLIC_TYPES
  */

/** @defgroup STM32_USBPD_APPLICATION_DPM_USER_EXPORTED_DEFINES Exported Defines
  * @{
  */
/* Exported define -----------------------------------------------------------*/
#define USBPD_FW_VERSION         0x30062021u    /*!< USBPD FW version         */
#define USBPD_START_PORT_NUMBER  1u             /*!< USBPD Start Port Number  */
/**
  * @} STM32_USBPD_APPLICATION_DPM_USER_EXPORTED_DEFINES
  */


/** @defgroup STM32_USBPD_APPLICATION_DPM_USER_EXPORTED_VARIABLES Exported Variables
  * @{
  */
/* Exported variables --------------------------------------------------------*/
#if !defined(USBPD_DPM_USER_C)
/**
  * @brief  USBPD DPM Ports exported variable
  */
extern USBPD_HandleTypeDef DPM_Ports[USBPD_PORT_COUNT];
#else
/**
  * @brief  USBPD DPM Ports declaration
  */
USBPD_HandleTypeDef DPM_Ports[USBPD_PORT_COUNT] =
{
  {
    .DPM_Reserved = 0,
#if _SRC_CAPA_EXT
    .FlagSendGetSrcCapaExtended = 0,
    .DPM_TimerSRCExtendedCapa = 0,                  /*!< timer to request the extended capa                                   */
    .DPM_RcvSRCExtendedCapa = {0},                  /*!< SRC Extended Capability received by port partner                     */
#endif /* _SRC_CAPA_EXT */
#if _MANU_INFO
    .DPM_GetManufacturerInfo = {0},                 /*!< Get Manufacturer Info                                                */
#endif /* _MANU_INFO */
#if _ALERT
    .DPM_TimerAlert = 0,                            /*!< Timer used to monitor current and trig an ALERT                      */
    .DPM_SendAlert = {0},                           /*!< Save the Alert sent to port partner                                  */
    .DPM_RcvAlert = {0},                            /*!< Save the Alert received by port partner                              */
#endif /* _ALERT */
#if USBPD_PORT_COUNT >= 2
  },
  {
    .DPM_Reserved = 0,
#if _SRC_CAPA_EXT
    .FlagSendGetSrcCapaExtended = 0,
    .DPM_TimerSRCExtendedCapa = 0,                  /*!< timer to request the extended capa                                   */
    .DPM_RcvSRCExtendedCapa = {0},                  /*!< SRC Extended Capability received by port partner                     */
#endif /* _SRC_CAPA_EXT */
#if _MANU_INFO
    .DPM_GetManufacturerInfo = {0},                 /*!< Get Manufacturer Info                                                */
#endif /* _MANU_INFO */
#if _ALERT
    .DPM_TimerAlert = 0,                            /*!< Timer used to monitor current and trig an ALERT                      */
    .DPM_SendAlert = {0},                           /*!< Save the Alert sent to port partner                                  */
    .DPM_RcvAlert = {0},                            /*!< Save the Alert received by port partner                              */
#endif /* _ALERT */
  }
#else
  }
#endif /*USBPD_PORT_COUNT >= 2*/
};
#endif /* !USBPD_DPM_USER_C */
/**
  * @} STM32_USBPD_APPLICATION_DPM_USER_EXPORTED_VARIABLES
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup USBPD_USER_EXPORTED_FUNCTIONS Exported Functions
  * @{
  */
/** @defgroup USBPD_USER_EXPORTED_FUNCTIONS_GROUP1 Called by DPM CORE
  * @{
  */
void                USBPD_DPM_Notification(uint8_t PortNum, USBPD_NotifyEventValue_TypeDef EventVal);
USBPD_StatusTypeDef USBPD_DPM_UserInit(void);
void                USBPD_DPM_SetNotification_GUI(GUI_NOTIFICATION_FORMAT_SEND PtrFormatSend,
                                                  GUI_NOTIFICATION_POST PtrPost, GUI_SAVE_INFO PtrSaveInfo);
void                USBPD_DPM_WaitForTime(uint32_t Time);
#if (osCMSIS < 0x20000U)
void                USBPD_DPM_UserExecute(void const *argument);
#else
void                USBPD_DPM_UserExecute(void *argument);
#endif /* osCMSIS < 0x20000U */
void                USBPD_DPM_UserTimerCounter(uint8_t PortNum);
void                USBPD_DPM_TimerCounterUpdate(uint8_t PortNum, uint32_t Tick);
void                USBPD_DPM_UserCableDetection(uint8_t PortNum, USBPD_CAD_EVENT State);

/**
  * @} USBPD_USER_EXPORTED_FUNCTIONS_GROUP1
  */

/** @defgroup USBPD_USER_EXPORTED_FUNCTIONS_GROUP2 Called by PE
  * @{
  */
USBPD_StatusTypeDef USBPD_DPM_SetupNewPower(uint8_t PortNum);
void                USBPD_DPM_HardReset(uint8_t PortNum, USBPD_PortPowerRole_TypeDef CurrentRole,
                                        USBPD_HR_Status_TypeDef Status);
USBPD_StatusTypeDef USBPD_DPM_EvaluatePowerRoleSwap(uint8_t PortNum);
void                USBPD_DPM_ExtendedMessageReceived(uint8_t PortNum, USBPD_ExtendedMsg_TypeDef MsgType,
                                                      uint8_t *ptrData, uint16_t DataSize);
void                USBPD_DPM_GetDataInfo(uint8_t PortNum, USBPD_CORE_DataInfoType_TypeDef DataId, uint8_t *Ptr,
                                          uint32_t *Size);
void                USBPD_DPM_SetDataInfo(uint8_t PortNum, USBPD_CORE_DataInfoType_TypeDef DataId, uint8_t *Ptr,
                                          uint32_t Size);
USBPD_StatusTypeDef USBPD_DPM_EvaluateRequest(uint8_t PortNum, USBPD_CORE_PDO_Type_TypeDef *PtrPowerObject);

void                USBPD_DPM_EnterErrorRecovery(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_DPM_EvaluateDataRoleSwap(uint8_t PortNum);
USBPD_FunctionalState USBPD_DPM_IsPowerReady(uint8_t PortNum, USBPD_VSAFE_StatusTypeDef Vsafe);

/**
  * @} USBPD_USER_EXPORTED_FUNCTIONS_GROUP2
  */

/** @defgroup USBPD_USER_EXPORTED_FUNCTIONS_GROUP3 PD messages requests
  * @{
  */
USBPD_StatusTypeDef USBPD_DPM_RequestHardReset(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_DPM_RequestCableReset(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_DPM_RequestGotoMin(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_DPM_RequestPing(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_DPM_RequestMessageRequest(uint8_t PortNum, uint8_t IndexSrcPDO, uint16_t RequestedVoltage);
USBPD_StatusTypeDef USBPD_DPM_RequestGetSourceCapability(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_DPM_RequestGetSinkCapability(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_DPM_RequestDataRoleSwap(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_DPM_RequestPowerRoleSwap(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_DPM_RequestVconnSwap(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_DPM_RequestSoftReset(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType);
USBPD_StatusTypeDef USBPD_DPM_RequestSourceCapability(uint8_t PortNum);
#if defined(_UVDM)
USBPD_StatusTypeDef USBPD_DPM_RequestUVDMMessage(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType);
#endif /* _UVDM */
USBPD_StatusTypeDef USBPD_DPM_RequestAlert(uint8_t PortNum, USBPD_ADO_TypeDef Alert);
USBPD_StatusTypeDef USBPD_DPM_RequestGetSourceCapabilityExt(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_DPM_RequestGetSinkCapabilityExt(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_DPM_RequestGetManufacturerInfo(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType,
                                                         uint8_t *pManuInfoData);
USBPD_StatusTypeDef USBPD_DPM_RequestGetStatus(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_DPM_RequestFastRoleSwap(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_DPM_RequestGetPPS_Status(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_DPM_RequestGetCountryCodes(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_DPM_RequestGetCountryInfo(uint8_t PortNum, uint16_t CountryCode);
USBPD_StatusTypeDef USBPD_DPM_RequestGetBatteryCapability(uint8_t PortNum, uint8_t *pBatteryCapRef);
USBPD_StatusTypeDef USBPD_DPM_RequestGetBatteryStatus(uint8_t PortNum, uint8_t *pBatteryStatusRef);
USBPD_StatusTypeDef USBPD_DPM_RequestSecurityRequest(uint8_t PortNum);
USBPD_StatusTypeDef USBPD_DPM_RequestFirwmwareUpdate(uint8_t PortNum, USBPD_ExtendedMsg_TypeDef MessageType,
                                                     uint8_t *pPayload, uint16_t Size);
/**
  * @} USBPD_USER_EXPORTED_FUNCTIONS_GROUP3
  */
    
/**
  * @} USBPD_USER_EXPORTED_FUNCTIONS
  */

/**
  * @} STM32_USBPD_APPLICATION_DPM_USER
  */

/**
  * @} STM32_USBPD_APPLICATION
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBPD_DPM_USER_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
