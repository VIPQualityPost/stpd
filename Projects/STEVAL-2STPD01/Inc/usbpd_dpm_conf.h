/**
  ******************************************************************************
  * @file    usbpd_dpm_conf.h
  * @version 1.0.1
  * @date    2021-11-08
  * @author  MCD Application Team and SRA.SLDU.SC Team
  * @brief   Header file for stack/application settings file
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

#ifndef __USBPD_DPM_CONF_H_
#define __USBPD_DPM_CONF_H_

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup STM32_USBPD_APPLICATION
  * @{
  */

/** @defgroup STM32_USBPD_APPLICATION_DPM_Config DPM Config
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "usbpd_pdo_defs.h"
#include "usbpd_dpm_user.h"
#if defined(_GUI_INTERFACE)
#include "gui_api.h"
#include "usbpd_gui_memmap.h"
#endif /* _GUI_INTERFACE */

/* Define   ------------------------------------------------------------------*/
/** @defgroup STM32_USBPD_APPLICATION_DPM_Config_Defines Defines
  * @{
  */
/** @defgroup STM32_USBPD_APPLICATION_DPM_Config_Defines_Manu Manufacturer Parameters
  * @{
  */
/* Define VID, PID,... manufacturer parameters */
#define USBPD_VID (0x0000u)     /*!< Vendor ID (assigned by the USB-IF)                     */
#define USBPD_PID (0x0000u)     /*!< Product ID (assigned by the manufacturer)              */
#define USBPD_XID (0x0000AAAAU) /*!< Value provided by the USB-IF assigned to the product   */
#define USBPD_PORT_NA   (0xFFU)              /*!< Port NA identifier */
/**
  * @} STM32_USBPD_APPLICATION_DPM_Config_Defines_Manu
  */
/**
  * @} STM32_USBPD_APPLICATION_DPM_Config_Defines
  */  

/* Exported typedef ----------------------------------------------------------*/
/* Exported variables ---------------------------------------------------------*/
/** @defgroup STM32_USBPD_APPLICATION_DPM_Config_Variables Variables
  * @{
  */
