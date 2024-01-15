/**
  ******************************************************************************
  * @file    usbpd_power_monitor.c
  * @version 1.0.0
  * @date    June, 30th 2021
  * @author  S&C USBPD Team
  * @brief   This file contains power monitor functions for STEVAL-2STPD01.
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

/** @addtogroup USBPD_POWER_MONITOR
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "usbpd_power_monitor.h"
#include "usbpd_power_monitor_app.h"

#include "stdlib.h"
#include "string.h"
#include "cmsis_os.h"
#include "usbpd_trace.h"

#include "usbpd_core.h"
#include "usbpd_def.h"
#include "usbpd_dpm_core.h"
#include "usbpd_dpm_conf.h"
#include "usbpd_dpm_user.h"

/* Private typedef -----------------------------------------------------------*/

#ifndef __NO_DOXY
#if !defined(PM_PORT_COUNT)
#error "Port number not specified"
#endif  /* PM_PORT_COUNT */

#if PM_PORT_COUNT>2
#error "Invalid value port number (supported only 1 or 2 ports)"
#endif  /* PM_PORT_COUNT */

/* info about of the module version */
#define PM_FWHEADER_NAME  "PM Module for STEVAL-2STPD01" 
#define PM_FWHEADER_VER   "1.0.0" 
#define PM_FWHEADER_DATE  "June, 30th 2021" 

#define PM_FWHEADER_DESCR "USBPD " PM_FWHEADER_NAME " v" PM_FWHEADER_VER

/* IDE selection */
#if defined(__GNUC__) /* STM32CubeIDE */
#define PM_FWHEADER_IDE "STM32CubeIDE (STMicroelectronics)"
#elif defined (__ICCARM__) /* IAR EWARM */
#define PM_FWHEADER_IDE "EWARM (IAR Systems)"
/* to avoid wrong definition for inclusion order */
#undef __USED
#define __USED __root
#elif defined (__CC_ARM) /* Keil uVision */
#define PM_FWHEADER_IDE "uVision (ARM Keil)"
#else   /* Unknown */
#define PM_FWHEADER_IDE "Unknown"
#endif  /* IDE selection */

#define PM_FWHEADER \
      "\r\n[Header]" \
      "\r\nName:"  PM_FWHEADER_NAME \
      "\r\nVer:"   PM_FWHEADER_VER \
      "\r\nDate:"  PM_FWHEADER_DATE\
      "\r\nDescr:" PM_FWHEADER_DESCR \
      "\r\nIDE:"   PM_FWHEADER_IDE \
      "\r\n[/Header]\r\n"

__USED const char PM_FWHeader[] = PM_FWHEADER;
#endif /* __NO_DOXY */

/** @defgroup USBPD_POWER_MONITOR_Private_TypeDef Private Typedef
  * @{
  */
/** @brief Structure of a message sent to the monitor task
  * @note The message is sent internally with the defined API
  */
typedef struct 
{
  PM_Event_Typedef Event:8;     /*!< Type of event (Notification, Fault, ...) */ 
  uint8_t PortNum: 2;           /*!< Port number */ 
  uint32_t Parameter:22;        /*!< Parameters */ 
} PM_Msg_Typedef;

/** @brief Power monitor handle
  * @note Only 1 entity for the whole system
  * @note The suspending flag is used to perform a safe suspend/wake-up of the control task
  */
typedef struct 
{
  osMessageQId Monitor_QueueId;                 /*!< Monitor queue id for event message */ 
  osThreadId Monitor_ThreadId;                  /*!< Monitor thread id, monitor task to manage synchronous and asynchronous (events) operations */ 
  uint32_t Monitor_Delay;                       /*!< Monitor delay (ms), sleep time without events */ 
  uint8_t Control_Suspending;                   /*!< Control task suspending flag */ 
  osThreadId Control_ThreadId;                  /*!< Control thread id, control task to manage periodic data */ 
  uint32_t Control_Delay;                       /*!< Control delay (ms), hard deadline sleep (default 2ms) */ 
  PM_Data_Typedef Data[PM_PORT_COUNT];       /*!< Last data read of a specific port */ 
  osEvent Event;                                /*!< Last monitor event received */ 
  PM_Msg_Typedef LastMsg;                       /*!< Last message received with the event (data are copied to avoid inconsistency) */ 
  PM_Callback_Typedef Callbacks;                /*!< Callback to read and to notify */ 
} PM_Handle_Typedef;                             
/** 
 * @} USBPD_POWER_MONITOR_Private_TypeDef
 */

