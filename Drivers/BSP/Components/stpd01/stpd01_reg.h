/**
  ******************************************************************************
  * @file    stpd01_reg.h
  * @author  SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   This file contains all the functions prototypes for the stdp01_reg.c
  *          driver.
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
#ifndef STPD01_REG_H
#define STPD01_REG_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP_Components_STPD01
  * @{
  */

/** @defgroup BSP_Components_STPD01_RegisterLayer Register Layer
  * @{
  */
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported typedef ----------------------------------------------------------*/
/** @defgroup BSP_Components_STPD01_RegisterLayer_Exported_Types Exported Types
  * @{
  */
/**
  * @brief  Write function type
  */
typedef int32_t (*STPD01_Write_Func)(void *, uint16_t, uint8_t *, uint16_t);
/**
  * @brief  Read function type
  */
typedef int32_t (*STPD01_Read_Func)(void *, uint16_t, uint8_t *, uint16_t);

/**
  * @brief  Context structure
  */
typedef struct
{
	STPD01_Write_Func WriteReg;
	STPD01_Read_Func ReadReg;
	void *handle;
} STPD01_ctx_t;

/** @defgroup BSP_Components_STPD01_RegisterLayer_Exported_Types_Registers Registers
  * @{
  */
/**
  * @brief  STPD01 VOUT_SELECT register Structure definition
  *         Address: 00h
  *         Access: R/W
  * @note   This register  allows to set the output voltage
  */
typedef struct
{
	uint8_t VOUT_SELECT:			8;
} STPD01_VOUT_SELECT_REG_t;

/**
  * @brief  STPD01 ILIM_SELECT register Structure definition
  *         Address: 01h
  *         Access: R/W
  * @note   This register  allows to set the current limitation
  */
typedef struct
{
	uint8_t _Reserved_5_7:			3;
	uint8_t ILIM_SELECT:			5;
} STPD01_ILIM_SELECT_REG_t;

/**
  * @brief  STPD01 INT_STATUS register Structure definition
  *         Address: 02h
  *         Access: R
  * @note   This register indicates a bit value change has occurred in INT_STATUS register.
  *	        It alerts when the over voltage is met. (OVP)
  *		It alerts when the over current is met. (OCP)
  *		It alerts when the short circuit is met. (SHORT)
  *		It alerts when the power good is wmet. (PGOOD)
  *		It alerts when the watchdog is enable. (WD)
  *             It alerts when the temperature protection is met (OTP)
  *		It alerts when the over temperature warning is met.(OTW)
  *             It alerts when the inductor peak protection is met. (IPK)
  */
typedef struct
{
	uint8_t INT_STATUS_OVP:		1;
	uint8_t INT_STATUS_OCP:	    	1;
	uint8_t INT_STATUS_SHORT:  	1;
	uint8_t INT_STATUS_PGOOD:	1;
	uint8_t INT_STATUS_WD:        	1;
	uint8_t INT_STATUS_OTP:	    	1;
	uint8_t INT_STATUS_OTW:	    	1;
	uint8_t INT_STATUS_IPK:	    	1;
} STPD01_INT_STATUS_REG_t;

/**
  * @brief  STPD01 INT_LATCH register Structure definition
  *         Address: 03h
  *         Access: R/C
  * @note   This register is to be read to clear an interrupt
  */
typedef struct
{
	uint8_t INT_LATCH_OVP:			1;
	uint8_t INT_LATCH_OCP:			1;
	uint8_t INT_LATCH_SHORT:		1;
	uint8_t INT_LATCH_PGOOD:		1;
	uint8_t INT_LATCH_WD:			1;
	uint8_t INT_LATCH_OTP:			1;
	uint8_t INT_LATCH_OTW:			1;
	uint8_t INT_LATCH_IPK:			1;
} STPD01_INT_LATCH_REG_t;

/**
  * @brief  STPD01 INT_MASK register Structure definition
  *         Address: 04h
  *         Access: R/W
  * @note   This register enables or disables an interrupt
  */
