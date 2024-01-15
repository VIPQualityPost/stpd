/**
  ******************************************************************************
  * @file    steval_2stpd01.c
  * @author  SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   STEVAL-2STPD01 board support package.
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

/** @addtogroup BSP_STEVAL_2STPD01_COMMON
  * @{
  */
/* Includes ------------------------------------------------------------------*/
#include "steval_2stpd01.h"

/** @defgroup BSP_STEVAL_2STPD01_COMMON_Private_Defines Private Defines
  * @{
  */

/**
  * @brief  Driver version definition
  */
/* Version information */
#define STEVAL_2STPD01_BSP_VERSION_X (int32_t) (0x01) /*!< [31:24] main version */
#define STEVAL_2STPD01_BSP_VERSION_Y (int32_t) (0x00) /*!< [23:16] sub1 version */
#define STEVAL_2STPD01_BSP_VERSION_Z (int32_t) (0x00) /*!< [15:8] sub2 version */
#define STEVAL_2STPD01_BSP_VERSION_RC (int32_t) (0x00) /*!< [7:0] release candidate */
#define STEVAL_2STPD01_BSP_VERSION ((STEVAL_2STPD01_BSP_VERSION_X << 24)\
|(STEVAL_2STPD01_BSP_VERSION_Y << 16)\
|(STEVAL_2STPD01_BSP_VERSION_Z << 8)\
|(STEVAL_2STPD01_BSP_VERSION_RC))
/** 
  * @} BSP_STEVAL_2STPD01_COMMON_Private_Defines 
  */

/** @defgroup BSP_STEVAL_2STPD01_COMMON_Private_TypeDef Private Typedef and Struct
  * @{
  */
/**
  * @brief  Handle structure definition
  */
typedef struct {
  uint32_t ADC_Value[BSP_ADC_CH_NUM];
  ADC_HandleTypeDef hADC;
  DMA_HandleTypeDef hDMA;
} BSP_Handle_t;
/** 
  * @} BSP_STEVAL_2STPD01_COMMON_Private_TypeDef
  */

/** @defgroup BSP_STEVAL_2STPD01_COMMON_Private_Constants_Variables Private Constants and variables
  * @{
  */
/**
  * @brief  Global initialization of ADC Resources 
  */
const BSP_ADC_PortPin_Conf_t BSP_ADC_GPIO_Conf[BSP_ADC_CH_NUM] = {
  BSP_ADC_GPIO_Def(VIN),
  BSP_ADC_GPIO_Def(P0_VBUS),
  BSP_ADC_GPIO_Def(P0_IBUS_TCPP02),
  BSP_ADC_GPIO_Def(P0_IBUS_STPD01),
  BSP_ADC_GPIO_Def(P1_VBUS),
  BSP_ADC_GPIO_Def(P1_IBUS_TCPP02),
  BSP_ADC_GPIO_Def(P1_IBUS_STPD01),
};

/**
  * @brief  ADC conversion parameter (refer to type of variable for the unit of measure
  *         The value are calculated as the following formulas:
  *         <ul>
  *          <li>Vin            (mV): (R201 + R204) * 3300 / (R204 * 4095)   </li>
  *          <li>P0 Vbus        (mV): (R503 + R505) * 3300 / (R505 * 4095)   </li>
  *          <li>P0 Ibus TCPP02 (mA): 3300 / ((R500 + 2m) * 40 * 1.5 * 4095) </li>
  *          <li>P0 Ibus STPD01 (mA): 3300 / (2 * k * R400 * 4095)           </li>
  *          <li>P1 Vbus        (mV): (R703 + R705) * 3300 / (R705 * 4095)   </li>
  *          <li>P1 Ibus TCPP02 (mA): 3300/ ((R700 + 2m) * 40 * 1.5 * 4095)  </li>
  *          <li>P1 Ibus STPD01 (mA): 3300/ (2 * k * R600 * 4095)            </li>
  *         </ul>
  */
const uint32_t BSP_ADC_Conversion[BSP_ADC_CH_NUM] = {
  BSP_ADC_CONV_VIN,
  BSP_ADC_CONV_P0_VBUS,
  BSP_ADC_CONV_P0_IBUS_TCPP02,
  BSP_ADC_CONV_P0_IBUS_STPD01,
  BSP_ADC_CONV_P1_VBUS,
  BSP_ADC_CONV_P1_IBUS_TCPP02,
  BSP_ADC_CONV_P1_IBUS_STPD01,
};


