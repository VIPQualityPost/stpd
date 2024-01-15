/**
  ******************************************************************************
  * @file    tcpp0203.h
  * @author  MCD Application Team and SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   This file contains all the functions prototypes for the
  *          tcpp0203.c driver.
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
#ifndef TCPP0203_H
#define TCPP0203_H

/** @addtogroup BSP_Components
  * @{
  */

/** @defgroup BSP_Components_TCPP0203 TCPP0203
  * @brief     This file provides a set of functions needed to drive the
  *            TCPP02/03 Type-C port protection.
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "tcpp0203_reg.h"
#include <stddef.h>

/* Exported types ------------------------------------------------------------*/
/** @defgroup BSP_Components_TCPP0203_Exported_Types Exported Types
  * @{
  */
typedef int32_t (*TCPP0203_Init_Func)(void);                                            /*!< Init type function                 */
typedef int32_t (*TCPP0203_DeInit_Func)(void);                                          /*!< DeInit type function               */
typedef int32_t (*TCPP0203_GetTick_Func)(void);                                         /*!< GetTick type function              */
typedef int32_t (*TCPP0203_Delay_Func)(uint32_t);                                       /*!< Delay type function                */
typedef int32_t (*TCPP0203_WriteReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);     /*!< Write Register type function       */
typedef int32_t (*TCPP0203_ReadReg_Func)(uint16_t, uint16_t, uint8_t *, uint16_t);      /*!< Read Register type function        */

/**
  * @brief  Register IO Bus to component
  */
typedef struct
{
  TCPP0203_Init_Func        Init;
  TCPP0203_DeInit_Func      DeInit;
  uint16_t                  Address;
  TCPP0203_WriteReg_Func    WriteReg;
  TCPP0203_ReadReg_Func     ReadReg;
  TCPP0203_GetTick_Func     GetTick;
  TCPP0203_Delay_Func       Delay;
} TCPP0203_IO_t;

/**
  * @brief  Component object
  */
typedef struct
{
  TCPP0203_IO_t         IO;
  TCPP0203_ctx_t        Ctx;
  uint8_t               IsInitialized;
  uint8_t               PortNum;
  uint8_t               DeviceType;
} TCPP0203_Object_t;

/**
  * @brief  Driver functions
  */
typedef struct
{
  int32_t (*Init)(TCPP0203_Object_t *);
  int32_t (*DeInit)(TCPP0203_Object_t *);
  int32_t (*Reset)(TCPP0203_Object_t *);
  int32_t (*SetVConnSwitch)(TCPP0203_Object_t *, uint8_t);
  int32_t (*SetGateDriverProvider)(TCPP0203_Object_t *, uint8_t);
  int32_t (*SetGateDriverConsumer)(TCPP0203_Object_t *, uint8_t);
  int32_t (*SetPowerMode)(TCPP0203_Object_t *, uint8_t);
  int32_t (*SetVBusDischarge)(TCPP0203_Object_t *, uint8_t);
  int32_t (*SetVConnDischarge)(TCPP0203_Object_t *, uint8_t);
  int32_t (*GetVConnSwitchAck)(TCPP0203_Object_t *, uint8_t *);
  int32_t (*GetGateDriverProviderAck)(TCPP0203_Object_t *, uint8_t *);
  int32_t (*GetGateDriverConsumerAck)(TCPP0203_Object_t *, uint8_t *);
  int32_t (*GetPowerModeAck)(TCPP0203_Object_t *, uint8_t *);
  int32_t (*GetVBusDischargeAck)(TCPP0203_Object_t *, uint8_t *);
  int32_t (*GetVConnDischargeAck)(TCPP0203_Object_t *, uint8_t *);
  int32_t (*GetOCPVConnFlag)(TCPP0203_Object_t *, uint8_t *);
  int32_t (*GetOCPVBusFlag)(TCPP0203_Object_t *, uint8_t *);
  int32_t (*GetOVPVBusFlag)(TCPP0203_Object_t *, uint8_t *);
  int32_t (*GetOVPCCFlag)(TCPP0203_Object_t *, uint8_t *);
  int32_t (*GetOTPFlag)(TCPP0203_Object_t *, uint8_t *);
  int32_t (*GetVBusOkFlag)(TCPP0203_Object_t *, uint8_t *);
  int32_t (*ReadTCPPType)(TCPP0203_Object_t *, uint8_t *);
  int32_t (*ReadVCONNPower)(TCPP0203_Object_t *, uint8_t *);
  int32_t (*WriteCtrlRegister)(TCPP0203_Object_t *, uint8_t *);
  int32_t (*ReadAckRegister)(TCPP0203_Object_t *, uint8_t *);
  int32_t (*ReadFlagRegister)(TCPP0203_Object_t *, uint8_t *);
} TCPP0203_Drv_t;

