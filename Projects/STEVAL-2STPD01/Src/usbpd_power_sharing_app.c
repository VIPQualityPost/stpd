/**
  ******************************************************************************
  * @file    usbpd_power_sharing_app.c
  * @version 1.0.1
  * @date    2021-11-08
  * @author  SRA.SLDU.SC Team
  * @brief   File containing Power Sharing Callback implementation.
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

/** @addtogroup USBPD_POWER_SHARING_APP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
#include "usbpd_power_sharing_app.h"
#include "usbpd_trace.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/** @defgroup USBPD_POWER_SHARING_APP_Private_Defines Private Defines
  * @{
  */
#define PARSER_MAX_CHAR           20
#define PARSER_MIN_CHAR           3
#define PARSER_DELIMITER_STR      " "
#define PARSER_DELIMITER          ' '
#define PARSER_ASSIGNER           '='
#define PARSER_CHECK_DELIMITER(c) ((c)==PARSER_DELIMITER || (c)==',' || (c)==';')
#define PARSER_CHECK_ASSIGNER(c)  ((c)==PARSER_ASSIGNER || (c)==':')
#define PARSER_CHECK_COMMAND(c)   ((c)=='V' || (c)=='C' || (c)=='I' || (c)=='P')

#define TRACE_TEXT_MAX            100
#define TRACE_MAP_V               0x01
#define TRACE_MAP_C               0x02
#define TRACE_MAP_P               0x04
/** 
 * @} USBPD_POWER_SHARING_APP_Private_Defines
 */

/** @defgroup USBPD_POWER_SHARING_APP_Private_Variables Private Variables
  * @brief Power Sharing callback handlers implementation
  * @{
  */
/** @brief Init the module
  */
PS_Config_Typedef PS_Config = {
  .MinCurrent     = PS_CONF_MIN_CURRENT,
  .PowerNoLoad    = PS_CONF_POWER_NO_LOAD,
  .PCoefficient   = PS_CONF_PCOEFFICIENT,
  .VCoefficient   = PS_CONF_VCOEFFICIENT,
  .MaxPortVoltage = PS_CONF_MAX_PORT_VOLTAGE,
  .MaxPortCurrent = PS_CONF_MAX_PORT_CURRENT,
  .MinPortPower   = PS_CONF_MIN_PORT_POWER,
  .MaxPortPower   = PS_CONF_MAX_PORT_POWER,
  .PowerSupplyDef = {.Voltage = PS_CONF_PWRSUPPLY_VBUS, .Current = PS_CONF_PWRSUPPLY_IBUS},
};
/** 
 * @} USBPD_POWER_SHARING_APP_Private_Variables
 */

/** @defgroup USBPD_POWER_SHARING_APP_Private_Functions Private functions
  * @{
  */
uint8_t PS_App_Data_Check(PS_Data_Typedef *pData);
PS_Status_TypeDef PS_App_FlashRead(PS_Data_Typedef *pData);
PS_Status_TypeDef PS_App_FlashWrite(PS_Data_Typedef *pData);
PS_Status_TypeDef PS_App_ParserParam(PS_Data_Typedef *pData, char const * Text, uint8_t Len);
/** 
 * @} USBPD_POWER_SHARING_APP_Private_Functions
 */

/** @defgroup USBPD_POWER_SHARING_APP_Private_Functions_Callbacks_Handlers Callback Handlers
  * @{
  */
/* Handlers */
PS_Status_TypeDef PS_App_EnablePort_Handler(uint8_t PortNum, uint8_t Enable);
PS_Status_TypeDef PS_App_SendSourceCapa_Handler(uint8_t PortNum);
/** 
 * @} USBPD_POWER_SHARING_APP_Private_Functions_Callbacks_Handlers
 */


/** @addtogroup USBPD_POWER_SHARING_APP_Exported_Functions
  * @{
  */
/** @brief Init the module
  * @retval status condition error
  */
