/**
  ******************************************************************************
  * @file    steval_2stpd01_errno.h
  * @author  MCD Application Team and SRA.SLDU.SC Team
  * @date    1-May-2021
  * @version 1.0.0
  * @brief   Error Code definition file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#ifndef STEVAL_2STPD01_ERRNO_H
#define STEVAL_2STPD01_ERRNO_H

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup BSP_STEVAL_2STPD01
  * @{
  */

/** @defgroup BSP_STEVAL_2STPD01_ERRNO Error definition
  * @{
  */
  
/* Common Error codes */
#define BSP_ERROR_NONE                          0
#define BSP_ERROR_NO_INIT                      -1
#define BSP_ERROR_ALREADY_INIT                 -2
#define BSP_ERROR_WRONG_PARAM                  -3
#define BSP_ERROR_BUSY                         -4
#define BSP_ERROR_PERIPH_FAILURE               -5
#define BSP_ERROR_COMPONENT_FAILURE            -6
#define BSP_ERROR_UNKNOWN_FAILURE              -7
#define BSP_ERROR_UNKNOWN_COMPONENT            -8
#define BSP_ERROR_WRONG_COMPONENT              -9
#define BSP_ERROR_BUS_FAILURE                 -10
#define BSP_ERROR_CLOCK_FAILURE               -11
#define BSP_ERROR_MSP_FAILURE                 -12
#define BSP_ERROR_FEATURE_NOT_SUPPORTED       -13
#define BSP_ERROR_TCPP02_COMPONENT_FAILURE    -14
#define BSP_ERROR_STPD01_COMPONENT_FAILURE    -15
#define BSP_ERROR_BOTH_COMPONENT_FAILURE      -16
#define BSP_ERROR_VBUS_NOT_REACHED            -17

/* BSP BUS error codes */
#define BSP_ERROR_BUS_TRANSACTION_FAILURE    -100
#define BSP_ERROR_BUS_ARBITRATION_LOSS       -101
#define BSP_ERROR_BUS_ACKNOWLEDGE_FAILURE    -102
#define BSP_ERROR_BUS_PROTOCOL_FAILURE       -103

/**
  * @} BSP_STEVAL_2STPD01_ERRNO
  */

/**
  * @} BSP_STEVAL_2STPD01
  */
  
#endif /* STEVAL_2STPD01_ERRNO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