#if !defined(__USBPD_DPM_CORE_C) && !defined(__NO_DOXY)
extern USBPD_SettingsTypeDef            DPM_Settings[USBPD_PORT_COUNT];
extern USBPD_IdSettingsTypeDef          DPM_ID_Settings[USBPD_PORT_COUNT];
extern USBPD_USER_SettingsTypeDef       DPM_USER_Settings[USBPD_PORT_COUNT];
#else /* __USBPD_DPM_CORE_C */
#if USBPD_PORT_COUNT >= 3
#warning "Only 2 ports are configured"
#endif    
USBPD_SettingsTypeDef       DPM_Settings[USBPD_PORT_COUNT] =
{
  {
    .PE_SupportedSOP = USBPD_SUPPORTED_SOP_SOP, /* Supported SOP : SOP, SOP' SOP" SOP'Debug SOP"Debug      */
    .PE_SpecRevision = USBPD_SPECIFICATION_REV3,/* spec revision value                                     */
    .PE_DefaultRole = USBPD_PORTPOWERROLE_SRC,  /* Default port role                                       */
    .PE_RoleSwap = USBPD_FALSE,                 /* support port role swap                                  */
    .PE_VDMSupport = USBPD_FALSE,
    .PE_PingSupport = USBPD_FALSE,              /* support Ping (only for PD3.0)                           */
    .PE_CapscounterSupport = USBPD_FALSE,       /* support caps counter                                    */
    .CAD_RoleToggle = USBPD_FALSE,              /* cad role toggle                                         */
    .CAD_TryFeature = USBPD_FALSE,              /* cad try feature                                         */
    .CAD_AccesorySupport = USBPD_FALSE,         /* cas accessory support                                   */
    .PE_PD3_Support.d =                          /*!< PD3 SUPPORT FEATURE                                              */
    {
#if defined(_UNCHUNKED_SUPPORT)
      .PE_UnchunkSupport                = USBPD_TRUE,       /*!< Unchunked mode Support not supported   */
#else
      .PE_UnchunkSupport                = USBPD_FALSE,       /*!< Unchunked mode Support                */
#endif /* _UNCHUNKED_SUPPORT */
      .PE_FastRoleSwapSupport           = USBPD_FALSE,    /* support fast role swap only spec revsion 3.0                   */
      .Is_GetPPSStatus_Supported        = _PPS,           /*!< PPS message supported or not by DPM                          */
      .Is_SrcCapaExt_Supported          = _SRC_CAPA_EXT,  /*!< Source_Capabilities_Extended message supported or not by DPM */
      .Is_Alert_Supported               = _ALERT,         /*!< Alert message supported or not by DPM                        */
      .Is_GetStatus_Supported           = _STATUS,        /*!< Status message supported or not by DPM (Is_Alert_Supported should be enabled) */
      .Is_GetManufacturerInfo_Supported = _MANU_INFO,     /*!< Manufacturer_Info message supported or not by DPM            */
      .Is_GetCountryCodes_Supported     = USBPD_FALSE,    /*!< Country_Codes message supported or not by DPM                */
      .Is_GetCountryInfo_Supported      = USBPD_FALSE,    /*!< Country_Info message supported or not by DPM                 */
#if defined(_SECURITY_MSG)
      .Is_SecurityRequest_Supported     = USBPD_TRUE,     /*!< Security_Response message supported or not by PE */
#else
      .Is_SecurityRequest_Supported     = USBPD_FALSE,    /*!< Security_Response message supported or not by PE */
#endif /* _SECURITY_MSG */
      .Is_FirmUpdateRequest_Supported   = USBPD_FALSE,    /*!< Firmware update response message supported by PE             */
      .Is_GetBattery_Supported          = _BATTERY,          /*!< Get Battery Capabitity and Status messages supported by PE */
    },

    .CAD_SRCToggleTime = 40,                    /* uint8_t CAD_SRCToggleTime; */
    .CAD_SNKToggleTime = 40,                    /* uint8_t CAD_SNKToggleTime; */
    .CAD_DefaultResistor = vRp_3_0A,
#if USBPD_PORT_COUNT >= 2
  },
  {
    .PE_SupportedSOP = USBPD_SUPPORTED_SOP_SOP, /* Supported SOP : SOP, SOP' SOP" SOP'Debug SOP"Debug      */
    .PE_SpecRevision = USBPD_SPECIFICATION_REV3,/* spec revision value                                     */
    .PE_DefaultRole = USBPD_PORTPOWERROLE_SRC,  /* Default port role                                       */
    .PE_RoleSwap = USBPD_FALSE,                 /* support port role swap                                  */
    .PE_VDMSupport = USBPD_FALSE,
    .PE_RespondsToDiscovSOP = USBPD_FALSE,      /*!< Can respond successfully to a Discover Identity */
    .PE_AttemptsDiscovSOP = USBPD_FALSE,        /*!< Can send a Discover Identity */
    .PE_PingSupport = USBPD_FALSE,              /* support Ping (only for PD3.0)                           */
    .PE_CapscounterSupport = USBPD_FALSE,       /* support caps counter                                    */
    .CAD_RoleToggle = USBPD_FALSE,              /* cad role toggle                                         */
    .CAD_TryFeature = USBPD_FALSE,              /* cad try feature                                         */
    .CAD_AccesorySupport = USBPD_FALSE,         /* cas accessory support                                   */
    .PE_PD3_Support.d =                           /*!< PD3 SUPPORT FEATURE                                              */
    {
#if defined(_UNCHUNKED_SUPPORT)
      .PE_UnchunkSupport                = USBPD_TRUE,       /*!< Unchunked mode Support not supported   */
#else
      .PE_UnchunkSupport                = USBPD_FALSE,       /*!< Unchunked mode Support                */
#endif /* _UNCHUNKED_SUPPORT */
      .PE_FastRoleSwapSupport           = USBPD_FALSE,       /*!< Support fast role swap only spec revsion 3.0            */
      .Is_GetPPSStatus_Supported        = _PPS,              /*!< PPS message supported or not by DPM */
      .Is_SrcCapaExt_Supported          = _SRC_CAPA_EXT,     /*!< Source_Capabilities_Extended message supported or not by DPM */
      .Is_Alert_Supported               = _ALERT,            /*!< Alert message supported or not by DPM */
      .Is_GetStatus_Supported           = _STATUS,           /*!< Status message supported or not by DPM (Is_Alert_Supported should be enabled) */
      .Is_GetManufacturerInfo_Supported = _MANU_INFO,        /*!< Manufacturer_Info message supported or not by DPM */
      .Is_GetCountryCodes_Supported     = USBPD_TRUE,        /*!< Country_Codes message supported or not by DPM */
      .Is_GetCountryInfo_Supported      = USBPD_TRUE,        /*!< Country_Info message supported or not by DPM */
#if defined(_SECURITY_MSG)
      .Is_SecurityRequest_Supported     = USBPD_TRUE,        /*!< Security_Response message supported or not by PE */
#else
      .Is_SecurityRequest_Supported     = USBPD_FALSE,       /*!< Security_Response message supported or not by PE */
#endif /* _SECURITY_MSG */
      .Is_FirmUpdateRequest_Supported   = USBPD_FALSE,       /*!< Firmware update response message supported by PE */
      .Is_GetBattery_Supported          = _BATTERY,          /*!< Get Battery Capabitity and Status messages supported by PE */
    },

    .CAD_SRCToggleTime          = 40,           /* uint8_t CAD_SRCToggleTime; */
    .CAD_SNKToggleTime          = 40,           /* uint8_t CAD_SNKToggleTime; */
    .CAD_DefaultResistor = vRp_3_0A,
#endif /* USBPD_PORT_COUNT >= 2 */
  }
};

