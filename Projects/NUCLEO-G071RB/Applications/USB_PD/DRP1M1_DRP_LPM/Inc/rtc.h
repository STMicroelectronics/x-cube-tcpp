/**
  ******************************************************************************
  * @file   rtc.h
  * @author MCD Application Team
  * @brief  configuration of the RTC instances
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern RTC_HandleTypeDef hrtc;
/* Defines ------------------------------------------------------------*/
   
/* USER CODE BEGIN Private defines */
   
#if defined (STM32WL55xx)

/* RTC clock source */
typedef enum {
  RTC_CLOCK_SOURCE_LSE = 0,
  RTC_CLOCK_SOURCE_LSI,
  RTC_CLOCK_SOURCE_HSE,
  RTC_CLOCK_SOURCE_NB
} RTCClockSource_t;
#endif
/* USER CODE END Private defines */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
/**
  * @brief Initializes the RTC
  * @param None
  * @retval None
  */
void MX_RTC_Init( void );

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __RTC_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