PS_Status_TypeDef PS_App_Init(void)
{
  PS_Data_Typedef power_supply_data = {0};
  
  PS_App_FlashRead(&power_supply_data);
  if (!PS_App_Data_Check(&power_supply_data))
  {
    power_supply_data = PS_Config.PowerSupplyDef; /* performing a copy */
    PS_App_FlashWrite(&power_supply_data);  
  }

  PS_Status_TypeDef ret = PS_App_FlashWrite(&power_supply_data);  
  if (ret != PS_STATUS_OK)
  {
    Feedback_Handler(0, 20, 100, 2);
  }
    
  PS_Callback_Typedef const callbacks = {
    .PS_EnablePort = PS_App_EnablePort_Handler,
    .PS_SendSourceCapa = PS_App_SendSourceCapa_Handler,
  };
  
  return PS_Init(&power_supply_data, &callbacks);
}


/** @brief Acquire parameters from a formatted string
  * @param Text string with parameters
  * @param Len  valid string length
  * @retval status condition error
  */
PS_Status_TypeDef PS_App_ParametersFromString(char const * Text, uint8_t Len)
{
  PS_Status_TypeDef res = PS_STATUS_NA;
  PS_Data_Typedef power_supply_data = {0};
  
  PS_App_FlashRead(&power_supply_data);
  res = PS_App_ParserParam(&power_supply_data, Text, Len);
  
  if (PS_STATUS_OK == res)
  {
    PS_App_FlashWrite(&power_supply_data);
  
    PS_PowerSupply(&power_supply_data);
  
    /* Send a HR to both ports */
    USBPD_DPM_RequestHardReset(USBPD_PORT_0);
    USBPD_DPM_RequestHardReset(USBPD_PORT_1);
  }

  return PS_STATUS_OK;
}

/** 
  * @} USBPD_POWER_SHARING_APP_Exported_Functions
  */

/** @addtogroup USBPD_POWER_SHARING_APP_Private_Functions Private functions
  * @{
  */
/** @brief Check data
  * @param pData data structure 
  * @retval return condition
  */
uint8_t PS_App_Data_Check(PS_Data_Typedef *pData)
{
  assert_param(pData);
  uint32_t power = pData ? (pData->Voltage * pData->Current) / 1000 : 0; /* mW */

  return pData && 
        (pData->Current >= PS_APP_MIN_CURRENT && pData->Current <= PS_APP_MAX_CURRENT) &&   /* 1A <= C <= 10A     */
        (pData->Voltage >= PS_APP_MIN_VOTAGE  && pData->Voltage <= PS_APP_MAX_VOTAGE)  &&   /* 8.1V <= V <= 24V     */
        (power >= PS_APP_MIN_POWER);                                                        /* minimum 18.5W      */
}

/** @brief Read Power Supply parameters from flash
  * @param pData data structure 
  * @retval Power sharing status
  */
PS_Status_TypeDef PS_App_FlashRead(PS_Data_Typedef *pData)
{
  assert_param(pData);
    
  if (!pData)
  {
    return PS_STATUS_ERR;
  }

  /* Read Value from flash performing a copy */
  memcpy(pData, (void *)PS_POWER_SUPPLY_DATA_ADDRESS, sizeof(PS_Data_Typedef));
  
  return PS_STATUS_OK;
}

/** @brief Write Power Supply parameters from flash
  * @param pData data structure 
  * @retval Power sharing status
  */
PS_Status_TypeDef PS_App_FlashWrite(PS_Data_Typedef *pData)
{
  assert_param(pData);
  if (!pData)
  {
    return PS_STATUS_ERR;
  }
  FLASH_EraseInitTypeDef EraseInitStruct = {0};
  uint32_t ErasingPage = 62;
  uint32_t ErasingNbOfPages = 1;
  uint32_t PageError = 0;

    /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.Page        = ErasingPage;
  EraseInitStruct.NbPages     = ErasingNbOfPages;

  uint64_t *pFlashVar = (uint64_t *)pData;  
  HAL_StatusTypeDef ret;

  HAL_FLASH_Unlock();
  
  /* Erase the Block */
  HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
  
  /* Programming the data */
  ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, PS_POWER_SUPPLY_DATA_ADDRESS, *pFlashVar);
  
  HAL_FLASH_Lock();
  
  return ret == HAL_OK ? PS_STATUS_OK : PS_STATUS_ERR;
}

