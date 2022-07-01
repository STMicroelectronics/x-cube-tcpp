/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g0xx_it.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usbpd.h"
#include "tracer_emb.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbpd_def.h"
#include "usbpd_dpm_core.h"
#include "usbpd_hw_if.h"
#if defined(_GUI_INTERFACE)
#include "gui_api.h"
#endif /* _GUI_INTERFACE */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
#if defined(_GUI_INTERFACE)
  GUI_TimerCounter();
#endif /* _GUI_INTERFACE */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
#endif /* INCLUDE_xTaskGetSchedulerState */
  xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
  }
#endif /* INCLUDE_xTaskGetSchedulerState */
  USBPD_DPM_TimerCounter();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32G0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles UCPD1 and UCPD2 interrupts / UCPD1 and UCPD2 wake-up interrupts through EXTI lines 32 and 33.
  */
void UCPD1_2_IRQHandler(void)
{
  /* USER CODE BEGIN UCPD1_2_IRQn 0 */

  /* USER CODE END UCPD1_2_IRQn 0 */
  USBPD_PORT0_IRQHandler();

  /* USER CODE BEGIN UCPD1_2_IRQn 1 */

  /* USER CODE END UCPD1_2_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel 2 and channel 3 interrupts.
  */
void DMA1_Channel2_3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 0 */

  /* USER CODE END DMA1_Channel2_3_IRQn 0 */

  /* USER CODE BEGIN DMA1_Channel2_3_IRQn 1 */

  /* USER CODE END DMA1_Channel2_3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 channel 4, channel 5, channel 6, channel 7 and DMAMUX1 interrupts.
  */
void DMA1_Ch4_7_DMAMUX1_OVR_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Ch4_7_DMAMUX1_OVR_IRQn 0 */

  /* USER CODE END DMA1_Ch4_7_DMAMUX1_OVR_IRQn 0 */

  TRACER_EMB_IRQHandlerDMA();
  /* USER CODE BEGIN DMA1_Ch4_7_DMAMUX1_OVR_IRQn 1 */

  /* USER CODE END DMA1_Ch4_7_DMAMUX1_OVR_IRQn 1 */
}

/**
  * @brief This function handles USART3, USART4 and LPUART1 interrupts / LPUART1 wake-up interrupt through EXTI line 28.
  */
void USART3_4_LPUART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_4_LPUART1_IRQn 0 */

  /* USER CODE END USART3_4_LPUART1_IRQn 0 */

  TRACER_EMB_IRQHandlerUSART();
  /* USER CODE BEGIN USART3_4_LPUART1_IRQn 1 */

  /* USER CODE END USART3_4_LPUART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
