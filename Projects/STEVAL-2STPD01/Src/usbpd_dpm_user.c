/**
  ******************************************************************************
  * @file    usbpd_dpm_user.c
  * @version 1.0.1
  * @date    2021-11-08
  * @author  MCD Application Team and SRA.SLDU.SC Team
  * @brief   USBPD DPM user code
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

#define USBPD_DPM_USER_C

/** @addtogroup STM32_USBPD_APPLICATION_DPM_USER
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usbpd_pwr_if.h"
#include "usbpd_dpm_conf.h"

#if _ALERT
#include "usbpd_hw_if.h"
#endif /* _ALERT */

#ifdef USE_PM
#include "usbpd_power_monitor_app.h"
#endif /* USE_PM */

#ifdef USE_PS
#include "usbpd_power_sharing_app.h"
#endif /* USE_PS */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** @defgroup STM32_USBPD_APPLICATION_DPM_USER_PRIVATE_DEFINES Private Defines
  * @{
  */
#define DPM_TIMER_ENABLE_MSK      ((uint16_t)0x8000U)       /*!< Enable Timer Mask    */
#define DPM_TIMER_READ_MSK        ((uint16_t)0x7FFFU)       /*!< Read Timer Mask      */

#define DPM_BOX_MESSAGES_MAX      30u                       /*!< Max size queue msg   */
#if _SRC_CAPA_EXT

/* Timer used to send a GET_SRC_CAPA_EXT*/
#define DPM_TIMER_GET_SRC_CAPA_EXT  300u  /*!< 300ms */
#endif /* GENERATOR_SRC_CAPA_EXT_SUPPORT_YES */
#if _ALERT

/* Timer used to check if need to send an alert */
#define DPM_TIMER_ALERT             100u  /*!< 100ms */
#endif /* GENERATOR_ALERT_SUPPORT_YES */

#define DPM_NO_SRC_PDO_FOUND      0xFFU        /*!< No match found between Received SRC PDO and SNK capabilities                             */

#define DPM_GUI_NOTIF_ISCONNECTED       (1 << 5)
#define DPM_GUI_NOTIF_POWER_EVENT       (1 << 15)

#if (osCMSIS < 0x20000U)
#define DPM_USER_STACK_SIZE_ADDON_FOR_CMSIS              1
#else
#define DPM_USER_STACK_SIZE_ADDON_FOR_CMSIS              3
#endif /* osCMSIS < 0x20000U */
#define FREERTOS_DPM_PRIORITY                    osPriorityLow
#define FREERTOS_DPM_STACK_SIZE                 (300U * DPM_USER_STACK_SIZE_ADDON_FOR_CMSIS)
/**
  * @} STM32_USBPD_APPLICATION_DPM_USER_PRIVATE_DEFINES
  */

/** @defgroup STM32_USBPD_APPLICATION_DPM_USER_USER_TASK UserExecute Task definition
  * @{
  */
#if (osCMSIS < 0x20000U)
osThreadDef(DPM, USBPD_DPM_UserExecute, FREERTOS_DPM_PRIORITY, 0, FREERTOS_DPM_STACK_SIZE);
#else /* osCMSIS >= 0x20000U */
osThreadAttr_t DPM_Thread_Atrr =
{
  .name       = "DPM",
  .priority   = FREERTOS_DPM_PRIORITY, /*osPriorityLow,*/
  .stack_size = FREERTOS_DPM_STACK_SIZE
};
#endif /* osCMSIS < 0x20000U */
/**
  * @} STM32_USBPD_APPLICATION_DPM_USER_USER_TASK
  */

/* Private macro -------------------------------------------------------------*/
/** @defgroup STM32_USBPD_APPLICATION_DPM_USER_PRIVATE_MACROS Private Macros
  * @{
  */
#if defined(_TRACE)
#define DPM_USER_DEBUG_TRACE_SIZE       50u
#define DPM_USER_DEBUG_TRACE(_PORT_, ...)  do {                                                                \
      char _str[DPM_USER_DEBUG_TRACE_SIZE];                                                                    \
      uint8_t _size = snprintf(_str, DPM_USER_DEBUG_TRACE_SIZE, __VA_ARGS__);                                  \
      if (_size < DPM_USER_DEBUG_TRACE_SIZE)                                                                   \
        USBPD_TRACE_Add(USBPD_TRACE_DEBUG, (uint8_t)(_PORT_), 0, (uint8_t*)_str, strlen(_str));                \
      else                                                                                                     \
        USBPD_TRACE_Add(USBPD_TRACE_DEBUG, (uint8_t)(_PORT_), 0, (uint8_t*)_str, DPM_USER_DEBUG_TRACE_SIZE);   \
  } while(0)

#define DPM_USER_ERROR_TRACE(_PORT_, _STATUS_, ...)  do {                                                      \
    if (USBPD_OK != _STATUS_) {                                                                                \
        char _str[DPM_USER_DEBUG_TRACE_SIZE];                                                                  \
        uint8_t _size = snprintf(_str, DPM_USER_DEBUG_TRACE_SIZE, __VA_ARGS__);                                \
        if (_size < DPM_USER_DEBUG_TRACE_SIZE)                                                                 \
          USBPD_TRACE_Add(USBPD_TRACE_DEBUG, (uint8_t)(_PORT_), 0, (uint8_t*)_str, strlen(_str));              \
        else                                                                                                   \
          USBPD_TRACE_Add(USBPD_TRACE_DEBUG, (uint8_t)(_PORT_), 0, (uint8_t*)_str, DPM_USER_DEBUG_TRACE_SIZE); \
    }                                                                                                          \
  } while(0)
#else
#define DPM_USER_DEBUG_TRACE(_PORT_, ...)
#define DPM_USER_ERROR_TRACE(_PORT_, _STATUS_, ...)
#endif /* _TRACE */

#if (osCMSIS < 0x20000U)
#define DPM_START_TIMER(_PORT_,_TIMER_,_TIMEOUT_)   do{                                                               \
                                                      DPM_Ports[_PORT_]._TIMER_ = (_TIMEOUT_) |  DPM_TIMER_ENABLE_MSK;\
                                                      osMessagePut(DPMMsgBox,DPM_USER_EVENT_TIMER, 0);                \
                                                    }while(0);
#else
#define DPM_START_TIMER(_PORT_,_TIMER_,_TIMEOUT_)   do{                                                               \
                                                      uint32_t event = DPM_USER_EVENT_TIMER;                          \
                                                      DPM_Ports[_PORT_]._TIMER_ = (_TIMEOUT_) |  DPM_TIMER_ENABLE_MSK;\
                                                      (void)osMessageQueuePut(DPMMsgBox, &event, 0U, NULL);           \
                                                    }while(0);
#endif /* osCMSIS < 0x20000U */
#define IS_DPM_TIMER_RUNNING(_PORT_, _TIMER_)       ((DPM_Ports[_PORT_]._TIMER_ & DPM_TIMER_READ_MSK) > 0)
#define IS_DPM_TIMER_EXPIRED(_PORT_, _TIMER_)       (DPM_TIMER_ENABLE_MSK == DPM_Ports[_PORT_]._TIMER_)

#if defined(_DEBUG_TRACE)
#define __DPM_DEBUG_CALLBACK(_PORT_, __MESSAGE__)  USBPD_TRACE_Add(USBPD_TRACE_DEBUG,    (_PORT_), 0u,(__MESSAGE__), sizeof(__MESSAGE__) - 1u)
#else
#define __DPM_DEBUG_CALLBACK(_PORT_, __MESSAGE__)
#endif /* _DEBUG_TRACE */