/* Private define ------------------------------------------------------------*/
/** @defgroup USBPD_POWER_MONITOR_Private_Defines Private Defines
  * @{
  */
/* Signal definition */
/* Queue parameters */
#define PM_QUEUE_LEN              (10u)                  /*!< Length of the queue for events */ 

/* Private macro -------------------------------------------------------------*/
#define PM_EVENT_TYPE(_Event_)                  ((PM_EventType_Typedef)(((_Event_)>>5) & 0x07))  /*!< Get the event category */
#define PM_EVENT_TYPE_IS(_Event_, _EventType_)  (PM_EVENT_TYPE(_Event_) == (_EventType_))        /*!< Check the event type */
    
#if !defined(PM_LED_Toggle)
#define PM_LED_Toggle()     ((void)0U)
#endif /* PM_LED_Toggle */

#if !defined(PM_LED_On)
#define PM_LED_On()         ((void)0U)
#endif /* PM_LED_On */

#if !defined(PM_LED_Off)
#define PM_LED_Off()        ((void)0U)
#endif /* PM_LED_Off */
    
/** 
 * @} USBPD_POWER_MONITOR_Private_Defines
 */

/* Private functions ---------------------------------------------------------*/
/** @defgroup USBPD_POWER_MONITOR_Private_Functions Private Functions
  * @brief Current, vbus and/or power monitor modules exported functions
  * @{
  */
void PM_Control_Task(void const *argument);
void PM_Monitor_Task(void const *argument);
/** 
 * @} USBPD_POWER_MONITOR_Private_Functions
 */

/* Private variables ---------------------------------------------------------*/
/** @defgroup USBPD_POWER_MONITOR_Private_Variables Private Variables
  * @brief Local variable
  * @{
  */
#if defined(_TRACE)
/** @brief Event text list
  */
const char *PM_Event_Name[] = 
{
  "PM_EV_NONE                     ", 
  "PM_EV_SERVICE_STANDBY          ",
  "PM_EV_NOTIF_PD_CONN_STATUS     ",
  "PM_EV_NOTIF_PD_CONN_EXPLICIT   ",
  "PM_EV_NOTIF_PD_CONN_ROLE       ",
  "PM_EV_NOTIF_PD_REQ_RX          ",
  "PM_EV_NOTIF_PD_FIXED_ACCEPTED  ",
  "PM_EV_NOTIF_PD_APDO_ACCEPTED   ",
  "PM_EV_NOTIF_PD_VOLTAGE_CHANGE  ",
  "PM_EV_NOTIF_PD_CURRENT_CHANGE  ",
  "PM_EV_NOTIF_PD_PSREADY         ",
  "PM_EV_NOTIF_PD_HARDRESET       ",
  "PM_EV_FAULT_BUS_OVER_VOLTAGE   ",
  "PM_EV_FAULT_BUS_UNDER_VOLTAGE  ",
  "PM_EV_FAULT_BUS_OVER_CURRENT   ",
  "PM_EV_FAULT_VCONN_OVER_VOLTAGE ",
  "PM_EV_FAULT_VCONN_UNDER_VOLTAGE",
  "PM_EV_FAULT_VCONN_OVER_CURRENT ",
  "PM_EV_FAULT_THERMAL            ",
  "PM_EV_FAULT_CC_LINES           ",
  "PM_EV_FAULT_STATUS             ",
};
#endif /* _TRACE */