/**
  * @} BSP_Components_TCPP0203_Exported_Types
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines Exported Defines
  * @{
  */
/* I2C Address definition */
#define TCPP0203_I2C_ADDRESS_X68                (0x68U) /*!< I2C address 0x34 (ADDR0) */
#define TCPP0203_I2C_ADDRESS_X6A                (0x6AU) /*!< I2C address 0x35 (ADDR1) */

#define TCPP0203_OK                             (0)     /*!< Driver Response codes OK    */
#define TCPP0203_ERROR                          (-1)    /*!< Driver Response codes ERROR */

#define TCPP0203_REG0_RST_VALUE                   TCPP0203_GD_CONSUMER_SWITCH_CLOSED    /*!< TCPP0203 Reg0 Reset Value */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg0 Register 0
  * @{
  */
/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg0_VCONN_SWITCH VConn Switch
  * @brief  VConn Switch (2bits)
  * <table>
  * <tr><td><b>V1</b></td><td><b>V2</b></td><td><b>VC1</b></td><td><b>VC2</b></td></tr>
  * <tr><td> 0</td><td> 0</td><td> Open</td><td> Open</td></tr>
  * <tr><td> 0</td><td> 1</td><td> Open</td><td><em>Close</em></td></tr>
  * <tr><td> 1</td><td> 0</td><td><em>Close</em></td><td> Open</td></tr>
  * <tr><td> 1</td><td> 1</td><td> Open</td><td> Open</td></tr>
  * </table>
  * @{
  */
#define TCPP0203_VCONN_SWITCH_POS                 (0U)                                
#define TCPP0203_VCONN_SWITCH_MSK                 (0x03U << TCPP0203_VCONN_SWITCH_POS)
#define TCPP0203_VCONN_SWITCH_OPEN                (0x00U)                             
#define TCPP0203_VCONN_SWITCH_CC1                 (0x01U << TCPP0203_VCONN_SWITCH_POS)
#define TCPP0203_VCONN_SWITCH_CC2                 (0x02U << TCPP0203_VCONN_SWITCH_POS)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg0_VCONN_SWITCH
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg0_GD_PROVIDER_SWITCH GD Provider Switch
  * @brief  Gate Driver Provider change status
  * GDP = 1 : switch load closed
  * GDP = 0 : switch load open
  * @{
  */
#define TCPP0203_GD_PROVIDER_SWITCH_POS           (2U)
#define TCPP0203_GD_PROVIDER_SWITCH_MSK           (0x01U << TCPP0203_GD_PROVIDER_SWITCH_POS)
#define TCPP0203_GD_PROVIDER_SWITCH_OPEN          (0x00U)
#define TCPP0203_GD_PROVIDER_SWITCH_CLOSED        (TCPP0203_GD_PROVIDER_SWITCH_MSK)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg0_GD_PROVIDER_SWITCH
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg0_GD_CONSUMER_SWITCH GD Consumer Switch
  * @brief  Gate Driver Consumer, not used in TCPP02
  * @{
  */
#define TCPP0203_GD_CONSUMER_SWITCH_POS           (3U)
#define TCPP0203_GD_CONSUMER_SWITCH_MSK           (0x01U << TCPP0203_GD_CONSUMER_SWITCH_POS)
#define TCPP0203_GD_CONSUMER_SWITCH_CLOSED        (0x00U)
#define TCPP0203_GD_CONSUMER_SWITCH_OPEN          (TCPP0203_GD_CONSUMER_SWITCH_MSK)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg0_GD_CONSUMER_SWITCH
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg0_POWER_MODE Power Mode
  * @brief  Power Mode register (2bits)
  * <table>
  * <tr><td><b>PM1</b></td><td><b>PM2</b></td><td><b>Power Mode</b></td></tr>
  * <tr><td> 0</td><td> 0</td><td>Hibernate</td></tr>
  * <tr><td> 0</td><td> 1</td><td>Low Power</td></tr>
  * <tr><td> 1</td><td> 0</td><td>Normal</td></tr>
  * <tr><td> 1</td><td> 1</td><td>Not used</td></tr>
  * </table>
  * @{
  */
#define TCPP0203_POWER_MODE_POS                   (4U)
#define TCPP0203_POWER_MODE_MSK                   (0x03U << TCPP0203_POWER_MODE_POS)
#define TCPP0203_POWER_MODE_HIBERNATE             (0x00U)
#define TCPP0203_POWER_MODE_LOWPOWER              (0x02U << TCPP0203_POWER_MODE_POS)
#define TCPP0203_POWER_MODE_NORMAL                (0x01U << TCPP0203_POWER_MODE_POS)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg0_POWER_MODE
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg0_VBUS_DISCHARGE VBus Discharge
  * @brief  VBUS Discharge management
  * VBUSD = 1 : VBus discharge on
  * VBUSD = 0 : VBus discharge off
  * @{
  */
#define TCPP0203_VBUS_DISCHARGE_POS               (6U)
#define TCPP0203_VBUS_DISCHARGE_MSK               (0x01U << TCPP0203_VBUS_DISCHARGE_POS)
#define TCPP0203_VBUS_DISCHARGE_OFF               (0x00U)
#define TCPP0203_VBUS_DISCHARGE_ON                (TCPP0203_VBUS_DISCHARGE_MSK)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg0_VBUS_DISCHARGE
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg0_VCONN_DISCHARGE VConn Discharge
  * @brief  VConn Discharge management
  * VBCONND = 1 : VConn discharge on
  * VBCONND = 0 : VConn discharge off
  * @{
  */
#define TCPP0203_VCONN_DISCHARGE_POS              (7U)
#define TCPP0203_VCONN_DISCHARGE_MSK              (0x01U << TCPP0203_VCONN_DISCHARGE_POS)
#define TCPP0203_VCONN_DISCHARGE_OFF              (0x00U)
#define TCPP0203_VCONN_DISCHARGE_ON               (TCPP0203_VCONN_DISCHARGE_MSK)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg0_VCONN_DISCHARGE
  */
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg0
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg1 Register 1
  * @{
  */
/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg1_VCONN_SWITCH_ACK VConn Switch Ack
  * @brief  VCONN Switch Acknowledge (2bits)
  * @{
  */
#define TCPP0203_VCONN_SWITCH_ACK_POS             (0U)
#define TCPP0203_VCONN_SWITCH_ACK_MSK             (0x03U << TCPP0203_VCONN_SWITCH_ACK_POS)
#define TCPP0203_VCONN_SWITCH_ACK_OPEN            (0x00U)
#define TCPP0203_VCONN_SWITCH_ACK_CC1             (0x02U << TCPP0203_VCONN_SWITCH_ACK_POS)
#define TCPP0203_VCONN_SWITCH_ACK_CC2             (0x01U << TCPP0203_VCONN_SWITCH_ACK_POS)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg1_VCONN_SWITCH_ACK
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg1_GD_PROVIDER_SWITCH_ACK GD Provider Switch Ack
  * @brief  Gate Driver Provider Acknowledge
  * @{
  */
#define TCPP0203_GD_PROVIDER_SWITCH_ACK_POS       (2U)
#define TCPP0203_GD_PROVIDER_SWITCH_ACK_MSK       (0x01U << TCPP0203_GD_PROVIDER_SWITCH_ACK_POS)
#define TCPP0203_GD_PROVIDER_SWITCH_ACK_OPEN      (0x00U)
#define TCPP0203_GD_PROVIDER_SWITCH_ACK_CLOSED    (TCPP0203_GD_PROVIDER_SWITCH_ACK_MSK)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg1_GD_PROVIDER_SWITCH_ACK
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg1_GD_CONSUMER_SWITCH_ACK GD Consumer Switch Ack
  * @brief  Gate Driver Consumer Acknowledge, not used on TCPP02
  * @{
  */
#define TCPP0203_GD_CONSUMER_SWITCH_ACK_POS       (3U)
#define TCPP0203_GD_CONSUMER_SWITCH_ACK_MSK       (0x01U << TCPP0203_GD_CONSUMER_SWITCH_ACK_POS)
#define TCPP0203_GD_CONSUMER_SWITCH_ACK_CLOSED    (0x00U)
#define TCPP0203_GD_CONSUMER_SWITCH_ACK_OPEN      (TCPP0203_GD_CONSUMER_SWITCH_ACK_MSK)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg1_GD_CONSUMER_SWITCH_ACK
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg1_POWER_MODE_ACK Power Mode Ack
  * @brief  Power Mode Acknowledge (2bits)
  * @{
  */
#define TCPP0203_POWER_MODE_ACK_POS               (4U)
#define TCPP0203_POWER_MODE_ACK_MSK               (0x03U << TCPP0203_POWER_MODE_ACK_POS)
#define TCPP0203_POWER_MODE_ACK_HIBERNATE         (0x00U)
#define TCPP0203_POWER_MODE_ACK_LOWPOWER          (0x01U << TCPP0203_POWER_MODE_ACK_POS)
#define TCPP0203_POWER_MODE_ACK_NORMAL            (0x02U << TCPP0203_POWER_MODE_ACK_POS)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg1_POWER_MODE_ACK
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg1_VBUS_DISCHARGE_ACK VBus Discharge Ack
  * @brief  VBUS Discharge Acknowledge
  * @{
  */
#define TCPP0203_VBUS_DISCHARGE_ACK_POS           (6U)
#define TCPP0203_VBUS_DISCHARGE_ACK_MSK           (0x01U << TCPP0203_VBUS_DISCHARGE_ACK_POS)
#define TCPP0203_VBUS_DISCHARGE_ACK_OFF           (0x00U)
#define TCPP0203_VBUS_DISCHARGE_ACK_ON            (TCPP0203_VBUS_DISCHARGE_ACK_MSK)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg1_VBUS_DISCHARGE_ACK
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg1_VCONN_DISCHARGE_ACK VConn Discharge Ack
  * @brief  VConn Discharge Acknowledge
  * @{
  */
#define TCPP0203_VCONN_DISCHARGE_ACK_POS          (7U)
#define TCPP0203_VCONN_DISCHARGE_ACK_MSK          (0x01U << TCPP0203_VCONN_DISCHARGE_ACK_POS)
#define TCPP0203_VCONN_DISCHARGE_ACK_OFF          (0x00U)
#define TCPP0203_VCONN_DISCHARGE_ACK_ON           (TCPP0203_VCONN_DISCHARGE_ACK_MSK)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg1_VCONN_DISCHARGE_ACK
  */
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg1
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg2 Register 2
  * @{
  */
/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg2_FLAG_OCP_VCONN OCP VConn Flag
  * @brief  OCP (Over-Current Protection) VConn Flag management
  * @{
  */
#define TCPP0203_FLAG_OCP_VCONN_POS               (0U)
#define TCPP0203_FLAG_OCP_VCONN_MSK               (0x01U << TCPP0203_FLAG_OCP_VCONN_POS)
#define TCPP0203_FLAG_OCP_VCONN_SET               (TCPP0203_FLAG_OCP_VCONN_MSK)
#define TCPP0203_FLAG_OCP_VCONN_RESET             (0x00U)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg2_FLAG_OCP_VCONN
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg2_FLAG_OCP_VBUS OCP VBus Flag
  * @brief  OCP (Over-Current Protection) VBus Flag management
  * @{
  */
#define TCPP0203_FLAG_OCP_VBUS_POS                (1U)
#define TCPP0203_FLAG_OCP_VBUS_MSK                (0x01U << TCPP0203_FLAG_OCP_VBUS_POS)
#define TCPP0203_FLAG_OCP_VBUS_SET                (TCPP0203_FLAG_OCP_VBUS_MSK)
#define TCPP0203_FLAG_OCP_VBUS_RESET              (0x00U)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg2_FLAG_OCP_VBUS
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg2_FLAG_OVP_VBUS OVP VBus Flag
  * @brief  OVP (Over-Voltage Protection) VBus Flag management
  * @{
  */
#define TCPP0203_FLAG_OVP_VBUS_POS                (2U)
#define TCPP0203_FLAG_OVP_VBUS_MSK                (0x01U << TCPP0203_FLAG_OVP_VBUS_POS)
#define TCPP0203_FLAG_OVP_VBUS_SET                (TCPP0203_FLAG_OVP_VBUS_MSK)
#define TCPP0203_FLAG_OVP_VBUS_RESET              (0x00U)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg2_FLAG_OVP_VBUS
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg2_FLAG_OVP_CC OVP CC Flag
  * @brief  OVP (Over-Voltage Protection) CC Flag management
  * @{
  */
#define TCPP0203_FLAG_OVP_CC_POS                  (3U)
#define TCPP0203_FLAG_OVP_CC_MSK                  (0x01U << TCPP0203_FLAG_OVP_CC_POS)
#define TCPP0203_FLAG_OVP_CC_SET                  (TCPP0203_FLAG_OVP_CC_MSK)
#define TCPP0203_FLAG_OVP_CC_RESET                (0x00U)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg2_FLAG_OVP_CC
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg2_FLAG_OTP OTP Flag
  * @brief  OTP (Over-Temperature Protection) Flag management
  * @{
  */
#define TCPP0203_FLAG_OTP_POS                     (4U)
#define TCPP0203_FLAG_OTP_MSK                     (0x01U << TCPP0203_FLAG_OTP_POS)
#define TCPP0203_FLAG_OTP_SET                     (TCPP0203_FLAG_OTP_MSK)
#define TCPP0203_FLAG_OTP_RESET                   (0x00U)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg2_FLAG_OTP
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg2_FLAG_VBUS_OK VBus Ok Flag
  * @brief  VBus OK Flag management
  * @{
  */
#define TCPP0203_FLAG_VBUS_OK_POS                 (5U)
#define TCPP0203_FLAG_VBUS_OK_MSK                 (0x01U << TCPP0203_FLAG_VBUS_OK_POS)
#define TCPP0203_FLAG_VBUS_OK_SET                 (TCPP0203_FLAG_VBUS_OK_MSK)
#define TCPP0203_FLAG_VBUS_OK_RESET               (0x00U)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg2_FLAG_VBUS_OK
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg2_FLAG_VCONN_PWR VConn Power Flag
  * @brief  VConn Power Flag management
  * @{
  */
#define TCPP0203_FLAG_VCONN_PWR_POS               (6U)
#define TCPP0203_FLAG_VCONN_PWR_MSK               (0x01U << TCPP0203_FLAG_VCONN_PWR_POS)
#define TCPP0203_FLAG_VCONN_PWR_1W                (TCPP0203_FLAG_VCONN_PWR_MSK)
#define TCPP0203_FLAG_VCONN_PWR_0_1W              (0x00U)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg2_FLAG_VCONN_PWR
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Defines_Reg2_DEVICE_TYPE Device Type
  * @brief  Device Type
  * DT = 1 : TCPP02
  * DT = 0 : TCPP03
  * @{
  */
#define TCPP0203_DEVICE_TYPE_POS                  (7U)
#define TCPP0203_DEVICE_TYPE_MSK                  (0x01U << TCPP0203_DEVICE_TYPE_POS)
#define TCPP0203_DEVICE_TYPE_02                   (TCPP0203_DEVICE_TYPE_MSK)
#define TCPP0203_DEVICE_TYPE_03                   (0x00U)
#define TCPP0203_DEVICE_TYPE_NA                   (0xFFU)
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg2_DEVICE_TYPE
  */
/**
  * @} BSP_Components_TCPP0203_Exported_Defines_Reg2
  */
/**
  * @} BSP_Components_TCPP0203_Exported_Defines
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Functions Exported Functions
  * @{
  */
int32_t TCPP0203_RegisterBusIO(TCPP0203_Object_t *pObj, TCPP0203_IO_t *pIO);
int32_t TCPP0203_Init(TCPP0203_Object_t *pObj);
int32_t TCPP0203_DeInit(TCPP0203_Object_t *pObj);
int32_t TCPP0203_Reset(TCPP0203_Object_t *pObj);
int32_t TCPP0203_SetVConnSwitch(TCPP0203_Object_t *pObj, uint8_t VConnSwitch);
int32_t TCPP0203_SetGateDriverProvider(TCPP0203_Object_t *pObj, uint8_t GateDriverProvider);
int32_t TCPP0203_SetGateDriverConsumer(TCPP0203_Object_t *pObj, uint8_t GateDriverConsumer);
int32_t TCPP0203_SetPowerMode(TCPP0203_Object_t *pObj, uint8_t PowerMode);
int32_t TCPP0203_SetVBusDischarge(TCPP0203_Object_t *pObj, uint8_t VBusDischarge);
int32_t TCPP0203_SetVConnDischarge(TCPP0203_Object_t *pObj, uint8_t VConnDischarge);
int32_t TCPP0203_GetVConnSwitchAck(TCPP0203_Object_t *pObj, uint8_t *pVConnSwitchAck);
int32_t TCPP0203_GetGateDriverProviderAck(TCPP0203_Object_t *pObj, uint8_t *pGateDriverProviderAck);
int32_t TCPP0203_GetGateDriverConsumerAck(TCPP0203_Object_t *pObj, uint8_t *pGateDriverConsumerAck);
int32_t TCPP0203_GetPowerModeAck(TCPP0203_Object_t *pObj, uint8_t *pPowerModeAck);
int32_t TCPP0203_GetVBusDischargeAck(TCPP0203_Object_t *pObj, uint8_t *pVBusDischargeAck);
int32_t TCPP0203_GetVConnDischargeAck(TCPP0203_Object_t *pObj, uint8_t *pVConnDischargeAck);
int32_t TCPP0203_GetOCPVConnFlag(TCPP0203_Object_t *pObj, uint8_t *pOCPVConnFlag);
int32_t TCPP0203_GetOCPVBusFlag(TCPP0203_Object_t *pObj, uint8_t *pGetOCPVBusFlag);
int32_t TCPP0203_GetOVPVBusFlag(TCPP0203_Object_t *pObj, uint8_t *pOVPVBusFlag);
int32_t TCPP0203_GetOVPCCFlag(TCPP0203_Object_t *pObj, uint8_t *pOVPCCFlag);
int32_t TCPP0203_GetOTPFlag(TCPP0203_Object_t *pObj, uint8_t *pOTPFlag);
int32_t TCPP0203_GetVBusOkFlag(TCPP0203_Object_t *pObj, uint8_t *pVBusOkFlag);
int32_t TCPP0203_ReadTCPPType(TCPP0203_Object_t *pObj, uint8_t *pTCPPType);
int32_t TCPP0203_ReadVCONNPower(TCPP0203_Object_t *pObj, uint8_t *pVCONNPower);
int32_t TCPP0203_WriteCtrlRegister(TCPP0203_Object_t *pObj, uint8_t *pAckRegister);
int32_t TCPP0203_ReadAckRegister(TCPP0203_Object_t *pObj, uint8_t *pAckRegister);
int32_t TCPP0203_ReadFlagRegister(TCPP0203_Object_t *pObj, uint8_t *pFlagRegister);
/**
  * @} BSP_Components_TCPP0203_Exported_Functions
  */

/** @defgroup BSP_Components_TCPP0203_Exported_Variables Exported Variables
  * @{
  */
/**
  * @brief  Type-C port protection driver structure
  */
extern TCPP0203_Drv_t TCPP0203_Driver;
/**
  * @} BSP_Components_TCPP0203_Exported_Variables
  */

/**
  * @} BSP_Components_TCPP0203
  */

/**
  * @} BSP_Components
  */

#endif /* TCPP0203_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

