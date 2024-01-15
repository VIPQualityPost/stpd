/**
  ******************************************************************************
  * @file    steval_2stpd01_bus.c
  * @author  MCD Application Team and SRA.SLDU.SC Team
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

/* Includes ------------------------------------------------------------------*/
#include "steval_2stpd01_bus.h"

/** @addtogroup BSP_STEVAL_2STPD01_BUS
  * @{
  */

/** @defgroup BSP_STEVAL_2STPD01_BUS_Private_Macros Private Macros
  * @{
  */
#define DIV_ROUND_CLOSEST(x, d)  (((x) + ((d) / 2U)) / (d))
/**
  * @} BSP_STEVAL_2STPD01_BUS_Private_Macros
  */

/** @defgroup BSP_STEVAL_2STPD01_BUS_Private_Defines Private Defines
  * @{
  */
#define I2C_ANALOG_FILTER_ENABLE         1U
#define I2C_ANALOG_FILTER_DELAY_MIN      50U   /* ns */
#define I2C_ANALOG_FILTER_DELAY_MAX      260U  /* ns */
#define I2C_ANALOG_FILTER_DELAY_DEFAULT  2U    /* ns */

#define I2C_VALID_PRESC_NBR              100U
#define I2C_PRESC_MAX                    16U
#define I2C_SCLDEL_MAX                   16U
#define I2C_SDADEL_MAX                   16U
#define I2C_SCLH_MAX                     256U
#define I2C_SCLL_MAX                     256U
#define SEC2NSEC                         1000000000UL

/**
  * @} BSP_STEVAL_2STPD01_BUS_Private_Defines
  */

/** @defgroup BSP_STEVAL_2STPD01_BUS_Exported_Variables Exported Variables
  * @{
  */
I2C_HandleTypeDef hbus_i2c;
/**
  * @} BSP_STEVAL_2STPD01_BUS_Exported_Variables
  */

/** @defgroup BSP_STEVAL_2STPD01_BUS_Private_Variables Private Variables
  * @{
  */
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
static uint32_t IsI2c3MspCbValid = 0;           /*!< Validity of the callback */
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

/**
  * @brief struct i2c_charac
  * freq: I2C bus speed (Hz)
  * freq_min: 80% of I2C bus speed (Hz)
  * freq_max: 100% of I2C bus speed (Hz)
  * fall_max: Max fall time of both SDA and SCL signals (ns)
  * rise_max: Max rise time of both SDA and SCL signals (ns)
  * hddat_min: Min data hold time (ns)
  * vddat_max: Max data valid time (ns)
  * sudat_min: Min data setup time (ns)
  * l_min: Min low period of the SCL clock (ns)
  * h_min: Min high period of the SCL clock (ns)
  * trise: Rise time (ns)
  * tfall: Fall time (ns)
  * dnf: Digital filter coefficient (0-16)
  */
struct i2c_charac
{
  uint32_t freq;
  uint32_t freq_min;
  uint32_t freq_max;
  uint32_t hddat_min;
  uint32_t vddat_max;
  uint32_t sudat_min;
  uint32_t l_min;
  uint32_t h_min;
  uint32_t trise;
  uint32_t tfall;
  uint32_t dnf;
};

/**
  * @brief  i2c_speed enumeration 
  */
enum i2c_speed
{
  I2C_SPEED_STANDARD  = 0U, /* 100 kHz */
  I2C_SPEED_FAST      = 1U, /* 400 kHz */
  I2C_SPEED_FAST_PLUS = 2U, /* 1   MHz */
};

/**
  * @brief I2C timings config parameters
  * trise: Rise time (ns)
  * tfall: Fall time (ns)
  * dnf: Digital filter coefficient (0-16)
  */
struct i2c_timings_config
{
  uint32_t trise;
  uint32_t tfall;
  uint32_t dnf;
};

/**
  * @brief I2C  timings parameters
  * presc: Prescaler value
  * tscldel: Data setup time
  * tsdadel: Data hold time
  * sclh: SCL high period (master mode)
  * scll: SCL low period (master mode)
  */
struct i2c_timings
{
  uint32_t presc;
  uint32_t tscldel;
  uint32_t tsdadel;
  uint32_t sclh;
  uint32_t scll;
};