/**
  * @} STM32_USBPD_APPLICATION_DPM_USER_PRIVATE_MACROS
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup USBPD_USER_PRIVATE_VARIABLES Private Variables
  * @{
  */

GUI_NOTIFICATION_POST         DPM_GUI_PostNotificationMessage   = NULL;
GUI_NOTIFICATION_FORMAT_SEND  DPM_GUI_FormatAndSendNotification = NULL;
GUI_SAVE_INFO                 DPM_GUI_SaveInfo                  = NULL;

osMessageQId  DPMMsgBox;

/**
  * @} USBPD_USER_PRIVATE_VARIABLES
  */

/* Private function prototypes -----------------------------------------------*/
/** @defgroup USBPD_USER_PRIVATE_FUNCTIONS Private Functions
  * @{
  */
static USBPD_StatusTypeDef DPM_TurnOnPower(uint8_t PortNum, USBPD_PortPowerRole_TypeDef Role);
static USBPD_StatusTypeDef DPM_TurnOffPower(uint8_t PortNum, USBPD_PortPowerRole_TypeDef Role);
static uint32_t CheckDPMTimers(void);

#if _SRC_CAPA_EXT
static void DPM_ManageExtendedCapa(void);
#endif /* _SRC_CAPA_EXT */
#if _ALERT
static void DPM_ManageAlert(void);
#endif /* _ALERT */

/**
  * @} USBPD_USER_PRIVATE_FUNCTIONS
  */

/* Private functions ------- -------------------------------------------------*/
/** @addtogroup USBPD_USER_PRIVATE_FUNCTIONS
  * @{
  */

/**
  * @brief  Turn Off power supply.
  * @param  PortNum The current port number
  * @param  Role    Port power role
  * @retval USBPD_OK, USBPD_ERROR
  */
static USBPD_StatusTypeDef DPM_TurnOffPower(uint8_t PortNum, USBPD_PortPowerRole_TypeDef Role)
{
  USBPD_StatusTypeDef status = USBPD_OK;

  status = USBPD_PWR_IF_VBUSDisable(PortNum);
  return status;
}

/**
  * @brief  Turn On power supply.
  * @param  PortNum The current port number
  * @param  Role    Port power role
  * @retval USBPD_ACCEPT, USBPD_WAIT, USBPD_REJECT
  */
static USBPD_StatusTypeDef DPM_TurnOnPower(uint8_t PortNum, USBPD_PortPowerRole_TypeDef Role)
{
  USBPD_StatusTypeDef status = USBPD_OK;
  /* Enable the output */
  status = USBPD_PWR_IF_VBUSEnable(PortNum);
  if(USBPD_PORTPOWERROLE_SRC == Role)
  {
    /* Enable the output */
    USBPD_DPM_WaitForTime(30);
  }
  else
  {
    /* stop current sink */
  }

  return status;
}

/**
  * @brief  Periodically called to check timers.
  * @retval time waiting
  */
static uint32_t CheckDPMTimers(void)
{
  uint32_t _timing = osWaitForever;
#if _SRC_CAPA_EXT || _ALERT
  uint32_t _current_timing;

  /* Calculate the minimum timers to wake-up DPM tasks */
  for(uint8_t instance = 0; instance < USBPD_PORT_COUNT; instance++)
  {
#if _SRC_CAPA_EXT
    _current_timing = DPM_Ports[instance].DPM_TimerSRCExtendedCapa & DPM_TIMER_READ_MSK;
    if(_current_timing > 0)
    {
      if (_current_timing < _timing)
      {
        _timing = _current_timing;
      }
    }
#endif /* _SRC_CAPA_EXT */
#if _ALERT
    _current_timing = DPM_Ports[instance].DPM_TimerAlert & DPM_TIMER_READ_MSK;
    if(_current_timing > 0)
    {
      if (_current_timing < _timing)
      {
        _timing = _current_timing;
      }
    }
#endif /* _ALERT */
  }
#endif /* USBPD_REV30_SUPPORT */
  return _timing;
}

#if _SRC_CAPA_EXT
/**
  * @brief  Manage the send of the message GET EXTENDED CAPA.
  */
void DPM_ManageExtendedCapa(void)
{
  for(uint8_t _instance = 0; _instance < USBPD_PORT_COUNT; _instance++)
  {
    /* -------------------------------------------------  */
    /* Check if send SRC extended capa timer is expired   */
    /* -------------------------------------------------  */
    if (DPM_TIMER_ENABLE_MSK == DPM_Ports[_instance].DPM_TimerSRCExtendedCapa)
    {
      DPM_Ports[_instance].DPM_TimerSRCExtendedCapa = 0;
      USBPD_DPM_RequestGetSourceCapabilityExt(_instance);
    }
  }
}

#endif /* _SRC_CAPA_EXT */
#if _ALERT
/**
  * @brief  Manage the ALERT.
  */
void DPM_ManageAlert(void)
{
  for(uint8_t _instance = 0; _instance < USBPD_PORT_COUNT; _instance++)
  {
    /* check if Alert timer is expired */
    if (DPM_TIMER_ENABLE_MSK == DPM_Ports[_instance].DPM_TimerAlert)
    {
      /* Restart alert timer */
      DPM_START_TIMER(_instance, DPM_TimerAlert, DPM_TIMER_ALERT);
      DPM_Ports[_instance].DPM_MeasuredCurrent = HW_IF_PWR_GetCurrent(_instance);
      if (DPM_Ports[_instance].DPM_MeasuredCurrent > 3600)
      {
        USBPD_DPM_RequestHardReset(_instance);
      }
      else
      {
        if (DPM_Ports[_instance].DPM_MeasuredCurrent > 3400)
        {
          if (0 == (DPM_Ports[_instance].DPM_SendAlert.b.TypeAlert & USBPD_ADO_TYPE_ALERT_OCP))
          {
            USBPD_ADO_TypeDef alert = {0};
            alert.b.TypeAlert = USBPD_ADO_TYPE_ALERT_OCP;
            USBPD_DPM_RequestAlert(_instance, alert);
            DPM_Ports[_instance].DPM_SendAlert.b.TypeAlert |= alert.b.TypeAlert;
          }
        }
        else
        {
          /* Reset of the OCP bit */
          DPM_Ports[_instance].DPM_SendAlert.b.TypeAlert &= ~USBPD_ADO_TYPE_ALERT_OCP;
        }
      }
    }
  }
}

#endif /* _ALERT */
/**
  * @} USBPD_USER_PRIVATE_FUNCTIONS
  */

/* Exported functions ------- ------------------------------------------------*/
/** @addtogroup USBPD_USER_EXPORTED_FUNCTIONS_GROUP1
  * @{
  */

/**
  * @brief  Initialize DPM (port power role, PWR_IF, CAD and PE Init procedures)
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_UserInit(void)
{
  /* PWR SET UP */
  if(USBPD_OK !=  USBPD_PWR_IF_Init())
  {
    return USBPD_ERROR;
  }

#ifdef USE_PM
  /* PM init */
  if(PM_STATUS_OK !=  PM_App_Init())
  {
    Error_Handler(0);
    return USBPD_ERROR;
  }
#endif /* USE_PM */

#ifdef USE_PS
  /* PS init */
  if(PS_STATUS_OK !=  PS_App_Init())
  {
    //Error_Handler(0);
    //return USBPD_ERROR;
  }
#endif /* USE_SM */

#if (osCMSIS < 0x20000U)
  osMessageQDef(MsgBox, DPM_BOX_MESSAGES_MAX, uint32_t);
  DPMMsgBox = osMessageCreate(osMessageQ(MsgBox), NULL);
  if(NULL == osThreadCreate(osThread(DPM), &DPMMsgBox))
