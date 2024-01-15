/**
  ******************************************************************************
  * @file    usbpd_pwr_if.c
  * @version 1.0.1
  * @date    2021-11-08
  * @author  MCD Application Team and SRA.SLDU.SC Team
  * @brief   This file contains power interface control functions.
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

#define __USBPD_PWR_IF_C

/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx.h"
#include "stpd01_reg.h"
#include "usbpd_pwr_if.h"
#include "usbpd_hw_if.h"
#include "usbpd_pwr_if.h"
#include "usbpd_dpm_core.h"
#include "usbpd_dpm_user.h"
#include "usbpd_dpm_conf.h"
#include "usbpd_pdo_defs.h"
#include "usbpd_devices_conf.h"
#include "usbpd_core.h"

#ifdef USE_PM
#include "usbpd_power_monitor_app.h"
#endif /* USE_PM */

#ifdef USE_PS
#include "usbpd_power_sharing_app.h"
#endif /* USE_PS */

#ifdef _TRACE
#include "usbpd_trace.h"
#endif
#if defined(_GUI_INTERFACE)
#include "gui_api.h"
#endif /* _GUI_INTERFACE */

#include "string.h"
#include "stdio.h"
#if (osCMSIS >= 0x20000U)
#include "task.h"
#endif /* osCMSIS >= 0x20000U */

/** @defgroup STM32_USBPD_APPLICATION_POWER_IF_Private_Defines Private Defines
  * @{
  */
/* Private define ------------------------------------------------------------*/
#if ((PORT0_NB_SOURCEPDO+PORT0_NB_SOURCEAPDO) > USBPD_MAX_NB_PDO)
#error "Nb of Source PDO/APDO is exceeding stack capabilities"
#endif
#if ((PORT0_NB_SINKPDO+PORT0_NB_SINKAPDO) > USBPD_MAX_NB_PDO)
#error "Nb of Sink PDO/APDO is exceeding stack capabilities"
#endif
#if USBPD_PORT_COUNT == 2
#if ((PORT1_NB_SOURCEPDO+PORT1_NB_SOURCEAPDO) > USBPD_MAX_NB_PDO)
#error "Nb of Source PDO/APDO is exceeding stack capabilities"
#endif
#if ((PORT1_NB_SINKPDO+PORT1_NB_SINKAPDO) > USBPD_MAX_NB_PDO)
#error "Nb of Sink PDO/APDO is exceeding stack capabilities"
#endif
#endif /* USBPD_PORT_COUNT == 2 */

/**
  * @} STM32_USBPD_APPLICATION_POWER_IF_Private_Defines
  */
/** @defgroup STM32_USBPD_APPLICATION_POWER_IF_Private_Macros Private Macros
  * @{
  */
/* Private macros ------------------------------------------------------------*/
#define ABS(__VAL__) ((__VAL__) < 0 ? - (__VAL__) : (__VAL__))
#ifdef _TRACE
#define POWER_IF_TRACE(_PORT_,_MSG_,_SIZE_) USBPD_TRACE_Add(USBPD_TRACE_DEBUG,_PORT_,0,(uint8_t *)_MSG_, _SIZE_);
#else
#define POWER_IF_TRACE(_PORT_,_MSG_,_SIZE_)
#endif
/**
  * @} STM32_USBPD_APPLICATION_POWER_IF_Private_Macros
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup STM32_USBPD_APPLICATION_POWER_IF_Private_Variables Private Variables
  * @{
  */
/**
  * @brief  USBPD Port safety data
  */
uint32_t                 vbus_disconnect = 0;
uint32_t                 vbus_transition = 0;

/**
  * @brief  USBPD Port PDO Storage array declaration
  */
USBPD_PWR_Port_PDO_Storage_TypeDef PWR_Port_PDO_Storage[USBPD_PORT_COUNT];

/**
  * @} STM32_USBPD_APPLICATION_POWER_IF_Private_Variables
  */

/* Private function prototypes -----------------------------------------------*/
/** @defgroup STM32_USBPD_APPLICATION_POWER_IF_Private_Functions Private Functions
  * @{
  */
