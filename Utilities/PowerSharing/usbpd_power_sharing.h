/**
  ******************************************************************************
  * @file    usbpd_power_sharing.h
  * @version 1.1.0
  * @date    November, 2nd 2021
  * @author  S&C USBPD Team
  * @brief   Header file for usbpd_power_sharing.c file for STEVAL-2STPD01
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBPD_POWER_SHARING_H
#define __USBPD_POWER_SHARING_H

#ifdef __cplusplus
extern "C" {
#endif
  
/** @addtogroup UTILITIES
  * @{
  */

/** @defgroup USBPD_POWER_SHARING Power Sharing Module
  * @brief The power sharing checks the status of the power and forward the data.
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usbpd_def.h"
#include "usbpd_dpm_conf.h"
 
/* Exported define -----------------------------------------------------------*/ 
/** @defgroup USBPD_POWER_SHARING_Exported_Defines Exported Defines
  * @{
  */
#define PS_PORT_NA      USBPD_PORT_NA
#define PS_PORT_0       USBPD_PORT_0
#define PS_PORT_1       USBPD_PORT_1
#define PS_PORT_COUNT   USBPD_PORT_COUNT
#define PS_MAX_PDO      7
/** 
  * @} USBPD_POWER_SHARING_Exported_Defines
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup USBPD_POWER_SHARING_Exported_Macros Exported Macros
  * @{
  */
#define PS_OTHER_PORT(PortNum)        ((PortNum) == PS_PORT_0 ? PS_PORT_1 : PS_PORT_0) /*!< get the other port number */

#define PS_MATH_ROUND(v, p) ((((v) + ((p)>>1)) / (p)) * (p))    /*!< perfom a round to the near integer value */
#define PS_MATH_CEIL(v, p)  (((v) / (p)) * (p))                 /*!< perfom a ceil to the minor integer value */                
/** 
  * @} USBPD_POWER_SHARING_Exported_Macros
  */

/* Exported typedef ----------------------------------------------------------*/
/** @defgroup USBPD_POWER_SHARING_Exported_Typedef Exported Typedef
  * @{
  */

/** @brief Power Sharing Status condition returned from API
  */
typedef enum
{
  PS_STATUS_ERR = 0,            /*!< Error status condition     */
  PS_STATUS_OK  = 1,            /*!< Ok status condition        */
  PS_STATUS_NA  = 2,            /*!< Not applicable             */
} PS_Status_TypeDef;

/** @brief PDO_Items
  */
typedef struct
{
  uint32_t Voltage;             /*!< Data Voltage (mV)  */
  int32_t  Current;             /*!< Data Current (mA)  */
} PS_Data_Typedef;

/** @brief PDO_Items
  */
typedef struct
{
  uint32_t        PowerNew;               /*!< required power from the port             */
  uint32_t        PowerOld;               /*!< previously required power from the port  */
  uint32_t        PowerRemaining;         /*!< remaining power for the other port       */
  uint8_t         Connected;              /*!< connection status                        */
  uint32_t PDODef_List[PS_MAX_PDO];       /*!< default PDO list                         */
  uint32_t PDODef_Num;                    /*!< default PDO num                          */
} PS_Port_Typedef;

/** @brief Event occured
  */
typedef enum
{
  PS_EV_NONE               = 0x00,        /*!< No event */
  PS_EV_ATTACH             = 0x01,        /*!< Notify an attach event */
  PS_EV_DETACH             = 0x02,        /*!< Notify a detach event */
  PS_EV_SOURCE_CAPA        = 0x03,        /*!< Notify reading the source capa event */
  PS_EV_RECEIVED_REQUEST   = 0x04,        /*!< Notify a received request (from the pair port) event */
  PS_EV_EXPLICT_CONTRACT   = 0x05,        /*!< Notify an achieved explicit contract (with the pair port) event */
} PS_Event_Typedef; 

/** @brief Configuration structure
  */
typedef struct
{
  int32_t         MinCurrent;             /*!< Minmum current to enable a PDO */
  uint32_t        PowerNoLoad;            /*!< Power withoud load                       */
  uint32_t        PCoefficient;           /*!< System safety power coefficient  in/out  */
  uint32_t        VCoefficient;           /*!< Voltage conversion coefficient           */
  uint32_t        MinPortPower;           /*!< Minimum port power                       */
  uint32_t        MaxPortPower;           /*!< Maximum port power                       */
  uint32_t        MaxPortVoltage;         /*!< Maximum port voltage                     */
  uint32_t        MaxPortCurrent;         /*!< Maximum port current                     */
  PS_Data_Typedef PowerSupplyDef;         /*!< PDO Item Voltage (mV)                    */
} PS_Config_Typedef;

/** @brief Power Sharing Callbacks
  * <ul>
  *  <li><b>@ref PS_EnablePort          </b>Read data in input</li>
  *  <li><b>@ref PS_SendSourceCapa        </b>Notify data to output (PPS module, SR module)</li>
  * </ul>
  */
typedef struct 
{
  PS_Status_TypeDef (*PS_EnablePort)(uint8_t PortNum, uint8_t Enable);      /*!< Callback to enable/disable a port */ 
  PS_Status_TypeDef (*PS_SendSourceCapa)(uint8_t PortNum);                  /*!< Callback to send the Source Capabilities */ 
} PS_Callback_Typedef;

/** 
  * @} USBPD_POWER_SHARING_Exported_Typedef
  */

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @defgroup USBPD_POWER_SHARING_Exported_Functions Exported Functions
  * @{
  */
PS_Status_TypeDef PS_Init(PS_Data_Typedef * PowerSupplyConf, PS_Callback_Typedef const * Callbacks);
PS_Status_TypeDef PS_RegisterCallback(PS_Callback_Typedef const * Callbacks);
uint32_t PS_CalculatePowerSystem(PS_Data_Typedef * PowerSupplyParam);
PS_Status_TypeDef PS_PowerSupply(PS_Data_Typedef * PowerSupplyConf);
PS_Status_TypeDef PS_Notification(uint8_t PortNum, PS_Event_Typedef Event, uint32_t Param);
/** 
  * @} USBPD_POWER_SHARING_Exported_Functions
  */

/** 
  * @} USBPD_POWER_SHARING
  */

/** 
  * @} UTILITIES
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBPD_POWER_SHARING_H */
