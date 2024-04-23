/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usbpd_devices_conf.h
  * @author  MCD Application Team
  * @brief   This file contains the device define.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* CubeMX Generated */
#define CUBEMX_GENERATED

#ifndef USBPD_DEVICE_CONF_H
#define USBPD_DEVICE_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_ll_adc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_gpio.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_ucpd.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_tim.h"
//#include "usbpd_pwr_user.h"
#include "usbpd_pwr_if.h"

/* USER CODE BEGIN Includes */
#if defined(NUCLEO_MB1360A) && defined(TCPP0203_SUPPORT)
#include "drp1m1_usbpd_pwr.h"
#endif  /* NUCLEO_MB1360A && TCPP0203_SUPPORT */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* -----------------------------------------------------------------------------
      usbpd_hw.c
-------------------------------------------------------------------------------*/

/* defined used to configure function : USBPD_HW_GetUSPDInstance */
#define UCPD_INSTANCE0 UCPD1

/* defined used to configure function : USBPD_HW_Init_DMARxInstance,USBPD_HW_DeInit_DMARxInstance */
#define UCPDDMA_INSTANCE0_CLOCKENABLE_RX  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1)

#define UCPDDMA_INSTANCE0_DMA_RX  DMA1

#define UCPDDMA_INSTANCE0_REQUEST_RX   DMA_REQUEST_UCPD1_RX

#define UCPDDMA_INSTANCE0_LL_CHANNEL_RX   LL_DMA_CHANNEL_5

#define UCPDDMA_INSTANCE0_CHANNEL_RX   DMA1_Channel5

/* defined used to configure function : USBPD_HW_Init_DMATxInstance, USBPD_HW_DeInit_DMATxInstance */
#define UCPDDMA_INSTANCE0_CLOCKENABLE_TX  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1)

#define UCPDDMA_INSTANCE0_DMA_TX  DMA1

#define UCPDDMA_INSTANCE0_REQUEST_TX   DMA_REQUEST_UCPD1_TX

#define UCPDDMA_INSTANCE0_LL_CHANNEL_TX   LL_DMA_CHANNEL_3

#define UCPDDMA_INSTANCE0_CHANNEL_TX   DMA1_Channel3

/* defined used to configure  USBPD_HW_SetFRSSignalling */
#define UCPDFRS_INSTANCE0_FRSCC1
#define UCPDFRS_INSTANCE0_FRSCC2

#define UCPD_INSTANCE0_ENABLEIRQ   do{                                                                 \
                                        NVIC_SetPriority(UCPD1_2_IRQn,0);                              \
                                        NVIC_EnableIRQ(UCPD1_2_IRQn);                                  \
                                    } while(0)

/* -----------------------------------------------------------------------------
      Definitions for timer service feature
-------------------------------------------------------------------------------*/

#define TIMX                           TIM1
#define TIMX_CLK_ENABLE                LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1)
#define TIMX_CLK_DISABLE               LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM1)
#define TIMX_IRQ                       TIM1_BRK_UP_TRG_COM_IRQn
#define TIMX_CHANNEL_CH1               LL_TIM_CHANNEL_CH1
#define TIMX_CHANNEL_CH2               LL_TIM_CHANNEL_CH2
#define TIMX_CHANNEL_CH3               LL_TIM_CHANNEL_CH3
#define TIMX_CHANNEL_CH4               LL_TIM_CHANNEL_CH4
#define TIMX_CHANNEL1_SETEVENT         do{                                                                    \
                                          LL_TIM_OC_SetCompareCH1(TIMX, (TimeUs + TIMX->CNT) % TIM_MAX_TIME);\
                                          LL_TIM_ClearFlag_CC1(TIMX);                                         \
                                       }while(0)
#define TIMX_CHANNEL2_SETEVENT         do{                                                                    \
                                          LL_TIM_OC_SetCompareCH2(TIMX, (TimeUs + TIMX->CNT) % TIM_MAX_TIME);\
                                          LL_TIM_ClearFlag_CC2(TIMX);                                         \
                                       }while(0)
#define TIMX_CHANNEL3_SETEVENT         do{                                                                    \
                                          LL_TIM_OC_SetCompareCH3(TIMX, (TimeUs + TIMX->CNT) % TIM_MAX_TIME);\
                                          LL_TIM_ClearFlag_CC3(TIMX);                                         \
                                       }while(0)
#define TIMX_CHANNEL4_SETEVENT         do{                                                                    \
                                          LL_TIM_OC_SetCompareCH4(TIMX, (TimeUs + TIMX->CNT) % TIM_MAX_TIME);\
                                          LL_TIM_ClearFlag_CC4(TIMX);                                         \
                                       }while(0)
#define TIMX_CHANNEL1_GETFLAG          LL_TIM_IsActiveFlag_CC1
#define TIMX_CHANNEL2_GETFLAG          LL_TIM_IsActiveFlag_CC2
#define TIMX_CHANNEL3_GETFLAG          LL_TIM_IsActiveFlag_CC3
#define TIMX_CHANNEL4_GETFLAG          LL_TIM_IsActiveFlag_CC4

#ifdef __cplusplus
 }
#endif

#endif /* USBPD_DEVICE_CONF_H */