/** @brief Parser of string ang caught the parameters
  * @param pData output data structure 
  * @param Text string for parser 
  * @param Len lenght of the string
  * @retval Power sharing status
  */
PS_Status_TypeDef PS_App_ParserParam(PS_Data_Typedef *pData, char const * Text, uint8_t Len)
{
  /* 
  * Perform a parser of the string, valid format of each token [V|I|C|P][=|:]<value>[<space>|;|,|.|-];
  */
  assert_param(pData);
  assert_param(Text);
  assert_param(Len > 0);
  
  if (!pData || !Text || Len == 0)
  {
    return PS_STATUS_ERR;
  }
  
#if defined(_TRACE)
  /* used to prepare the string variable to send to the trace */
  static char text[TRACE_TEXT_MAX] = {0};
#endif
  
  uint8_t invalid_token = 0;
  char str[PARSER_MAX_CHAR] = {0};
  uint32_t value = 0;
  uint32_t P,V,C, Vadc;
  uint8_t map = 0;
  uint8_t check = 1;
  PS_Status_TypeDef ret = PS_STATUS_NA;
  
  /* Initialize with the external rounded value read from ADC */
  Vadc = PS_MATH_ROUND(BSPX_ADC_GetConvertedValue(BSP_ADC_VIN), 100);
  
  /* copy the received string */
  strncpy(str, Text, MIN(PARSER_MAX_CHAR - 1, Len));
  
  if (str[0] != '?' && str[0] != '!' && str[0] != 'i'  && str[0] != 'I')
  {
    
    /* convert to default char (delimiter and assigner) */
    for(int i=0; i<PARSER_MAX_CHAR; i++)
    {
      if (str[i] == 0)
      {
        /* the string is completed */
        break;
      }
      
      /* if found a delimiter char */
      if (PARSER_CHECK_DELIMITER(str[i]))
      {
        str[i]=PARSER_DELIMITER;
        continue;
      }
      
      /* if found a assigner char */
      if (PARSER_CHECK_ASSIGNER(str[i]))
      {
        str[i]=PARSER_ASSIGNER;
        continue;
      }
    }
    
#if _TRACE
    strcpy(text, "[Param] parser string: ");
    strcat(text, str);
    USBPD_TRACE_Add(USBPD_TRACE_DEBUG, 0xFF, 0, (uint8_t *)text, strlen(text));
#endif /* _TRACE */
    
    
    
    /* Initialize with the current default value */
    V = pData->Voltage;
    C = pData->Current;
    P = V * C / 1000;
    
    /* get the parameters from the received string */
    
    // Returns first token
    char * token = strtok(str, PARSER_DELIMITER_STR);
    
    /* iteration */
    while (token != NULL) 
    {
      /* get the param */
      
      invalid_token = 0;
      
      /* capitalize the first char */
      token[0] &= 0xDF; /* 1101 1111 */
      
      if (strlen(token) == 0)
      {
        continue;
      }
      
      /* check the size of token */
      if (strlen(token) < PARSER_MIN_CHAR)
      {
        invalid_token |= 0x01; /* not enough bytes */
      }
      
      /* check the type of var */
      if (!PARSER_CHECK_COMMAND(token[0]))
      {
        invalid_token |= 0x02; /* unknown command */
      }
      
      /* check equal */
      if (token[1] != PARSER_ASSIGNER)
      {
        invalid_token |= 0x04;
      }
      
      if (invalid_token == 0)
      {
        /* the string is valid */
        value = (uint32_t)PS_MATH_CEIL(((float)atof(&token[2]) * 1000.0), 100);
        switch (token[0])
        {
        case 'V':
          /* set the Power Supply Voltage */
          V = value; /* mV */
          map |= TRACE_MAP_V;
          break;
        case 'C':
        case 'I':
          /* set the Power Supply Current */
          C = (int32_t)value; /* mA */
          map |= TRACE_MAP_C;
          break;
        case 'P':
          P = value; /* mW */
          map |= TRACE_MAP_P;
          break;
        default:
          break;
        }
#if _TRACE
        //sprintf_s(text, TRACE_TEXT_MAX, "[Param] %c=%d", token[0], value);
        sprintf(text, "[Param] %c=%d", token[0], (uint32_t)(value));
        USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PS_PORT_NA, 0, (uint8_t *)text, strlen(text));
#endif /* _TRACE */
        
      }
      else
      {
        /* invalid token discard */
#if _TRACE
        strcpy(text, "[Param] invalid token '");
        strcat(text, token);
        strcat(text, "'");
        USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PS_PORT_NA, 0, (uint8_t *)text, strlen(text));
#endif /* _TRACE */
      }
      token = strtok(NULL, PARSER_DELIMITER_STR);
    }
    
    switch(map & 0x07)
    {
    case TRACE_MAP_V:
    case TRACE_MAP_V | TRACE_MAP_C:
    case TRACE_MAP_V | TRACE_MAP_C | TRACE_MAP_P:
      /* nothing : use V and C */
      break;
    case TRACE_MAP_C:
      V = Vadc;
      break;
    case TRACE_MAP_P:
      /* in case of only power consider the V read from ADC */
      V = Vadc; /* set V to the read value from adc */
      C = (P * 1000) / V;
      break;
    case TRACE_MAP_V | TRACE_MAP_P:
      /* use the new V value */
      C = (P * 1000) / V;
      break;
    case TRACE_MAP_C | TRACE_MAP_P:
      V = (P * 1000) / C;
      break;
    default:
      /* nothing to be assigned or calculated */
      break;
    }
    
    pData->Voltage = PS_MATH_ROUND(V, 100);
    pData->Current = PS_MATH_ROUND(C, 100);
    
    /* Check parameter */
    check = PS_App_Data_Check(pData);
    ret = check ? PS_STATUS_OK : PS_STATUS_ERR;
  }
  