/**
  * @brief  Local handle variable
  */
BSP_Handle_t BSP_Handle = {
  .ADC_Value = {0},
  .hADC = {0},
  .hDMA = {0},
};
/** 
  * @} BSP_STEVAL_2STPD01_COMMON_Private_Constants_Variables
  */

/** @defgroup BSP_STEVAL_2STPD01_COMMON_Private_Functions Private Function
  * @{
  */
static void BSPX_ADC_GPIO_Init(void);
static void BSPX_ADC_Init(ADC_HandleTypeDef * hadc);
static void BSPX_ADC_Start(ADC_HandleTypeDef * hadc, uint32_t * pADC_Value);
static void BSPX_ADC_DMA_Init(ADC_HandleTypeDef * hadc, DMA_HandleTypeDef * hdma, DMA_Channel_TypeDef * Instance, uint32_t Request);
/**
  * @} BSP_STEVAL_2STPD01_COMMON_Private_Functions
  */


/** @addtogroup BSP_STEVAL_2STPD01_COMMON_Exported_Functions Exported Functions
  * @{
  */
/**
  * @brief  To get the BSPX Driver version
  * @retval version: 0xXYZR (8bits for each decimal, R for RC)
  */
int32_t BSPX_GetVersion(void)
{
  return (int32_t)STEVAL_2STPD01_BSP_VERSION;
}

/**
  * @brief  Global BSPX module initialization
  * @retval BSP errorcode
  */
int32_t BSPX_Init(void)
{
  /* GPIO Ports Clock Enable */
  BSP_GPIOS_CLK_ENABLE();
  
  BSP_ADC_CLK_ENABLE();

  BSPX_ADC_GPIO_Init();
  
  BSPX_ADC_Init(&BSP_Handle.hADC);
  BSPX_ADC_Start(&BSP_Handle.hADC, BSP_Handle.ADC_Value);
  
  return BSP_ERROR_NONE;
}


/**
  * @brief  Return a raw ADC value
  * @param Type: adc variable type  @ref BSP_ADC_Type_t
  *         This parameter can be take one of the following values:
  *         @arg @ref BSP_ADC_VIN         
  *         @arg @ref BSP_ADC_VBUS0       
  *         @arg @ref BSP_ADC_IBUS0_TCPP02
  *         @arg @ref BSP_ADC_IBUS0_STPD01
  *         @arg @ref BSP_ADC_VBUS1       
  *         @arg @ref BSP_ADC_IBUS1_TCPP02
  *         @arg @ref BSP_ADC_IBUS1_STPD01
  * @retval BSP errorcode
  */
uint32_t BSPX_ADC_GetValue(BSP_ADC_Type_t Type)
{
  return BSP_Handle.ADC_Value[(uint8_t)Type];
}

/**
  * @brief  Return a converted ADC value
  * @param Type: adc variable type  @ref BSP_ADC_Type_t
  *         This parameter can be take one of the following values:
  *         @arg @ref BSP_ADC_VIN         
  *         @arg @ref BSP_ADC_VBUS0       
  *         @arg @ref BSP_ADC_IBUS0_TCPP02
  *         @arg @ref BSP_ADC_IBUS0_STPD01
  *         @arg @ref BSP_ADC_VBUS1       
  *         @arg @ref BSP_ADC_IBUS1_TCPP02
  *         @arg @ref BSP_ADC_IBUS1_STPD01
  * @retval BSP errorcode
  */
uint32_t BSPX_ADC_GetConvertedValue(BSP_ADC_Type_t Type)
{
  uint32_t adc_value = (uint32_t)BSP_Handle.ADC_Value[Type];
  uint32_t adc_conv = (uint32_t)BSP_ADC_Conversion[Type];
  
  return (adc_value * adc_conv) / 1000;
}

#ifndef __NO_DOXY
/**
* @brief HAL ADC MSP Initialization according to the weak function defined inside the HAL driver
* This function configures the hardware resources used in this example
* @param hadc: ADC handle pointer
*/
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  /* check the instancce in case it is not correct return */
  if(hadc->Instance != BSP_Handle.hADC.Instance) 
  {
    return;
  }

  /* call the DMA inititialization */
  BSPX_ADC_DMA_Init(hadc, &BSP_Handle.hDMA, BSP_ADC_DMA_INSTANCE, BSP_ADC_DMA_REQUEST);
}
#endif /* __NO_DOXY */