typedef struct
{
	uint8_t INT_MASK_OVP:	        1;
	uint8_t INT_MASK_OCP:	        1;
	uint8_t INT_MASK_SHORT:	        1;
	uint8_t INT_MASK_PGOOD:	        1;
	uint8_t INT_MASK_WD:	        1;
	uint8_t INT_MASK_OTP:  	        1;
	uint8_t INT_MASK_OTW:	        1;
	uint8_t INT_MASK_IPK:	        1;
} STPD01_INT_MASK_REG_t;

/**
  * @brief  STPD01 SERVICE register Structure definition
  *         Address: 05h
  *         Access: R/W
  * @note   This register indicates a bit value change has occurred in INT_MASK register
  *         It alerts also when the over temperature condition is met.
  */
typedef struct
{
	uint8_t SERVICE_WTCHDG_TIM:     2;
	uint8_t SERVICE_Reserved_5:  	1;
	uint8_t SERVICE_CDC:	        2;
	uint8_t SERVICE_SW_FRQ:	        1;
	uint8_t SERVICE_DITH:	        1;
	uint8_t SERVICE_AUTO_DIS:       1;
} STPD01_SERVICE_REG_t;

/**
  * @brief  STPD01 ENABLE register Structure definition
  *         Address: 06h
  *         Access: R/W
  * @note   This register enable / disable features
  */
typedef struct
{
	uint8_t ENABLE_Reserved_7_2:    6;
	uint8_t ENABLE_WD:  	        1;
	uint8_t ENABLE_DIG_EN:	        1;
} STPD01_DIG_ENABLE_REG_t;
/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Types_Registers
  */

/**
  * @brief  Device Register union
  * @note   the correct value depending to the register storaged
  */
typedef union
{
	STPD01_VOUT_SELECT_REG_t         STPD01_VOUT_SELECT_REG;
	STPD01_ILIM_SELECT_REG_t         STPD01_ILIM_SELECT_REG;
	STPD01_INT_STATUS_REG_t          STPD01_INT_STATUS_REG;
	STPD01_INT_LATCH_REG_t           STPD01_INT_LATCH_REG;
	STPD01_INT_MASK_REG_t            STPD01_INT_MASK_REG;
	STPD01_SERVICE_REG_t             STPD01_SERVICE_REG;
	STPD01_DIG_ENABLE_REG_t          STPD01_DIG_ENABLE_REG;
	uint8_t                      byte;
} STPD01_REGISTER_t;

/** @defgroup BSP_Components_STPD01_RegisterLayer_Exported_Types_RegStruct Registers structures
  * @{
  */
/**
  * @brief STPD01 VOUT_SELECT  structures definition
  *        <ul>
  *        <li>20mV  step :  3000mV -   5900mV (0x00 - 0x91)</li>
  *        <li>100mV step :  6000mV -  11000mV (0x92 - 0xC4)</li>
  *        <li>200mV step : 11200mV -  20000mV (0xC5 - 0xF1)</li>
  *        </ul>
  *        Address: 0x00 - Bit 7:0
  */
