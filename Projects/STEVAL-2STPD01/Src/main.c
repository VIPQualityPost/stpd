/**
  ******************************************************************************
  * @file    main.c
  * @version 1.0.1
  * @date    2021-11-08
  * @author  SRA.SLDU.SC Team
  * @brief   Main file of the STEVAL-2STPD01 reference design firmware
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

/**
  * @mainpage STSW-2STPD01 Main Page
  *
  * <h1><center>STSW-2STPD01 Documentation</center></h1>
  * 
  * <p>The <b>STSW-USBPD27S</b> software package contains the application source code designed to demonstrate the capabilities of the <b>STEVAL-2STPD01</b> evaluation kit</p>
  * <p>The <b>STEVAL-2STPD01X</b> expansion board features two Type-C ports which integrates two <b>STPD01</b> programmable buck converters for USB Power Delivery and a <b>TCPP02-M18</b> USB-C protection for Source applications.</p>
  * <p>The solution exploits the characteristics of the USB PD peripheral embedded in the <b>STM32G071RBT6</b> microcontroller and its firmware stack to implement a Dual Port USB Type-C and Power Delivery Source solution. Thanks to the integrated STPD01 devices, the STEVAL-2STPD01 can supply up to 120 W (60 W per port).</p>
  * <p>The expansion board is designed to be plugged on top of the NUCLEO-G071RB development board and exploits the microcontroller capability of managing two UCPD peripherals at the same time.</p>
  * <p>The TCPP02-M18 provides USB Type-C port protections for both ports while the L7983 synchronous step-down switching regulator supplies the solution.</p>
  * <p>The STEVAL-2STPD01 complies with the USB Type-C and Power Delivery specifications.</p>
  * 
  * @image html ./STEVAL-2STPD01-Low.png "STEVAL-2STPD01 Kit" width=400
  * 
  * <h2> Architecture </h2>
  * The STSW-2STPD01 architecture is organized in different levels:
  * <ol>
  *  <li>The <b>FW Application</b> contains the functional blocks specifically designed to comply the functionality of the adapter</li>
  *  <li>The <b>USB-PD Library</b>, in part provided as a compiled library and containing the main blocks constituting the ST USB-PD Middleware stack</li>
  *  <li>The <b>BSP</b> and <b>STM32CubeG0 HAL Library</b> FW drivers of the main components integrated in the hardware platforms: STPD01 and TCPP02 devices and </li>
  * </ol>
  * 
  * @image html ./STEVAL-2STPD01-Arch-Low.png "STSW-2STPD01 Architecture" width=400
  * 
  * <h3>Supported IDEs:</h3>
  * <ul>
  *  <li><i>STM32CubeIDE v1.6.1</i> (STMicroelectronics)</li>
  *  <li><i>EWARM v8.50.5      </i> (IAR Systems)</li>
  *  <li><i>MDK-ARM v5.34      </i> (Keil)</li>
  * </ul>
  * 
  * <h3>Document tabsheets description:</h3>
  * <ul>
  *  <li>"Modules"        : List the different modules.</li>
  *  <li>"Data Structures": Firmware Directory hierarchy.</li>
  *  <li>"Files"          : List all the files and globals.</li>
  * </ul>
  * 
  * 
  * @image html ./logo_main.jpg "STMicroelectronics" width=150
  *
  */

/** @addtogroup APPLICATION_MAIN
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usbpd.h"

#if (osCMSIS >= 0x20000U)
#include "task.h"
#endif /* osCMSIS >= 0x20000U */

#include "stpd01_reg.h"
#include "steval_2stpd01_bus.h"
#include "steval_2stpd01_stpd01_conf.h"
#include "steval_2stpd01_tcpp02_conf.h"

