 /*******************************************************************************
 * File Name          : stm32_timer_if.c
 * Description        : This file provides the ll driver for  the time server
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "stm32_timer_if.h"
#include "rtc.h"
#include "stm32g0xx_ll_rtc.h"
#include <math.h>

/* External variables ---------------------------------------------------------*/
extern RTC_HandleTypeDef hrtc;
#define RtcHandle hrtc

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint32_t Rtc_Time; /* Reference time */

  RTC_TimeTypeDef RTC_Calndr_Time; /* Reference time in calendar format */

  RTC_DateTypeDef RTC_Calndr_Date; /* Reference date in calendar format */

} RtcTimerContext_t;
/* Private define ------------------------------------------------------------*/
#define MIN_ALARM_DELAY               2 /* in ticks */

/* subsecond number of bits */  /* Now defined in main.h via MX GUI */
#define RTC_N_PREDIV_S                 10
#define RTC_PREDIV_S                  ((1<<RTC_N_PREDIV_S)-1)      /* RTC_SYNCH_PREDIV; */
#define RTC_PREDIV_A                  (1<<(15-RTC_N_PREDIV_S))-1    /* RTC_ASYNCH_PREDIV; */

/* Sub-second mask definition  */
#define RTC_ALARMSUBSECONDMASK    RTC_N_PREDIV_S<<RTC_ALRMASSR_MASKSS_Pos

/* RTC Time base in us */
#define USEC_NUMBER               1000000
#define MSEC_NUMBER               (USEC_NUMBER/1000)
#define RTC_ALARM_TIME_BASE       (USEC_NUMBER>>RTC_N_PREDIV_S)

#define COMMON_FACTOR             3
#define CONV_NUMER                (MSEC_NUMBER>>COMMON_FACTOR)
#define CONV_DENOM                (1<<(RTC_N_PREDIV_S-COMMON_FACTOR))

#define  DAYS_IN_LEAP_YEAR        (uint32_t)366

#define  DAYS_IN_YEAR             (uint32_t)365

#define  SECONDS_IN_1DAY          (uint32_t)86400

#define  SECONDS_IN_1HOUR         (uint32_t)3600

#define  SECONDS_IN_1MINUTE       (uint32_t)60

#define  MINUTES_IN_1HOUR         (uint32_t)60

#define  HOURS_IN_1DAY            (uint32_t)24

#define  DAYS_IN_MONTH_CORRECTION_NORM     ((uint32_t) 0x99AAA0 )
#define  DAYS_IN_MONTH_CORRECTION_LEAP     ((uint32_t) 0x445550 )


/* Calculates ceiling(X/N) */
#define DIVC(X,N)   ( ( (X) + (N) -1 ) / (N) )
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/*!
 * Number of days in each month on a normal year
 */
static const uint8_t DaysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/*!
 * Number of days in each month on a leap year
 */