typedef enum
{
        /* 20mV range */
	VOUT_SELECT_3000_MV             = 0x00,
	VOUT_SELECT_3020_MV             = 0x01,
	VOUT_SELECT_3040_MV             = 0x02,
	VOUT_SELECT_3060_MV             = 0x03,
	VOUT_SELECT_3080_MV             = 0x04,
	VOUT_SELECT_3100_MV             = 0x05,
	VOUT_SELECT_3120_MV             = 0x06,
	VOUT_SELECT_3140_MV             = 0x07,
	VOUT_SELECT_3160_MV             = 0x08,
	VOUT_SELECT_3180_MV             = 0x09,
	VOUT_SELECT_3200_MV             = 0x0A,
	VOUT_SELECT_3220_MV             = 0x0B,
	VOUT_SELECT_3240_MV             = 0x0C,
	VOUT_SELECT_3260_MV             = 0x0D,
	VOUT_SELECT_3280_MV             = 0x0E,
	VOUT_SELECT_3300_MV             = 0x0F,
	VOUT_SELECT_3320_MV             = 0x10,
	VOUT_SELECT_3340_MV             = 0x11,
	VOUT_SELECT_3360_MV             = 0x12,
	VOUT_SELECT_3380_MV             = 0x13,
	VOUT_SELECT_3400_MV             = 0x14,
	VOUT_SELECT_3420_MV             = 0x15,
	VOUT_SELECT_3440_MV             = 0x16,
	VOUT_SELECT_3460_MV             = 0x17,
	VOUT_SELECT_3480_MV             = 0x18,
	VOUT_SELECT_3500_MV             = 0x19,
	VOUT_SELECT_3520_MV             = 0x1A,
	VOUT_SELECT_3540_MV             = 0x1B,
	VOUT_SELECT_3560_MV             = 0x1C,
	VOUT_SELECT_3580_MV             = 0x1D,
	VOUT_SELECT_3600_MV             = 0x1E,
	VOUT_SELECT_3620_MV             = 0x1F,
	VOUT_SELECT_3640_MV             = 0x20,
	VOUT_SELECT_3660_MV             = 0x21,
	VOUT_SELECT_3680_MV             = 0x22,
	VOUT_SELECT_3700_MV             = 0x23,
	VOUT_SELECT_3720_MV             = 0x24,
	VOUT_SELECT_3740_MV             = 0x25,
	VOUT_SELECT_3760_MV             = 0x26,
	VOUT_SELECT_3780_MV             = 0x27,
	VOUT_SELECT_3800_MV             = 0x28,
	VOUT_SELECT_3820_MV             = 0x29,
	VOUT_SELECT_3840_MV             = 0x2A,
	VOUT_SELECT_3860_MV             = 0x2B,
	VOUT_SELECT_3880_MV             = 0x2C,
	VOUT_SELECT_3900_MV             = 0x2D,
	VOUT_SELECT_3920_MV             = 0x2E,
	VOUT_SELECT_3940_MV             = 0x2F,
	VOUT_SELECT_3960_MV             = 0x30,
	VOUT_SELECT_3980_MV             = 0x31,
	VOUT_SELECT_4000_MV             = 0x32,
	VOUT_SELECT_4020_MV             = 0x33,
	VOUT_SELECT_4040_MV             = 0x34,
	VOUT_SELECT_4060_MV             = 0x35,
	VOUT_SELECT_4080_MV             = 0x36,
	VOUT_SELECT_4100_MV             = 0x37,
	VOUT_SELECT_4120_MV             = 0x38,
	VOUT_SELECT_4140_MV             = 0x39,
	VOUT_SELECT_4160_MV             = 0x3A,
	VOUT_SELECT_4180_MV             = 0x3B,
	VOUT_SELECT_4200_MV             = 0x3C,
	VOUT_SELECT_4220_MV             = 0x3D,
	VOUT_SELECT_4240_MV             = 0x3E,
	VOUT_SELECT_4260_MV             = 0x3F,
	VOUT_SELECT_4280_MV             = 0x40,
	VOUT_SELECT_4300_MV             = 0x41,
	VOUT_SELECT_4320_MV             = 0x42,
	VOUT_SELECT_4340_MV             = 0x43,
	VOUT_SELECT_4360_MV             = 0x44,
	VOUT_SELECT_4380_MV             = 0x45,
	VOUT_SELECT_4400_MV             = 0x46,
	VOUT_SELECT_4420_MV             = 0x47,
	VOUT_SELECT_4440_MV             = 0x48,
	VOUT_SELECT_4460_MV             = 0x49,
	VOUT_SELECT_4480_MV             = 0x4A,
	VOUT_SELECT_4500_MV             = 0x4B,
	VOUT_SELECT_4520_MV             = 0x4C,
	VOUT_SELECT_4540_MV             = 0x4D,
	VOUT_SELECT_4560_MV             = 0x4E,
	VOUT_SELECT_4580_MV             = 0x4F,
	VOUT_SELECT_4600_MV             = 0x50,
	VOUT_SELECT_4620_MV             = 0x51,
	VOUT_SELECT_4640_MV             = 0x52,
	VOUT_SELECT_4660_MV             = 0x53,
	VOUT_SELECT_4680_MV             = 0x54,
	VOUT_SELECT_4700_MV             = 0x55,
	VOUT_SELECT_4720_MV             = 0x56,
	VOUT_SELECT_4740_MV             = 0x57,
	VOUT_SELECT_4760_MV             = 0x58,
	VOUT_SELECT_4780_MV             = 0x59,
	VOUT_SELECT_4800_MV             = 0x5A,
	VOUT_SELECT_4820_MV             = 0x5B,
	VOUT_SELECT_4840_MV             = 0x5C,
	VOUT_SELECT_4860_MV             = 0x5D,
	VOUT_SELECT_4880_MV             = 0x5E,
	VOUT_SELECT_4900_MV             = 0x5F,
	VOUT_SELECT_4920_MV             = 0x60,
	VOUT_SELECT_4940_MV             = 0x61,
	VOUT_SELECT_4960_MV             = 0x62,
	VOUT_SELECT_4980_MV             = 0x63,
	VOUT_SELECT_5000_MV             = 0x64,
	VOUT_SELECT_5020_MV             = 0x65,
	VOUT_SELECT_5040_MV             = 0x66,
	VOUT_SELECT_5060_MV             = 0x67,
	VOUT_SELECT_5080_MV             = 0x68,
	VOUT_SELECT_5100_MV             = 0x69,
	VOUT_SELECT_5120_MV             = 0x6A,
	VOUT_SELECT_5140_MV             = 0x6B,
	VOUT_SELECT_5160_MV             = 0x6C,
	VOUT_SELECT_5180_MV             = 0x6D,
	VOUT_SELECT_5200_MV             = 0x6E,
	VOUT_SELECT_5220_MV             = 0x6F,
	VOUT_SELECT_5240_MV             = 0x70,
	VOUT_SELECT_5260_MV             = 0x71,
	VOUT_SELECT_5280_MV             = 0x72,
	VOUT_SELECT_5300_MV             = 0x73,
	VOUT_SELECT_5320_MV             = 0x74,
	VOUT_SELECT_5340_MV             = 0x75,
	VOUT_SELECT_5360_MV             = 0x76,
	VOUT_SELECT_5380_MV             = 0x77,
	VOUT_SELECT_5400_MV             = 0x78,
	VOUT_SELECT_5420_MV             = 0x79,
	VOUT_SELECT_5440_MV             = 0x7A,
	VOUT_SELECT_5460_MV             = 0x7B,
	VOUT_SELECT_5480_MV             = 0x7C,
	VOUT_SELECT_5500_MV             = 0x7D,
	VOUT_SELECT_5520_MV             = 0x7E,
	VOUT_SELECT_5540_MV             = 0x7F,
	VOUT_SELECT_5560_MV             = 0x80,
	VOUT_SELECT_5580_MV             = 0x81,
	VOUT_SELECT_5600_MV             = 0x82,
	VOUT_SELECT_5620_MV             = 0x83,
	VOUT_SELECT_5640_MV             = 0x84,
	VOUT_SELECT_5660_MV             = 0x85,
	VOUT_SELECT_5680_MV             = 0x86,
	VOUT_SELECT_5700_MV             = 0x87,
	VOUT_SELECT_5720_MV             = 0x88,
	VOUT_SELECT_5740_MV             = 0x89,
	VOUT_SELECT_5760_MV             = 0x8A,
	VOUT_SELECT_5780_MV             = 0x8B,
	VOUT_SELECT_5800_MV             = 0x8C,
	VOUT_SELECT_5820_MV             = 0x8D,
	VOUT_SELECT_5840_MV             = 0x8E,
	VOUT_SELECT_5860_MV             = 0x8F,
	VOUT_SELECT_5880_MV             = 0x90,
	VOUT_SELECT_5900_MV             = 0x91,

        /* 100mV range */
	VOUT_SELECT_6000_MV             = 0x92,
	VOUT_SELECT_6100_MV             = 0x93,
	VOUT_SELECT_6200_MV             = 0x94,
	VOUT_SELECT_6300_MV             = 0x95,
	VOUT_SELECT_6400_MV             = 0x96,
	VOUT_SELECT_6500_MV             = 0x97,
	VOUT_SELECT_6600_MV             = 0x98,
	VOUT_SELECT_6700_MV             = 0x99,
	VOUT_SELECT_6800_MV             = 0x9A,
	VOUT_SELECT_6900_MV             = 0x9B,
	VOUT_SELECT_7000_MV             = 0x9C,
	VOUT_SELECT_7100_MV             = 0x9D,
	VOUT_SELECT_7200_MV             = 0x9E,
	VOUT_SELECT_7300_MV             = 0x9F,
	VOUT_SELECT_7400_MV             = 0xA0,
	VOUT_SELECT_7500_MV             = 0xA1,
	VOUT_SELECT_7600_MV             = 0xA2,
	VOUT_SELECT_7700_MV             = 0xA3,
	VOUT_SELECT_7800_MV             = 0xA4,
	VOUT_SELECT_7900_MV             = 0xA5,
	VOUT_SELECT_8000_MV             = 0xA6,
	VOUT_SELECT_8100_MV             = 0xA7,
	VOUT_SELECT_8200_MV             = 0xA8,
	VOUT_SELECT_8300_MV             = 0xA9,
	VOUT_SELECT_8400_MV             = 0xAA,
	VOUT_SELECT_8500_MV             = 0xAB,
	VOUT_SELECT_8600_MV             = 0xAC,
	VOUT_SELECT_8700_MV             = 0xAD,
	VOUT_SELECT_8800_MV             = 0xAE,
	VOUT_SELECT_8900_MV             = 0xAF,
	VOUT_SELECT_9000_MV             = 0xB0,
	VOUT_SELECT_9100_MV             = 0xB1,
	VOUT_SELECT_9200_MV             = 0xB2,
	VOUT_SELECT_9300_MV             = 0xB3,
	VOUT_SELECT_9400_MV             = 0xB4,
	VOUT_SELECT_9500_MV             = 0xB5,
	VOUT_SELECT_9600_MV             = 0xB6,
	VOUT_SELECT_9700_MV             = 0xB7,
	VOUT_SELECT_9800_MV             = 0xB8,
	VOUT_SELECT_9900_MV             = 0xB9,
	VOUT_SELECT_10000_MV            = 0xBA,
	VOUT_SELECT_10100_MV            = 0xBB,
	VOUT_SELECT_10200_MV            = 0xBC,
	VOUT_SELECT_10300_MV            = 0xBD,
	VOUT_SELECT_10400_MV            = 0xBE,
	VOUT_SELECT_10500_MV            = 0xBF,
	VOUT_SELECT_10600_MV            = 0xC0,
	VOUT_SELECT_10700_MV            = 0xC1,
	VOUT_SELECT_10800_MV            = 0xC2,
	VOUT_SELECT_10900_MV            = 0xC3,
	VOUT_SELECT_11000_MV            = 0xC4,

        /* 200mV range */
	VOUT_SELECT_11200_MV            = 0xC5,
	VOUT_SELECT_11400_MV            = 0xC6,
	VOUT_SELECT_11600_MV            = 0xC7,
	VOUT_SELECT_11800_MV            = 0xC8,
	VOUT_SELECT_12000_MV            = 0xC9,
	VOUT_SELECT_12200_MV            = 0xCA,
	VOUT_SELECT_12400_MV            = 0xCB,
	VOUT_SELECT_12600_MV            = 0xCC,
	VOUT_SELECT_12800_MV            = 0xCD,
	VOUT_SELECT_13000_MV            = 0xCE,
	VOUT_SELECT_13200_MV            = 0xCF,
	VOUT_SELECT_13400_MV            = 0xD0,
	VOUT_SELECT_13600_MV            = 0xD1,
	VOUT_SELECT_13800_MV            = 0xD2,
	VOUT_SELECT_14000_MV            = 0xD3,
	VOUT_SELECT_14200_MV            = 0xD4,
	VOUT_SELECT_14400_MV            = 0xD5,
	VOUT_SELECT_14600_MV            = 0xD6,
	VOUT_SELECT_14800_MV            = 0xD7,
	VOUT_SELECT_15000_MV            = 0xD8,
	VOUT_SELECT_15200_MV            = 0xD9,
	VOUT_SELECT_15400_MV            = 0xDA,
	VOUT_SELECT_15600_MV            = 0xDB,
	VOUT_SELECT_15800_MV            = 0xDC,
	VOUT_SELECT_16000_MV            = 0xDD,
	VOUT_SELECT_16200_MV            = 0xDE,
	VOUT_SELECT_16400_MV            = 0xDF,
	VOUT_SELECT_16600_MV            = 0xE0,
	VOUT_SELECT_16800_MV            = 0xE1,
	VOUT_SELECT_17000_MV            = 0xE2,
	VOUT_SELECT_17200_MV            = 0xE3,
	VOUT_SELECT_17400_MV            = 0xE4,
	VOUT_SELECT_17600_MV            = 0xE5,
	VOUT_SELECT_17800_MV            = 0xE6,
	VOUT_SELECT_18000_MV            = 0xE7,
	VOUT_SELECT_18200_MV            = 0xE8,
	VOUT_SELECT_18400_MV            = 0xE9,
	VOUT_SELECT_18600_MV            = 0xEA,
	VOUT_SELECT_18800_MV            = 0xEB,
	VOUT_SELECT_19000_MV            = 0xEC,
	VOUT_SELECT_19200_MV            = 0xED,
	VOUT_SELECT_19400_MV            = 0xEE,
	VOUT_SELECT_19600_MV            = 0xEF,
	VOUT_SELECT_19800_MV            = 0xF0,
	VOUT_SELECT_20000_MV            = 0xF1,
        
        VOUT_Invalid,
} STPD01_VOUT_SELECT_t;