#if _TRACE
  sprintf(text, "Power Supply ADC read Voltage=%dmV", Vadc);
  USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PS_PORT_NA, 0, (uint8_t *)text, strlen(text));
  
  if (!check)
  {
    sprintf(text, "[Param] Error: invalid parameters");
    USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PS_PORT_NA, 0, (uint8_t *)text, strlen(text));
  }
  else
  {
    sprintf(text, " [Param] Parameters: Voltage=%dmV, Current=%dmA, Ports-Power=%dmW", 
            pData->Voltage, 
            pData->Current, 
            PS_CalculatePowerSystem(pData));
    
    
    USBPD_TRACE_Add(USBPD_TRACE_DEBUG, PS_PORT_NA, 0, (uint8_t *)text, strlen(text));
  }
  
#endif /* _TRACE */
  
  return ret;
}
/** 
  * @} USBPD_POWER_SHARING_APP_Private_Functions
  */

/** @addtogroup USBPD_POWER_SHARING_APP_Private_Functions_Callbacks_Handlers
  * @{
  */
/** @brief Enable / Disable Port handler
  * @param PortNum port number
  * @param Enable enable / disable the port
  * @retval status condition error
  */
PS_Status_TypeDef PS_App_EnablePort_Handler(uint8_t PortNum, uint8_t Enable)
{
  return BSP_ERROR_NONE == BSP_USBPD_PWR_EnablePort(PortNum, Enable) ? PS_STATUS_OK : PS_STATUS_ERR;
}

/** @brief Send capabilities callback handler
  * @param PortNum port number
  * @retval status condition error
  */
PS_Status_TypeDef PS_App_SendSourceCapa_Handler(uint8_t PortNum)
{
  __USED static uint8_t LastPortSourceCapa = 0xFF;
  /* Nothing */
  UNUSED(PortNum);
  LastPortSourceCapa = PortNum;
  USBPD_DPM_RequestSourceCapability(PortNum);
  
  return PS_STATUS_OK;
}
/**
  * @} USBPD_POWER_SHARING_APP_Private_Functions_Callbacks_Handlers
  */

/** 
  * @} USBPD_POWER_SHARING_APP
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