/** @defgroup USBPD_POWER_MONITOR_Private_Variables_OS_DEF OS Definition
  * @brief All private variables
  * @{
  */

/** @brief Monitor Queue Definition
  */
osMessageQDef(PM_QUEUE, PM_QUEUE_LEN, PM_Msg_Typedef);

/** @brief Monitor Thread Definition
  */
osThreadDef(PM_MONITOR, PM_Monitor_Task, osPriorityAboveNormal, 0, 100);

/** @brief Control Thread Definition
  */
osThreadDef(PM_CONTROL, PM_Control_Task, osPriorityRealtime, 0, 100);
/** 
 * @} USBPD_POWER_MONITOR_Private_Variables
 */

/** @brief Power Monitor Handle 
  */
static PM_Handle_Typedef PM_Handle = {
  .Monitor_QueueId = NULL,
  .Monitor_ThreadId = NULL,
  .Monitor_Delay = PM_TASK_TIME_MS,
  .Control_ThreadId = NULL,
  .Control_Delay = PM_CTRL_TIME_MS,
  .Control_Suspending = 0,
  .Event = {
    .status = osOK, 
    .value.v = 0},
  .LastMsg = {
    .Event = PM_EV_NONE,
    .PortNum = 0, 
    .Parameter = 0 },
  .Data = {
    {.PortNum = PM_PORT_0, .VBus = 0, .IBus = 0, .Transition = 0},
#if PM_PORT_COUNT>=2
    {.PortNum = PM_PORT_1, .VBus = 0, .IBus = 0, .Transition = 0},
#endif /* PM_PORT_COUNT */
  },

  /* static link to internal handler */
  .Callbacks = {
    .PM_ReadData                = NULL,
    .PM_NotifyData              = NULL,
    .PM_CheckStatus             = NULL,
    .PM_FaultCondition          = NULL,
    .PM_CriticalCondition       = NULL,
  }
};
/** 
 * @} USBPD_POWER_MONITOR_Private_Variables
 */

/** @addtogroup USBPD_POWER_MONITOR_Exported_Functions
  * @{
  */
/** @brief Power Monitor module initialization function
  * @param pCallbacks callback array
  * @retval status condition error
  */
PM_Status_TypeDef PM_Monitor_Init(PM_Callback_Typedef * pCallbacks)
{
  /* Callbacks Init */
  PM_RegisterCallbacks(pCallbacks);
                       
  /* Init PWR queue */
  PM_Handle.Monitor_QueueId = osMessageCreate(osMessageQ(PM_QUEUE), NULL);
  if(NULL == PM_Handle.Monitor_QueueId)
  {
     return PM_STATUS_ERR;
  }

  /* Create the Monitor task */
  PM_Handle.Monitor_ThreadId = osThreadCreate(osThread(PM_MONITOR), NULL);
  if(NULL == PM_Handle.Monitor_ThreadId)
  {
     return PM_STATUS_ERR;
  }

  /* Create the Control task */
  PM_Handle.Control_ThreadId = osThreadCreate(osThread(PM_CONTROL), NULL);
  if(NULL == PM_Handle.Control_ThreadId)
  {
     return PM_STATUS_ERR;
  }
  
  return PM_STATUS_OK;
}

/** @brief Power Monitor module callback registration function
  * @param pCallbacks callback array
  * @retval status condition error
  */
PM_Status_TypeDef PM_RegisterCallbacks(PM_Callback_Typedef * pCallbacks)
{
  assert_param(pCallbacks);
  if (!pCallbacks)
  {
    return PM_STATUS_ERR;
  }
  
  /* copy callback functions */
  memcpy(&PM_Handle.Callbacks, pCallbacks, sizeof(PM_Callback_Typedef));

  return PM_STATUS_OK;
}

/** @brief Function to notify an event: fault or notification
  * @param PortNum port number
  * @param Event type of event
  * @param Parameter parameter
  * @param Timeout_ms timeout to perform the operation
  * @retval status of the operation
  */
