/**
  ******************************************************************************
  * @file    steval_2stpd01_stpd01.c
  * @author  SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   STPD01 component support for STEVAL-2STPD01.
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

/* Includes ------------------------------------------------------------------*/
#include "steval_2stpd01_stpd01.h"
#include "steval_2stpd01_stpd01_conf.h"
#include "steval_2stpd01_usbpd_pwr.h"
#include "steval_2stpd01_bus.h"

/** @addtogroup BSP_STEVAL_2STPD01_STPD01
  * @{
  */

/** @defgroup BSP_STEVAL_2STPD01_STPD01_Private_Variables Private Variables
  * @{
  */
/**
  * @brief  STPD01 Port GPIO Configuration
  */
static const BSP_PortPin_Conf_t BSP_STPD01_GPIO_Conf[USBPD_PWR_INSTANCES_NBR][BSP_STPD01_GPIO_NUM] = {
  { 
    BSP_GPIO_Def(STPD01_P0_ENABLE),
    BSP_GPIO_Def(STPD01_P0_ALERT),
    BSP_GPIO_Def(STPD01_P0_PGOOD),
  },
  { 
    BSP_GPIO_Def(STPD01_P1_ENABLE),
    BSP_GPIO_Def(STPD01_P1_ALERT),
    BSP_GPIO_Def(STPD01_P1_PGOOD),
  },
};

/**
  * @brief  STPD01 Port Component object 
  */
static STPD01_Object_t  BSP_STPD01_PortCompObj[USBPD_PWR_INSTANCES_NBR] = {0};
/**
  * @} BSP_STEVAL_2STPD01_STPD01_Private_Variables
  */

/** @defgroup BSP_STEVAL_2STPD01_STPD01_Private_Functions Private Functions
  * @brief STPD01 functions useful locally
  * @{
  */
static void     BSP_STPD01_GPIO_Init(uint32_t PortNum);
static void     BSP_STPD01_IT_Init(uint32_t PortNum);
static int32_t  BSP_STPD01_BUS_Init(uint32_t PortNum, uint16_t Address);
/** 
  * @} BSP_STEVAL_2STPD01_STPD01_Private_Functions
  */

/** @defgroup BSP_STEVAL_2STPD01_STPD01_Exported_Functions Exported Functions
  * @{
  */
/**
  * @brief  Initialize STPD01 HW resources associated to Type-C port
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  Address I2C address
  * @retval BSP errorcode
  */
int32_t BSP_STPD01_Init(uint32_t PortNum, uint16_t Address)
{
  BSP_PWR_PORT_ASSERT(PortNum);
  
  int32_t ret = BSP_ERROR_NONE;

  /* Initialize required GPIOs */
  BSP_STPD01_GPIO_Init(PortNum);

  /* Enabling the component */
  BSP_STPD01_Enable(PortNum, BSP_PWR_ON);

  /* Initialize EXTI for ALERT signal */
  BSP_STPD01_IT_Init(PortNum);

  /* Initialize bus and component */
  ret = BSP_STPD01_BUS_Init(PortNum, Address);
  
  return ret;
}

/**
  * @brief  De-Initialize STPD01 HW resources associated to Type-C port
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP errorcode
  */
int32_t BSP_STPD01_DeInit(uint32_t PortNum)
{
  BSP_PWR_PORT_ASSERT(PortNum);
  UNUSED(PortNum);
  
  /* It's not possible to de-init this part */
  return BSP_ERROR_NONE;
}

/**
  * @brief  Enable STPD01 component according to the port
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  Enable On / Off
  *         @arg @ref BSP_PWR_ON
  *         @arg @ref BSP_PWR_OFF
  * @retval BSP errorcode
  */
int32_t BSP_STPD01_Enable(uint32_t PortNum, uint32_t Enable)
{
  /* Check the port */
  BSP_PWR_PORT_ASSERT(PortNum);

  /* Set/Reset the corresponding gpio */
  HAL_GPIO_WritePin(BSP_STPD01_GPIO_Conf[PortNum][BSP_STPD01_GPIO_ENABLE].Port,
                    BSP_STPD01_GPIO_Conf[PortNum][BSP_STPD01_GPIO_ENABLE].Pin,
                    Enable == BSP_PWR_ON ? GPIO_PIN_SET : GPIO_PIN_RESET);

  return BSP_ERROR_NONE;
}

/**
  * @brief  Get the enabling status of the STPD01 component
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP_PWR_ON | BSP_PWR_OFF or a BSP errorcode
  */
int32_t BSP_STPD01_IsEnable(uint32_t PortNum)
{
  return HAL_GPIO_ReadPin(BSP_STPD01_GPIO_Conf[PortNum][BSP_STPD01_GPIO_ENABLE].Port,
                          BSP_STPD01_GPIO_Conf[PortNum][BSP_STPD01_GPIO_ENABLE].Pin) == GPIO_PIN_SET ? BSP_PWR_ON : BSP_PWR_OFF;
}

/**
  * @brief  Initialize VBUS for STPD01
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP errorcode
  */
int32_t BSP_STPD01_VBus_Init(uint32_t PortNum)
{
  /* Check the port */
  BSP_PWR_PORT_ASSERT(PortNum);

  /* set default value */
  BSP_STPD01_VBus_Value(PortNum, BSP_PWR_VSAFE);
  
  return BSP_ERROR_NONE;
}

/**
  * @brief  De-Initialize VBUS for STPD01
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval BSP errorcode
  */
