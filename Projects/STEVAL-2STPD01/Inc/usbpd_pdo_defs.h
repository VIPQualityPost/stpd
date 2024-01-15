/**
  ******************************************************************************
  * @file    usbpd_pdo_defs.h
  * @version 1.0.1
  * @date    2021-11-08
  * @author  MCD Application Team and SRA.SLDU.SC Team
  * @brief   Header file for definition of PDO/APDO values for 2 ports(SRC/SRC) 
  *          configuration
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

#ifndef __USBPD_PDO_DEF_H_
#define __USBPD_PDO_DEF_H_

#ifdef __cplusplus
 extern "C" {
#endif 

/** @addtogroup STM32_USBPD_APPLICATION
  * @{
  */

/** @defgroup STM32_USBPD_APPLICATION_PDO PDO Definition
  * @{
  */
   
/* Includes ------------------------------------------------------------------*/
#if defined(_GUI_INTERFACE)
#include "usbpd_gui_memmap.h"
#endif /* _GUI_INTERFACE */

/* Define   ------------------------------------------------------------------*/
/* Exported typedef ----------------------------------------------------------*/
/** @defgroup STM32_USBPD_APPLICATION_PDO_Typedef Typedef
  * @brief Typedef to support the PDO definition
  * @{
  */
/**
  * @brief  USBPD Port PDO Structure definition
  */
typedef struct
{
  uint32_t *ListOfPDO;                          /*!< Pointer on Power Data Objects list, defining
                                                     port capabilities */
  uint8_t  *NumberOfPDO;                        /*!< Number of Power Data Objects defined in ListOfPDO
                                                     This parameter must be set at max to USBPD_MAX_NB_PDO value */
} USBPD_PortPDO_TypeDef;

/**
  * @brief  USBPD Port PDO Storage Structure definition
  */
typedef struct
{
  USBPD_PortPDO_TypeDef    SourcePDO;        /*!< SRC Power Data Objects */
  USBPD_PortPDO_TypeDef    SinkPDO;          /*!< SNK Power Data Objects */
}USBPD_PWR_Port_PDO_Storage_TypeDef;
/**
  * @} STM32_USBPD_APPLICATION_PDO_Typedef
  */

/* Exported define -----------------------------------------------------------*/
/** @defgroup STM32_USBPD_APPLICATION_PDO_Defines Defines
  * @brief PDO Defines and configuration
  * @{
  */
#define USBPD_PDP_SRC_IN_WATTS                  60      /*!< SRC PD Power in Watts */

/* Definitions of nb of PDO and APDO for each port */
#define USBPD_CORE_PDO_SRC_FIXED_MAX_CURRENT    3       /*!< Max current (A)       */

/* Definitions of nb of PDO and APDO for each port */
#define PORT0_NB_SOURCEPDO         4   /*!< Number of Source PDOs (applicable for port 0)   */
#define PORT0_NB_SINKPDO           0   /*!< Number of Sink PDOs (applicable for port 0)     */
#define PORT1_NB_SOURCEPDO         4   /*!< Number of Source PDOs (applicable for port 1)   */
#define PORT1_NB_SINKPDO           0   /*!< Number of Sink PDOs (applicable for port 1)     */

#define PORT0_NB_SOURCEAPDO        0   /*!< Number of Source APDOs (applicable for port 0)  */
#define PORT0_NB_SINKAPDO          0   /*!< Number of Sink APDOs (applicable for port 0)    */
#define PORT1_NB_SOURCEAPDO        0   /*!< Number of Source APDOs (applicable for port 1)  */
#define PORT1_NB_SINKAPDO          0   /*!< Number of Sink APDOs (applicable for port 1)    */
/**
  * @} STM32_USBPD_APPLICATION_PDO_Defines
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/** @defgroup STM32_USBPD_APPLICATION_PDO_Variables Variables
  * @brief PDO variable and configuration
  * @{
  */