PM_Status_TypeDef PM_Notify(uint8_t PortNum, PM_Event_Typedef Event, uint32_t Parameter, uint32_t Timeout_ms)
{
  if (PM_Handle.Monitor_QueueId == NULL)
  {
    return PM_STATUS_ERR;
  }
  
  /* creating the message */
  PM_Msg_Typedef PM_Msg = {.PortNum = PortNum, .Event = Event, .Parameter = Parameter};
  
  /* insert into the queue */
  osStatus status = osMessagePut(PM_Handle.Monitor_QueueId, *((uint32_t *)&PM_Msg), Timeout_ms);
  if (osOK != status)
  {
    return PM_STATUS_ERR;
  }

  return PM_STATUS_OK;
}

/** @brief Require a stand-by mode, in case of stand-by, it is reached in safe mode, waiting for cycle completation
  * @param PortNum port number
  * @param Mode stand-by mode
  * @retval Operation Status
  */
PM_Status_TypeDef PM_StandBy(uint8_t PortNum, uint8_t Mode)
{
  if (Mode == PM_STANDBY_MODE_WAKEUP)
  {
    /* wake-up */
    
    /* 
      1. re-create the queue (if need)
      2. re-create the thread (if need)
      3. resume the thread (if need)
    */
    
    /* recreate the queue */
    if (PM_Handle.Monitor_QueueId == NULL)
    {
      PM_Handle.Monitor_QueueId = osMessageCreate(osMessageQ(PM_QUEUE), NULL);
      if (PM_Handle.Monitor_QueueId == NULL)
      {
        return PM_STATUS_ERR;
      }
    }
    
    /* not exist => create it */
    if (PM_Handle.Monitor_ThreadId == NULL)
    {
      PM_Handle.Monitor_ThreadId = osThreadCreate(osThread(PM_MONITOR), NULL);
      if (PM_Handle.Monitor_ThreadId == NULL)
      {
        return PM_STATUS_ERR;
      }
    }
    
    /* suspended => resume it */
    if (osThreadIsSuspended(PM_Handle.Monitor_ThreadId) == osOK)
    {
      if (osOK != osThreadResume(PM_Handle.Monitor_ThreadId))
      {
        return PM_STATUS_ERR;
      }
    }
  }
  
  /* send a notification to wake-up the thread */
  PM_Notify(PortNum, PM_EV_SERVICE_STANDBY, Mode, 5);
  
  return PM_STATUS_OK;
}

/** @brief Back from a stand-by condition and perform a wake-up of the Monitor Task
  * @param PortNum port number
  * @retval Operation Status
  */
PM_Status_TypeDef PM_Wakeup(uint8_t PortNum)
{
  return PM_StandBy(PortNum, PM_STANDBY_MODE_WAKEUP);
}

/** @brief Require an enable/disable to the control (to guarantee the task consistence)
  * @param Enable enabled / disable (as condition)
  * @param Delay set the new delay value, a negative value is ignored (no changes)
  * @note Currently the default delay value is <b>2ms</b>, according to the request of the PPS algorithm
  */
void PM_Control_Enable(uint8_t Enable, int32_t Delay)
{
  if (PM_Handle.Control_ThreadId == NULL || Enable >= 2)
  {
    /* an error case, exit from the function, no error reported */
    return;
  }

  /* set the delay, -1 don't change */
  if (Delay >= 0)
  {
    PM_Handle.Control_Delay = Delay;
  }

  if (Enable)
  {
    /* wake-up the control task */
    osThreadResume(PM_Handle.Control_ThreadId);
  }
  else
  {
    /* update the flag only if it is in the correct state (0) */
    if (PM_Handle.Control_Suspending == 0)
    {
      /* require a suspending action to the the control task */
      PM_Handle.Control_Suspending = 1;
    }
  }
}

/** 
  * @} USBPD_POWER_MONITOR_Exported_Functions
  */

/** @addtogroup USBPD_POWER_MONITOR_Private_Functions
  * @{
  */
