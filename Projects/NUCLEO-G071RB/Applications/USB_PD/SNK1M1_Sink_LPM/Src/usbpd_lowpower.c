/**
  ******************************************************************************
  * @file    usbpd_lowpower.c
  * @author  MCD Application Team
  * @brief   USBPD dpm core file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#define __USBPD_LOWPOWER_C

/* Includes ------------------------------------------------------------------*/
#ifndef _RTOS
#error "low power management only compatible with the RTOS mode"
#endif

#include "usbpd_core.h"
#include "usbpd_lowpower.h"
#include "stm32_lpm.h"
#include "stm32_timer.h"
#include "usbpd_trace.h"
#include "tracer_emb.h"
#include "stm32g0xx.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_rtc.h"
#include "cmsis_os.h"
#include "string.h"

/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UTIL_TIMER_Object_t TIMER_Lowpower;
UTIL_TIMER_Time_t TIMER_CurrentTime;

/* Private functions ---------------------------------------------------------*/
void LPM_Program_SleepTime(uint32_t *Tick);
void LPM_Exit_SleepTime(uint32_t Tick);
void LPM_ConfigureExternalTime(void);
void USBPD_LOWPOWER_vApplicationSleep(TickType_t TickType);
void dummyFunc(void *dummyArg);

/**
  * @brief  Low power initialisation function
  * @retval None
  */
void USBPD_LOWPOWER_Init(void)
{
  /* disable the strobing on the second port */
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  SET_BIT(SYSCFG->CFGR1, SYSCFG_CFGR1_UCPD2_STROBE);

  /* Configure the LPTIMER used to count the time */
  LPM_ConfigureExternalTime();

  /* Configure the LPM module */
  UTIL_LPM_Init();
}

/**
  * @brief  RTOS HOOK function used to handle the tickless feature
  * @retval None
  */
void USBPD_LOWPOWER_vApplicationSleep(TickType_t TickType)
{
  /* Disable all the interrupt */
  HAL_SuspendTick();
  uint32_t primask= __get_PRIMASK();
  __disable_irq();

  /* Manage the wakeup time */
  LPM_Program_SleepTime(&TickType);

  /* Manage the low power mode */
  UTIL_LPM_EnterLowPower();

  /* Manage sleep time duration */
  LPM_Exit_SleepTime(TickType);

  /* Re-Enable the interrupt */
  __set_PRIMASK(primask);
  HAL_ResumeTick();
}

void dummyFunc(void *dummyArg)
{
  /* Nothing to do */
}

/**
  * @brief  Initialize the count system used during lpm mode
  * @retval None
  */
void LPM_ConfigureExternalTime(void)
{
  UTIL_TIMER_Init();
  UTIL_TIMER_Create(&TIMER_Lowpower, 100, UTIL_TIMER_ONESHOT, dummyFunc, NULL);
}

/**
  * @brief  Program the sleep time
  * @retval None
  */
void LPM_Program_SleepTime(uint32_t *Tick)
{
  if(*Tick > 32000) *Tick = 32000;
  UTIL_TIMER_StartWithPeriod(&TIMER_Lowpower, *Tick);
  TIMER_CurrentTime = UTIL_TIMER_GetCurrentTime();
}

/**
  * @brief  Exit the sleep time
  * @retval None
  */
void LPM_Exit_SleepTime(uint32_t Tick)
{
  uint32_t time = UTIL_TIMER_GetElapsedTime(TIMER_CurrentTime);
  if ( time > Tick) time = Tick;
  uwTick += time;
  vTaskStepTick(time);
  USBPD_PE_TimerCounteUpdate(0, time);
}

void TRACER_EMB_LowPowerInit(void)
{
  /* nothing to do */
}

void TRACER_EMB_LowPowerSendData(void)
{
  /* Disable LPM mode according when transferring data */
  UTIL_LPM_SetStopMode(LPM_TRACE,UTIL_LPM_DISABLE);
  UTIL_LPM_SetOffMode(LPM_TRACE,UTIL_LPM_DISABLE);
}

void TRACER_EMB_LowPowerSendDataComplete(void)
{
    /* Allow the stop mode if no data send ongoing */
  UTIL_LPM_SetStopMode(LPM_TRACE,UTIL_LPM_ENABLE);
  UTIL_LPM_SetOffMode(LPM_TRACE,UTIL_LPM_ENABLE);
}