uint32_t _PWR_SRCFixedPDO(float  C, float V,
                          USBPD_CORE_PDO_PeakCurr_TypeDef PK,
                          USBPD_CORE_PDO_DRDataSupport_TypeDef DRDSupport,
                          USBPD_CORE_PDO_USBCommCapable_TypeDef UsbCommCapable,
                          USBPD_CORE_PDO_ExtPowered_TypeDef ExtPower,
                          USBPD_CORE_PDO_USBSuspendSupport_TypeDef UsbSuspendSupport,
                          USBPD_CORE_PDO_DRPowerSupport_TypeDef DRPSupport);
uint32_t _PWR_SRCVariablePDO(float MAXV, float MINV, float C);
uint32_t _PWR_SRCBatteryPDO(float MAXV, float MINV, float POWER);
void     _PWR_CheckPDOContent(uint8_t PortNum);
/**
  * @} STM32_USBPD_APPLICATION_POWER_IF_Private_Functions
  */

/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Exported_Functions
  * @{
  */

/**
  * @brief  Initialize structures and variables related to power board profiles
  *         used by Sink and Source, for all available ports.
  * @retval USBPD status
  */
USBPD_StatusTypeDef USBPD_PWR_IF_Init(void)
{
  USBPD_StatusTypeDef _status = USBPD_OK;

  /* Set links to PDO values and number for Port 0 (defined in PDO arrays in H file).
    */
  PWR_Port_PDO_Storage[USBPD_PORT_0].SourcePDO.ListOfPDO = (uint32_t *) PORT0_PDO_ListSRC;
  PWR_Port_PDO_Storage[USBPD_PORT_0].SourcePDO.NumberOfPDO = &USBPD_NbPDO[1];


#if USBPD_PORT_COUNT == 2
  /* Set links to PDO values and number for Port 1 (defined in PDO arrays in H file).
   */
  PWR_Port_PDO_Storage[USBPD_PORT_1].SourcePDO.ListOfPDO = (uint32_t *)PORT1_PDO_ListSRC;
  PWR_Port_PDO_Storage[USBPD_PORT_1].SourcePDO.NumberOfPDO = &USBPD_NbPDO[3];

#endif /* USBPD_PORT_COUNT == 2 */

  /* Add consistency check on PDO definition
     Could help to detect unexpected flash settings content */
  _PWR_CheckPDOContent(USBPD_PORT_0);
#if USBPD_PORT_COUNT == 2
  _PWR_CheckPDOContent(USBPD_PORT_1);
#endif /* USBPD_PORT_COUNT == 2 */

  return _status;
}

/**
  * @brief  Sets the required power profile, now it works only with Fixed ones
  * @param  PortNum               Port number
  * @retval USBPD status
  */
USBPD_StatusTypeDef USBPD_PWR_IF_SetProfile(uint8_t PortNum)
{
  /* USER CODE BEGIN USBPD_PWR_IF_SetProfile */
  USBPD_StatusTypeDef      _ret = USBPD_ERROR;
  USBPD_PDO_TypeDef        _pdo;
  USBPD_SNKRDO_TypeDef     _rdo;

  uint32_t _vbusTargetInmv = 0;
  uint32_t _PowerProfileSelected =  DPM_Ports[PortNum].DPM_RDOPosition - 1;
//  uint8_t counter = 0;

  /* Check if valid port */
  /* Check if profile nb is valid for this port */
  if ((!USBPD_PORT_IsValid(PortNum)) || (_PowerProfileSelected >= *PWR_Port_PDO_Storage[PortNum].SourcePDO.NumberOfPDO)
      || (0 == *PWR_Port_PDO_Storage[PortNum].SourcePDO.NumberOfPDO))
  {
    return _ret;
  }

  _pdo.d32 = PWR_Port_PDO_Storage[PortNum].SourcePDO.ListOfPDO[_PowerProfileSelected];
  _rdo.d32 = DPM_Ports[PortNum].DPM_RcvRequestDOMsg;

  switch (_pdo.GenericPDO.PowerObject)
  {
    case USBPD_CORE_PDO_TYPE_FIXED :
    {
      _vbusTargetInmv               = _pdo.SRCFixedPDO.VoltageIn50mVunits * 50;
      uint32_t _OperatingCurrent    = _rdo.FixedVariableRDO.OperatingCurrentIn10mAunits * 10;
      uint32_t _MaxOperatingCurrent = _rdo.FixedVariableRDO.MaxOperatingCurrent10mAunits * 10;

      _ret = BSP_USBPD_PWR_VBUSSetVoltage_Fixed(PortNum, _vbusTargetInmv, _OperatingCurrent, _MaxOperatingCurrent) == BSP_ERROR_NONE ? USBPD_OK : USBPD_ERROR;
      break;
    }
    case USBPD_CORE_PDO_TYPE_VARIABLE :
    {
      _vbusTargetInmv               = _pdo.SRCVariablePDO.MinVoltageIn50mVunits * 50;
      uint32_t _vbusTargetMaxInmv   = _pdo.SRCVariablePDO.MaxVoltageIn50mVunits * 50;
      uint32_t _OperatingCurrent    = _rdo.FixedVariableRDO.OperatingCurrentIn10mAunits * 10;
      uint32_t _MaxOperatingCurrent = _rdo.FixedVariableRDO.MaxOperatingCurrent10mAunits * 10;

      _ret = BSP_USBPD_PWR_VBUSSetVoltage_Variable(PortNum, _vbusTargetInmv, _vbusTargetMaxInmv, _OperatingCurrent, _MaxOperatingCurrent) == BSP_ERROR_NONE ? USBPD_OK : USBPD_ERROR;
      break;
    }
    case USBPD_CORE_PDO_TYPE_APDO :
    case USBPD_CORE_PDO_TYPE_BATTERY :
    default :
    {
      /* not yet implemented */
      return _ret;
    }
  }

  USBPD_DPM_WaitForTime(20);
  
//  if (_ret == USBPD_OK)
//  {
//    BSP_USBPD_PWR_VBUSWaitValue(PortNum, _vbusTargetInmv, (_vbusTargetInmv * 5) / 100, 200, 25, USBPD_DPM_WaitForTime);
//  }

  return _ret;
}

/**
  * @brief  Checks if the power on a specified port is ready
  * @param  PortNum Port number
  * @param  Vsafe   Vsafe status based on USBPD_VSAFE_StatusTypeDef
  * @retval USBPD status
  */
USBPD_StatusTypeDef USBPD_PWR_IF_SupplyReady(uint8_t PortNum, USBPD_VSAFE_StatusTypeDef Vsafe)
{
  USBPD_StatusTypeDef status = USBPD_ERROR;
  uint32_t _voltage;

  /* check for valid port */
  if (!USBPD_PORT_IsValid(PortNum))
  {
    return USBPD_ERROR;
  }

  BSP_USBPD_PWR_VBUSGetVoltage(PortNum, &_voltage);
  if (USBPD_VSAFE_0V == Vsafe)
  {
    /* Vsafe0V */
    status = ((_voltage < USBPD_PWR_LOW_VBUS_THRESHOLD) ? USBPD_OK : USBPD_ERROR);
  }
  else
  {
    /* Vsafe5V */
    status = ((_voltage > USBPD_PWR_HIGH_VBUS_THRESHOLD) ? USBPD_OK : USBPD_ERROR);
  }

  return status;
}

/**
  * @brief  Enables VBUS power on a specified port
  * @param  PortNum Port number
  * @retval USBPD status
  */
USBPD_StatusTypeDef USBPD_PWR_IF_VBUSEnable(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_ERROR;

  /* check for valid port */
  if (USBPD_PORT_IsValid(PortNum))
  {
    POWER_IF_TRACE(PortNum, "VBUS_1", 7);
    /* Set the new state */
    _status = (USBPD_StatusTypeDef)HW_IF_PWR_Enable(PortNum, USBPD_ENABLE, DPM_Params[PortNum].VconnCCIs, DPM_Params[PortNum].VconnStatus, USBPD_PORTPOWERROLE_SRC);
  }
  return _status;
}

/**
  * @brief  Disbale VBUS/VCONN the power on a specified port
  * @param  PortNum Port number
  * @retval USBPD status
  */
USBPD_StatusTypeDef USBPD_PWR_IF_VBUSDisable(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_ERROR;

  /* check for valid port */
  if (USBPD_PORT_IsValid(PortNum))
  {
    POWER_IF_TRACE(PortNum, "VBUS_0", 7);
    /* Set the new state */
    _status = (USBPD_StatusTypeDef)HW_IF_PWR_Enable(PortNum, USBPD_DISABLE, DPM_Params[PortNum].VconnCCIs, DPM_Params[PortNum].VconnStatus, USBPD_PORTPOWERROLE_SRC);
    DPM_Params[PortNum].VconnStatus = USBPD_FALSE;
  }
  return _status;
}

/**
  * @brief  Checks if the power on a specified port is enabled
  * @param  PortNum Port number
  * @retval USBPD_ENABLE or USBPD_DISABLE
  */
USBPD_FunctionalState USBPD_PWR_IF_VBUSIsEnabled(uint8_t PortNum)
{
  /* Get the Status of the port */
  return USBPD_PORT_IsValid(PortNum) ? (USBPD_FunctionalState)HW_IF_PWR_VBUSIsEnabled(PortNum) : USBPD_DISABLE;
}

/**
  * @brief  Reads the voltage and the current on a specified port
  * @param  PortNum Port number
  * @param  pVoltage: The Voltage in mV
  * @param  pCurrent: The Current in mA
  * @retval USBPD_ERROR or USBPD_OK
  */
USBPD_StatusTypeDef USBPD_PWR_IF_ReadVA(uint8_t PortNum, uint16_t *pVoltage, uint16_t *pCurrent)
{
  /* check for valid port */
  if (!USBPD_PORT_IsValid(PortNum))
  {
    return USBPD_ERROR;
  }

  /* USBPD_OK if at least one pointer is not null, otherwise USBPD_ERROR */
  USBPD_StatusTypeDef ret = USBPD_ERROR;

  /* Get values from HW_IF */
  if (pVoltage != NULL)
  {
    *pVoltage = HW_IF_PWR_GetVoltage(PortNum);
    ret = USBPD_OK;
  }
  if (pCurrent != NULL)
  {
    *pCurrent = HW_IF_PWR_GetCurrent(PortNum);
    ret = USBPD_OK;
  }

  return ret;
}

/**
  * @brief  Allow PDO data reading from PWR_IF storage.
  * @param  PortNum Port number
  * @param  DataId Type of data to be read from PWR_IF
  *         This parameter can be one of the following values:
  *           @arg USBPD_CORE_DATATYPE_SRC_PDO Source PDO reading requested
  *           @arg USBPD_CORE_DATATYPE_SNK_PDO Sink PDO reading requested
  * @param  Ptr Pointer on address where PDO values should be written (u8 pointer)
  * @param  Size Pointer on nb of u32 written by PWR_IF (nb of PDOs)
  */
void USBPD_PWR_IF_GetPortPDOs(uint8_t PortNum, USBPD_CORE_DataInfoType_TypeDef DataId, uint8_t *Ptr, uint32_t *Size)
{
  uint32_t   nbpdo, index, nb_valid_pdo = 0;
  uint32_t   *ptpdoarray = NULL;
  USBPD_PDO_TypeDef pdo_first;

  /* Check if valid port */
  if (USBPD_PORT_IsValid(PortNum))
  {
    /* According to type of PDO to be read, set pointer on values and nb of elements */
    switch (DataId)
    {
      case USBPD_CORE_DATATYPE_SRC_PDO:
#ifdef USE_PS
        PS_Notification(PortNum, PS_EV_SOURCE_CAPA, 0);
#endif /* USE_PS */
        nbpdo       = *PWR_Port_PDO_Storage[PortNum].SourcePDO.NumberOfPDO;
        ptpdoarray  = PWR_Port_PDO_Storage[PortNum].SourcePDO.ListOfPDO;
        /* Save the 1st PDO */
        pdo_first.d32 = *ptpdoarray;
        /* Reset unchunked bit if current revision is PD2.0*/
        if (USBPD_SPECIFICATION_REV2 == DPM_Params[PortNum].PE_SpecRevision)
        {
          pdo_first.SRCFixedPDO.UnchunkedExtendedMessage  = USBPD_PDO_SRC_FIXED_UNCHUNK_NOT_SUPPORTED;
        }
        break;
      default:
        nbpdo = 0;
        break;
    }

    /* Copy PDO data in output buffer */
    for (index = 0; index < nbpdo; index++)
    {
      {
        /* Copy 1st PDO as potentially FRS or UNCHUNKED bits have been reset */
        if (0 == index)
        {
          (void)memcpy(Ptr, (uint8_t *)&pdo_first.d32, 4u);
        }
        else
        {
          (void)memcpy((Ptr + (nb_valid_pdo * 4u)), (uint8_t *)ptpdoarray, 4u);
        }
        nb_valid_pdo++;
      }
      ptpdoarray++;
    }
    /* Set nb of read PDO (nb of u32 elements); */
    *Size = nb_valid_pdo;
  }
}

/**
  * @brief  Find out SRC PDO pointed out by a position provided in a Request DO (from Sink).
  * @param  PortNum Port number
  * @param  RdoPosition RDO Position in list of provided PDO
  * @param  Pdo Pointer on PDO value pointed out by RDO position (u32 pointer)
  * @retval Status of search
  *         USBPD_OK : Src PDO found for requested DO position (output Pdo parameter is set)
  *         USBPD_FAIL : Position is not compliant with current Src PDO for this port (no corresponding PDO value)
  */
USBPD_StatusTypeDef USBPD_PWR_IF_SearchRequestedPDO(uint8_t PortNum, uint32_t RdoPosition, uint32_t *Pdo)
{
  if ((RdoPosition == 0) || (RdoPosition > *PWR_Port_PDO_Storage[PortNum].SourcePDO.NumberOfPDO))
  {
    /* Invalid PDO index */
    return USBPD_FAIL;
  }

  *Pdo = PWR_Port_PDO_Storage[PortNum].SourcePDO.ListOfPDO[RdoPosition - 1];
  return USBPD_OK;
}

/**
  * @brief  Function called in case of critical issue is detected to switch in safety mode.
  * @param  ErrorType Type of error detected by monitoring (based on @ref USBPD_PWR_IF_ERROR)
  */
void USBPD_PWR_IF_AlarmType(USBPD_PWR_IF_ERROR ErrorType)
{
  /* Disable the power on SRC */
  BSP_USBPD_PWR_VBUSOff(USBPD_PORT_0);

  /* Remove the resitor */
  USBPD_CAD_EnterErrorRecovery(USBPD_PORT_0);
#if USBPD_PORT_COUNT == 2
  /* Disable the power on SRC */
  BSP_USBPD_PWR_VBUSOff(USBPD_PORT_1);

  USBPD_CAD_EnterErrorRecovery(USBPD_PORT_1);
#endif /* USBPD_PORT_COUNT == 2 */

  /* Stop RTOS scheduling */
  vTaskSuspendAll();

  /* disable all interrupt to lock the system */
  __disable_irq();
  Error_Handler(0);
}

/**
  * @brief  Function called in case of critical issue is detected to switch in safety mode.
  */
void USBPD_PWR_IF_Alarm(void)
{
  USBPD_PWR_IF_AlarmType(USBPD_PWR_IF_OTHER);
}

/**
  * @brief Function is called to get VBUS power status.
  * @param PortNum Port number
  * @param PowerTypeStatus  Power type status based on @ref USBPD_VBUSPOWER_STATUS
  * @retval UBBPD_TRUE or USBPD_FALSE
  */
uint8_t USBPD_PWR_IF_GetVBUSStatus(uint8_t PortNum, USBPD_VBUSPOWER_STATUS PowerTypeStatus)
{
  uint8_t _status = USBPD_FALSE;
  uint32_t _vbus = HW_IF_PWR_GetVoltage(PortNum);
#if defined (_TRACE)
  uint8_t str[20];
#endif /* _TRACE */

  switch (PowerTypeStatus)
  {
    case USBPD_PWR_BELOWVSAFE0V :
      if (_vbus < USBPD_PWR_LOW_VBUS_THRESHOLD)
      {
        _status = USBPD_TRUE;
      }
      break;
    case USBPD_PWR_VSAFE5V :
      if (_vbus >= USBPD_PWR_HIGH_VBUS_THRESHOLD)
      {
        _status = USBPD_TRUE;
      }
      vbus_disconnect = vbus_transition = USBPD_PWR_HIGH_VBUS_THRESHOLD;
      break;
    case USBPD_PWR_SNKDETACH:
      if (vbus_transition != vbus_disconnect)
      {
        if (vbus_transition > vbus_disconnect)
        {
          /* Voltage increase the disconnect value is under vbus_transition */
          if (_vbus > (vbus_transition * 1.1))
          {
            /* the power transition is complete so disconnect thresold shall be updated */
            vbus_disconnect = vbus_transition;
#if defined (_TRACE)
            POWER_IF_TRACE(PortNum, "TRANSITION COMPLETE", 19);
            sprintf((char *)str, "THRESHOLD::%d", vbus_disconnect);
            POWER_IF_TRACE(PortNum, str, strlen((char *)str));
#endif /* _TRACE */
          }
        }
        else
        {
          /* voltage decrease, the disconnect voltage shall be switched to new level */
          vbus_disconnect = vbus_transition;
#if defined (_TRACE)
          POWER_IF_TRACE(PortNum, "NEW THRESHOLD", 14);
          sprintf((char *)str, "THRESHOLD::%d", vbus_disconnect);
          POWER_IF_TRACE(PortNum, str, strlen((char *)str));
#endif /* _TRACE */
        }
      }

      /* check disconnect according the current threshold */
      if (_vbus < vbus_disconnect)
      {
        _status = USBPD_TRUE;
      }
      break;
    default :
      break;
  }
  return _status;
}

/**
  * @brief Function is called to set the VBUS threshold when a request has been accepted.
  * @param PortNum Port number
  */
void USBPD_PWR_IF_UpdateVbusThreshold(uint8_t PortNum)
{
  USBPD_SNKRDO_TypeDef rdo;              /* get the requested RDO */
  USBPD_PDO_TypeDef    SelectedPDO;

  rdo.d32 = DPM_Ports[PortNum].DPM_RequestDOMsg;
  SelectedPDO.d32 = DPM_Ports[PortNum].DPM_ListOfRcvSRCPDO[rdo.GenericRDO.ObjectPosition - 1];

  switch (SelectedPDO.GenericPDO.PowerObject)
  {
    case USBPD_CORE_PDO_TYPE_FIXED :
    {
      switch (SelectedPDO.SRCFixedPDO.VoltageIn50mVunits * 50)
      {
        case 5000 :
          vbus_transition = USBPD_PWR_VBUS_THRESHOLD_5V;
          break;
        case 9000 :
          vbus_transition = USBPD_PWR_VBUS_THRESHOLD_9V;
          break;
        case 15000 :
          vbus_transition = USBPD_PWR_VBUS_THRESHOLD_15V;
          break;
        case 20000 :
          vbus_transition = USBPD_PWR_VBUS_THRESHOLD_20V;
          break;
      }
      break;
    }

#if defined(USBPDCORE_PPS)
    case USBPD_CORE_PDO_TYPE_APDO :
    {
      vbus_transition = USBPD_PWR_VBUS_THRESHOLD_APDO;
      break;
    }
#endif /*_USBPD_REV30_SUPPORT && PPS*/

    case USBPD_CORE_PDO_TYPE_BATTERY :
    case USBPD_CORE_PDO_TYPE_VARIABLE :
    {
      /* Not yet handled */
      break;
    }
  }
}

/**
  * @brief Function is called to reset the VBUS threshold when there is a power reset.
  * @param PortNum Port number
  */
void USBPD_PWR_IF_ResetVbusThreshold(uint8_t PortNum)
{
  POWER_IF_TRACE(PortNum, "RESET THRESHOLD", 15);
  vbus_disconnect = vbus_transition = USBPD_PWR_HIGH_VBUS_THRESHOLD;
}

/**
  * @} STM32_USBPD_APPLICATION_POWER_IF_Exported_Functions
  */

/** @addtogroup STM32_USBPD_APPLICATION_POWER_IF_Private_Functions
  * @{
  */
/**
  * @brief  Create SRC Fixed PDO object
  * @param  C                   Current in A
  * @param  V                   voltage in V
  * @param  PK                  The peak of current
  * @param  DRDSupport          Data Role Swap support indication
  * @param  UsbCommCapable      USB communications capability indication
  * @param  ExtPower            Port externally powered indication
  * @param  UsbSuspendSupport   USB Suspend support indication
  * @param  DRPSupport          Dual Role Power support indication
  * @retval PDO object value (expressed on u32)
  */
uint32_t _PWR_SRCFixedPDO(float  C, float V,
                          USBPD_CORE_PDO_PeakCurr_TypeDef PK,
                          USBPD_CORE_PDO_DRDataSupport_TypeDef DRDSupport,
                          USBPD_CORE_PDO_USBCommCapable_TypeDef UsbCommCapable,
                          USBPD_CORE_PDO_ExtPowered_TypeDef ExtPower,
                          USBPD_CORE_PDO_USBSuspendSupport_TypeDef UsbSuspendSupport,
                          USBPD_CORE_PDO_DRPowerSupport_TypeDef DRPSupport)
{
  USBPD_PDO_TypeDef pdo;

  pdo.d32 = 0;
  pdo.SRCFixedPDO.MaxCurrentIn10mAunits       = PWR_A_10MA(C);
  pdo.SRCFixedPDO.VoltageIn50mVunits          = PWR_V_50MV(V);
  pdo.SRCFixedPDO.PeakCurrent                 = PK;
  pdo.SRCFixedPDO.DataRoleSwap                = DRDSupport;
  pdo.SRCFixedPDO.USBCommunicationsCapable    = UsbCommCapable;
  pdo.SRCFixedPDO.ExternallyPowered           = ExtPower;
  pdo.SRCFixedPDO.USBSuspendSupported         = UsbSuspendSupport;
  pdo.SRCFixedPDO.DualRolePower               = DRPSupport;
  pdo.SRCFixedPDO.FixedSupply                 = USBPD_CORE_PDO_TYPE_FIXED;
  return pdo.d32;
}

/**
  * @brief  Create SRC Variable PDO object
  * @param  MAXV  Max voltage in V
  * @param  MINV  Min voltage in V
  * @param  C     Max current in A
  * @retval PDO object value (expressed on u32)
  */
uint32_t _PWR_SRCVariablePDO(float MAXV, float MINV, float C)
{
  USBPD_PDO_TypeDef pdo;

  pdo.d32 = 0;
  pdo.SRCVariablePDO.MaxCurrentIn10mAunits = PWR_A_10MA(C);
  pdo.SRCVariablePDO.MaxVoltageIn50mVunits = PWR_V_50MV(MAXV);
  pdo.SRCVariablePDO.MinVoltageIn50mVunits = PWR_V_50MV(MINV);
  pdo.SRCVariablePDO.VariableSupply        = USBPD_CORE_PDO_TYPE_VARIABLE;
  return pdo.d32;
}

/**
  * @brief  Create SRC Battery PDO object
  * @param  MAXV        Max voltage in V
  * @param  MINV        Min voltage in V
  * @param  POWER       Max allowable power in W
  * @retval PDO object value (expressed on u32)
  */
uint32_t _PWR_SRCBatteryPDO(float MAXV, float MINV, float POWER)
{
  USBPD_PDO_TypeDef pdo;

  pdo.d32 = 0;
  pdo.SRCBatteryPDO.MaxAllowablePowerIn250mWunits = PWR_W(POWER);
  pdo.SRCBatteryPDO.MinVoltageIn50mVunits         = PWR_V_50MV(MINV);
  pdo.SRCBatteryPDO.MaxVoltageIn50mVunits         = PWR_V_50MV(MAXV);
  pdo.SRCBatteryPDO.Battery                       = USBPD_CORE_PDO_TYPE_BATTERY;
  return pdo.d32;
}

/**
  * @brief  Add consistency check on PDO definition (Could help to detect unexpected flash settings content)
  * @param  PortNum Port number
  */
void _PWR_CheckPDOContent(uint8_t PortNum)
{
  uint32_t firstpdo;
  uint8_t error_in_content = 0U;

  /* Add consistency check on PDO definition for Ports 0 and 1 */

  /* SRC PDO
     - Nb of SRC PDO should be < USBPD_MAX_NB_PDO
     - if SRC PDO are defined, 1st SRC PDO should at least have following characteristics :
         Type Fixed (bits 31-30)
         bits 23-22 shall be reserved
         Voltage in 50mv units = 5V (bits19-10 should be equal to 100)
  */
  if (*PWR_Port_PDO_Storage[PortNum].SourcePDO.NumberOfPDO > USBPD_MAX_NB_PDO)
  {
    error_in_content = 1U;
  }
  else
  {
    if (*PWR_Port_PDO_Storage[PortNum].SourcePDO.NumberOfPDO >= 1U)
    {
      firstpdo = (PWR_Port_PDO_Storage[PortNum].SourcePDO.ListOfPDO[0U])
                 & (USBPD_PDO_TYPE_Msk | USBPD_PDO_SRC_FIXED_VOLTAGE_Msk | (0x3U << 22U));
      if (firstpdo != (USBPD_PDO_TYPE_FIXED | (100U << USBPD_PDO_SRC_FIXED_VOLTAGE_Pos)))
      {
        error_in_content = 1U;
      }
    }
  }


  if (error_in_content == 1U)
  {
    /* Stop RTOS scheduling */
    vTaskSuspendAll();


    /* disable all interrupt to lock the system */
    __disable_irq();
    while (1);
  }
}

/**
  * @} STM32_USBPD_APPLICATION_POWER_IF_Private_Functions
  */

/**
  * @} STM32_USBPD_APPLICATION_POWER_IF
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
