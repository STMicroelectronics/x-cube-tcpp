/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tracer_emb_conf.h
  * @author  MCD Application Team
  * @brief   This file contains the Trace HW related defines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef TRACER_EMB_CONF_H
#define TRACER_EMB_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_usart.h"
#include "stm32g0xx_ll_lpuart.h"
/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* -----------------------------------------------------------------------------
      Definitions for TRACE feature
-------------------------------------------------------------------------------*/

#define TRACER_EMB_BAUDRATE                          921600UL

#define TRACER_EMB_DMA_MODE                          1UL
#define TRACER_EMB_IT_MODE                           0UL

#define TRACER_EMB_BUFFER_SIZE                       1024UL

/* -----------------------------------------------------------------------------
      Definitions for TRACE Hw information
-------------------------------------------------------------------------------*/

#define TRACER_EMB_IS_INSTANCE_LPUART_TYPE           1UL /* set to 1UL if LPUART is used instead of USART */

#define TRACER_EMB_USART_INSTANCE                    LPUART1

#define TRACER_EMB_TX_GPIO                           GPIOA
#define TRACER_EMB_TX_PIN                            LL_GPIO_PIN_2
#define TRACER_EMB_TX_AF                             LL_GPIO_AF_6
#define TRACER_EMB_TX_GPIO_ENABLE_CLOCK()            LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA)
#define TRACER_EMB_RX_GPIO                           GPIOA
#define TRACER_EMB_RX_PIN                            LL_GPIO_PIN_3
#define TRACER_EMB_RX_AF                             LL_GPIO_AF_6
#define TRACER_EMB_RX_GPIO_ENABLE_CLOCK()            LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA)

#define TRACER_EMB_ENABLE_CLK_USART()                LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_LPUART1)
#define TRACER_EMB_DISABLE_CLK_USART()               LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_LPUART1)
#define TRACER_EMB_SET_CLK_SOURCE_USART()            /* No need for clock source selection in case of USART3 // LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK2) */
#define TRACER_EMB_USART_IRQ                         USART3_4_5_6_LPUART1_IRQn
#define TRACER_EMB_USART_IRQHANDLER                  USART3_4_5_6_LPUART1_IRQHandler
#define TRACER_EMB_TX_AF_FUNCTION                    LL_GPIO_SetAFPin_0_7
#define TRACER_EMB_RX_AF_FUNCTION                    LL_GPIO_SetAFPin_0_7
#define TRACER_EMB_TX_IRQ_PRIORITY                   3
 /* TRACER_EMB_DMA_MODE == 1UL */
#define TRACER_EMB_DMA_INSTANCE                      DMA1
#define TRACER_EMB_ENABLE_CLK_DMA()                  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1)
#define TRACER_EMB_TX_DMA_REQUEST                    LL_DMAMUX_REQ_LPUART1_TX
#define TRACER_EMB_TX_DMA_CHANNEL                    LL_DMA_CHANNEL_3
#define TRACER_EMB_ENABLECHANNEL                     LL_DMA_EnableChannel
#define TRACER_EMB_DISABLECHANNEL                    LL_DMA_DisableChannel
#define TRACER_EMB_TX_DMA_IRQ                        DMA1_Channel2_3_IRQn
#define TRACER_EMB_TX_DMA_IRQHANDLER                 DMA1_Channel2_3_IRQHandler
#define TRACER_EMB_TX_DMA_ACTIVE_FLAG                LL_DMA_IsActiveFlag_TC3
#define TRACER_EMB_TX_DMA_CLEAR_FLAG                 LL_DMA_ClearFlag_GI3
#define TRACER_EMB_TX_DMA_PRIORITY                   0
 /* TRACER_EMB_DMA_MODE == 1UL */

#ifdef __cplusplus
}
#endif

#endif /* TRACER_EMB_CONF_H */