/**
  * @brief STPD01 ILIM_SELECT structures definition
  *        Address: 0x01 - Bit 4:0
  */
typedef enum
{
	ILIM_SELECT_100_MA              = 0x0,
	ILIM_SELECT_200_MA              = 0x1,
	ILIM_SELECT_300_MA              = 0x2,
	ILIM_SELECT_400_MA              = 0x3,
	ILIM_SELECT_500_MA              = 0x4,
	ILIM_SELECT_600_MA              = 0x5,
	ILIM_SELECT_700_MA              = 0x6,
	ILIM_SELECT_800_MA              = 0x7,
	ILIM_SELECT_900_MA              = 0x8,
	ILIM_SELECT_1000_MA             = 0x9,
	ILIM_SELECT_1100_MA             = 0xA,
	ILIM_SELECT_1200_MA             = 0xB,
	ILIM_SELECT_1300_MA             = 0xC,
	ILIM_SELECT_1400_MA             = 0xD,
	ILIM_SELECT_1500_MA             = 0xE,
	ILIM_SELECT_1600_MA             = 0xF,
	ILIM_SELECT_1700_MA             = 0x10,
	ILIM_SELECT_1800_MA             = 0x11,
	ILIM_SELECT_1900_MA             = 0x12,
	ILIM_SELECT_2000_MA             = 0x13,
	ILIM_SELECT_2100_MA             = 0x14,
	ILIM_SELECT_2200_MA             = 0x15,
	ILIM_SELECT_2300_MA             = 0x16,
	ILIM_SELECT_2400_MA             = 0x17,
	ILIM_SELECT_2500_MA             = 0x18,
	ILIM_SELECT_2600_MA             = 0x19,
	ILIM_SELECT_2700_MA             = 0x1A,
	ILIM_SELECT_2800_MA             = 0x1B,
	ILIM_SELECT_2900_MA             = 0x1C,
	ILIM_SELECT_3000_MA             = 0x1D,

        /* Special enum */
        ILIM_SELECT_NOLIMIT             = 0x1F,
        
} STPD01_ILIM_SELECT_t;