#else
  DPMMsgBox = osMessageQueueNew (DPM_BOX_MESSAGES_MAX, sizeof(uint32_t), NULL);
  if (NULL == osThreadNew(USBPD_DPM_UserExecute, &DPMMsgBox, &DPM_Thread_Atrr))
#endif /* osCMSIS < 0x20000U */
  {
    return USBPD_ERROR;
  }


  return USBPD_OK;
}

/**
  * @brief  Function to set the function ptr linked to GUI interface
  * @param  PtrFormatSend Pointer on function to format and send GUI notifications
  * @param  PtrPost       Pointer on function to send GUI notifications
  * @param  PtrSaveInfo   Pointer on function to save information from Port Partner
  */
void USBPD_DPM_SetNotification_GUI(GUI_NOTIFICATION_FORMAT_SEND PtrFormatSend, GUI_NOTIFICATION_POST PtrPost,
                                   GUI_SAVE_INFO PtrSaveInfo)
{
  DPM_GUI_PostNotificationMessage   = PtrPost;
  DPM_GUI_FormatAndSendNotification = PtrFormatSend;
  DPM_GUI_SaveInfo                  = PtrSaveInfo;
}

/**
  * @brief  User delay implementation which is OS dependent
  * @param  Time time in ms
  */
void USBPD_DPM_WaitForTime(uint32_t Time)
{
  osDelay(Time);
}

/**
  * @brief  User processing time, it is recommended to avoid blocking task for long time
  * @param  argument  DPM User event
  */





#if (osCMSIS < 0x20000U)
void USBPD_DPM_UserExecute(void const *argument)
#else
void USBPD_DPM_UserExecute(void *argument)
#endif /* osCMSIS < 0x20000U */
{

  uint32_t _timing = osWaitForever;
  osMessageQId  queue = *(osMessageQId *)argument;
  
  do
  {
#if (osCMSIS < 0x20000U)
    osEvent event = osMessageGet(queue, _timing);
    switch (((DPM_USER_EVENT)event.value.v & 0xF))
#else
    uint32_t event;
    (void)osMessageQueueGet(queue, &event, NULL, _timing);
    switch (((DPM_USER_EVENT)event & 0xF))
#endif /* osCMSIS < 0x20000U */
    {
    case DPM_USER_EVENT_TIMER:
      {

#if _SRC_CAPA_EXT
        /* Manage the extended capa */
        DPM_ManageExtendedCapa();

#endif /* _SRC_CAPA_EXT */
#if _ALERT
        DPM_ManageAlert();

#endif /* _ALERT */
        break;
      }

    default:
      break;
    }


    _timing = CheckDPMTimers();
  }
  while(1);
}

/**
  * @brief  UserCableDetection reporting events on a specified port from CAD layer.
  * @param  PortNum The handle of the port
  * @param  State CAD state
  */
void USBPD_DPM_UserCableDetection(uint8_t PortNum, USBPD_CAD_EVENT State)
{
  switch(State)
  {
  case USBPD_CAD_EVENT_ATTEMC:
    {
  case USBPD_CAD_EVENT_ATTACHED:
    /* Format and send a notification to GUI if enabled */
    if (NULL != DPM_GUI_FormatAndSendNotification)
    {
      DPM_GUI_FormatAndSendNotification(PortNum, DPM_GUI_NOTIF_ISCONNECTED, 0);
    }

    if(USBPD_PORTPOWERROLE_SRC == DPM_Params[PortNum].PE_PowerRole)
    {
      if (USBPD_OK != USBPD_PWR_IF_VBUSEnable(PortNum))
      {
        /* Should not occurr */
        USBPD_DPM_WaitForTime(6000);
        Error_Handler(1);
        NVIC_SystemReset();
      }
    }
    DPM_Ports[PortNum].DPM_IsConnected = 1;
#ifdef USE_PS
    PS_Notification(PortNum, PS_EV_ATTACH, 0);
#endif /* USE_PS */
    break;
    }

  case USBPD_CAD_EVENT_DETACHED :
  case USBPD_CAD_EVENT_EMC :
  default :
    {

    /* reset all values received from port partner */
    memset(&DPM_Ports[PortNum], 0, sizeof(DPM_Ports[PortNum]));

    /* Format and send a notification to GUI if enabled */
    if (NULL != DPM_GUI_FormatAndSendNotification)
    {
      DPM_GUI_FormatAndSendNotification(PortNum, DPM_GUI_NOTIF_ISCONNECTED | DPM_GUI_NOTIF_POWER_EVENT, 0);
    }

    if(USBPD_PORTPOWERROLE_SRC == DPM_Params[PortNum].PE_PowerRole)
    {
      USBPD_PWR_IF_VBUSDisable(PortNum);
    }
#ifdef USE_PS
    PS_Notification(PortNum, PS_EV_DETACH, 0);
#endif /* USE_PS */
    break;
    }
  }
}

/**
  * @brief  function used to manage user timer.
  * @param  PortNum Port number
  */
void USBPD_DPM_UserTimerCounter(uint8_t PortNum)
{

#if _SRC_CAPA_EXT
  if((DPM_Ports[PortNum].DPM_TimerSRCExtendedCapa & DPM_TIMER_READ_MSK) > 0)
  {
    DPM_Ports[PortNum].DPM_TimerSRCExtendedCapa--;
  }
#endif /* _SRC_CAPA_EXT */
#if _ALERT
  if((DPM_Ports[PortNum].DPM_TimerAlert & DPM_TIMER_READ_MSK) > 0)
  {
    DPM_Ports[PortNum].DPM_TimerAlert--;
  }
#endif /* _ALERT */
}

void USBPD_DPM_TimerCounterUpdate(uint8_t PortNum, uint32_t Tick)
{
#if _SRC_CAPA_EXT
  if(DPM_Ports[PortNum].DPM_TimerSRCExtendedCapa != 0)
  {
    if((DPM_Ports[PortNum].DPM_TimerSRCExtendedCapa & DPM_TIMER_READ_MSK) > Tick)
      DPM_Ports[PortNum].DPM_TimerSRCExtendedCapa-= Tick;
    else
      DPM_Ports[PortNum].DPM_TimerSRCExtendedCapa = DPM_TIMER_ENABLE_MSK;
  }
#endif /* _SRC_CAPA_EXT */
#if _ALERT
  if(DPM_Ports[PortNum].DPM_TimerAlert  != 0)
  {
    if((DPM_Ports[PortNum].DPM_TimerAlert & DPM_TIMER_READ_MSK) > Tick)
      DPM_Ports[PortNum].DPM_TimerAlert-= Tick;
    else
      DPM_Ports[PortNum].DPM_TimerAlert = DPM_TIMER_ENABLE_MSK;
  }
#endif /* _ALERT */
}

/**
  * @} USBPD_USER_EXPORTED_FUNCTIONS_GROUP1
  */

/** @addtogroup USBPD_USER_EXPORTED_FUNCTIONS_GROUP2
  * @{
  */

/**
  * @brief  Callback function called by PE layer when HardReset message received from PRL
  * @param  PortNum     The current port number
  * @param  CurrentRole the current role
  * @param  Status      status on hard reset event
  */