/* Section where Variable can be added */
USBPD_IdSettingsTypeDef          DPM_ID_Settings[USBPD_PORT_COUNT] =
{
  {
    .XID = USBPD_XID,     /*!< Value provided by the USB-IF assigned to the product   */
    .VID = USBPD_VID,     /*!< Vendor ID (assigned by the USB-IF)                     */
    .PID = USBPD_PID,     /*!< Product ID (assigned by the manufacturer)              */
      },
#if USBPD_PORT_COUNT >= 2
  {
    .XID = USBPD_XID,     /*!< Value provided by the USB-IF assigned to the product   */
    .VID = USBPD_VID,     /*!< Vendor ID (assigned by the USB-IF)                     */
    .PID = USBPD_PID,     /*!< Product ID (assigned by the manufacturer)              */
  }
#endif /* USBPD_PORT_COUNT >= 2 */
};

USBPD_USER_SettingsTypeDef DPM_USER_Settings[USBPD_PORT_COUNT] =
{
  {
#if defined(_DATA_ROLE_SWAP_DISABLED)
      .PE_DataSwap = USBPD_FALSE,                  /* support data swap                                       */
      .PE_DR_Swap_To_DFP = USBPD_FALSE,                  /*  Support of DR Swap to DFP                                  */
      .PE_DR_Swap_To_UFP = USBPD_FALSE,                  /*  Support of DR Swap to UFP                                  */
#else /* _DISCO */
      .PE_DataSwap = USBPD_TRUE,                  /* support data swap                                       */
      .PE_DR_Swap_To_DFP = USBPD_TRUE,                  /*  Support of DR Swap to DFP                                  */
      .PE_DR_Swap_To_UFP = USBPD_TRUE,                  /*  Support of DR Swap to UFP                                  */
#endif /* _DISCO */
      .PE_VconnSwap = USBPD_FALSE,                /* support VCONN swap                                  */
#if _SRC_CAPA_EXT
    .DPM_SRCExtendedCapa =                        /*!< SRC Extended Capability           */
      {
        .VID = USBPD_VID,      /*!< Vendor ID (assigned by the USB-IF)                   */
        .PID = USBPD_PID,      /*!< Product ID (assigned by the manufacturer)            */
        .XID = USBPD_XID,      /*!< Value provided by the USB-IF assigned to the product */
        .FW_revision = 1,      /*!< Firmware version number                              */
        .HW_revision = 2,      /*!< Hardware version number                              */
        .Voltage_regulation= 0,/*!< Voltage Regulation                                   */
        .Holdup_time = 0,      /*!< Holdup Time                                          */
        .Compliance = 0,       /*!< Compliance                                           */
        .TouchCurrent = 0,     /*!< Touch Current                                        */
        .PeakCurrent1 = 0,     /*!< Peak Current1                                        */
        .PeakCurrent2 = 0,     /*!< Peak Current2                                        */
        .PeakCurrent3 = 0,     /*!< Peak Current3                                        */
        .Touchtemp = 0,        /*!< Touch Temp                                           */
        .Source_inputs = 0,    /*!< Source Inputs                                        */
        .NbBatteries = 0,      /*!< Number of Batteries/Battery Slots                    */
        .SourcePDP = (uint8_t)USBPD_PDP_SRC_IN_WATTS,       /*!< Source PDP  5V*3A                                    */
      },
#endif /* _SRC_CAPA_EXT && (_SRC || _DRP) */
#if _MANU_INFO
    .DPM_ManuInfoPort =                      /*!< Manufacturer information used for the port            */
    {
      .VID = USBPD_VID,      /*!< Vendor ID (assigned by the USB-IF)                   */
      .PID = USBPD_PID,      /*!< Product ID (assigned by the manufacturer)            */
      .ManuString = "STMicroelectronics",    /*!< Vendor defined byte array                 */
    },
#endif /* _MANU_INFO */
#if defined(_GUI_INTERFACE)
    .PWR_AccessoryDetection     = USBPD_FALSE,  /*!< It enables or disables powered accessory detection */
    .PWR_AccessoryTransition    = USBPD_FALSE,  /*!< It enables or disables transition from Powered.accessory to Try.SNK */
    .PWR_UnconstrainedPower     = USBPD_CORE_PDO_NOT_EXT_POWERED, /*!< UUT has an external power source available that is sufficient to adequately power the system while charging external devices or the UUT’s primary function is to charge external devices. */
    .PWR_RpResistorValue        = vRd_3_0A,     /*!< RP resitor value based on @ref CAD_SNK_Source_Current_Adv_Typedef */
    .USB_Support                = USBPD_CORE_PDO_USBCOMM_NOT_CAPABLE, /*!< USB_Comms_Capable, is the UUT capable of enumerating as a USB host or device? */
    .USB_Device                 = USBPD_FALSE,  /*!< Type_C_Can_Act_As_Device, Indicates whether the UUT can communicate with USB 2.0 or USB 3.1 as a device or as the Upstream Facing Port of a hub. */
    .USB_Host                   = USBPD_FALSE,  /*!<  Type_C_Can_Act_As_Host, Indicates whether the UUT can communicate with USB 2.0 or USB 3.1 as a host or as the Downstream Facing Port of a hub */
    .USB_SuspendSupport         = USBPD_CORE_PDO_USBSUSP_NOT_SUPPORTED, /*!<  USB Suspend support values in PDO definition (Source) */
    .CAD_tDRP                   = 80,           /*!<  Type_C_Can_Act_As_Host, Indicates whether the UUT can communicate with USB 2.0 or USB 3.1 as a host or as the Downstream Facing Port of a hub */
    .CAD_dcSRC_DRP              = 50,           /*!<  USB Suspend support values in PDO definition (Source) */
#endif /* _GUI_INTERFACE */
  },
#if USBPD_PORT_COUNT >= 2
  {
      .PE_DataSwap = USBPD_TRUE,                  /* support data swap                                       */
      .PE_DR_Swap_To_DFP = USBPD_TRUE,                  /*  Support of DR Swap to DFP                                  */
      .PE_DR_Swap_To_UFP = USBPD_TRUE,                  /*  Support of DR Swap to UFP                                  */
      .PE_VconnSwap = USBPD_FALSE,                /* support VCONN swap                                  */
#if _SRC_CAPA_EXT
    .DPM_SRCExtendedCapa =                /*!< SRC Extended Capability                              */
      {
        .VID                = USBPD_VID,  /*!< Vendor ID (assigned by the USB-IF)                   */
        .PID                = USBPD_PID,  /*!< Product ID (assigned by the manufacturer)            */
        .XID                = USBPD_XID,  /*!< Value provided by the USB-IF assigned to the product */
        .FW_revision        = 1,          /*!< Firmware version number                              */
        .HW_revision        = 2,          /*!< Hardware version number                              */
        .Voltage_regulation = 0,          /*!< Voltage Regulation                                   */
        .Holdup_time        = 0,          /*!< Holdup Time                                          */
        .Compliance         = 0,          /*!< Compliance                                           */
        .TouchCurrent       = 0,          /*!< Touch Current                                        */
        .PeakCurrent1       = 0,          /*!< Peak Current1                                        */
        .PeakCurrent2       = 0,          /*!< Peak Current2                                        */
        .PeakCurrent3       = 0,          /*!< Peak Current3                                        */
        .Touchtemp          = 0,          /*!< Touch Temp                                           */
        .Source_inputs      = 0,          /*!< Source Inputs                                        */
        .NbBatteries        = 0,          /*!< Number of Batteries/Battery Slots                    */
        .SourcePDP = (uint8_t)USBPD_PDP_SRC_IN_WATTS,       /*!< Source PDP  5V*3A                                    */
      },
#endif /* _SRC_CAPA_EXT && (_SRC || _DRP) */
#if _MANU_INFO
    .DPM_ManuInfoPort =                      /*!< Manufacturer information used for the port            */
    {
      .VID = USBPD_VID,                      /*!< Vendor ID (assigned by the USB-IF)        */
      .PID = USBPD_PID,                      /*!< Product ID (assigned by the manufacturer) */
      .ManuString = "STMicroelectronics",    /*!< Vendor defined byte array                 */
    },
#endif /* _MANU_INFO */
#if defined(_GUI_INTERFACE)
    .PWR_AccessoryDetection     = USBPD_FALSE,  /*!< It enables or disables powered accessory detection */
    .PWR_AccessoryTransition    = USBPD_FALSE,  /*!< It enables or disables transition from Powered.accessory to Try.SNK */
    .PWR_UnconstrainedPower     = USBPD_CORE_PDO_NOT_EXT_POWERED, /*!< UUT has an external power source available that is sufficient to adequately power the system while charging external devices or the UUT’s primary function is to charge external devices. */
    .PWR_RpResistorValue        = vRd_3_0A,     /*!< RP resitor value based on @ref CAD_SNK_Source_Current_Adv_Typedef */
    .USB_Support                = USBPD_CORE_PDO_USBCOMM_NOT_CAPABLE, /*!< USB_Comms_Capable, is the UUT capable of enumerating as a USB host or device? */
    .USB_Device                 = USBPD_FALSE,  /*!< Type_C_Can_Act_As_Device, Indicates whether the UUT can communicate with USB 2.0 or USB 3.1 as a device or as the Upstream Facing Port of a hub. */
    .USB_Host                   = USBPD_FALSE,  /*!<  Type_C_Can_Act_As_Host, Indicates whether the UUT can communicate with USB 2.0 or USB 3.1 as a host or as the Downstream Facing Port of a hub */
    .USB_SuspendSupport         = USBPD_CORE_PDO_USBSUSP_NOT_SUPPORTED, /*!<  USB Suspend support values in PDO definition (Source) */
    .CAD_tDRP                   = 80,           /*!<  Type_C_Can_Act_As_Host, Indicates whether the UUT can communicate with USB 2.0 or USB 3.1 as a host or as the Downstream Facing Port of a hub */
    .CAD_dcSRC_DRP              = 50,           /*!<  USB Suspend support values in PDO definition (Source) */
#endif /* _GUI_INTERFACE */
  }
#endif /* USBPD_PORT_COUNT >= 2 */
};
#endif /* !__USBPD_DPM_CORE_C */
/**
  * @} STM32_USBPD_APPLICATION_DPM_Variables
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/**
  * @} STM32_USBPD_APPLICATION_DPM_Config
  */

/**
  * @} STM32_USBPD_APPLICATION
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBPD_DPM_CONF_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
