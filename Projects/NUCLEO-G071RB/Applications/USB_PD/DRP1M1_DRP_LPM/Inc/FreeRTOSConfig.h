/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
*/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * These parameters and more are described within the 'configuration' section of the
 * FreeRTOS API documentation available on the FreeRTOS.org web site.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* Ensure stdint is only used by the compiler, and not the assembler. */
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
    #include <stdint.h>
//debug low_power
void xPortSysTickHandler(void);
// end debug
#if defined(CMSIS_OS_V2)
    #include "cmsis_os.h"
#endif
    extern uint32_t SystemCoreClock;
#endif

/*  CMSIS-RTOSv2 defines 56 levels of priorities. To be able to use them
 *  all and avoid application misbehavior, configUSE_PORT_OPTIMISED_TASK_SELECTION
 *  must be set to 0 and configMAX_PRIORITIES to 56
 *
 */
#if !defined(CMSIS_OS_V2)
/* #define configUSE_PORT_OPTIMISED_TASK_SELECTION	0*/
#define configMAX_PRIORITIES                     ( 7 )
#else
#define configUSE_PORT_OPTIMISED_TASK_SELECTION  0
#define configMAX_PRIORITIES                     ( 56 )
#endif /* osCMSIS < 0x20000U */

#define configUSE_PREEMPTION                     1
#define configSUPPORT_DYNAMIC_ALLOCATION         1
#define configUSE_IDLE_HOOK                      0
#define configUSE_TICK_HOOK                      0
#define configCPU_CLOCK_HZ                       ( SystemCoreClock )
#define configTICK_RATE_HZ                       ((TickType_t)1000)

#if defined(_CPU_STAT)
#define configGENERATE_RUN_TIME_STATS            1
#define configSUPPORT_DYNAMIC_ALLOCATION         1
#define configUSE_STATS_FORMATTING_FUNCTIONS     1
#define configUSE_TRACE_FACILITY                 1
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()   ConfigureTimerRunTimeStat();
#define portGET_RUN_TIME_COUNTER_VALUE()           GetRunTimeCounter();
#else
#define configGENERATE_RUN_TIME_STATS            0
#define configUSE_STATS_FORMATTING_FUNCTIONS     0
#define configUSE_TRACE_FACILITY                 0
#endif

#ifdef _LOW_POWER
#define portSUPPRESS_TICKS_AND_SLEEP( xIdleTime ) USBPD_LOWPOWER_vApplicationSleep( xIdleTime )
#define configUSE_TICKLESS_IDLE                  1
#define configMINIMAL_STACK_SIZE                 ((uint16_t)300)
#else
#define configUSE_TICKLESS_IDLE                  0
#if defined(_GUI_INTERFACE)
#define configMINIMAL_STACK_SIZE                 ((uint16_t)300)
#elif defined(__AUTHENTICATION__)
#define configMINIMAL_STACK_SIZE                 ((uint16_t)30)
#else
#define configMINIMAL_STACK_SIZE                 ((uint16_t)300)
#endif /* _GUI_INTERFACE */
#endif /* _LOW_POWER */

#ifdef _FORCE_SINK
#define BASE_HEAP_SIZE                           18000
#else
#define BASE_HEAP_SIZE                           8000
#endif /* _FORCE_SINK */

#if defined(__AUTHENTICATION__)
#if defined(USE_LCD)
#define ADDON_HEAP_SIZE                           16400
#else
#define ADDON_HEAP_SIZE                           17000
#endif
#elif defined(_GUI_INTERFACE)
#define ADDON_HEAP_SIZE                           2000
#elif defined(_VDM)
#define ADDON_HEAP_SIZE                           1500
#else
#define ADDON_HEAP_SIZE                           1000
#endif

#define configTOTAL_HEAP_SIZE                    ((size_t)BASE_HEAP_SIZE + ADDON_HEAP_SIZE)

#define configMAX_TASK_NAME_LEN                  ( 8 )
#define configUSE_16_BIT_TICKS                   0
#define configIDLE_SHOULD_YIELD                  1
#if defined(_DISCO) || defined(_UCSI_OPM)
#define configUSE_MUTEXES                        1
#define configQUEUE_REGISTRY_SIZE                30
#define configUSE_QUEUE_SETS                     0
#else
#define configQUEUE_REGISTRY_SIZE                6
#define configUSE_MUTEXES                        0
#endif /* _DISCO || _UCSI_OPM */
#define configCHECK_FOR_STACK_OVERFLOW           1
#define configUSE_RECURSIVE_MUTEXES              0
#define configUSE_MALLOC_FAILED_HOOK             0
#define configUSE_APPLICATION_TASK_TAG           0
#define configUSE_COUNTING_SEMAPHORES            0

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES                    0
#define configMAX_CO_ROUTINE_PRIORITIES          ( 2 )

/* Software timer definitions. */
#ifdef _DISCO
#if !defined(__AUTHENTICATION__)
#define configUSE_TIMERS                        1
#else
#define configUSE_TIMERS                        0
#endif /* __AUTHENTICATION__ */
#define configTIMER_TASK_PRIORITY               ( 2 )
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            ( configMINIMAL_STACK_SIZE * 2 )
#else
#define configUSE_TIMERS                        0
#define configTIMER_TASK_PRIORITY               ( 2 )
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            ( configMINIMAL_STACK_SIZE * 2 )
#endif

#define INCLUDE_eTaskGetState                1
#define INCLUDE_vTaskPrioritySet             1
#define INCLUDE_uxTaskPriorityGet            1
#define INCLUDE_vTaskDelete                  1
#define INCLUDE_vTaskCleanUpResources        0
#define INCLUDE_vTaskSuspend                 1
#define INCLUDE_vTaskDelayUntil              0
#define INCLUDE_vTaskDelay                   1
#define INCLUDE_xTaskGetSchedulerState       1
#define INCLUDE_xTaskGetCurrentTaskHandle    1

/* When using CMSIS-RTOSv2 set configSUPPORT_STATIC_ALLOCATION to 1
 * is mandatory to avoid compile errors.
 */
#if !defined(CMSIS_OS_V2)
#define configSUPPORT_STATIC_ALLOCATION     0
#else
#define configSUPPORT_STATIC_ALLOCATION     1
#endif /* osCMSIS < 0x20000U */

/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
 /* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
 #define configPRIO_BITS         __NVIC_PRIO_BITS
#else
 #define configPRIO_BITS         2
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY   3

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 3

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY         ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#define configASSERT( x ) if ((x) == 0) {taskDISABLE_INTERRUPTS(); for( ;; );}

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler    SVC_Handler
#define xPortPendSVHandler PendSV_Handler

/* IMPORTANT: FreeRTOS is using the SysTick as internal time base, thus make sure the system and peripherials are
              using a different time base (TIM based for example).
 */
/* #define xPortSysTickHandler SysTick_Handler */

#endif /* FREERTOS_CONFIG_H */