void USBPD_DPM_HardReset(uint8_t PortNum, USBPD_PortPowerRole_TypeDef CurrentRole, USBPD_HR_Status_TypeDef Status)
{
#if _ALERT
  /* Stop Alert timer */
  DPM_Ports[PortNum].DPM_TimerAlert    = 0;
  DPM_Ports[PortNum].DPM_SendAlert.d32 = 0;
#endif /* USBPD_REV30_SUPPORT && _ALERT */

  switch (Status)
  {
  case USBPD_HR_STATUS_WAIT_VBUS_VSAFE0V:
    if (USBPD_PORTPOWERROLE_SRC == CurrentRole)
    {
      /* Reset the power supply */
      DPM_TurnOffPower(PortNum, USBPD_PORTPOWERROLE_SRC);
    }
    break;
  case USBPD_HR_STATUS_WAIT_VBUS_VSAFE5V:
    if (CurrentRole == USBPD_PORTPOWERROLE_SRC)
    {
      /* Power on the power supply */
      DPM_TurnOnPower(PortNum, CurrentRole);
    }
    break;
  case USBPD_HR_STATUS_COMPLETED:
    break;
  case USBPD_HR_STATUS_FAILED:
    break;
  default:
      break;
  }
}

/**
  * @brief  Request the DPM to setup the new power level.
  * @param  PortNum The current port number
  * @retval USBPD status
  */
USBPD_StatusTypeDef USBPD_DPM_SetupNewPower(uint8_t PortNum)
{
  return  USBPD_PWR_IF_SetProfile(PortNum);
}

/**
  * @brief  Evaluate the swap request from PE.
  * @param  PortNum The current port number
  * @retval USBPD_ACCEPT, USBPD_WAIT, USBPD_REJECT
  */
USBPD_StatusTypeDef USBPD_DPM_EvaluatePowerRoleSwap(uint8_t PortNum)
{
  return USBPD_REJECT;
}

/**
  * @brief  Callback function called by PE to inform DPM about PE event.
  * @param  PortNum The current port number
  * @param  EventVal USBPD_NotifyEventValue_TypeDef
  */
void USBPD_DPM_Notification(uint8_t PortNum, USBPD_NotifyEventValue_TypeDef EventVal)
{
  switch(EventVal)
  {
    /***************************************************************************
                              Power Notification
    */
    case USBPD_NOTIFY_POWER_EXPLICIT_CONTRACT :
    {
      /* Power ready means an explicit contract has been establish and Power is available */

#if _ALERT
      if ((USBPD_SPECIFICATION_REV3 == DPM_Params[PortNum].PE_SpecRevision)
       && (USBPD_PORTPOWERROLE_SRC == DPM_Params[PortNum].PE_PowerRole))
      {
        DPM_START_TIMER(PortNum, DPM_TimerAlert, DPM_TIMER_ALERT);
      }
#endif /* _ALERT */

#ifdef USE_PS
      PS_Notification(PortNum, PS_EV_EXPLICT_CONTRACT, 0);
#endif /* USE_PS */
    break;
    }
    /*
                              End Power Notification
     ***************************************************************************/


    case USBPD_NOTIFY_STATE_SRC_DISABLED:
      {
        /* SINK Port Partner is not PD capable. Legacy cable may have been connected
           In this state, VBUS is set to 5V */
      }
      break;

#if _ALERT
    case USBPD_NOTIFY_ALERT_RECEIVED :
      {
        /*!< Over-Current Protection event when set (Source only, for Sink Reserved and Shall be set to zero) */
        /* Bit reserved for a Sink */
        if ((USBPD_PORTPOWERROLE_SNK == DPM_Params[PortNum].PE_PowerRole)
                 && (USBPD_ADO_TYPE_ALERT_OCP == (DPM_Ports[PortNum].DPM_RcvAlert.b.TypeAlert & USBPD_ADO_TYPE_ALERT_OCP)))
        {
          /* Reset OCP bit */
          DPM_Ports[PortNum].DPM_RcvAlert.b.TypeAlert &= ~USBPD_ADO_TYPE_ALERT_OCP;
          /* Send a Get_Status */
          goto _ctrl_msg;
        }
        /*!< Over-Temperature Protection event when set. */
        else if (USBPD_ADO_TYPE_ALERT_OTP == (DPM_Ports[PortNum].DPM_RcvAlert.b.TypeAlert & USBPD_ADO_TYPE_ALERT_OTP))
        {
          /* Reset OTP bit */
          DPM_Ports[PortNum].DPM_RcvAlert.b.TypeAlert &= ~USBPD_ADO_TYPE_ALERT_OTP;
          /* Send a Get_Status */
          goto _ctrl_msg;
        }
        /*!< Operating Condition Change when set */
      else if (USBPD_ADO_TYPE_ALERT_OPERATING_COND == (DPM_Ports[PortNum].DPM_RcvAlert.b.TypeAlert &
                                                       USBPD_ADO_TYPE_ALERT_OPERATING_COND))
        {
          /* Reset OP COND bit */
          DPM_Ports[PortNum].DPM_RcvAlert.b.TypeAlert &= ~USBPD_ADO_TYPE_ALERT_OPERATING_COND;
          /* Send a Get_Status */
          goto _ctrl_msg;
        }
        /*!< Source Input Change Event when set */
      else if (USBPD_ADO_TYPE_ALERT_SRC_INPUT == (DPM_Ports[PortNum].DPM_RcvAlert.b.TypeAlert &
                                                  USBPD_ADO_TYPE_ALERT_SRC_INPUT))
        {
          /* Reset SRC input bit */
          DPM_Ports[PortNum].DPM_RcvAlert.b.TypeAlert &= ~USBPD_ADO_TYPE_ALERT_SRC_INPUT;
          /* Send a Get_Status */
          goto _ctrl_msg;
        }
        /*!< Over-Voltage Protection event when set (Sink only, for Source Reserved and Shall be set to zero) */
        /* Bit reserved for a Source */
        else
        {
          if ((USBPD_PORTPOWERROLE_SRC == DPM_Params[PortNum].PE_PowerRole)
                 && (USBPD_ADO_TYPE_ALERT_OVP == (DPM_Ports[PortNum].DPM_RcvAlert.b.TypeAlert & USBPD_ADO_TYPE_ALERT_OVP)))
          {
            /* Reset OVP bit */
            DPM_Ports[PortNum].DPM_RcvAlert.b.TypeAlert &= ~USBPD_ADO_TYPE_ALERT_OVP;
            /* Send a Get_Status */
            goto _ctrl_msg;
          }
        }

        _ctrl_msg:
          /* Post GET_STATUS message */
          USBPD_DPM_RequestGetStatus(PortNum);
      }
      break;
#endif /* ALERT */
    case USBPD_NOTIFY_MSG_NOT_SUPPORTED :
      {
      }
      break;

  
  default :
    break;
  }

  /* Forward PE notifications to GUI if enabled */
  if (NULL != DPM_GUI_PostNotificationMessage)
  {
    DPM_GUI_PostNotificationMessage(PortNum, EventVal);
  }
}

/**
  * @brief  DPM callback to allow PE to retrieve information from DPM/PWR_IF.
  * @param  PortNum Port number
  * @param  DataId  Type of data to be updated in DPM based on USBPD_CORE_DataInfoType_TypeDef
  * @param  Ptr     Pointer on address where DPM data should be written (u8 pointer)
  * @param  Size    Pointer on nb of u8 written by DPM
  */