/**
  * @brief STPD01 SERVICE_SW_FRQ structures definition
  *        Address: 0x05 - Bit 2
  */
typedef enum
{
	SERVICE_SW_FRQ_500KHZ   	= 0,
	SERVICE_SW_FRQ_750KHZ   	= 1,
} STPD01_SERVICE_SW_FRQ_t;

/**
  * @brief STPD01 SERVICE_CDC structures definition
  *        Address: 0x05 - Bit 4:3
  */
typedef enum
{
	SERVICE_CDC_DISABLE   	        = 0x00,
	SERVICE_CDC_HALF	        = 0x01,
	SERVICE_CDC_FULL    	        = 0x02
} STPD01_SERVICE_CDC_t;

/**
  * @brief STPD01 SERVICE_WTCHDG_TIM structures definition
  *        Address: 0x05 - Bit 7:6
  */
typedef enum
{
	SERVICE_WTCHDG_TIM_100MS	= 0x00,
	SERVICE_WTCHDG_TIM_1S    	= 0x01,
	SERVICE_WTCHDG_TIM_10S    	= 0x02,
	SERVICE_WTCHDG_TIM_100S         = 0x03,
} STPD01_SERVICE_WTCHDG_TIM_t;

/**
  * @brief STPD01 FUNCTIONALSTATE structures definition
  */
