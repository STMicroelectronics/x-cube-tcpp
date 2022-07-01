/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef enum
{
  USBnoPD_CC1 = 0u,
  USBnoPD_CC2
} USBnoPD_CCTypeDef;

typedef enum
{
  USBnoPD_State_DETACHED = 0u, /* IDLE, nothing connected         */
  USBnoPD_State_ATTACHING,     /* Attachment ongoing - debouncing */
  USBnoPD_State_ATTACHED,      /* Attached                        */
  USBnoPD_State_DETACHING,     /* Detachment ongoing - debouncing */
  USBnoPD_State_DISCHARGING,   /* Vbus discharge ongoing          */
  USBnoPD_State_FAULT          /* Hardware fault                  */
} USBnoPD_StatesTypeDef;

typedef enum
{
  USBnoPD_ADC_Index_CC1 = 0u, /* CC1 index in adc buffer    */
  USBnoPD_ADC_Index_CC2,      /* CC2 index in adc buffer    */
  USBnoPD_ADC_Index_VBUSC,    /* VBus index in adc buffer   */
  USBnoPD_ADC_Index_ISENSE,   /* Isense index in adc buffer */
  USBnoPD_ADC_Index_VPROV     /* Vprov index in adc buffer  */
} USBnoPD_ADCBufIDTypeDef;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define USBNOPD_ADC_USED_CHANNELS     5u      /* Number of used ADC channels                                   */

#define USBNOPD_CC_VOLTAGE_MAXRA      800u    /* CC line Max voltage when Ra is connected (in mV)              */
#define USBNOPD_CC_VOLTAGE_MINRD      850u    /* CC line Min voltage when connected to Rd (in mV)              */
#define USBNOPD_CC_VOLTAGE_MAXRD      2450u   /* CC line Max voltage when connected to Rd (in mV)              */
#define USBNOPD_CC_VOLTAGE_MINOPEN    2750u   /* CC line Minimum voltage when not connected (in mV)            */
#define USBNOPD_VBUS_VOLTAGE_MAX      5500u   /* Vbus Maximum allowed voltage (in mV)                          */
#define USBNOPD_VPROV_VOLTAGE_MIN     4500u   /* Vprov Minimum voltage (in mV)                                 */
#define USBNOPD_VSAFE_VOLTAGE_MAX     100u    /* Vbus safe voltage to end vbus discharge (in mV)               */

#define USBNOPD_SRC1M1_NORA           0u      /* No voltage divider on CC lines                                */
#define USBNOPD_SRC1M1_NORB           0u      /* No voltage divider on CC lines                                */

#define USBNOPD_DEBOUNCE_ATTACH_TICKS 120u    /* Number of ms needed to complete attaching state debouncing    */
#define USBNOPD_DEBOUNCE_DETACH_TICKS 10u     /* Number of ms needed to complete detaching state debouncing    */

#define ADC_FULL_SCALE                0x0FFFu /* Maximum digital value of the ADC output (12 Bits)             */
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void USBnoPD_ProcessADC(void);
void USBnoPD_IncrementDebounceCount(void);
void USBnoPD_TCPPFaultHandling(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GREEN_LED_Pin GPIO_PIN_5
#define GREEN_LED_GPIO_Port GPIOA
#define FLGN_Pin GPIO_PIN_4
#define FLGN_GPIO_Port GPIOC
#define FLGN_EXTI_IRQn EXTI4_IRQn
#define ENABLE_Pin GPIO_PIN_8
#define ENABLE_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