void USBPD_DPM_GetDataInfo(uint8_t PortNum, USBPD_CORE_DataInfoType_TypeDef DataId, uint8_t *Ptr, uint32_t *Size)
{
  /* Check type of information targeted by request */
  switch (DataId)
  {
    /* Case Port Source PDO Data information :
    Case Port SINK PDO Data information :
    Call PWR_IF PDO reading request.
    */
  case USBPD_CORE_DATATYPE_SRC_PDO :
  case USBPD_CORE_DATATYPE_SNK_PDO :
    USBPD_PWR_IF_GetPortPDOs(PortNum, DataId, Ptr, Size);
    *Size *= 4;
    break;

    /* Case Requested voltage value Data information */
  case USBPD_CORE_DATATYPE_REQ_VOLTAGE :
    *Size = 4;
    (void)memcpy((uint8_t*)Ptr, (uint8_t *)&DPM_Ports[PortNum].DPM_RequestedVoltage, *Size);
    break;

#if _SRC_CAPA_EXT
  case USBPD_CORE_EXTENDED_CAPA :
    {
      *Size = sizeof(USBPD_SCEDB_TypeDef);
      memcpy((uint8_t*)Ptr, (uint8_t *)&DPM_USER_Settings[PortNum].DPM_SRCExtendedCapa, *Size);
    }
    break;
#endif /* _SRC_CAPA_EXT && (_SRC || _DRP) */
#if _STATUS
  case USBPD_CORE_INFO_STATUS :
    {
      USBPD_SDB_TypeDef  infostatus =
      {
        .InternalTemp = 0,          /*!< Source or Sink internal temperature in degrees centigrade */
        .PresentInput = 0,          /*!< Present Input                                             */
        .PresentBatteryInput = 0,   /*!< Present Battery Input                                     */
        .EventFlags = 0,            /*!< Event Flags                                               */
        .TemperatureStatus = 0,     /*!< Temperature                                               */
        .PowerStatus = 0,           /*!< Power Status based on combination of @ref USBPD_SDB_POWER_STATUS*/
      };

      *Size = sizeof(USBPD_SDB_TypeDef);
      memcpy((uint8_t *)Ptr, &infostatus, *Size);
    }
    break;
#endif /* _STATUS */
#if _MANU_INFO
  case USBPD_CORE_MANUFACTURER_INFO :
    {
      USBPD_MIDB_TypeDef manu_info;
      /* If the Manufacturer Info Target field or Manufacturer Info Ref field in the Get_Manufacturer_Info Message is
         unrecognized the field Shall return a null terminated ascii text string "Not Supported".*/
      if((DPM_Ports[PortNum].DPM_GetManufacturerInfo.ManufacturerInfoTarget > USBPD_MANUFINFO_TARGET_BATTERY)
      || (DPM_Ports[PortNum].DPM_GetManufacturerInfo.ManufacturerInfoRef > USBPD_MANUFINFO_REF_MAX_VALUES))
      {
        char *_notsupported = "Not Supported\0";
        memcpy((uint8_t*)manu_info.ManuString, (uint8_t *)_notsupported, 14);
        *Size = 4 + 14; /* VID (2) + .PID(2) + sizeof("Not Supported\0")*/
      }
      else
      {
        uint8_t size_string = strlen((char*)(DPM_USER_Settings[PortNum].DPM_ManuInfoPort.ManuString));
        *Size = 4 + size_string;
        memcpy((uint8_t *)manu_info.ManuString, (uint8_t *)(DPM_USER_Settings[PortNum].DPM_ManuInfoPort.ManuString),
               size_string);
      }

      /* If the Manufacturer Info Target field in the Get_Manufacturer_Info Message is Invalid, this VID field Shall be
         0xFFFF, and the associated PID field Should be set to 0x0000. */
      /* If the Manufacturer Info Target field in the  Get_Manufacturer_Info Message equals Battery (01b) and the Manufacturer Info Ref field is Invalid, this VID field
         Shall be 0xFFFF, and the associated PID field Should be set to 0x0000.*/
      if ((DPM_Ports[PortNum].DPM_GetManufacturerInfo.ManufacturerInfoTarget > USBPD_MANUFINFO_TARGET_BATTERY)
        ||
         ((DPM_Ports[PortNum].DPM_GetManufacturerInfo.ManufacturerInfoTarget == USBPD_MANUFINFO_TARGET_BATTERY)
         && (DPM_Ports[PortNum].DPM_GetManufacturerInfo.ManufacturerInfoRef > USBPD_MANUFINFO_REF_MAX_VALUES)))
      {
        manu_info.VID = 0xFFFFU;
        manu_info.PID = 0x0000U;
      }
      else
      {
        manu_info.VID = DPM_USER_Settings[PortNum].DPM_ManuInfoPort.VID;
        manu_info.PID = DPM_USER_Settings[PortNum].DPM_ManuInfoPort.PID;
      }
      memcpy((uint8_t*)Ptr, (uint8_t *)&manu_info, *Size);
    }
    break;
#endif /* _MANU_INFO */
  default :
    *Size = 0;
    break;
  }
}

/**
  * @brief  DPM callback to allow PE to update information in DPM/PWR_IF.
  * @param  PortNum Port number
  * @param  DataId  Type of data to be updated in DPM based on USBPD_CORE_DataInfoType_TypeDef
  * @param  Ptr     Pointer on the data
  * @param  Size    Nb of bytes to be updated in DPM
  */
void USBPD_DPM_SetDataInfo(uint8_t PortNum, USBPD_CORE_DataInfoType_TypeDef DataId, uint8_t *Ptr, uint32_t Size)
{
  uint32_t index;

  /* Check type of information targeted by request */
  switch (DataId)
  {
    /* Case requested DO position Data information :
    */
  case USBPD_CORE_DATATYPE_RDO_POSITION :
    if (Size == 4)
    {
      uint8_t* temp;
      temp = (uint8_t*)&DPM_Ports[PortNum].DPM_RDOPosition;
      (void)memcpy(temp, Ptr, Size);
      DPM_Ports[PortNum].DPM_RDOPositionPrevious = *Ptr;
      temp = (uint8_t*)&DPM_Ports[PortNum].DPM_RDOPositionPrevious;
      (void)memcpy(temp, Ptr, Size);
    }
    break;

    /* Case Received Source PDO values Data information :
    */
  case USBPD_CORE_DATATYPE_RCV_SRC_PDO :
    if (Size <= (USBPD_MAX_NB_PDO * 4))
    {
      uint8_t* rdo;
      DPM_Ports[PortNum].DPM_NumberOfRcvSRCPDO = (Size / 4);
      /* Copy PDO data in DPM Handle field */
      for (index = 0; index < (Size / 4); index++)
      {
        rdo = (uint8_t*)&DPM_Ports[PortNum].DPM_ListOfRcvSRCPDO[index];
        (void)memcpy(rdo, (Ptr + (index * 4u)), (4u * sizeof(uint8_t)));
      }
    }
    break;

    /* Case Received Sink PDO values Data information :
    */
  case USBPD_CORE_DATATYPE_RCV_SNK_PDO :
    if (Size <= (USBPD_MAX_NB_PDO * 4))
    {
      uint8_t* rdo;
      DPM_Ports[PortNum].DPM_NumberOfRcvSNKPDO = (Size / 4);
      /* Copy PDO data in DPM Handle field */
      for (index = 0; index < (Size / 4); index++)
      {
        rdo = (uint8_t*)&DPM_Ports[PortNum].DPM_ListOfRcvSNKPDO[index];
        (void)memcpy(rdo, (Ptr + (index * 4u)), (4u * sizeof(uint8_t)));
      }
    }
    break;

    /* Case Received Request PDO Data information :
    */
  case USBPD_CORE_DATATYPE_RCV_REQ_PDO :
    if (Size == 4)
    {
      uint8_t* rdo;
      rdo = (uint8_t*)&DPM_Ports[PortNum].DPM_RcvRequestDOMsg;
      (void)memcpy(rdo, Ptr, Size);
    }
    break;

#if _STATUS
  case USBPD_CORE_INFO_STATUS :
    {
      uint8_t* info_status;
      info_status = (uint8_t*)&DPM_Ports[PortNum].DPM_RcvStatus;
      memcpy(info_status, Ptr, Size);
    }
    break;
#endif /* _STATUS */
#if _SRC_CAPA_EXT
  case USBPD_CORE_EXTENDED_CAPA :
    {
      uint8_t*  ext_capa;
      ext_capa = (uint8_t*)&DPM_Ports[PortNum].DPM_RcvSRCExtendedCapa;
      memcpy(ext_capa, Ptr, Size);
    }
    break;
#endif /* _SRC_CAPA_EXT */
#if _MANU_INFO
  case USBPD_CORE_GET_MANUFACTURER_INFO:
    {
      uint8_t* temp = (uint8_t*)Ptr;
      DPM_Ports[PortNum].DPM_GetManufacturerInfo.ManufacturerInfoTarget = *temp;
      DPM_Ports[PortNum].DPM_GetManufacturerInfo.ManufacturerInfoRef    = *(temp + 1);
    }
    break;
#endif /* _MANU_INFO */
#if _ALERT
    case USBPD_CORE_ALERT:
      {
        uint8_t*  alert;
        alert = (uint8_t*)&DPM_Ports[PortNum].DPM_RcvAlert.d32;
        memcpy(alert, Ptr, Size);
      }
      break;
#endif /* _ALERT */

  /* In case of unexpected data type (Set request could not be fulfilled) :
   */
  default :
    break;
  }
  /* Forward info to GUI if enabled */
  if (NULL != DPM_GUI_SaveInfo)
  {
    DPM_GUI_SaveInfo(PortNum, DataId, Ptr, Size);
  }
}