/** @brief Control task function to perform fast periodic operations, in particular
  *        the task calls read and notify callbacks in real-time, respecting the current delay
  * @param argument not used, defined in the prototype
  */
void PM_Control_Task(void const *argument)
{
 UNUSED(argument);
 
#if INCLUDE_vTaskDelayUntil
 uint32_t PreviousWakeTime = 0;
#endif /* INCLUDE_vTaskDelayUntil */
 
 for(;;)
 {

   /* normal running, no suspending is required */
   if (PM_Handle.Control_Suspending == 0)
   {
     /* Schedule with fixed and respected timing */
#if INCLUDE_vTaskDelayUntil
     osDelayUntil(&PreviousWakeTime, PM_Handle.Control_Delay);
#else
     osDelay(PM_Handle.Control_Delay);
#endif /* INCLUDE_vTaskDelayUntil */
     
     /* check the validity of the function pointers */
     assert_param(PM_Handle.Callbacks.PM_ReadData != NULL);
     assert_param(PM_Handle.Callbacks.PM_NotifyData != NULL);
     
#if PM_PORT_COUNT>=1
     /* call the read data function handler */
     PM_Handle.Callbacks.PM_ReadData(&PM_Handle.Data[PM_PORT_0]);
     
     /* call the notification data function handler */
     PM_Handle.Callbacks.PM_NotifyData(&PM_Handle.Data[PM_PORT_0]);
#endif /* PM_PORT_COUNT==1 */

#if PM_PORT_COUNT>=2
     /* call the read data function handler */
     PM_Handle.Callbacks.PM_ReadData(&PM_Handle.Data[PM_PORT_1]);
       
     /* call the notification data function handler */
     PM_Handle.Callbacks.PM_NotifyData(&PM_Handle.Data[PM_PORT_1]);
#endif /* PM_PORT_COUNT==2 */

   }
   else
   {
     /* set the suspending flag to the 2 state */
     PM_Handle.Control_Suspending = 2;
     
     /* waiting for a resume */
     osThreadSuspend(PM_Handle.Control_ThreadId);
     
     /* reset variables to start the control */
     PM_Handle.Control_Suspending = 0;
#if INCLUDE_vTaskDelayUntil
     PreviousWakeTime = 0;
#endif /* INCLUDE_vTaskDelayUntil */

   }
 }
}

/** @brief Monitor task function, this task wait for a event o a timeout.
  *        In case of timeout perform cycle tasks (PM_CheckStatus)
  *        The events are:
  *        <ul>
  *        <li>service (i.e. wake-up / stand-by)</li>
  *        <li>notification (i.e. attach / detach)</li>
  *        <li>fault (i.e. over current PM_FaultCondition and PM_CriticalCondition)</li>
  *        </ul>
  * @param argument not used, defined in the task prototype
  */