typedef enum
{
	STPD01_DISABLE			=0X00,
	STPD01_ENABLE			=0X01,
} STPD01_FUNCTIONALSTATE_t;
/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Types_RegStruct
  */

/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Types
  */


/* Exported define -----------------------------------------------------------*/
/** @defgroup BSP_Components_STPD01_RegisterLayer_Exported_Defines Exported Defines
  * @{
  */
/* Common Error codes */
#define STPD01_STATUS_OK 			( 1 )    /*!< Driver OK               */
#define STPD01_ERROR_GENERIC 		        (-1 )    /*!< Driver Generic Error    */
#define STPD01_ERROR_BUSY		        (-2 )    /*!< Driver Busy Error       */
#define STPD01_ERROR_TIMEOUT 		        (-3 )    /*!< Driver Timeout Error    */

/* Register Address */
#define STPD01_VOUT_SELECT	  		(0x00)   /*!< Reg 0 Vout selection    */
#define STPD01_ILIM_SELECT	 		(0x01)   /*!< Reg 1 Ilim selection    */
#define STPD01_INT_STATUS	  		(0x02)   /*!< Reg 2 Int status        */
#define STPD01_INT_LATCH	  		(0x03)   /*!< Reg 3 Int latched       */
#define STPD01_INT_MASK				(0x04)   /*!< Reg 4 Int masked        */
#define STPD01_SERVICE		  		(0x05)   /*!< Reg 5 Service           */
#define STPD01_DIG_ENABLE		  	(0x06)   /*!< Reg 6 Enabling          */
/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Defines
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup BSP_Components_STPD01_RegisterLayer_Exported_Functions Exported Functions
  * @{
  */
int32_t STPD01_WriteReg( STPD01_ctx_t *ctx, uint8_t reg, uint8_t *data, uint16_t len );
int32_t STPD01_ReadReg( STPD01_ctx_t *ctx, uint8_t reg, uint8_t *data, uint16_t len );


/** @defgroup BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg0 Reg0
  * @{
  */
/* 0x00*/
int32_t STPD01_Vout_Select_Get(STPD01_ctx_t *ctx, STPD01_VOUT_SELECT_t *val);
int32_t STPD01_Vout_Select_Set(STPD01_ctx_t *ctx, STPD01_VOUT_SELECT_t *val);
/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg0
  */

/** @defgroup BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg1 Reg1
  * @{
  */
/*0x01*/
int32_t STPD01_Ilim_Select_Get(STPD01_ctx_t *ctx, STPD01_ILIM_SELECT_t *val);
int32_t STPD01_Ilim_Select_Set(STPD01_ctx_t *ctx, STPD01_ILIM_SELECT_t *val);
/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg1
  */

/** @defgroup BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg2 Reg2
  * @{
  */
/*0x02*/
int32_t STPD01_INT_STATUS_Get(STPD01_ctx_t *ctx, STPD01_REGISTER_t *val);

int32_t STPD01_INT_STATUS_OVP_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_STATUS_OCP_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_STATUS_SHORT_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_STATUS_PGOOD_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_STATUS_WD_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_STATUS_OTP_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_STATUS_OTW_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_STATUS_IPK_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg2
  */

/** @defgroup BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg3 Reg3
  * @{
  */
/*0x03*/
int32_t STPD01_INT_LATCH_Get(STPD01_ctx_t *ctx, STPD01_REGISTER_t *val);

int32_t STPD01_INT_LATCH_OVP_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_LATCH_OCP_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_LATCH_SHORT_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_LATCH_PGOOD_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_LATCH_WD_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_LATCH_OTP_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_LATCH_OTW_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_LATCH_IPK_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg3
  */

/** @defgroup BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg4 Reg4
  * @{
  */
/*0x04*/
int32_t STPD01_INT_MASK_Get(STPD01_ctx_t *ctx, STPD01_REGISTER_t *val);
int32_t STPD01_INT_MASK_Set(STPD01_ctx_t *ctx, STPD01_REGISTER_t *val);

int32_t STPD01_INT_MASK_OVP_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_MASK_OCP_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_MASK_SHORT_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_MASK_PGOOD_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_MASK_WD_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_MASK_OTP_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_MASK_OTW_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_MASK_IPK_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);