/**
  * @brief  Evaluate received Request Message from Sink port
  * @param  PortNum Port number
  * @param  PtrPowerObject  Pointer on the power data object
  * @retval USBPD status : USBPD_ACCEPT, USBPD_REJECT, USBPD_WAIT, USBPD_GOTOMIN
  */
USBPD_StatusTypeDef USBPD_DPM_EvaluateRequest(uint8_t PortNum, USBPD_CORE_PDO_Type_TypeDef *PtrPowerObject)
{
  USBPD_SNKRDO_TypeDef rdo;
  USBPD_PDO_TypeDef pdo;
  uint32_t pdomaxcurrent = 0;
  uint32_t rdomaxcurrent = 0, rdoopcurrent = 0, rdoobjposition = 0;
  uint32_t rdovoltage;
  USBPD_HandleTypeDef *pdhandle = &DPM_Ports[PortNum];

  rdo.d32 = pdhandle->DPM_RcvRequestDOMsg;
  rdoobjposition  = rdo.GenericRDO.ObjectPosition;
#if defined(_UNCHUNKED_SUPPORT)
  /* Set unchuncked bit if supported by ports */
  DPM_Params[PortNum].PE_UnchunkSupport   = USBPD_FALSE;
  if ((USBPD_TRUE == rdo.GenericRDO.UnchunkedExtendedMessage)
   && (USBPD_TRUE == DPM_Settings[PortNum].PE_PD3_Support.d.PE_UnchunkSupport))
  {
    DPM_Params[PortNum].PE_UnchunkSupport   = USBPD_TRUE;
  }
#endif /* USBPD_REV30_SUPPORT && _UNCHUNKED_SUPPORT */
  pdhandle->DPM_RDOPosition = 0;

  /* Check if RDP can be met within the supported PDOs by the Source port */
  /* USBPD_DPM_EvaluateRequest: Evaluate Sink Request\r */
  /* USBPD_DPM_EvaluateRequest: Check if RDP can be met within the supported PDOs by the Source port\r */

  /* Search PDO in Port Source PDO list, that corresponds to Position provided in Request RDO */
  if (USBPD_PWR_IF_SearchRequestedPDO(PortNum, rdoobjposition, &pdo.d32) != USBPD_OK)
  {
    /* Invalid PDO index */
    /* USBPD_DPM_EvaluateRequest: Invalid PDOs index */
    //PS_Notification(PortNum, PS_EV_REQUEST_REJECT, 0x01000000 | rdoobjposition);
    return USBPD_REJECT;
  }

  switch(pdo.GenericPDO.PowerObject)
  {
  case USBPD_CORE_PDO_TYPE_FIXED:
    {
      pdomaxcurrent = pdo.SRCFixedPDO.MaxCurrentIn10mAunits * 10;
      rdomaxcurrent = rdo.FixedVariableRDO.MaxOperatingCurrent10mAunits * 10;
      rdoopcurrent  = rdo.FixedVariableRDO.OperatingCurrentIn10mAunits * 10;
      rdovoltage    = pdo.SRCFixedPDO.VoltageIn50mVunits * 50;
      
      if(rdoopcurrent > pdomaxcurrent)
      {
        /* Sink requests too much operating current */
        /* USBPD_DPM_EvaluateRequest: Sink requests too much operating current*/
        //PS_Notification(PortNum, PS_EV_REQUEST_REJECT, 0x02000000 | rdoopcurrent);
        return USBPD_REJECT;
      }
      
      if(rdomaxcurrent > pdomaxcurrent)
      {
        /* Sink requests too much maximum operating current */
        /* USBPD_DPM_EvaluateRequest: Sink requests too much maximum operating current */
        //PS_Notification(PortNum, PS_EV_REQUEST_REJECT, 0x03000000 | rdomaxcurrent);
        return USBPD_REJECT;
        
      }        
      DPM_Ports[PortNum].DPM_RequestedCurrent = rdoopcurrent;
      DPM_Ports[PortNum].DPM_RequestedMaxCurrent = rdomaxcurrent;
    }
    break;
  case USBPD_CORE_PDO_TYPE_BATTERY:
  case USBPD_CORE_PDO_TYPE_VARIABLE:
  case USBPD_CORE_PDO_TYPE_APDO:
  default:
    {
      //PS_Notification(PortNum, PS_EV_REQUEST_REJECT, 0xFF000000 | pdo.GenericPDO.PowerObject);
      return USBPD_REJECT;
    }
  }

  /* Set RDO position and requested voltage in DPM port structure */
  DPM_Ports[PortNum].DPM_RequestedVoltage = rdovoltage;
  pdhandle->DPM_RDOPositionPrevious = pdhandle->DPM_RDOPosition;
  pdhandle->DPM_RDOPosition = rdoobjposition;

  /* Save the power object */
  *PtrPowerObject = pdo.GenericPDO.PowerObject;

  /* Accept the requested power */
  /* USBPD_DPM_EvaluateRequest: Sink requested %d mV %d mA for operating current from %d to %d mA\r",
               pdo.SRCFixedPDO.VoltageIn50mVunits * 50, pdo.SRCFixedPDO.MaxCurrentIn10mAunits * 10,
               rdo.FixedVariableRDO.MaxOperatingCurrent10mAunits * 10, rdo.FixedVariableRDO.OperatingCurrentIn10mAunits * 10 */
  /* USBPD_DPM_EvaluateRequest: Source accepts the requested power */
#ifdef USE_PS
  PS_Notification(PortNum, PS_EV_RECEIVED_REQUEST, 0);
#endif /* USE_PS */

  return USBPD_ACCEPT;
}

/**
  * @brief  DPM callback to allow PE to forward extended message information.
  * @param  PortNum   Port number
  * @param  MsgType   Type of message to be handled in DPM
  *         This parameter can be one of the following values:
  *           @arg USBPD_EXT_SECURITY_REQUEST Security Request extended message
  *           @arg USBPD_EXT_SECURITY_RESPONSE Security Response extended message
  * @param  ptrData   Pointer on address Extended Message data could be read (u8 pointer)
  * @param  DataSize  Nb of u8 that compose Extended message
  */