static const struct i2c_charac i2c_charac[] =
{
  [I2C_SPEED_STANDARD] =
  {
    .freq = 100000,
    .freq_min = 100000,
    .freq_max = 120000,
    .hddat_min = 0,
    .vddat_max = 3450,
    .sudat_min = 250,
    .l_min = 4700,
    .h_min = 4000,
    .trise = 400,
    .tfall = 100,
    .dnf = I2C_ANALOG_FILTER_DELAY_DEFAULT,
  },
  [I2C_SPEED_FAST] =
  {
    .freq = 400000,
    .freq_min = 320000,
    .freq_max = 400000,
    .hddat_min = 0,
    .vddat_max = 900,
    .sudat_min = 100,
    .l_min = 1300,
    .h_min = 600,
    .trise = 250,
    .tfall = 100,
    .dnf = I2C_ANALOG_FILTER_DELAY_DEFAULT,
  },
  [I2C_SPEED_FAST_PLUS] =
  {
    .freq = 1000000,
    .freq_min = 800000,
    .freq_max = 1000000,
    .hddat_min = 0,
    .vddat_max = 450,
    .sudat_min = 50,
    .l_min = 500,
    .h_min = 260,
    .trise = 60,
    .tfall = 100,
    .dnf = I2C_ANALOG_FILTER_DELAY_DEFAULT,
  },
};

static struct i2c_timings valid_timing[I2C_VALID_PRESC_NBR];
static uint32_t valid_timing_nbr = 0;

/**
  * @} BSP_STEVAL_2STPD01_BUS_Private_Variables
  */

/** @defgroup BSP_STEVAL_2STPD01_BUS_Private_Functions Private Functions
  * @{
  */
static void I2C_MspInit(I2C_HandleTypeDef *hi2c);
static void I2C_MspDeInit(I2C_HandleTypeDef *hi2c);
static uint32_t I2C_GetTiming(uint32_t clock_src_freq, uint32_t i2c_freq);
static uint32_t Compute_SCLL_SCLH (uint32_t clock_src_freq, uint32_t I2C_speed);
static void Compute_PRESC_SCLDEL_SDADEL(uint32_t clock_src_freq, uint32_t I2C_speed);
/**
  * @} BSP_STEVAL_2STPD01_BUS_Private_Functions
  */

/** @addtogroup BSP_STEVAL_2STPD01_BUS_Exported_Functions
  * @{
  */
/**
  * @brief  MX I2C1 Initialization as generated by CubeMX.
  * @param  phi2c : I2C handle.
  * @param  timing : I2C timings as described in the I2C peripheral V2 and V3.
  * @retval Status
  */