#if defined(_GUI_INTERFACE) && defined(USE_PS)
#include "usbpd_power_sharing_app.h"
#endif /* _GUI_INTERFACE && USE_PS */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/** @defgroup APPLICATION_MAIN_Private_Variables Private Variables
  * @{
  */
/** @brief Application firmware version
  */
__USED const char APP_FWHeader[] = APP_FWHEADER;
/** 
  * @} APPLICATION_MAIN_Private_Variables
  */

/** @defgroup APPLICATION_MAIN_Private_Functions Private Functions
  * @{
  */
/* Private function prototypes -----------------------------------------------*/
void StartDefaultTask(void const *argument);
void SystemClock_Config(void);
void vApplicationStackOverflowHook(TaskHandle_t xTask,
                                   signed char *pcTaskName);

#ifdef _GUI_INTERFACE
USBPD_StatusTypeDef FreeTextWrapper(uint8_t PortNum, uint8_t* pData, uint16_t Size);
#endif /* _GUI_INTERFACE */


/** 
  * @} APPLICATION_MAIN_Private_Functions
  */

/** @addtogroup APPLICATION_MAIN_Private_Functions
  * @{
  */
/**
  * @brief  The application entry point.
  * @retval error condition
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  /* Get back current CPU clock config */
  SystemCoreClockUpdate();

  if (SystemCoreClock != 64000000)
  {
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock @ CPU max */
    SystemClock_Config();
  }

  /* ## Backup register access ## */
  RCC->APBENR1 |= RCC_APBENR1_PWREN;

  /* Led initilization and blinking */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_On(LED_GREEN);
  HAL_Delay(200);
  BSP_LED_Off(LED_GREEN);
  HAL_Delay(100);
  

  /* USBPD initialisation ---------------------------------*/
#if defined(_GUI_INTERFACE)
  /* Initialize GUI before retrieving PDO from RAM */
  GUI_RegisterCallback_FreeText(FreeTextWrapper);
#endif /* _GUI_INTERFACE */
  
  /* FLR: da rivedere l'ordine delle funzioni successive  MX_USBPD_Init */
//  MX_USBPD_Init();

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
#if (osCMSIS < 0x20000U)
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  osThreadCreate(osThread(defaultTask), NULL);
#endif /* (osCMSIS < 0x20000U) */
  /* add threads, ... */
  
  /* Start scheduler */
  osKernelStart();

  /* if not start the OS entering in Error condition */
  Error_Handler(0);
  
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Hook of the stack overflow
  * @param  xTask task generated the hook
  * @param  pcTaskName name of the task
  */
void vApplicationStackOverflowHook(TaskHandle_t xTask,
                                   signed char *pcTaskName)
{
  while (1);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI)
  *            SYSCLK(Hz)                     = 64000000
  *            HCLK(Hz)                       = 64000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 16000000
  *            PLLM                           = 4
  *            PLLN                           = 64
  *            PLLR                           = 4
  *            Flash Latency(WS)              = 2
  */
void SystemClock_Config(void)
{
  /* LSI configuration and activation */
  /* Reset Value is LSI enabled */
  LL_RCC_LSI_Enable();
  while (LL_RCC_LSI_IsReady() != 1)
  {
  };

  /* HSI configuration and activation */
  /* Reset Value is HSI enabled */
  LL_RCC_HSI_Enable();
  while (LL_RCC_HSI_IsReady() != 1)
  {
  };

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_4, 64, LL_RCC_PLLR_DIV_4);
  LL_RCC_PLL_Enable();
  LL_RCC_PLL_EnableDomain_SYS();
  while (LL_RCC_PLL_IsReady() != 1)
  {
  };

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  /* Sysclk activation on the main PLL */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };

  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  /* Set systick to 1ms in using frequency set to 64MHz */
  /* This frequency can be calculated through LL RCC macro */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ(__LL_RCC_CALC_HSI_FREQ(),
  LL_RCC_PLLM_DIV_4, 64, LL_RCC_PLLR_DIV_4)*/
  LL_Init1msTick(64000000);
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
  HAL_NVIC_SetPriority(SysTick_IRQn, TICK_INT_PRIORITY, 0U);