void USBPD_DPM_ExtendedMessageReceived(uint8_t PortNum, USBPD_ExtendedMsg_TypeDef MsgType, uint8_t *ptrData,
                                       uint16_t DataSize)
{
  if (DataSize == 0)
  {
    /* No data received. */
    return;
  }

  switch(MsgType)
  {
#ifdef _SECURITY_MSG
    case USBPD_EXT_SECURITY_REQUEST :
      {
        uint8_t data[60] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60};
        USBPD_PE_SendExtendedMessage(PortNum, USBPD_SOPTYPE_SOP, USBPD_EXT_SECURITY_RESPONSE, data, 60);
      }
      break;
    case USBPD_EXT_SECURITY_RESPONSE :
      break;
#endif /* _SECURITY_MSG */

#ifdef _COUNTRY_MSG
    case USBPD_EXT_COUNTRY_INFO :
    case USBPD_EXT_COUNTRY_CODES :
      break;
#endif /* _COUNTRY_MSG */
    default:
      break;
  }
}

/**
  * @brief  DPM callback to allow PE to enter ERROR_RECOVERY state.
  * @param  PortNum Port number
  */
void USBPD_DPM_EnterErrorRecovery(uint8_t PortNum)
{
  /* Inform CAD to enter recovery mode */
  USBPD_CAD_EnterErrorRecovery(PortNum);
}

/**
  * @brief  Callback used to ask application the reply status for a DataRoleSwap request
  * @note   if the callback is not set (ie NULL) the stack will automatically reject the request
  * @param  PortNum Port number
  * @retval Returned values are:
            USBPD_ACCEPT if DRS can be accepted
            USBPD_REJECT if DRS is not accepted in one data role (DFP or UFP) or in PD2.0 config
            USBPD_NOTSUPPORTED if DRS is not supported at all by the application (in both data roles) - P3.0 only
  */
USBPD_StatusTypeDef USBPD_DPM_EvaluateDataRoleSwap(uint8_t PortNum)
{
  USBPD_StatusTypeDef status = USBPD_REJECT;
  /* Sent NOT_SUPPORTED if DRS is not supported at all by the application (in both data roles) - P3.0 only */
  if ((USBPD_FALSE == DPM_USER_Settings[PortNum].PE_DataSwap)
      || ((USBPD_FALSE == DPM_USER_Settings[PortNum].PE_DR_Swap_To_DFP)
          && (USBPD_FALSE == DPM_USER_Settings[PortNum].PE_DR_Swap_To_UFP)))
  {
    status = USBPD_NOTSUPPORTED;
  }
  else
  {
    /* ACCEPT DRS if at least supported by 1 data role */
    if (((USBPD_TRUE == DPM_USER_Settings[PortNum].PE_DR_Swap_To_DFP)
         && (USBPD_PORTDATAROLE_UFP == DPM_Params[PortNum].PE_DataRole))
        || ((USBPD_TRUE == DPM_USER_Settings[PortNum].PE_DR_Swap_To_UFP) && (USBPD_PORTDATAROLE_DFP == DPM_Params[PortNum].PE_DataRole)))
    {
      status = USBPD_ACCEPT;
    }
  }
  return status;
}

/**
  * @brief  Callback to be used by PE to check is VBUS is ready or present
  * @param  PortNum Port number
  * @param  Vsafe   Vsafe status based on USBPD_VSAFE_StatusTypeDef
  * @retval USBPD_DISABLE or USBPD_ENABLE
  */
USBPD_FunctionalState USBPD_DPM_IsPowerReady(uint8_t PortNum, USBPD_VSAFE_StatusTypeDef Vsafe)
{
  return ((USBPD_OK == USBPD_PWR_IF_SupplyReady(PortNum, Vsafe)) ? USBPD_ENABLE : USBPD_DISABLE);
}

/**
  * @} USBPD_USER_EXPORTED_FUNCTIONS_GROUP2
  */

/** @addtogroup USBPD_USER_EXPORTED_FUNCTIONS_GROUP3
  * @{
  */

/**
  * @brief  Request the PE to send a hard reset
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestHardReset(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_ERROR;
  if (DPM_Ports[PortNum].DPM_IsConnected)
  {
    _status = USBPD_PE_Request_HardReset(PortNum);
    DPM_USER_ERROR_TRACE(PortNum, _status, "HARD RESET not accepted by the stack");
  }
  /*
  else
  {
    DPM_USER_ERROR_TRACE(PortNum, _status, "HARD RESET not send, port not connected");
  }
  */
  return _status;
}

/**
  * @brief  Request the PE to send a cable reset.
  * @note   Only a DFP Shall generate Cable Reset Signaling. A DFP Shall only generate Cable Reset Signaling within an Explicit Contract.
            The DFP has to be supplying VCONN prior to a Cable Reset
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestCableReset(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CableReset(PortNum);
  DPM_USER_ERROR_TRACE(PortNum, _status, "CABLE RESET not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a GOTOMIN message
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGotoMin(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_GOTOMIN, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "GOTOMIN not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a PING message
  * @note   In USB-PD stack, only ping management for P3.0 is implemented.
  *         If PD2.0 is used, PING timer needs to be implemented on user side.
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestPing(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_PING, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "PING not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a request message.
  * @param  PortNum     The current port number
  * @param  IndexSrcPDO Index on the selected SRC PDO (value between 1 to 7)
  * @param  RequestedVoltage Requested voltage (in MV and use mainly for APDO)
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestMessageRequest(uint8_t PortNum, uint8_t IndexSrcPDO, uint16_t RequestedVoltage)
{
  USBPD_StatusTypeDef _status = USBPD_ERROR;
  DPM_USER_ERROR_TRACE(PortNum, _status, "REQUEST not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a GET_SRC_CAPA message
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetSourceCapability(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_GET_SRC_CAP, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "GET_SRC_CAPA not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a GET_SNK_CAPA message
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetSinkCapability(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_GET_SNK_CAP, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "GET_SINK_CAPA not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to perform a Data Role Swap.
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestDataRoleSwap(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_DR_SWAP, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "DRS not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to perform a Power Role Swap.
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestPowerRoleSwap(uint8_t PortNum)
{
  DPM_USER_ERROR_TRACE(PortNum, USBPD_ERROR, "PRS not accepted by the stack");
  return USBPD_ERROR;
}

/**
  * @brief  Request the PE to perform a VCONN Swap.
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestVconnSwap(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_VCONN_SWAP, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "VCS not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a soft reset
  * @param  PortNum The current port number
  * @param  SOPType SOP Type based on USBPD_SOPType_TypeDef
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestSoftReset(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_SOFT_RESET, SOPType);
  DPM_USER_ERROR_TRACE(PortNum, _status, "SOFT_RESET not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a Source Capability message.
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestSourceCapability(uint8_t PortNum)
{
  /* PE will directly get the PDO saved in structure @ref PWR_Port_PDO_Storage */
  USBPD_StatusTypeDef _status = USBPD_PE_Request_DataMessage(PortNum, USBPD_DATAMSG_SRC_CAPABILITIES, NULL);
  DPM_USER_ERROR_TRACE(PortNum, _status, "SRC_CAPA not accepted by the stack");
  return _status;
}

#if defined(_UVDM)
/**
  * @brief  Called by DPM to request the PE to send a UVDM message.
  * @param  PortNum Index of current used port
  * @param  SOPType Received message type based on @ref USBPD_SOPType_TypeDef
  * @retval USBPD status : USBPD_BUSY, USBPD_OK, USBPD_FAIL
  */