#if !defined(__USBPD_PWR_IF_C) && !defined(__NO_DOXY)
extern uint8_t USBPD_NbPDO[4];
extern uint32_t PORT0_PDO_ListSRC[USBPD_MAX_NB_PDO];
#if  (USBPD_PORT_COUNT == 2)
extern uint32_t PORT1_PDO_ListSRC[USBPD_MAX_NB_PDO];
#endif /* USBPD_PORT_COUNT == 2 */
#else
/**
  * @brief Number of PDO
  */
uint8_t USBPD_NbPDO[4] = {(PORT0_NB_SINKPDO + PORT0_NB_SINKAPDO), 
                          ((PORT0_NB_SOURCEPDO + PORT0_NB_SOURCEAPDO)),
                          ((PORT1_NB_SINKPDO + PORT1_NB_SINKAPDO)),
                          ((PORT1_NB_SOURCEPDO + PORT1_NB_SOURCEAPDO))};

/**
  * @brief Definition of Source PDO for Port 0
  */
uint32_t PORT0_PDO_ListSRC[USBPD_MAX_NB_PDO] =
{
  /* PDO 1 */
        ( ((PWR_A_10MA(USBPD_CORE_PDO_SRC_FIXED_MAX_CURRENT)) << USBPD_PDO_SRC_FIXED_MAX_CURRENT_Pos) |
          ((PWR_V_50MV(5)) << USBPD_PDO_SRC_FIXED_VOLTAGE_Pos)                                        |
          USBPD_PDO_SRC_FIXED_PEAKCURRENT_EQUAL                                                       |
#if defined(_UNCHUNKED_SUPPORT)
          USBPD_PDO_SRC_FIXED_UNCHUNK_SUPPORTED                                                       |
#else
          USBPD_PDO_SRC_FIXED_UNCHUNK_NOT_SUPPORTED                                                   |
#endif /* _UNCHUNKED_SUPPORT */
          USBPD_PDO_SRC_FIXED_DRD_NOT_SUPPORTED                                                       |
          USBPD_PDO_SRC_FIXED_USBCOMM_NOT_SUPPORTED                                                   |
          USBPD_PDO_SRC_FIXED_EXT_POWER_AVAILABLE                                                     |
          USBPD_PDO_SRC_FIXED_USBSUSPEND_NOT_SUPPORTED                                                |
          USBPD_PDO_SRC_FIXED_DRP_NOT_SUPPORTED                                                       |
          USBPD_PDO_TYPE_FIXED
        ),
  /* PDO 2 */
        ( ((PWR_A_10MA(USBPD_CORE_PDO_SRC_FIXED_MAX_CURRENT)) << USBPD_PDO_SRC_FIXED_MAX_CURRENT_Pos) |
          ((PWR_V_50MV(9)) << USBPD_PDO_SRC_FIXED_VOLTAGE_Pos)                                        |
          USBPD_PDO_SRC_FIXED_PEAKCURRENT_EQUAL                                                       |
          USBPD_PDO_SRC_FIXED_DRD_NOT_SUPPORTED                                                       |
          USBPD_PDO_SRC_FIXED_USBCOMM_NOT_SUPPORTED                                                   |
          USBPD_PDO_SRC_FIXED_EXT_POWER_NOT_AVAILABLE                                                 |
          USBPD_PDO_SRC_FIXED_USBSUSPEND_NOT_SUPPORTED                                                |
          USBPD_PDO_SRC_FIXED_DRP_NOT_SUPPORTED                                                       |
          USBPD_PDO_TYPE_FIXED
        ),
  /* PDO 3 */
        ( ((PWR_A_10MA(USBPD_CORE_PDO_SRC_FIXED_MAX_CURRENT)) << USBPD_PDO_SRC_FIXED_MAX_CURRENT_Pos) |
          ((PWR_V_50MV(15)) << USBPD_PDO_SRC_FIXED_VOLTAGE_Pos)                                       |
          USBPD_PDO_SRC_FIXED_PEAKCURRENT_EQUAL                                                       |
          USBPD_PDO_SRC_FIXED_DRD_NOT_SUPPORTED                                                       |
          USBPD_PDO_SRC_FIXED_USBCOMM_NOT_SUPPORTED                                                   |
          USBPD_PDO_SRC_FIXED_EXT_POWER_NOT_AVAILABLE                                                 |
          USBPD_PDO_SRC_FIXED_USBSUSPEND_NOT_SUPPORTED                                                |
          USBPD_PDO_SRC_FIXED_DRP_NOT_SUPPORTED                                                       |
          USBPD_PDO_TYPE_FIXED
        ),
#if PORT0_NB_SOURCEPDO > 3
  /* PDO 4 */
        ( ((PWR_A_10MA(USBPD_CORE_PDO_SRC_FIXED_MAX_CURRENT)) << USBPD_PDO_SRC_FIXED_MAX_CURRENT_Pos) |
          ((PWR_V_50MV(20)) << USBPD_PDO_SRC_FIXED_VOLTAGE_Pos)                                       |
          USBPD_PDO_SRC_FIXED_PEAKCURRENT_EQUAL                                                       |
          USBPD_PDO_SRC_FIXED_DRD_NOT_SUPPORTED                                                       |
          USBPD_PDO_SRC_FIXED_USBCOMM_NOT_SUPPORTED                                                   |
          USBPD_PDO_SRC_FIXED_EXT_POWER_NOT_AVAILABLE                                                 |
          USBPD_PDO_SRC_FIXED_USBSUSPEND_NOT_SUPPORTED                                                |
          USBPD_PDO_SRC_FIXED_DRP_NOT_SUPPORTED                                                       |
          USBPD_PDO_TYPE_FIXED
        ),
#else
  /* PDO 4 */ (0x00000000U),
#endif /* PORT0_NB_SOURCEPDO > 3 */
  /* PDO 5 */ (0x00000000U),
  /* PDO 6 */ (0x00000000U),
  /* PDO 7 */ (0x00000000U)

};

