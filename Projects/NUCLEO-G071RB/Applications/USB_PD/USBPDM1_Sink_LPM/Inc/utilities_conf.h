/**
 ******************************************************************************
 * @file    utilities_conf.h
 * @author  MCD Application Team
 * @brief   Configuration file to utilities
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
#ifndef __UTILITIES_CONF_H
#define __UTILITIES_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cmsis_compiler.h"
#include "string.h"
#include "stdint.h"
#if defined(USE_STM32_UTILITY_OS)
#include "stm32_seq.h"
#include "stm32_timer.h"
#endif /* USE_STM32_UTILITY_OS */

/******************************************************************************
 * common
 ******************************************************************************/
#define UTILS_ENTER_CRITICAL_SECTION( )         uint32_t primask_bit = __get_PRIMASK( );\
                                                __disable_irq( )

#define UTILS_EXIT_CRITICAL_SECTION( )          __set_PRIMASK( primask_bit )

#define UTILS_MEMSET8( dest, value, size )      memset( dest, value, size);

/******************************************************************************
 * tiny low power manager
 * (any macro that does not need to be modified can be removed)
 ******************************************************************************/
#define UTIL_LPM_INIT_CRITICAL_SECTION( )
#define UTIL_LPM_ENTER_CRITICAL_SECTION( )      UTILS_ENTER_CRITICAL_SECTION( )
#define UTIL_LPM_EXIT_CRITICAL_SECTION( )       UTILS_EXIT_CRITICAL_SECTION( )

typedef enum
{
  LPM_CAD   = 1 << 0,
  LPM_DPM   = 1 << 1,
  LPM_PE_0  = 1 << 2,
  LPM_PE_1  = 1 << 3,
  LPM_TRACE = 1 << 4,
} LPM_Id_t;


#if defined(USE_STM32_UTILITY_OS)
/******************************************************************************
 * sequencer
 * (any macro that does not need to be modified can be removed)
 ******************************************************************************/
#define UTIL_SEQ_INIT_CRITICAL_SECTION( )
#define UTIL_SEQ_ENTER_CRITICAL_SECTION( )      UTILS_ENTER_CRITICAL_SECTION( )
#define UTIL_SEQ_EXIT_CRITICAL_SECTION( )       UTILS_EXIT_CRITICAL_SECTION( )
#define UTIL_SEQ_CONF_TASK_NBR                  (32)
#define UTIL_SEQ_CONF_PRIO_NBR                  (2)
#define UTIL_SEQ_MEMSET8( dest, value, size )   UTILS_MEMSET8( dest, value, size )

typedef enum {
  TASK_CAD  = 1 << 0,
  TASK_PE_0 = 1 << 1,
  TASK_PE_1 = 1 << 2,
  TASK_TRACE= 1 << 3,
  TASK_USER = 1 << 4,
  TASK_GUI  = 1 << 5,
  TASK_DEMO = 1 << 6,
}USBPD_Task;
#endif /* USE_STM32_UTILITY_OS */

/******************************************************************************
 * advanded trace
 * (any macro that does not need to be modified can be removed)
 ******************************************************************************/
#define UTIL_ADV_TRACE_DEBUG(...)
#define UTIL_ADV_TRACE_OVERRUN_NA
#define UTIL_ADV_TRACE_CONDITIONNAL_NA
#define UTIL_ADV_TRACE_UNCHUNK_MODE_NA
#define UTIL_ADV_TRACE_INIT_CRITICAL_SECTION()
#define UTIL_ADV_TRACE_ENTER_CRITICAL_SECTION()     UTILS_ENTER_CRITICAL_SECTION()
#define UTIL_ADV_TRACE_EXIT_CRITICAL_SECTION()      UTILS_EXIT_CRITICAL_SECTION()
#define UTIL_ADV_TRACE_TMP_BUF_SIZE                 (128U)
#define UTIL_ADV_TRACE_TMP_MAX_TIMESTMAP_SIZE       (10U)
#define UTIL_ADV_TRACE_FIFO_SIZE                    (1024U)
#define UTIL_ADV_TRACE_MEMSET8( dest, value, size ) UTILS_MEMSET8( dest, value, size )

#ifdef __cplusplus
}
#endif

#endif /*__UTILITIES_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