USBPD_StatusTypeDef USBPD_DPM_RequestUVDMMessage(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType)
{
  USBPD_StatusTypeDef _status = USBPD_PE_UVDM_RequestMessage(PortNum, SOPType);
  DPM_USER_ERROR_TRACE(PortNum, _status, "UVDM not accepted by the stack");
  return _status;
}

#endif /* _UVDM */
/**
  * @brief  Request the PE to send an ALERT to port partner
  * @param  PortNum The current port number
  * @param  Alert   Alert based on USBPD_ADO_TypeDef
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestAlert(uint8_t PortNum, USBPD_ADO_TypeDef Alert)
{
#if _ALERT
  USBPD_StatusTypeDef _status = USBPD_PE_Request_DataMessage(PortNum, USBPD_DATAMSG_ALERT, (uint32_t*)&Alert.d32);
  DPM_USER_ERROR_TRACE(PortNum, _status, "ALERT not accepted by the stack");
  return _status;
#else
  DPM_USER_ERROR_TRACE(PortNum, USBPD_ERROR, "ALERT not accepted by the stack");
  return USBPD_ERROR;
#endif /* _ALERT */
}

/**
  * @brief  Request the PE to get a source capability extended
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetSourceCapabilityExt(uint8_t PortNum)
{
#if _SRC_CAPA_EXT
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_GET_SRC_CAPEXT, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "GET_SRC_CAPA_EXT not accepted by the stack");
  return _status;
#else
  DPM_USER_ERROR_TRACE(PortNum, USBPD_ERROR, "GET_SRC_CAPA_EXT not accepted by the stack");
  return USBPD_ERROR;
#endif /* _SRC_CAPA_EXT*/
}

/**
  * @brief  Request the PE to get a sink capability extended
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetSinkCapabilityExt(uint8_t PortNum)
{
  DPM_USER_ERROR_TRACE(PortNum, USBPD_ERROR, "GET_SINK_CAPA_EXT not accepted by the stack");
  return USBPD_ERROR;
}

/**
  * @brief  Request the PE to get a manufacturer info
  * @param  PortNum The current port number
  * @param  SOPType SOP Type
  * @param  pManuInfoData Pointer on manufacturer info based on USBPD_GMIDB_TypeDef
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetManufacturerInfo(uint8_t PortNum, USBPD_SOPType_TypeDef SOPType,
                                                         uint8_t *pManuInfoData)
{
#if _MANU_INFO
  USBPD_StatusTypeDef _status = USBPD_PE_SendExtendedMessage(PortNum, SOPType, USBPD_EXT_GET_MANUFACTURER_INFO, (uint8_t*)pManuInfoData, sizeof(USBPD_GMIDB_TypeDef));
  DPM_USER_ERROR_TRACE(PortNum, _status, "GET_MANU_INFO not accepted by the stack");
  return _status;
#else
  DPM_USER_ERROR_TRACE(PortNum, USBPD_ERROR, "GET_MANU_INFO not accepted by the stack");
  return USBPD_ERROR;
#endif /* _MANU_INFO */
}

/**
  * @brief  Request the PE to request a GET_PPS_STATUS
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetPPS_Status(uint8_t PortNum)
{
  DPM_USER_ERROR_TRACE(PortNum, USBPD_ERROR, "GET_PPS_STATUS not accepted by the stack");
  return USBPD_ERROR;
}

/**
  * @brief  Request the PE to request a GET_STATUS
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetStatus(uint8_t PortNum)
{
#if _STATUS
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_GET_STATUS, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "GET_STATUS not accepted by the stack");
  return _status;
#else
  DPM_USER_ERROR_TRACE(PortNum, USBPD_ERROR, "GET_STATUS not accepted by the stack");
  return USBPD_ERROR;
#endif /* _STATUS*/
}

/**
  * @brief  Request the PE to perform a Fast Role Swap.
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestFastRoleSwap(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_FR_SWAP, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "FRS not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a GET_COUNTRY_CODES message
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetCountryCodes(uint8_t PortNum)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_CtrlMessage(PortNum, USBPD_CONTROLMSG_GET_COUNTRY_CODES, USBPD_SOPTYPE_SOP);
  DPM_USER_ERROR_TRACE(PortNum, _status, "GET_COUNTRY_CODES not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a GET_COUNTRY_INFO message
  * @param  PortNum     The current port number
  * @param  CountryCode Country code (1st character and 2nd of the Alpha-2 Country)
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetCountryInfo(uint8_t PortNum, uint16_t CountryCode)
{
  USBPD_StatusTypeDef _status = USBPD_PE_Request_DataMessage(PortNum, USBPD_DATAMSG_GET_COUNTRY_INFO, (uint32_t*)&CountryCode);
  DPM_USER_ERROR_TRACE(PortNum, _status, "GET_COUNTRY_INFO not accepted by the stack");
  return _status;
}

/**
  * @brief  Request the PE to send a GET_BATTERY_CAPA
  * @param  PortNum         The current port number
  * @param  pBatteryCapRef  Pointer on the Battery Capability reference
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetBatteryCapability(uint8_t PortNum, uint8_t *pBatteryCapRef)
{
  DPM_USER_ERROR_TRACE(PortNum, USBPD_ERROR, "GET_BATTERY_CAPA not accepted by the stack");
  return USBPD_ERROR;
}

/**
  * @brief  Request the PE to send a GET_BATTERY_STATUS
  * @param  PortNum           The current port number
  * @param  pBatteryStatusRef Pointer on the Battery Status reference
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestGetBatteryStatus(uint8_t PortNum, uint8_t *pBatteryStatusRef)
{
  DPM_USER_ERROR_TRACE(PortNum, USBPD_ERROR, "GET_BATTERY_STATUS not accepted by the stack");
  return USBPD_ERROR;
}

/**
  * @brief  Request the PE to send a SECURITY_REQUEST
  * @param  PortNum The current port number
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestSecurityRequest(uint8_t PortNum)
{
#if defined(_SECURITY_MSG)
  USBPD_StatusTypeDef _status = USBPD_ERROR;
  uint8_t data[60] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60};
  _status = USBPD_PE_SendExtendedMessage(PortNum, USBPD_SOPTYPE_SOP, USBPD_EXT_SECURITY_REQUEST, data, 60);

  DPM_USER_ERROR_TRACE(PortNum, _status, "SECURITY_REQUEST not accepted by the stack");
  return _status;
#else
  DPM_USER_ERROR_TRACE(PortNum, USBPD_ERROR, "SECURITY_REQUEST not accepted by the stack");
  return USBPD_ERROR;
#endif /* _SECURITY_MSG */
}

/**
  * @brief  Request the PE to send a FIRWMARE_UPDATE_REQUEST or FIRWMARE_UPDATE_RESPONSE
  * @param  PortNum     The current port number
  * @param  MessageType Type of the message (REQUEST or RESPONSE)
  * @param  pPayload    Pointer of the Payload to send to port partner
  * @param  Size        Size of the payload
  * @retval USBPD Status
  */
USBPD_StatusTypeDef USBPD_DPM_RequestFirwmwareUpdate(uint8_t PortNum, USBPD_ExtendedMsg_TypeDef MessageType,
                                                     uint8_t *pPayload, uint16_t Size)
{
  DPM_USER_ERROR_TRACE(PortNum, USBPD_ERROR, "FWUPDATE_REQUEST not accepted by the stack");
  return USBPD_ERROR;
}

/**
  * @} USBPD_USER_EXPORTED_FUNCTIONS_GROUP3
  */

/**
  * @} STM32_USBPD_APPLICATION_DPM_USER
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
