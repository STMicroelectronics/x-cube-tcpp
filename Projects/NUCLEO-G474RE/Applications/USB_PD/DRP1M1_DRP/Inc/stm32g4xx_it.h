/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g4xx_it.h
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#ifndef STM32G4xx_IT_H
#define STM32G4xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#if defined(_TRACE)||defined(_GUI_INTERFACE)
#include "tracer_emb.h"
#endif
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void DebugMon_Handler(void);
void SysTick_Handler(void);
void UCPD1_IRQHandler(void);
#if defined(_TRACE) || defined(_GUI_INTERFACE)
void TRACER_EMB_USART_IRQHANDLER(void);
#if TRACER_EMB_DMA_MODE == 1UL
void TRACER_EMB_TX_DMA_IRQHANDLER(void);
#endif
#endif /*_TRACE || _GUI_INTERFACE */

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#if defined(TCPP0203_SUPPORT)
void I2C1_IRQHandler(void);

void TCPP0203_PORT0_FLG_EXTI_IRQHANDLER(void);
void TCPP0203_PORT0_ADC_IRQHANDLER(void);
void TCPP0203_PORT0_ADC_DMA_IRQHANDLER(void);
#endif /* TCPP0203_SUPPORT */

#ifdef __cplusplus
}
#endif

#endif /* STM32G4xx_IT_H */