__weak HAL_StatusTypeDef MX_I2C_Init(I2C_HandleTypeDef *phi2c, uint32_t timing)
{
  HAL_StatusTypeDef ret = HAL_OK;

  phi2c->Init.Timing           = timing;
  phi2c->Init.OwnAddress1      = 0;
  phi2c->Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
  phi2c->Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
  phi2c->Init.OwnAddress2      = 0;
  phi2c->Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
  phi2c->Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

  if(HAL_I2C_Init(phi2c) != HAL_OK)
  {
    ret = HAL_ERROR;
  }
  else if (HAL_I2CEx_ConfigAnalogFilter(phi2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    ret = HAL_ERROR;
  }
  else
  {
    if (HAL_I2CEx_ConfigDigitalFilter(phi2c, 0) != HAL_OK)
    {
       ret = HAL_ERROR;
    }
  }

  return ret;
}

/**
  * @brief  Initializes I2C HAL.
  * @retval BSP status
  */
int32_t BSP_BUS_I2C_Init(void)
{
  int32_t ret = BSP_ERROR_NONE;

  hbus_i2c.Instance  = BUS_I2C_INSTANCE;

  if(HAL_I2C_GetState(&hbus_i2c) == HAL_I2C_STATE_RESET)
  {
#if (USE_HAL_I2C_REGISTER_CALLBACKS == 0)
    /* Init the I2C Msp */
    I2C_MspInit(&hbus_i2c);
#else
    if(IsI2c3MspCbValid == 0U)
    {
      if(BSP_I2C_RegisterDefaultMspCallbacks() != BSP_ERROR_NONE)
      {
        return BSP_ERROR_MSP_FAILURE;
      }
    }
#endif
    /* Init the I2C */
    if(MX_I2C_Init(&hbus_i2c, I2C_GetTiming(HAL_RCC_GetPCLK1Freq(), BUS_I2C_FREQUENCY)) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
    else if( HAL_I2CEx_ConfigAnalogFilter(&hbus_i2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
      ret = BSP_ERROR_BUS_FAILURE;
    }
    else
    {
      ret = BSP_ERROR_NONE;
    }
  }
  return ret;
}

/**
  * @brief  DeInitializes I2C HAL.
  * @retval BSP status
  */
int32_t BSP_BUS_I2C_DeInit(void)
{
  int32_t ret  = BSP_ERROR_NONE;

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 0)
  I2C_MspDeInit(&hbus_i2c);
#endif /* (USE_HAL_I2C_REGISTER_CALLBACKS == 0) */

  /* Init the I2C */
  if (HAL_I2C_DeInit(&hbus_i2c) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }

  return ret;
}

/**
  * @brief  Check whether the I2C bus is ready.
  * @param  DevAddr  I2C Device address on Bus.
  * @param  Trials   Check trials number
  * @retval BSP status
  */
int32_t BSP_BUS_I2C_IsReady(uint16_t DevAddr, uint32_t Trials)
{
  int32_t ret;

  if(HAL_I2C_IsDeviceReady(&hbus_i2c, DevAddr, Trials, BUS_I2C_POLL_TIMEOUT) != HAL_OK)
  {
    ret = BSP_ERROR_BUSY;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  return ret;
}

/**
  * @brief  Write a 8bit value in a register of the device through BUS.
  * @param  DevAddr  I2C Device address on Bus.
  * @param  Reg      The target register address to write
  * @param  pData    The target register value to written
  * @param  Length   buffer size to write
  * @retval BSP status
  */
int32_t BSP_BUS_I2C_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret;

  if(HAL_I2C_Mem_Write(&hbus_i2c, (uint8_t)DevAddr,
                       (uint16_t)Reg, I2C_MEMADD_SIZE_8BIT,
                       (uint8_t *)pData, Length, 0x1000) == HAL_OK)
  {
    ret = BSP_ERROR_NONE;
  }
  else
  {
    if( HAL_I2C_GetError(&hbus_i2c) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Read a 8bit register of the device through BUS
  * @param  DevAddr Device address on BUS
  * @param  Reg     The target register address to read
  * @param  pData   Pointer to data buffer
  * @param  Length  Length of the data
  * @retval BSP status
  */
int32_t BSP_BUS_I2C_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret;

  if (HAL_I2C_Mem_Read(&hbus_i2c, DevAddr, (uint16_t)Reg,
                       I2C_MEMADD_SIZE_8BIT, pData,
                       Length, 0x1000) == HAL_OK)
  {
    ret = BSP_ERROR_NONE;
  }
  else
  {
    if( HAL_I2C_GetError(&hbus_i2c) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Write a 16bit value in a register of the device through BUS.
  * @param  DevAddr Device address on Bus.
  * @param  Reg     The target register address to write
  * @param  pData   The target register value to be written
  * @param  Length  buffer size to be written
  * @retval BSP status
  */
int32_t BSP_BUS_I2C_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret;

  if(HAL_I2C_Mem_Write(&hbus_i2c, (uint8_t)DevAddr,
                       (uint16_t)Reg, I2C_MEMADD_SIZE_16BIT,
                       (uint8_t *)pData, Length, 0x1000) == HAL_OK)
  {
    ret = BSP_ERROR_NONE;
  }
  else
  {
    if( HAL_I2C_GetError(&hbus_i2c) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

/**
  * @brief  Read a 16bit register of the device through BUS
  * @param  DevAddr Device address on BUS
  * @param  Reg     The target register address to read
  * @param  pData   Pointer to data buffer
  * @param  Length  Length of the data
  * @retval BSP status
  */
int32_t BSP_BUS_I2C_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  int32_t ret;

  if (HAL_I2C_Mem_Read(&hbus_i2c, DevAddr, (uint16_t)Reg,
                       I2C_MEMADD_SIZE_16BIT, pData,
                       Length, 0x1000) == HAL_OK)
  {
    ret = BSP_ERROR_NONE;
  }
  else
  {
    if( HAL_I2C_GetError(&hbus_i2c) == HAL_I2C_ERROR_AF)
    {
      ret = BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE;
    }
    else
    {
      ret =  BSP_ERROR_PERIPH_FAILURE;
    }
  }
  return ret;
}

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
/**
  * @brief Register Default I2C1 Bus Msp Callbacks
  * @retval BSP status
  */
int32_t BSP_I2C1_RegisterDefaultMspCallbacks (void)
{
  int32_t ret = BSP_ERROR_NONE;

  __HAL_I2C_RESET_HANDLE_STATE(&hi2c1);

  /* Register default MspInit/MspDeInit Callback */
  if(HAL_I2C_RegisterCallback(&hbus_i2c, HAL_I2C_MSPINIT_CB_ID, I2C1_MspInit) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else if(HAL_I2C_RegisterCallback(&hbus_i2c, HAL_I2C_MSPDEINIT_CB_ID, I2C1_MspDeInit) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    IsI2c3MspCbValid = 1U;
  }

  /* BSP status */
  return ret;
}

/**
  * @brief Register I2C1 Bus Msp Callback registering
  * @param Callbacks     pointer to I2C1 MspInit/MspDeInit callback functions
  * @retval BSP status
  */
int32_t BSP_I2C1_RegisterMspCallbacks (BSP_I2C1_Cb_t *Callback)
{
  int32_t ret = BSP_ERROR_NONE;

  __HAL_I2C_RESET_HANDLE_STATE(&hi2c1);

  /* Register MspInit/MspDeInit Callbacks */
  if(HAL_I2C_RegisterCallback(&hbus_i2c, HAL_I2C_MSPINIT_CB_ID, Callback->pMspI2cInitCb) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else if(HAL_I2C_RegisterCallback(&hbus_i2c, HAL_I2C_MSPDEINIT_CB_ID, Callback->pMspI2cDeInitCb) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    IsI2c3MspCbValid = 1U;
  }

  /* BSP status */
  return ret;
}


#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

/**
  * @brief  Return system tick (ms) function.
  * @retval Current HAL time base time stamp
  */
int32_t BSP_BUS_GetTick(void)
{
  return HAL_GetTick();
}

/**
  * @} BSP_STEVAL_2STPD01_BUS_Exported_Functions
  */

/** @defgroup BSP_STEVAL_2STPD01_BUS_Private_Functions Private Functions
  * @{
  */

/**
  * @brief  Initializes I2C MSP.
  * @param  hi2c  I2C handler
  */
static void I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
  GPIO_InitTypeDef  gpio_init_structure;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);

  /*##-1- Configure the I2C clock source. The clock is derived from the SYSCLK #*/
  RCC_PeriphCLKInitTypeDef  RCC_PeriphCLKInitStruct;
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  RCC_PeriphCLKInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

  /*** Configure the GPIOs ***/
  /* Enable SCL GPIO clock */
  BUS_I2C_SCL_GPIO_CLK_ENABLE();
  /* Enable SDA GPIO clock */
  BUS_I2C_SDA_GPIO_CLK_ENABLE();

  /* Configure I2C SCL as alternate function */
  gpio_init_structure.Pin       = BUS_I2C_SCL_GPIO_PIN;
  gpio_init_structure.Mode      = GPIO_MODE_AF_OD;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Alternate = BUS_I2C_SCL_GPIO_AF;
  HAL_GPIO_Init(BUS_I2C_SCL_GPIO_PORT, &gpio_init_structure);

  /* Configure I2C SDA as alternate function */
  gpio_init_structure.Pin       = BUS_I2C_SDA_GPIO_PIN;
  gpio_init_structure.Mode      = GPIO_MODE_AF_OD;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_HIGH;
  gpio_init_structure.Alternate = BUS_I2C_SDA_GPIO_AF;
  HAL_GPIO_Init(BUS_I2C_SDA_GPIO_PORT, &gpio_init_structure);

  /*** Configure the I2C peripheral ***/
  /* Enable I2C clock */
  BUS_I2C_CLK_ENABLE();
}

/**
  * @brief  DeInitializes I2C MSP.
  * @param  hi2c  I2C handler
  */
static void I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
  GPIO_InitTypeDef  gpio_init_structure;

  /* Prevent unused argument(s) compilation warning */
  UNUSED(hi2c);

  /* Configure I2C SCL, SDA as alternate function */
  gpio_init_structure.Pin = BUS_I2C_SCL_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_I2C_SDA_GPIO_PORT, gpio_init_structure.Pin );
  gpio_init_structure.Pin = BUS_I2C_SDA_GPIO_PIN;
  HAL_GPIO_DeInit(BUS_I2C_SDA_GPIO_PORT, gpio_init_structure.Pin);

  /** Disable GPIO clock **/
  /* Disable SCL GPIO clock */
  BUS_I2C_SCL_GPIO_CLK_DISABLE();
  /* Disable SDA GPIO clock */
  BUS_I2C_SDA_GPIO_CLK_DISABLE();
  /* Disable I2C clock */
  BUS_I2C_CLK_DISABLE();
}

/**
  * @brief  Convert the I2C Frequency into I2C timing.
  * @param  clock_src_freq : I2C source clock in Hz.
  * @param  i2c_freq : I2C frequency in Hz.
  * @retval I2C timing value
  */
static uint32_t I2C_GetTiming(uint32_t clock_src_freq, uint32_t i2c_freq)
{
  uint32_t ret = 0;
  uint32_t speed;
  uint32_t itiming;

  if((clock_src_freq != 0U) && (i2c_freq != 0U))
  {
    for ( speed = 0 ; speed <=  (uint32_t)I2C_SPEED_FAST_PLUS ; speed++)
    {
      if ((i2c_freq >= i2c_charac[speed].freq_min) &&
          (i2c_freq <= i2c_charac[speed].freq_max))
      {
        Compute_PRESC_SCLDEL_SDADEL(clock_src_freq, speed);
        itiming = Compute_SCLL_SCLH(clock_src_freq, speed);

        if (itiming < I2C_VALID_PRESC_NBR)
        {
          ret = ((valid_timing[itiming].presc  & 0x0FU) << 28) |
            ((valid_timing[itiming].tscldel & 0x0FU) << 20) |
              ((valid_timing[itiming].tsdadel & 0x0FU) << 16) |
                ((valid_timing[itiming].sclh & 0xFFU) << 8) |
                  ((valid_timing[itiming].scll & 0xFFU) << 0);
        }
        break;
      }
    }
  }

  return ret;
}


/**
  * @brief  Compute PRESC, SCLDEL and SDADEL.
  * @param  clock_src_freq : I2C source clock in Hz.
  * @param  i2c_speed : I2C frequency (index).
  */
static void Compute_PRESC_SCLDEL_SDADEL(uint32_t clock_src_freq, uint32_t i2c_speed)
{
  uint32_t prev_presc = I2C_PRESC_MAX;
  uint32_t ti2cclk;
  int32_t  tsdadel_min;
  int32_t  tscldel_min;
  uint32_t presc, iscl, isda;

  ti2cclk   = (SEC2NSEC + (clock_src_freq / 2U))/ clock_src_freq;

  /* tDNF = DNF x tI2CCLK
     tPRESC = (PRESC+1) x tI2CCLK
     SDADEL >= {tf +tHD;DAT(min) - tAF(min) - tDNF - [3 x tI2CCLK]} / {tPRESC}
     SDADEL <= {tVD;DAT(max) - tr - tAF(max) - tDNF- [4 x tI2CCLK]} / {tPRESC} */

  tsdadel_min = (int32_t)i2c_charac[i2c_speed].tfall + (int32_t)i2c_charac[i2c_speed].hddat_min -
    (int32_t)I2C_ANALOG_FILTER_DELAY_MIN - (int32_t)(((int32_t)i2c_charac[i2c_speed].dnf + 3) * (int32_t)ti2cclk);

  /* {[tr+ tSU;DAT(min)] / [tPRESC]} - 1 <= SCLDEL */
  tscldel_min = (int32_t)i2c_charac[i2c_speed].trise + (int32_t)i2c_charac[i2c_speed].sudat_min;

  if (tsdadel_min <= 0)
  {
    tsdadel_min = 0;
  }

  for (presc = 0; presc < I2C_PRESC_MAX; presc++)
  {
    for (iscl = 0; iscl < I2C_SCLDEL_MAX; iscl++)
    {
      /* TSCLDEL = (SCLDEL+1) * (PRESC+1) * TI2CCLK */
      uint32_t tscldel = (iscl + 1U) * (presc + 1U) * ti2cclk;

      if (tscldel >= (uint32_t)tscldel_min)
      {
        for (isda = 0; isda < I2C_SDADEL_MAX; isda++)
        {
          /* TSDADEL = SDADEL * (PRESC+1) * TI2CCLK */
          uint32_t tsdadel = (isda * (presc + 1U)) * ti2cclk;

          if (tsdadel >= (uint32_t)tsdadel_min)
          {
            if(presc != prev_presc)
            {
              valid_timing[valid_timing_nbr].presc = presc;
              valid_timing[valid_timing_nbr].tscldel = iscl;
              valid_timing[valid_timing_nbr].tsdadel = isda;
              prev_presc = presc;
              valid_timing_nbr ++;

              if(valid_timing_nbr >= I2C_VALID_PRESC_NBR)
              {
                return;
              }
            }
          }
        }
      }
    }
  }
}

/**
  * @brief  Calculate SCLL and SCLH and find best configuration.
  * @param  clock_src_freq : I2C source clock in HZ.
  * @param  I2C_speed : I2C frequency (index).
  * @retval > 0 : config index, otherwise no valid config
  */
static uint32_t Compute_SCLL_SCLH (uint32_t clock_src_freq, uint32_t I2C_speed)
{
  uint32_t ret = I2C_VALID_PRESC_NBR;
  uint32_t ti2cclk;
  uint32_t ti2cspeed;
  uint32_t prev_clk_error;
  uint32_t dnf_delay;
  uint32_t clk_min, clk_max;
  uint32_t l, h;

  ti2cclk   = (SEC2NSEC + (clock_src_freq / 2U))/ clock_src_freq;
  ti2cspeed   = (SEC2NSEC + (i2c_charac[I2C_speed].freq / 2U))/ i2c_charac[I2C_speed].freq;

  /* tDNF = DNF x tI2CCLK */
  dnf_delay = i2c_charac[I2C_speed].dnf * ti2cclk;

  clk_max = SEC2NSEC / i2c_charac[I2C_speed].freq_min;
  clk_min = SEC2NSEC / i2c_charac[I2C_speed].freq_max;

  prev_clk_error = ti2cspeed;

  for (uint32_t count = 0; count < valid_timing_nbr; count++)
  {
    /* tPRESC = (PRESC+1) x tI2CCLK*/
    uint32_t tpresc = (valid_timing[count].presc + 1U) * ti2cclk;

    for (l = 0; l < I2C_SCLL_MAX; l++)
    {
      /* tLOW(min) <= tAF(min) + tDNF + 2 x tI2CCLK + [(SCLL+1) x tPRESC ] */
      uint32_t tscl_l = I2C_ANALOG_FILTER_DELAY_MIN + dnf_delay + (2U * ti2cclk) + ((l + 1U) * tpresc);


      /* The I2CCLK period tI2CCLK must respect the following conditions:
         tI2CCLK < (tLOW - tfilters) / 4 and tI2CCLK < tHIGH */
      if ((tscl_l < i2c_charac[I2C_speed].l_min) || (ti2cclk >= ((tscl_l - I2C_ANALOG_FILTER_DELAY_MIN - dnf_delay) / 4U)))
      {
        continue;
      }

      for (h = 0; h < I2C_SCLH_MAX; h++)
      {
        /* tHIGH(min) <= tAF(min) + tDNF + 2 x tI2CCLK + [(SCLH+1) x tPRESC] */
        uint32_t tscl_h = I2C_ANALOG_FILTER_DELAY_MIN + dnf_delay + (2U * ti2cclk) + ((h + 1U) * tpresc);

        /* tSCL = tf + tLOW + tr + tHIGH */
        uint32_t tscl = tscl_l + tscl_h + i2c_charac[I2C_speed].trise + i2c_charac[I2C_speed].tfall;

        if ((tscl >= clk_min) && (tscl <= clk_max) && (tscl_h >= i2c_charac[I2C_speed].h_min) && (ti2cclk < tscl_h))
        {
          int32_t clk_error = (int32_t)tscl - (int32_t)ti2cspeed;

          if (clk_error < 0)
          {
            clk_error = -clk_error;
          }

          /* look for the timings with the lowest clock error */
          if ((uint32_t)clk_error < prev_clk_error)
          {
            prev_clk_error = (uint32_t)clk_error;
            valid_timing[count].scll = l;
            valid_timing[count].sclh = h;
            ret = count;
          }
        }
      }
    }
  }

  return ret;
}

/**
  * @} BSP_STEVAL_2STPD01_BUS_Private_Functions
  */

/**
  * @} BSP_STEVAL_2STPD01_BUS
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