void PM_Monitor_Task(void const *argument)
{
  UNUSED(argument);
  
  /* return of the called function */
  PM_Status_TypeDef ret;
  uint8_t skip_periodic_task = 0;
  
#if defined(_TRACE)
  /* used to prepare the string variable to send to the trace */
  static char text[100] = {0};
  static uint8_t event_id = 0;
#endif
  
  for(;;)
  {
    /* led feedback */
    PM_LED_Toggle();
    if (PM_Handle.Monitor_Delay == 0) 
      PM_LED_Off();

    /* wait for a message (to notify an power monitor event) or the timeout */
    PM_Handle.Event = osMessageGet(PM_Handle.Monitor_QueueId, PM_Handle.Monitor_Delay > 0 ? PM_Handle.Monitor_Delay : osWaitForever);

    
    /* if not a valid event reporting the error and continue */
    if (PM_Handle.Event.status != osOK && PM_Handle.Event.status != osEventTimeout && PM_Handle.Event.status != osEventMessage)
    {
      if (PM_Handle.Callbacks.PM_CriticalCondition)
      {
        PM_Handle.Callbacks.PM_CriticalCondition(PM_PORT_NA, 200); 
      }
      
      continue;
    }
    
    skip_periodic_task = 0;
    
    /* the event is a message */
    if (PM_Handle.Event.status == osEventMessage)
    {
      /* copy the received message in the handle 
      (to avoid overwrite during the management)
      
      Note: the generic unsigned 32-bit data value is cast to a PM message 
      */
      PM_Handle.LastMsg = *((PM_Msg_Typedef *)&PM_Handle.Event.value);
      
#if defined(_TRACE)
      /* send a event debug text to the trace */
      event_id = (uint8_t)PM_Handle.LastMsg.Event & 0x1F;
      
      /* delete the text memory area */
      memset(text, 0, 100);
#if !defined(__GNUC__)
      sprintf(text, "PM: Event=%s (0x%04X)", PM_Event_Name[event_id], event_id);
#else
      {
        /* create manually the string to avoid the printf */
        uint8_t pos,tmp;
        strcpy(text, "PM: Event=");
        strcat(text, PM_Event_Name[event_id]);
        strcat(text, " (0x00");
        pos = strlen(text);
        tmp = (event_id>>4) & 0xF;
        text[pos++] = tmp < 0xA ? tmp + '0' : tmp - 0xA + 'A';
        tmp = event_id & 0xF;
        text[pos++] = tmp < 0xA ? tmp + '0' : tmp - 0xA + 'A';
        strcat(text, ")");
      }
      //text[strlen(text)] = (event_id>>4 & 0x0f) + '0';
      //text[strlen(text)] = (event_id & 0x0f) + '0';
#endif
      USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PM_Handle.LastMsg.PortNum, 0, (uint8_t *)text, strlen(text));
#endif
      /* get the event type */
      PM_EventType_Typedef event_type = PM_EVENT_TYPE(PM_Handle.LastMsg.Event);
      switch(event_type)
      {
      case PM_EV_TYPE_NONE:
        /* nothing */
        break;
      case PM_EV_TYPE_SERVICE:
        if (PM_Handle.LastMsg.Event == PM_EV_SERVICE_STANDBY)
        {
          PM_StandBy_Mode_Typedef standby_mode = (PM_StandBy_Mode_Typedef)PM_Handle.LastMsg.Parameter;
          
          switch(standby_mode)
          {
          case PM_STANDBY_MODE_WAKEUP:
            /* Wake-up!!! we're ready, set the default and continue */
            PM_Handle.Monitor_Delay = PM_TASK_TIME_MS;
            
            /* Operative frequency for the control task */
            PM_Control_Enable(1, PM_CTRL_TIME_MS);
            /* turn off the led */
            PM_LED_On();
            
            break;
            
          case PM_STANDBY_MODE_LOW_RATE:
            /* turn off the led */
            PM_LED_Off();
            
            /* Low frequency for the control task */
            PM_Control_Enable(1, PM_CTRL_TIME_MS);
            
            /* No TO */
            PM_Handle.Monitor_Delay = PM_TASK_TIME_LOWRATE_MS;
            break;
            
          case PM_STANDBY_MODE_WAIT_FOR_MSG:
            /* turn off the led */
            PM_LED_Off();
            
            /* Low frequency for the control task */
            PM_Control_Enable(1, PM_CTRL_TIME_LP_MS);
            
            /* No TO */
            PM_Handle.Monitor_Delay = 0;
            break;
            
          case PM_STANDBY_MODE_SUSPEND:
            /* turn off the led */
            PM_LED_Off();
            
            /* Low frequency for the control task */
            PM_Control_Enable(1, PM_CTRL_TIME_LP_MS);
            
            /* Suspend the task */
            osThreadSuspend(NULL);
            break;
            
          case PM_STANDBY_MODE_KILL:
            /* turn off the led */
            PM_LED_Off();
            /* Low frequency for the control task */
            PM_Control_Enable(1, PM_CTRL_TIME_LP_MS);
            
            /* Destroy the queue */
            osMessageDelete(PM_Handle.Monitor_QueueId);
            PM_Handle.Monitor_QueueId = NULL;
            
            /* Destroy this task */
            PM_Handle.Monitor_ThreadId = NULL;
            osThreadTerminate(NULL);
            break;
          }
        }
        skip_periodic_task = 1;
        break;
      case PM_EV_TYPE_NOTIF:
        {
          /* if a notification we perform an handle data update */
          switch(PM_Handle.LastMsg.Event)
          {
          case PM_EV_NOTIF_PD_CONN_STATUS     :
          case PM_EV_NOTIF_PD_CONN_EXPLICIT   :
            PM_Control_Enable(1, PM_CTRL_TIME_MS); //PM_CTRL_TIME_LP_MS
            PM_Handle.Data[PM_Handle.LastMsg.PortNum].Transition = 0;
          case PM_EV_NOTIF_PD_CONN_ROLE       :
            /* update handle */
            break;
          case PM_EV_NOTIF_PD_FIXED_ACCEPTED  :
          case PM_EV_NOTIF_PD_APDO_ACCEPTED   :
            /* change over and under voltage parameters */
            break;
          case PM_EV_NOTIF_PD_VOLTAGE_CHANGE  :
            /* Inform a transition is needed */
            PM_Handle.Data[PM_Handle.LastMsg.PortNum].Transition = 1;
            /* User Application has to clear it*/
            break;
          case PM_EV_NOTIF_PD_CURRENT_CHANGE  :
            break;
          case PM_EV_NOTIF_PD_PSREADY         :
            break;
          case PM_EV_NOTIF_PD_REQ_RX:
            /* Setup default timing for CTRL Task */
            PM_Control_Enable(1, PM_CTRL_TIME_MS);
            break;
          case PM_EV_NOTIF_PD_HARDRESET       :
            /* reset the parameter */
            break;
          }
        }
        break;
      case PM_EV_TYPE_FAULT:
        {
          PM_Status_TypeDef status = PM_STATUS_ERR;
          /* in case of fault condition detected, it calls the corresponding function */
          if (PM_Handle.Callbacks.PM_FaultCondition)
          {
            status = PM_Handle.Callbacks.PM_FaultCondition(PM_Handle.LastMsg.PortNum, *(uint32_t *)&PM_Handle.LastMsg);
          }
          
          /* in case of an error, it calls the corresponding function */
          if (status == PM_STATUS_ERR && PM_Handle.Callbacks.PM_CriticalCondition)
          {
            PM_Handle.Callbacks.PM_CriticalCondition(PM_Handle.LastMsg.PortNum, *(uint32_t *)&PM_Handle.LastMsg);
          }
          skip_periodic_task = 1;
        }
        break;
      default:
        /* nothing : unknown event_type */
        skip_periodic_task = 1;
        break;
      }
    }
    
    if (skip_periodic_task)
    {
      continue;
    }
    
    /* Perform periodic cycle tasks */
    
    /* Check status Port 0 */
#if PM_PORT_COUNT>=1
    /* check the status */
    ret = PM_Handle.Callbacks.PM_CheckStatus(&PM_Handle.Data[PM_PORT_0]);
    if (ret != PM_STATUS_OK)
    {
      /* generating a fail */
      PM_Notify(PM_PORT_0, PM_EV_FAULT_STATUS, 0, 0);
    }
#endif /* PM_PORT_COUNT>=1 */

#if PM_PORT_COUNT>=2
    /* check the status */
    ret = PM_Handle.Callbacks.PM_CheckStatus(&PM_Handle.Data[PM_PORT_1]);
    if (ret != PM_STATUS_OK)
    {
      /* generating a fail */
      PM_Notify(PM_PORT_1, PM_EV_FAULT_STATUS, 0, 0);
    }
#endif /* PM_PORT_COUNT>=2 */
  }
}
/** 
  * @} USBPD_POWER_MONITOR_Private_Functions
  */

/** 
  * @} USBPD_POWER_MONITOR
  */