int32_t STPD01_INT_MASK_OVP_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_MASK_OCP_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_MASK_SHORT_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_MASK_PGOOD_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_MASK_WD_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_MASK_OTP_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_MASK_OTW_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_INT_MASK_IPK_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg4
  */

/** @defgroup BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg5 Reg5
  * @{
  */
/*0x05*/
int32_t STPD01_Service_Auto_Dis_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_Service_Auto_Dis_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);

int32_t STPD01_Service_Dith_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_Service_Dith_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);

int32_t STPD01_Service_SW_Frq_Get(STPD01_ctx_t *ctx, STPD01_SERVICE_SW_FRQ_t *val);
int32_t STPD01_Service_SW_Frq_Set(STPD01_ctx_t *ctx, STPD01_SERVICE_SW_FRQ_t *val);

int32_t STPD01_Service_CDC_Get(STPD01_ctx_t *ctx, STPD01_SERVICE_CDC_t *val);
int32_t STPD01_Service_CDC_Set(STPD01_ctx_t *ctx, STPD01_SERVICE_CDC_t *val);

int32_t STPD01_Service_Wtchg_Tim_Get(STPD01_ctx_t *ctx, STPD01_SERVICE_WTCHDG_TIM_t *val);
int32_t STPD01_Service_Wtchg_Tim_Set(STPD01_ctx_t *ctx, STPD01_SERVICE_WTCHDG_TIM_t *val);
/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg5
  */

/** @defgroup BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg6 Reg6
  * @{
  */
/*0x06*/
int32_t STPD01_Enable_Dig_En_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_Enable_Dig_En_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);

int32_t STPD01_Enable_WD_Get(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
int32_t STPD01_Enable_WD_Set(STPD01_ctx_t *ctx, STPD01_FUNCTIONALSTATE_t *val);
/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Functions_Reg6
  */
/**
  * @} BSP_Components_STPD01_RegisterLayer_Exported_Functions
  */

/**
  * @} BSP_Components_STPD01_RegisterLayer
  */

/**
  * @} BSP_Components_STPD01
  */

#ifdef __cplusplus
}
#endif

#endif /* STPD01_REG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/ 