#if defined(NUCLEO_MB1360A)
  /* Enable and set line 4_15 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
#endif /* NUCLEO_MB1360A */

  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(64000000);
}

#if defined(_GUI_INTERFACE)
/**
  * @brief  Callback of the freetext message of the GUI
  * @param  PortNum task generated the hook
  * @param  pData text received
  * @param  Size size of the text
  */
USBPD_StatusTypeDef FreeTextWrapper(uint8_t PortNum, uint8_t* pData, uint16_t Size)
{
#ifdef USE_PS
  /* Call the parser of the parameters */
  if (PS_STATUS_OK != PS_App_ParametersFromString((char const *)pData, (uint8_t)Size))
  {
    /* In case of error provide a feedback */
    Feedback_Handler(0, 10, 100, 2);
  }
#endif /* USE_PS */

  return USBPD_OK;
}
#endif /* _GUI_INTERFACE */

/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument default parameter of the prototype, not used
  */
void StartDefaultTask(void const *argument)
{

  /* USBPD initialisation ---------------------------------*/
  MX_USBPD_Init();

  /* Send a welcome message to the trace*/
#if defined(_TRACE)
  char *text = APP_FWHEADER_DESCR;
  USBPD_TRACE_Add(USBPD_TRACE_DEBUG, USBPD_PORT_NA, 0, (uint8_t *)text, strlen(text));
#endif
  
  osThreadTerminate(osThreadGetId());
}

/**
  * @brief  Standard callback of the EXTI intterupt
  * @param  GPIO_Pin pin generated the interrupt
  */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
  /* according to the GPIO_Pin call the relative callback */
  switch(GPIO_Pin)
  {
  case BSP_TCPP02_P0_FLG_PIN:
    /* Event generated from TCPP02 for Port 0 */
    BSP_TCPP02_EventCallback(USBPD_PORT_0);
    break;
  case BSP_TCPP02_P1_FLG_PIN:
    /* Event generated from TCPP02 for Port 1 */
    BSP_TCPP02_EventCallback(USBPD_PORT_1);
    break;
  case BSP_STPD01_P0_ALERT_PIN:
    break;
  case BSP_STPD01_P1_ALERT_PIN:
    break;
  case BSP_STPD01_P0_PGOOD_PIN:
    break;
  case BSP_STPD01_P1_PGOOD_PIN:
    break;
  default:
    /* nothing */
    break;
  }
}

/**
  * @brief  Programming service routine
  */

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file pointer to the source file name
  * @param  line assert_param error line source number
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */

/** 
  * @} APPLICATION_MAIN_Private_Functions
  */

/** @addtogroup APPLICATION_MAIN_Exported_Functions
  * @{
  */

/**
  * @brief  Blocking point in case of error.
  * @param  Code a code to identify the handler
  */
void Error_Handler(uint32_t Code)
{
  UNUSED(Code);

  while (1)
  {
    BSP_LED_Toggle(LED_GREEN);
    for(uint32_t i = 0; i < 0xFFFFF; i++) __NOP();
  }
}

/**
  * @brief  This function is used to provide a led feedback to user
  * @param  Code a code to identify the handler
  * @param  Count number of periods
  * @param  Delay delay time (half period)
  * @param  Mode delay mode (def=for-cycle, 1=HAL_Delay, 2=osDelay)
  */
void Feedback_Handler(uint32_t Code, uint32_t Count, uint32_t Delay, uint8_t Mode)
{
  UNUSED(Code);

  uint32_t count = Count<<2;
  while (count--)
  {
    BSP_LED_Toggle(LED_GREEN);
    switch(Mode)
    {
    case 1:
      HAL_Delay(Delay);
      break;
    case 2:
      osDelay(Delay);
      break;
    default:
      for(uint32_t i = 0; i < 0xFFFFF; i++) __NOP();  
      break;
    }
  }
  BSP_LED_Off(LED_GREEN);
}
/** 
  * @} APPLICATION_MAIN_Exported_Functions
  */

/**
  * @} APPLICATION_MAIN
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