#if  (USBPD_PORT_COUNT == 2)
/**
  * @brief Definition of Source PDO for Port 1
  */
uint32_t PORT1_PDO_ListSRC[USBPD_MAX_NB_PDO] = 
{
  /* PDO 1 */
        ( ((PWR_A_10MA(USBPD_CORE_PDO_SRC_FIXED_MAX_CURRENT)) << USBPD_PDO_SRC_FIXED_MAX_CURRENT_Pos) |
          ((PWR_V_50MV(5)) << USBPD_PDO_SRC_FIXED_VOLTAGE_Pos)                                        |
          USBPD_PDO_SRC_FIXED_PEAKCURRENT_EQUAL                                                       |
#if defined(_UNCHUNKED_SUPPORT)
          USBPD_PDO_SRC_FIXED_UNCHUNK_SUPPORTED                                                       |
#else
          USBPD_PDO_SRC_FIXED_UNCHUNK_NOT_SUPPORTED                                                   |
#endif /* _UNCHUNKED_SUPPORT */
          USBPD_PDO_SRC_FIXED_DRD_NOT_SUPPORTED                                                       |
          USBPD_PDO_SRC_FIXED_USBCOMM_NOT_SUPPORTED                                                   |
          USBPD_PDO_SRC_FIXED_EXT_POWER_AVAILABLE                                                     |
          USBPD_PDO_SRC_FIXED_USBSUSPEND_NOT_SUPPORTED                                                |
          USBPD_PDO_SRC_FIXED_DRP_NOT_SUPPORTED                                                       |
          USBPD_PDO_TYPE_FIXED
        ),
        
  /* PDO 2 */
        ( ((PWR_A_10MA(USBPD_CORE_PDO_SRC_FIXED_MAX_CURRENT)) << USBPD_PDO_SRC_FIXED_MAX_CURRENT_Pos) |
          ((PWR_V_50MV(9)) << USBPD_PDO_SRC_FIXED_VOLTAGE_Pos)                                        |
          USBPD_PDO_SRC_FIXED_PEAKCURRENT_EQUAL                                                       |
          USBPD_PDO_SRC_FIXED_DRD_NOT_SUPPORTED                                                       |
          USBPD_PDO_SRC_FIXED_USBCOMM_NOT_SUPPORTED                                                   |
          USBPD_PDO_SRC_FIXED_EXT_POWER_NOT_AVAILABLE                                                 |
          USBPD_PDO_SRC_FIXED_USBSUSPEND_NOT_SUPPORTED                                                |
          USBPD_PDO_SRC_FIXED_DRP_NOT_SUPPORTED                                                       |
          USBPD_PDO_TYPE_FIXED
        ),
  /* PDO 3 */
        ( ((PWR_A_10MA(USBPD_CORE_PDO_SRC_FIXED_MAX_CURRENT)) << USBPD_PDO_SRC_FIXED_MAX_CURRENT_Pos) |
          ((PWR_V_50MV(15)) << USBPD_PDO_SRC_FIXED_VOLTAGE_Pos)                                       |
          USBPD_PDO_SRC_FIXED_PEAKCURRENT_EQUAL                                                       |
          USBPD_PDO_SRC_FIXED_DRD_NOT_SUPPORTED                                                       |
          USBPD_PDO_SRC_FIXED_USBCOMM_NOT_SUPPORTED                                                   |
          USBPD_PDO_SRC_FIXED_EXT_POWER_NOT_AVAILABLE                                                 |
          USBPD_PDO_SRC_FIXED_USBSUSPEND_NOT_SUPPORTED                                                |
          USBPD_PDO_SRC_FIXED_DRP_NOT_SUPPORTED                                                       |
          USBPD_PDO_TYPE_FIXED
        ),
#if PORT1_NB_SOURCEPDO > 3
  /* PDO 4 */
        ( ((PWR_A_10MA(USBPD_CORE_PDO_SRC_FIXED_MAX_CURRENT)) << USBPD_PDO_SRC_FIXED_MAX_CURRENT_Pos) |
          ((PWR_V_50MV(20)) << USBPD_PDO_SRC_FIXED_VOLTAGE_Pos)                                       |
          USBPD_PDO_SRC_FIXED_PEAKCURRENT_EQUAL                                                       |
          USBPD_PDO_SRC_FIXED_DRD_NOT_SUPPORTED                                                       |
          USBPD_PDO_SRC_FIXED_USBCOMM_NOT_SUPPORTED                                                   |
          USBPD_PDO_SRC_FIXED_EXT_POWER_NOT_AVAILABLE                                                 |
          USBPD_PDO_SRC_FIXED_USBSUSPEND_NOT_SUPPORTED                                                |
          USBPD_PDO_SRC_FIXED_DRP_NOT_SUPPORTED                                                       |
          USBPD_PDO_TYPE_FIXED
        ),
#else
  /* PDO 4 */ (0x00000000U),
#endif /* PORT0_NB_SOURCEPDO > 3 */


  /* PDO 5 */ (0x00000000U),
  /* PDO 6 */ (0x00000000U),
  /* PDO 7 */ (0x00000000U)
};

#endif /*USBPD_PORT_COUNT == 2*/

#endif /* __USBPD_PWR_IF_C */
/**
  * @} STM32_USBPD_APPLICATION_PDO_Variables
  */

/**
  * @} STM32_USBPD_APPLICATION_PDO
  */

/**
  * @} STM32_USBPD_APPLICATION
  */

#ifdef __cplusplus
}
#endif

#endif /* __USBPD_PDO_DEF_H_ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