static const uint8_t DaysInMonthLeapYear[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static RTC_AlarmTypeDef RTC_AlarmStructure;

/*!
 * Keep the value of the RTC timer when the RTC alarm is set
 * Set with the RTC_SetTimerContext function
 * Value is kept as a Reference to calculate alarm
 */
RtcTimerContext_t RtcTimerContext;

/* Private function prototypes -----------------------------------------------*/
/*!
 * @brief start wake up alarm
 * @note  alarm in RtcTimerContext.Rtc_Time + timeoutValue
 * @param timeoutValue in ticks
 * @retval none
 */
static void     RTC_StartWakeUpAlarm( uint32_t timeoutValue );

/*!
 * @brief get current time from calendar in ticks
 * @param pointer to RTC_DateStruct
 * @param pointer to RTC_TimeStruct
 * @retval time in ticks
 */
static uint32_t RTC_GetCalendarValue( RTC_DateTypeDef* RTC_DateStruct, RTC_TimeTypeDef* RTC_TimeStruct );

/* Exported variables ---------------------------------------------------------*/
const UTIL_TIMER_Driver_s UTIL_TimerDriver =
{
    RTC_Init,
    RTC_DeInit,

    RTC_StartTimer,
    RTC_StopTimer,

    RTC_SetTimerContext,
    RTC_GetTimerContext,

    RTC_GetTimerElapsedTime,
    RTC_GetTimerValue,
    RTC_GetMinimumTimeout,

    RTC_ms2Tick,
    RTC_Tick2ms,
};

/*!
 * @brief Initializes the PPP timer
 * @note The timer is based on the PPP
 * @param none
 * @retval none
 */
UTIL_TIMER_Status_t RTC_Init( void )
{
  MX_RTC_Init();
  return UTIL_TIMER_OK;
}

/*!
 * @brief Initializes the PPP timer
 * @note The timer is based on the PPP
 * @param none
 * @retval none
 */
UTIL_TIMER_Status_t RTC_DeInit( void )
{
  return UTIL_TIMER_OK;
}

/*!
 * @brief Set the timer
 * @note The timer is set at now (read in this function) + timeout
 * @param timeout Duration of the Timer ticks
 */
UTIL_TIMER_Status_t RTC_StartTimer( uint32_t timeout )
{
  RTC_StartWakeUpAlarm( timeout );
  return UTIL_TIMER_OK;
}

/*!
 * @brief Stop the timer
 * @param none
 * @retval none
 */
UTIL_TIMER_Status_t RTC_StopTimer( void )
{
  /* Clear RTC Alarm Flag */
  __HAL_RTC_ALARM_CLEAR_FLAG( &RtcHandle, RTC_FLAG_ALRAF);

  /* Disable the Alarm A interrupt */
  HAL_RTC_DeactivateAlarm(&RtcHandle, RTC_ALARM_A );
  return UTIL_TIMER_OK;
}


/*!
 * @brief returns the wake up time in ticks
 * @param none
 * @retval wake up time in ticks
 */
uint32_t RTC_GetMinimumTimeout( void )
{
  return( MIN_ALARM_DELAY );
}

/*!
 * @brief converts time in ms to time in ticks
 * @param [IN] time in milliseconds
 * @retval returns time in timer ticks
 */
uint32_t RTC_ms2Tick( uint32_t timeMicroSec )
{
  return ( uint32_t) ( ( ((uint64_t)timeMicroSec) * CONV_DENOM ) / CONV_NUMER );
}

/*!
 * @brief converts time in ticks to time in ms
 * @param [IN] time in timer ticks
 * @retval returns time in milliseconds
 */
uint32_t RTC_Tick2ms( uint32_t tick )
{
  return  ( ( (uint64_t)( tick )* CONV_NUMER ) / CONV_DENOM );
}

/*!
 * @brief Get the PPP timer elapsed time since the last timer was set
 * @param none
 * @retval PPP Elapsed time in ticks
 */
uint32_t RTC_GetTimerElapsedTime( void )
{
  RTC_TimeTypeDef RTC_TimeStruct;
  RTC_DateTypeDef RTC_DateStruct;

  uint32_t CalendarValue = RTC_GetCalendarValue(&RTC_DateStruct, &RTC_TimeStruct );

  return( ( uint32_t )( CalendarValue - RtcTimerContext.Rtc_Time ));
}

/*!
 * @brief Get the PPP timer value
 * @param none
 * @retval PPP Timer value in ticks
 */
uint32_t RTC_GetTimerValue( void )
{
  RTC_TimeTypeDef RTC_TimeStruct;
  RTC_DateTypeDef RTC_DateStruct;

  uint32_t CalendarValue = (uint32_t) RTC_GetCalendarValue(&RTC_DateStruct, &RTC_TimeStruct );

  return( CalendarValue );
}

/*!
 * @brief PPP IRQ Handler on the PPP timer
 * @param none
 * @retval none
 */
void RTC_IrqHandler ( void )
{
  RTC_HandleTypeDef* pRtcHandle=&RtcHandle;

  /* Get the AlarmA interrupt source enable status */
  if(__HAL_RTC_ALARM_GET_IT_SOURCE(pRtcHandle, RTC_IT_ALRA) != RESET)
  {
    /* Get the pending status of the AlarmA Interrupt */
    if(__HAL_RTC_ALARM_GET_FLAG(pRtcHandle, RTC_FLAG_ALRAF) != RESET)
    {
      /* Clear the AlarmA interrupt pending bit */
      __HAL_RTC_ALARM_CLEAR_FLAG(pRtcHandle, RTC_FLAG_ALRAF);
      /* Clear the EXTI's line Flag for RTC Alarm */
      /* AlarmA callback */
      UTIL_TIMER_IRQ_Handler();
    }
  }
}

/*!
 * @brief set Time Reference set also the sDate and sTime
 * @param none
 * @retval Timer Value
 */
uint32_t RTC_SetTimerContext( void )
{
//  char str[30];
  RtcTimerContext.Rtc_Time = RTC_GetCalendarValue( &RtcTimerContext.RTC_Calndr_Date, &RtcTimerContext.RTC_Calndr_Time );
//  sprintf(str,"=>%d(%d:%d:%d)", RtcTimerContext.Rtc_Time, RtcTimerContext.RTC_Calndr_Time.Minutes, RtcTimerContext.RTC_Calndr_Time.Seconds, 1023 - RtcTimerContext.RTC_Calndr_Time.SubSeconds);
//  USBPD_TRACE_Add(6, 0, 0, str, strlen(str));
  return ( uint32_t ) RtcTimerContext.Rtc_Time;
}

/*!
 * @brief Get the PPP timer Reference
 * @param none
 * @retval Timer Value in  Ticks
 */
uint32_t RTC_GetTimerContext( void )
{
  return (uint32_t) RtcTimerContext.Rtc_Time;
}

static void RTC_StartWakeUpAlarm( uint32_t timeoutValue )
{
  uint16_t rtcAlarmSubSeconds = 0;
  uint16_t rtcAlarmSeconds = 0;
  uint16_t rtcAlarmMinutes = 0;
  uint16_t rtcAlarmHours = 0;
  uint16_t rtcAlarmDays = 0;
  RTC_TimeTypeDef RTC_TimeStruct = RtcTimerContext.RTC_Calndr_Time;
  RTC_DateTypeDef RTC_DateStruct = RtcTimerContext.RTC_Calndr_Date;

  RTC_StopTimer();

  /*reverse counter */
  rtcAlarmSubSeconds =  RTC_PREDIV_S - RTC_TimeStruct.SubSeconds;
  rtcAlarmSubSeconds+= ( timeoutValue & RTC_PREDIV_S);
  /* convert timeout  to seconds */
  timeoutValue >>= RTC_N_PREDIV_S;  /* convert timeout  in seconds */

  /*convert microsecs to RTC format and add to 'Now' */
  rtcAlarmDays =  RTC_DateStruct.Date;
  while (timeoutValue >= SECONDS_IN_1DAY)
  {
    timeoutValue -= SECONDS_IN_1DAY;
    rtcAlarmDays++;
  }

  /* calc hours */
  rtcAlarmHours = RTC_TimeStruct.Hours;
  while (timeoutValue >= SECONDS_IN_1HOUR)
  {
    timeoutValue -= SECONDS_IN_1HOUR;
    rtcAlarmHours++;
  }

  /* calc minutes */
  rtcAlarmMinutes = RTC_TimeStruct.Minutes;
  while (timeoutValue >= SECONDS_IN_1MINUTE)
  {
    timeoutValue -= SECONDS_IN_1MINUTE;
    rtcAlarmMinutes++;
  }

  /* calc seconds */
  rtcAlarmSeconds =  RTC_TimeStruct.Seconds + timeoutValue;

  /***** correct for modulo********/
  while (rtcAlarmSubSeconds >= (RTC_PREDIV_S+1))
  {
    rtcAlarmSubSeconds -= (RTC_PREDIV_S+1);
    rtcAlarmSeconds++;
  }

  while (rtcAlarmSeconds >= SECONDS_IN_1MINUTE)
  {
    rtcAlarmSeconds -= SECONDS_IN_1MINUTE;
    rtcAlarmMinutes++;
  }

  while (rtcAlarmMinutes >= MINUTES_IN_1HOUR)
  {
    rtcAlarmMinutes -= MINUTES_IN_1HOUR;
    rtcAlarmHours++;
  }

  while (rtcAlarmHours >= HOURS_IN_1DAY)
  {
    rtcAlarmHours -= HOURS_IN_1DAY;
    rtcAlarmDays++;
  }

  if( RTC_DateStruct.Year % 4 == 0 )
  {
    if( rtcAlarmDays > DaysInMonthLeapYear[ RTC_DateStruct.Month - 1 ] )
    {
      rtcAlarmDays = rtcAlarmDays % DaysInMonthLeapYear[ RTC_DateStruct.Month - 1 ];
    }
  }
  else
  {
    if( rtcAlarmDays > DaysInMonth[ RTC_DateStruct.Month - 1 ] )
    {
      rtcAlarmDays = rtcAlarmDays % DaysInMonth[ RTC_DateStruct.Month - 1 ];
    }
  }

  /* Set RTC_AlarmStructure with calculated values*/
  RTC_AlarmStructure.AlarmTime.SubSeconds = RTC_PREDIV_S-rtcAlarmSubSeconds;
  RTC_AlarmStructure.AlarmSubSecondMask  = RTC_ALARMSUBSECONDMASK;
  RTC_AlarmStructure.AlarmTime.Seconds = rtcAlarmSeconds;
  RTC_AlarmStructure.AlarmTime.Minutes = rtcAlarmMinutes;
  RTC_AlarmStructure.AlarmTime.Hours   = rtcAlarmHours;
  RTC_AlarmStructure.AlarmDateWeekDay    = ( uint8_t )rtcAlarmDays;
  RTC_AlarmStructure.AlarmTime.TimeFormat   = RTC_TimeStruct.TimeFormat;
  RTC_AlarmStructure.AlarmDateWeekDaySel   = RTC_ALARMDATEWEEKDAYSEL_DATE;
  RTC_AlarmStructure.AlarmMask       = RTC_ALARMMASK_NONE;
  RTC_AlarmStructure.Alarm = RTC_ALARM_A;
  RTC_AlarmStructure.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  RTC_AlarmStructure.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;

  /* Set RTC_Alarm */
  HAL_RTC_SetAlarm_IT( &RtcHandle, &RTC_AlarmStructure, RTC_FORMAT_BIN );
}

static uint32_t RTC_GetCalendarValue( RTC_DateTypeDef* RTC_DateStruct, RTC_TimeTypeDef* RTC_TimeStruct )
{
  uint32_t calendarValue = 0;
  uint32_t first_read;
  uint32_t correction;

  /* Get Time and Date*/
//  HAL_RTC_GetTime( &RtcHandle, RTC_TimeStruct, RTC_FORMAT_BIN );

  /* make sure it is correct due to asynchronus nature of RTC*/
  do {
    first_read = LL_RTC_TIME_GetSubSecond(RTC);
    HAL_RTC_GetTime( &RtcHandle, RTC_TimeStruct, RTC_FORMAT_BIN );
  } while (first_read != LL_RTC_TIME_GetSubSecond(RTC) );
  HAL_RTC_GetDate( &RtcHandle, RTC_DateStruct, RTC_FORMAT_BIN );

  /* calculte amount of elapsed days since 01/01/2000 */
  calendarValue= DIVC( (DAYS_IN_YEAR*3 + DAYS_IN_LEAP_YEAR)* RTC_DateStruct->Year , 4);

  correction = ( (RTC_DateStruct->Year % 4) == 0 ) ? DAYS_IN_MONTH_CORRECTION_LEAP : DAYS_IN_MONTH_CORRECTION_NORM ;

  calendarValue +=( DIVC( (RTC_DateStruct->Month-1)*(30+31) ,2 ) - (((correction>> ((RTC_DateStruct->Month-1)*2) )&0x3)));

  calendarValue += (RTC_DateStruct->Date -1);

  /* convert from days to seconds */
  calendarValue *= SECONDS_IN_1DAY;

  calendarValue += ( ( uint32_t )RTC_TimeStruct->Seconds +
                     ( ( uint32_t )RTC_TimeStruct->Minutes * SECONDS_IN_1MINUTE ) +
                     ( ( uint32_t )RTC_TimeStruct->Hours * SECONDS_IN_1HOUR ) ) ;

  calendarValue = (calendarValue<<RTC_N_PREDIV_S);
  calendarValue+= ((RTC_PREDIV_S - RTC_TimeStruct->SubSeconds)); ///*1000)/1024;

  return( calendarValue );
}