/**
  * @} BSP_STEVAL_2STPD01_COMMON_Exported_Functions
  */

/** @addtogroup BSP_STEVAL_2STPD01_COMMON_Private_Functions
  * @{
  */
/**
  * @brief ADC Initialization Function
  * @param hadc: ADC handle pointer
  */
static void BSPX_ADC_Init(ADC_HandleTypeDef * hadc)
{
  assert_param(hadc != NULL);
  
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc->Instance = BSP_ADC_INSTANCE;
  hadc->Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc->Init.Resolution = ADC_RESOLUTION_12B;
  hadc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc->Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc->Init.LowPowerAutoWait = DISABLE;
  hadc->Init.LowPowerAutoPowerOff = DISABLE;
  hadc->Init.ContinuousConvMode = ENABLE;
  hadc->Init.NbrOfConversion = 7;
  hadc->Init.DiscontinuousConvMode = DISABLE;
  hadc->Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc->Init.DMAContinuousRequests = ENABLE;
  hadc->Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc->Init.SamplingTimeCommon1 = ADC_SAMPLETIME_160CYCLES_5;
  hadc->Init.SamplingTimeCommon2 = ADC_SAMPLETIME_160CYCLES_5;
  hadc->Init.OversamplingMode = DISABLE;
  hadc->Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  HAL_ADC_Init(hadc);

  /** Configure Regular Channel
  */
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;

  for(uint8_t idx = 0; idx < BSP_ADC_CH_NUM; idx++)
  {
    sConfig.Channel = BSP_ADC_GPIO_Conf[idx].Ch;
    sConfig.Rank = BSP_ADC_GPIO_Conf[idx].Rank;
    HAL_ADC_ConfigChannel(hadc, &sConfig);
  }
}

/**
  * @brief  Start the ADC acquisition 
  * @param hadc: ADC handle pointer
  * @param pADC_Value: ADC data array pointer
  * @retval None
  */
static void BSPX_ADC_Start(ADC_HandleTypeDef * hadc, uint32_t * pADC_Value)
{
  assert_param(hadc != NULL);

  HAL_ADCEx_Calibration_Start(hadc);

  /*  Start Conversion */
  HAL_ADC_Start_DMA(hadc, pADC_Value, BSP_ADC_CH_NUM);
}

/**
  * @brief  ADC GPIO Inititialization (ports and pins)
  * @retval  None
  */
static void BSPX_ADC_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Enable clock of gpios */
  BSP_ADC_GPIOS_CLK_ENABLE();
  
  /* Configure GPIO pins */
  for(uint8_t idx = 0; idx < BSP_ADC_CH_NUM; idx++)
  {
    GPIO_InitStruct.Pin = BSP_ADC_GPIO_Conf[idx].PortPin.Pin;
    GPIO_InitStruct.Mode = BSP_ADC_GPIO_Conf[idx].PortPin.Mode;
    GPIO_InitStruct.Pull = BSP_ADC_GPIO_Conf[idx].PortPin.PuPd;
    HAL_GPIO_Init(BSP_ADC_GPIO_Conf[idx].PortPin.Port, &GPIO_InitStruct);
  }
}

/**
* @brief Local DMA inititialization function
* @param hadc: ADC handle pointer
* @param hdma: DMA ADC handle pointer
* @param Instance: DMA ADC used instance
* @param Request: DMA ADC request
* @retval None
*/
static void BSPX_ADC_DMA_Init(ADC_HandleTypeDef * hadc, DMA_HandleTypeDef * hdma, DMA_Channel_TypeDef * Instance, uint32_t Request)
{
  assert_param(hdma != NULL);
  assert_param(Instance != NULL);

  /* enabling DMA ADC clock */
  BSP_ADC_DMA_CLK_ENABLE();
  
  /* ADC DMA Init */
  hdma->Instance = Instance;
  hdma->Init.Request = Request;
  hdma->Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma->Init.PeriphInc = DMA_PINC_DISABLE;
  hdma->Init.MemInc = DMA_MINC_ENABLE;
  hdma->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma->Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma->Init.Mode = DMA_CIRCULAR;
  hdma->Init.Priority = DMA_PRIORITY_LOW;
  HAL_DMA_Init(hdma);
  
  __HAL_LINKDMA(hadc, DMA_Handle, *hdma);
}

/**
  * @} BSP_STEVAL_2STPD01_COMMON_Private_Functions
  */

/**
  * @} BSP_STEVAL_2STPD01_COMMON
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