int32_t BSP_STPD01_VBus_DeInit(uint32_t PortNum)
{
  /* Check the port */
  BSP_PWR_PORT_ASSERT(PortNum);

  /* set default value */
  BSP_STPD01_VBus_Value(PortNum, BSP_PWR_VSAFE);

  return BSP_ERROR_NONE;
}

/**
  * @brief  Enable / Disable VBUS for STPD01
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  Enable On / Off
  *         @arg @ref BSP_PWR_ON
  *         @arg @ref BSP_PWR_OFF
  * @retval BSP errorcode
  */
int32_t BSP_STPD01_VBus(uint32_t PortNum, uint32_t Enable)
{
  BSP_PWR_PORT_ASSERT(PortNum);
  if (Enable == BSP_PWR_OFF)
  {
    BSP_STPD01_VBus_Value(PortNum, BSP_PWR_VSAFE);
  }

  
  return BSP_ERROR_NONE;
}

/**
  * @brief  Set a required value of VBUS for STPD01
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  Voltage required voltage value (mV)
  * @retval BSP errorcode
  */
int32_t BSP_STPD01_VBus_Value(uint32_t PortNum, int32_t Voltage)
{
  BSP_PWR_PORT_ASSERT(PortNum);

  if (STPD01_Driver.SetVoltage(&BSP_STPD01_PortCompObj[PortNum], Voltage) != STPD01_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }
  return BSP_ERROR_NONE;
}
/**
  * @} BSP_STEVAL_2STPD01_STPD01_Exported_Functions
  */

/** @addtogroup BSP_STEVAL_2STPD01_STPD01_Private_Functions
  * @{
  */
/**
  * @brief  Configure GPIO for STPD01.
  * @note   GPIO used for STPD01, ENABLE pin driving, the Bus measurement are configured in ADC section
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval None
  */
static void BSP_STPD01_GPIO_Init(uint32_t PortNum)
{
  BSP_PWR_PORT_ASSERT(PortNum);

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  
  /*Configure GPIO pins */
  for(uint8_t idx = 0; idx < BSP_STPD01_GPIO_NUM; idx++)
  {
    if (USBPD_PWR_INVALID_VALUE == BSP_STPD01_GPIO_Conf[PortNum][idx].Mode)
    {
      continue;
    }

    GPIO_InitStruct.Pin = BSP_STPD01_GPIO_Conf[PortNum][idx].Pin;
    GPIO_InitStruct.Mode = BSP_STPD01_GPIO_Conf[PortNum][idx].Mode;
    GPIO_InitStruct.Pull = BSP_STPD01_GPIO_Conf[PortNum][idx].PuPd;
    HAL_GPIO_Init(BSP_STPD01_GPIO_Conf[PortNum][idx].Port, &GPIO_InitStruct);
  }
}

/**
  * @brief  Configure STPD01 low level interrupt.
  * @note   Corresponds to EXTI mapped onto FLGn pin
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @retval None
  */
static void BSP_STPD01_IT_Init(uint32_t PortNum)
{
  BSP_PWR_PORT_ASSERT(PortNum);
  
  IRQn_Type irq_array[BSP_STPD01_IRQ_NUM] = BSP_STPD01_IRQ;
  
  for(uint8_t i = 0; i < BSP_STPD01_IRQ_NUM; i++)
  {
    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(irq_array[i], 0, 0);
    HAL_NVIC_EnableIRQ(irq_array[i]);
  }
}

/**
  * @brief  I2C BUS regsitration for STPD01 communication
  * @param  PortNum Type-C port identifier
  *         This parameter can be take one of the following values:
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_1
  *         @arg @ref USBPD_PWR_TYPE_C_PORT_2
  * @param  Address I2C address
  * @retval BSP errorcode
  */
static int32_t BSP_STPD01_BUS_Init(uint32_t PortNum, uint16_t Address)
{
  STPD01_IO_t              STPD01_IO_Ctx;

  /* Configure the STPD01 I2C driver */
  STPD01_IO_Ctx.Address     = Address;
  STPD01_IO_Ctx.Init        = BSP_BUS_I2C_Init;
  STPD01_IO_Ctx.DeInit      = BSP_BUS_I2C_DeInit;
  STPD01_IO_Ctx.ReadReg     = BSP_BUS_I2C_ReadReg;
  STPD01_IO_Ctx.WriteReg    = BSP_BUS_I2C_WriteReg;
  STPD01_IO_Ctx.GetTick     = BSP_BUS_GetTick;

  /* Initialization */
  BSP_STPD01_PortCompObj[PortNum].PortNum = PortNum;
  BSP_STPD01_PortCompObj[PortNum].IsInitialized = 0;
  
  /* Register the component on BUS IO */
  if (STPD01_RegisterBusIO(&BSP_STPD01_PortCompObj[PortNum], &STPD01_IO_Ctx) != STPD01_OK)
  {
    return BSP_ERROR_BUS_FAILURE;
  }
  
  /* Initialisation step */
  if (STPD01_Driver.Init(&BSP_STPD01_PortCompObj[PortNum]) != STPD01_OK)
  {
    return BSP_ERROR_COMPONENT_FAILURE;
  }  
  
  return BSP_ERROR_NONE;
}
/**
  * @} BSP_STEVAL_2STPD01_STPD01_Private_Functions
  */

/**
  * @} BSP_STEVAL_2STPD01_STPD01
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
